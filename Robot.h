#ifndef ROBOT_H
#define ROBOT_H

#include "Motor.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"

// Sensor pin defines
#define PANSENSORLOWAOF  6
#define PANSENSORLOWAON  7
#define PANSENSORHIGHAOF 8
#define PANSENSORHIGHAON 9

#define TILTSENSORLOWAOF  10
#define TILTSENSORLOWAON  11
#define TILTSENSORHIGHAOF 12
#define TILTSENSORHIGHAON 13

// Motor pin defines
#define PANMOTORSTEP 2
#define PANMOTORDIR 3 

#define TILTMOTORSTEP 4
#define TILTMOTORDIR 5

class Robot {
public:
    int Pan = 0;
    int Tilt = 0;

    Robot() = default;
    void Init();

private:
    struct repeating_timer timer; // heartbeat timer
    Motor pan_motor;
    Motor tilt_motor;

    // Timer callback wrapper
    static inline bool timer_callback(struct repeating_timer *t);
    bool on_timer();
};

#endif // ROBOT_H