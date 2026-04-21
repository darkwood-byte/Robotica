#ifndef ROBOT_H
#define ROBOT_H

#include "Motor.h"

class Robot {
public:
    int Pan;
    int Tilt;
    void Init();
private:
    Motor pan_motor;
    Motor tilt_motor;
};

#endif