#ifndef SENSOR_H
#define SENSOR_H

#include <cstdint>

#define ERROR -1
#define CLOSED 0
#define OPEN 1

class Sensor {
public:
    int Check();
    void init();

    int AON;
    int AOF;
};

#endif