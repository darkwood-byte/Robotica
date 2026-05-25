#ifndef SENSOR_H
#define SENSOR_H

#define SENSOR_ERROR  -1
#define OPEN           0
#define PRESSED        1

class Sensor {
public:
    int AON = 0;
    int AOF = 0;

    void init();
    int  Check();
};

#endif