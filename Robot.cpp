#include "Robot.h"

// Timer callback wrapper implementation
inline bool Robot::timer_callback(struct repeating_timer *t) {
    Robot* self = static_cast<Robot*>(t->user_data);
    return self->on_timer();
}

bool Robot::on_timer() {
    pan_motor.Use();
    tilt_motor.Use();
    if(pan_motor.Ready && tilt_motor.Ready){
        Push_finished();
        return false;
    } 
    return true;
}

void Robot::Init() {
    Init_Status_Leds();
    Push_working();
    
    // Pan motor en sensor init
    Sensor pansensorlow;
    pansensorlow.AOF = PANSENSORLOWAOF;
    pansensorlow.AON = PANSENSORLOWAON;

    Sensor pansensorhigh;
    pansensorhigh.AOF = PANSENSORHIGHAOF;
    pansensorhigh.AON = PANSENSORHIGHAON;

    pan_motor.Init(PANMOTORDIR, PANMOTORSTEP, pansensorlow, pansensorhigh);

    // Tilt motor en sensor init
    Sensor tiltsensorlow;
    tiltsensorlow.AOF = TILTSENSORLOWAOF;
    tiltsensorlow.AON = TILTSENSORLOWAON;

    Sensor tiltsensorhigh;
    tiltsensorhigh.AOF = TILTSENSORHIGHAOF;
    tiltsensorhigh.AON = TILTSENSORHIGHAON;

    tilt_motor.Init(TILTMOTORDIR, TILTMOTORSTEP, tiltsensorlow, tiltsensorhigh);
 
    sleep_ms(1000);
    Push_finished();
    sleep_ms(500);
    
    add_repeating_timer_us(-500, timer_callback, this, &timer);
}