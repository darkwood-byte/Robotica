#ifndef ROBOT_H
#define ROBOT_H

#include "pico/stdlib.h"
#include "Motor.h"
#include "error.h"
#include <stdio.h>

#define PAN_SENSOR_HIGH_AOF   7
#define PAN_SENSOR_HIGH_AON   6
#define PAN_SENSOR_LOW_AOF  9
#define PAN_SENSOR_LOW_AON  8

#define TILT_SENSOR_LOW_AOF  12
#define TILT_SENSOR_LOW_AON  13
#define TILT_SENSOR_HIGH_AOF 10
#define TILT_SENSOR_HIGH_AON 11

#define PAN_MOTOR_STEP   2
#define PAN_MOTOR_DIR    3
#define TILT_MOTOR_STEP  4
#define TILT_MOTOR_DIR   5

#define MOTOR_TICK_US 1200

class Robot {
public:
    void  Init();
    void  Move(int pan_goal, int tilt_goal);
    bool  IsReady();

    const Motor& GetPanMotor()  const;
    const Motor& GetTiltMotor() const;

private:
    Motor pan_motor;
    Motor tilt_motor;

    struct repeating_timer timer;

    static bool timer_callback(struct repeating_timer *t);
    bool on_timer();
    void update_leds();
};

#endif