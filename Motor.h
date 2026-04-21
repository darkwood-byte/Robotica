#ifndef MOTOR_H
#define MOTOR_H

#include "pico/stdlib.h"
#include "Sensor.h"
#include "error.h"
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

    int s_pin;
    int d_pin;

    int steps;
    int max;
    int delta;

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