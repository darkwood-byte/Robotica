#include "pico/stdlib.h"
#include "Motor.h"
#include <stdio.h>

// ------------------------------------------------------------------ Init ----
int Motor::GetRange() {
    return range;
}

void Motor::Init(int dir_pin, int step_pin, Sensor low, Sensor high) {
    d_pin       = dir_pin;
    s_pin       = step_pin;
    sensor_low  = low;
    sensor_high = high;

    sensor_low.init();
    sensor_high.init();

    gpio_init(d_pin); gpio_set_dir(d_pin, GPIO_OUT);
    gpio_init(s_pin); gpio_set_dir(s_pin, GPIO_OUT);

    position       = 0;
    range          = RANGE_NOT_DEFINED ;
    total_steps    = 0;
    debounce_count = 0;
    pulse_active   = false;
    goal           = 0;
    status         = Status::Error;

    while (sensor_low.Check() == SENSOR_ERROR || sensor_high.Check() == SENSOR_ERROR) {
        printf("sensor error, check wiring\n");
        sleep_ms(500);
    }

    status = Status::Calibrating;
    state  = &Motor::state_find_base;
}

// ------------------------------------------------------------------ Tick ----
void Motor::Tick() {
    if (pulse_active) {
        gpio_put(s_pin, false);
        pulse_active = false;
        return;
    }

    if (sensor_low.Check() == SENSOR_ERROR || sensor_high.Check() == SENSOR_ERROR) {
        status = Status::Error;
        return;
    }

    (this->*state)();
}

// --------------------------------------------------------- State machine ----
void Motor::state_find_base() {
    if (sensor_low.Check() == PRESSED) {
        debounce_count = 0;
        state = &Motor::state_leave_base;
        return;
    }
    step_low();
}

void Motor::state_leave_base() {
    if (sensor_low.Check() == OPEN) {
        if (++debounce_count >= DEBOUNCE_THRESHOLD) {
            debounce_count = 0;
            position       = 0;
            state          = &Motor::state_measure_range;
        }
        return;
    }
    debounce_count = 0;
    step_high();
}

void Motor::state_measure_range() {
    if (sensor_high.Check() == PRESSED) {
        range       = position;
        total_steps = 0;
        status      = Status::Calibrating;
        state       = &Motor::state_leave_base_after_calc;
        return;
    }
    step_high();
    position++;
}

void Motor::state_leave_base_after_calc() {
    if (sensor_high.Check() == OPEN) {
        if (++debounce_count >= DEBOUNCE_THRESHOLD) {
            debounce_count = 0;
            status      = Status::Running;
            state          = &Motor::state_run;
        }
        return;
    }
    debounce_count = 0;
    step_low();
}

void Motor::state_run() {
    if (position == goal) {
        status = Status::Ready;
        return;
    }

    status = Status::Running;

    if (position < goal) {
        if (sensor_high.Check() == PRESSED) {
            position = range;
            status   = Status::Running;
            return;
        }
        step_high();
        position++;
    } else {
        if (sensor_low.Check() == PRESSED) {
            position = 0;
            status   = Status::Running;
            return;
        }
        step_low();
        position--;
    }

    if (++total_steps > range * RECALIBRATE_AFTER_STEPS_FACTOR) {
        total_steps = 0;
        status      = Status::Calibrating;
        state       = &Motor::state_find_base;
    }
}


// ------------------------------------------------------------ Step helpers --
void Motor::step_high() { gpio_put(d_pin, true);  pulse(); }
void Motor::step_low()  { gpio_put(d_pin, false); pulse(); }

void Motor::pulse() {
    gpio_put(s_pin, true);
    pulse_active = true;
}