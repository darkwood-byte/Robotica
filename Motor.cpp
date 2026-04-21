#define CALIBRATION_FACTOR 2

#include "pico/stdlib.h"
#include "Motor.h"
#include "Sensor.h"
#include "error.h"

void Motor::Init(int dir_pin, int step_pin, Sensor sensor_low, Sensor sensor_high){
    Ready = false;

    Goal = 0;

    d_pin = dir_pin;
    s_pin = step_pin;

    steps = 0;
    max = 0;
    delta = 0;

    sensor_L = sensor_low;
    sensor_H = sensor_high;

    sensor_L.init();
    sensor_H.init();
    
    gpio_init(d_pin);
    gpio_init(s_pin);

    gpio_set_dir(d_pin, GPIO_OUT);
    gpio_set_dir(s_pin, GPIO_OUT);

    init_calibration();//check de range
}

void Motor::Use(){
    check();

    if(delta == Goal){
        Ready = true;
    }
    else{
        Ready = false;
        if(delta < Goal)step_high();
        else step_low();
    } 

}

void Motor::step_high(){
    gpio_put(d_pin, 0);
    step();
};

void Motor::step_low(){
    gpio_put(d_pin, 1);
    step();
};

void Motor::step(){
    gpio_put(s_pin, 1);
    sleep_us(10);
    gpio_put(s_pin, 0);
    sleep_us(10);
}

void Motor::check(){
    if(steps > max * CALIBRATION_FACTOR)prepare_for_calibration();
}

void Motor::prepare_for_calibration(){
    if(max == 0)init_calibration();
    else{
        if(max - delta > delta)calibrate_high();
        else calibrate_low();
    }
}

#define JITTERBUFFER 4
#define RESETBUFFER 20

void Motor::init_calibration(){
    Push_working();
    //zoek de onderste sensor
    while(sensor_L.Check() == OPEN && sensor_H.Check() == OPEN){
        step_low();
    }
    for(int i = 0; i < JITTERBUFFER; i++)step_low();
    if(sensor_L.Check() == OPEN && sensor_H.Check() == OPEN){
        //er is jitter dus probeer opniew
        init_calibration();
        return;
    }

    //check welke sensor er daadwerkelijk geraakt is

    if(sensor_H.Check() == OPEN && sensor_L.Check() == CLOSED ){
        //sensoren staan al goed, doe niks
    }

    else if(sensor_H.Check() == CLOSED && sensor_L.Check() == OPEN){
        //zet de laage sensor daadwerkelijk laag
        Sensor T;
        T = sensor_L;
        sensor_L = sensor_H;
        sensor_H = T;
    }
    else if(sensor_L.Check() != CLOSED && sensor_H.Check() != CLOSED){
        //er is iets fout gegaan
        Push_error();
        sleep_ms(2000);
        for(int i = 0; i < RESETBUFFER; i++)step_high();
        init_calibration();
        return;
    }
    //check hoeveel steps er voor een rotatatie nodig zijn.
};

int Motor::calibrate_low(){
    //calibreer naar de lage sensor
}

int Motor::calibrate_high(){
    //calibreer naar de hoge sensor
}