#ifndef MOTOR_H
#define MOTOR_H

#include "pico/stdlib.h"
#include "Sensor.h"
#include "error.h"

#define CALC_FINISHED 0
#define CALC_RETURNING_TO_BASE 1
#define CALC_FINETUNING 2
#define CALC_COUNTING 3

#define CALIBRATE_FINISHED 0
#define CALIBRATE_HEADING_HOME 1
#define CALIBRATE_FINETUNING 2
class Motor {
public:
    bool Ready;
    int Goal;

    void Init(int dir_pin, int step_pin, Sensor sensor_low, Sensor sensor_high);
    void Use();
    void step_fall();   // <-- REQUIRED (ISR LOW pulse end)

private:
    Sensor sensor_L;
    Sensor sensor_H;

    volatile int s_pin;
    volatile int d_pin;

    volatile int steps;
    volatile int max;
    volatile int delta;

    volatile int calculating;
    volatile int calibrating;

    void calculating_func();
    // -------------------- STEP CONTROL --------------------
    bool pulse_active = false;

    void step();
  
    void step_high();
    void step_low();

    // -------------------- TIMER / ALARM SYSTEM --------------------
    void init_alarm_system();
    void register_motor();

    // -------------------- LOGIC --------------------
    void check();
    void prepare_for_calibration();
    void init_calibration();
    int calibrate_low();
    int calibrate_high();
};

#endif