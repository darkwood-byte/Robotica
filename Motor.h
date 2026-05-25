#ifndef MOTOR_H
#define MOTOR_H

#include "pico/stdlib.h"
#include "Sensor.h"

#define RECALIBRATE_AFTER_STEPS_FACTOR 2
#define DEBOUNCE_THRESHOLD             4

class Motor {
public:
    enum class Status { Calibrating, Running, Ready, Error };

    volatile Status status = Status::Calibrating;
    volatile int    goal   = 0;

    void Init(int dir_pin, int step_pin, Sensor low, Sensor high);
    void Tick();
    void SetGoal(int g);
    int  GetRange();

private:
    int  d_pin         = 0;
    int  s_pin         = 0;
    int  position      = 0;
    int  range         = 0;
    int  total_steps   = 0;
    int  debounce_count = 0;
    bool pulse_active  = false;

    Sensor sensor_low;
    Sensor sensor_high;

    using State = void (Motor::*)();
    State state = nullptr;

    void state_find_base();
    void state_leave_base();
    void state_measure_range();
    void state_run();

    void step_high();
    void step_low();
    void pulse();
};

#endif
