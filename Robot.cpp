#include "Motor.h"
class Robot {
public:
    int Pan = 0;
    int Tilt = 0;
    void Init(){
         struct repeating_timer timer;
        add_repeating_timer_us(-500, robot_timer_callback, NULL, &timer);
    }
private:
    Motor pan_motor;
    Motor tilt_motor;
    void robot_timer_callback(void){

    }
};