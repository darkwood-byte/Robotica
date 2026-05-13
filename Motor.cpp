#define CALIBRATION_FACTOR 2

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"

#include "Motor.h"
#include "Sensor.h"
#include "error.h"

#define MAX_MOTORS 2
#define JITTERBUFFER 4
#define RESETBUFFER 20

// -------------------- GLOBAL ALARM STATE --------------------

static Motor* motors[MAX_MOTORS] = {nullptr};
static uint64_t fall_time[MAX_MOTORS] = {0};
static bool alarm_init_done = false;

// -------------------- ALARM CALLBACK --------------------

static void motor_alarm_callback(uint alarm_num) {
    uint64_t now = time_us_64();

    for (int i = 0; i < MAX_MOTORS; i++) {
        if (motors[i] && fall_time[i] != 0 && now >= fall_time[i]) {
            motors[i]->step_fall();
            fall_time[i] = 0;
        }
    }

    // plan volgende interrupt indien nodig
    uint64_t next = UINT64_MAX;
    bool active = false;

    for (int i = 0; i < MAX_MOTORS; i++) {
        if (fall_time[i] != 0) {
            active = true;
            if (fall_time[i] < next) next = fall_time[i];
        }
    }

    if (active) {
        hardware_alarm_set_target(alarm_num, next);
    }
}

// -------------------- MOTOR CLASS --------------------

void Motor::init_alarm_system() {
    if (alarm_init_done) return;

    hardware_alarm_claim(0);
    hardware_alarm_set_callback(0, motor_alarm_callback);

    alarm_init_done = true;
}

void Motor::register_motor() {
    for (int i = 0; i < MAX_MOTORS; i++) {
        if (motors[i] == nullptr) {
            motors[i] = this;
            return;
        }
    }
}

// -------------------- INIT --------------------

void Motor::Init(int dir_pin, int step_pin, Sensor sensor_low, Sensor sensor_high) {
    Ready = false;
    Goal = 1;//om te voorkomen dat er niet gecalbireert wordt

    d_pin = dir_pin;
    s_pin = step_pin;

    steps = 0;
    max = 0;
    delta = 0;

    error = false;

    sensor_L = sensor_low;
    sensor_H = sensor_high;

    sensor_L.init();
    sensor_H.init();

    gpio_init(d_pin);
    gpio_init(s_pin);

    gpio_set_dir(d_pin, GPIO_OUT);
    gpio_set_dir(s_pin, GPIO_OUT);

    pulse_active = false;

    init_alarm_system();
    register_motor();
    //check de sensor conecties
    Push_error();
    while(sensor_H.Check() == ERROR || sensor_L.Check() == ERROR)sleep_ms(500);
    Push_incoming();
     //check de sensor conecties
    calculating = CALC_RETURNING_TO_BASE;
    calibrating = CALIBRATE_FINISHED;
}

// -------------------- USE --------------------

void Motor::Use() {
    //sensor error afhandeling
    if(sensor_H.Check() == ERROR || sensor_L.Check() == ERROR){
        Push_error();
        error = true;
        return;
    }
    else if(error == true){
        error = false;
        Push_working();
    }
    //calibreer als je teveel loopt
     if (steps > max * CALIBRATION_FACTOR)calibrating = true;
    //beweging
    if (delta == Goal) {
        Ready = true;
    } else {
        Ready = false;
        if(calculating == CALC_FINISHED && calibrating == CALIBRATE_FINISHED){
            if(sensor_H.Check() == PRESED || sensor_L.Check() == PRESED){
                calibrating = CALIBRATE_HEADING_HOME;
                return;
            }
            if (delta < Goal) {step_high(); delta++;}
            else {step_low();delta--;}
            steps++;
        }
        //bereken range
        else if (calculating != CALC_FINISHED){
            calculating_func();
        }
        //calibratie
        else if(calibrating == CALIBRATE_HEADING_HOME){
            step_low();
            if(sensor_L.Check() == PRESED)calibrating = CALC_FINETUNING;
        }
        else if(calibrating == CALIBRATE_FINETUNING){
            if(sensor_L.Check() == PRESED){
                step_high();
            }
            else calibrating = CALC_FINISHED;
        }
    }
}

void Motor::calculating_func(){
    //calibreer een kant op
    if(calculating == CALC_RETURNING_TO_BASE){
        step_low();
        if(sensor_L.Check() == PRESED){
            calculating =  CALC_FINETUNING;
        }
        if(sensor_H.Check() == PRESED){
            Sensor t;
            t = sensor_H;
            sensor_H = sensor_L;
            sensor_L = t;
            calculating = CALC_FINETUNING;
        }
    }
    //verlaat de eikings sensor
    else  if(calculating == CALC_FINETUNING){
        if(sensor_L.Check() == PRESED){
            step_high();
        }
        else if(sensor_L.Check() == OPEN){
            calculating = CALC_COUNTING;
            max = 0;
        }
    }
    //tel de stapjes van de motor om een range te vinden
    else if(calculating == CALC_COUNTING){
        step_high();
        max++;
        if(sensor_H.Check() == PRESED){
            calculating = CALC_FINISHED;
            delta = max;
        }
    }
   
}
// -------------------- STEP CONTROL --------------------

void Motor::step_high() {
    gpio_put(d_pin, false);
    step();
}

void Motor::step_low() {
    gpio_put(d_pin, true);
    step();
}

// -------------------- STEP (10 µs PULSE) --------------------

void Motor::step() {
    if (pulse_active) return;
    pulse_active = true;

    gpio_put(s_pin, true);

    // bereken wanneer de pulse weer laag moet
    uint64_t t = time_us_64() + STEPERDRIVERPULSELENGHTUS;

    // sla deze tijd op
    for (int i = 0; i < MAX_MOTORS; i++) {
        if (motors[i] == this) {
            fall_time[i] = t;
            break;
        }
    }
    //slotje
    uint64_t next = UINT64_MAX;

    for (int i = 0; i < MAX_MOTORS; i++) {
        if (fall_time[i] != 0 && fall_time[i] < next) {
            next = fall_time[i];
        }
    }

    if (next != UINT64_MAX) {
        hardware_alarm_set_target(0, next);
    }
}

// -------------------- STEP FALL (ISR LOW) --------------------

void Motor::step_fall() {
    gpio_put(s_pin, false);
    pulse_active = false;
}