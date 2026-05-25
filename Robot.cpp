#include "pico/stdlib.h"
#include "Robot.h"

// ------------------------------------------------------------------ Timer ---

bool Robot::timer_callback(struct repeating_timer *t) {
    return static_cast<Robot*>(t->user_data)->on_timer();
}

bool Robot::on_timer() {
    pan_motor.Tick();
    tilt_motor.Tick();
    update_leds();
    return true;
}

// ------------------------------------------------------------------- LEDs ---

void Robot::update_leds() {
    if (pan_motor.status == Motor::Status::Error ||
        tilt_motor.status == Motor::Status::Error) {
        Push_error();
        return;
    }
    if (pan_motor.status == Motor::Status::Calibrating ||
        tilt_motor.status == Motor::Status::Calibrating) {
        Push_working();
        return;
    }
    if (pan_motor.status == Motor::Status::Running ||
        tilt_motor.status == Motor::Status::Running) {
        Push_incoming();
        return;
    }
    Push_finished();
}

// ------------------------------------------------------------------- Init ---

void Robot::Init() {
    Init_Status_Leds();
    Push_working();

    Sensor pan_low;
    pan_low.AOF = PAN_SENSOR_LOW_AOF;
    pan_low.AON = PAN_SENSOR_LOW_AON;

    Sensor pan_high;
    pan_high.AOF = PAN_SENSOR_HIGH_AOF;
    pan_high.AON = PAN_SENSOR_HIGH_AON;

    Sensor tilt_low;
    tilt_low.AOF = TILT_SENSOR_LOW_AOF;
    tilt_low.AON = TILT_SENSOR_LOW_AON;

    Sensor tilt_high;
    tilt_high.AOF = TILT_SENSOR_HIGH_AOF;
    tilt_high.AON = TILT_SENSOR_HIGH_AON;

    pan_motor.Init (PAN_MOTOR_DIR,  PAN_MOTOR_STEP,  pan_low,  pan_high);
    tilt_motor.Init(TILT_MOTOR_DIR, TILT_MOTOR_STEP, tilt_low, tilt_high);

    add_repeating_timer_us(-MOTOR_TICK_US, timer_callback, this, &timer);
    printf("robot online\n");
}

// ------------------------------------------------------------------- Move ---


void Robot::Move(int pan_goal, int tilt_goal) {
    int clamped = (pan_goal < PAN_MIN_DEGREES)? PAN_MIN_DEGREES : (pan_goal > PAN_MAX_DEGREES)? PAN_MAX_DEGREES: pan_goal;
    pan_motor.goal = (pan_motor.GetRange() * clamped) / PAN_MAX_DEGREES;

    int tilt_clamped = (tilt_goal < TILT_MIN_DEGREES)? TILT_MIN_DEGREES: (tilt_goal > TILT_MAX_DEGREES)? TILT_MAX_DEGREES: tilt_goal;
    tilt_motor.goal = (tilt_motor.GetRange() * (tilt_clamped - TILT_MIN_DEGREES)) / (TILT_MAX_DEGREES - TILT_MIN_DEGREES);
}

bool Robot::IsReady() {
    return pan_motor.status == Motor::Status::Ready &&
           tilt_motor.status == Motor::Status::Ready;
}

const Motor& Robot::GetPanMotor()  const { return pan_motor;  }
const Motor& Robot::GetTiltMotor() const { return tilt_motor; }
