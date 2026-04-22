#ifndef SENSOR_H
#define SENSOR_H

#include <cstdint>

#define ERROR -1
#define OPEN 0
#define PRESED 1

class Sensor {
public:
    int Check();
    void init();

    int AON;
    int AOF;
};

#endif