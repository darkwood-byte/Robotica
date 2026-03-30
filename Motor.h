#ifndef MOTOR_H
#define MOTOR_H

#include "Sensor.h"

class Motor {
public:
    bool Ready;
    int Goal;
    void Init(int dir_pin, int step_pin, Sensor sensor_low, Sensor sensor_high);
    void Use();

private:
    Sensor sensor_L;
    Sensor sensor_H;
    int s_pin;
    int d_pin;
    int steps;
    int max;
    int delta;

    void step();
    void check();
    void prepare_for_calibration();
    void init_calibration();
    int calibrate_low();
    int calibrate_high();
    void step_high();
    void step_low();
};

#endif