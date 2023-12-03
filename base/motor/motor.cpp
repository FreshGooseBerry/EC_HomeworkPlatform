//
// Created by ASUS on 2023/11/26.
//

#include "motor.h"

Motor::Motor(const MotorInitTypeDef& motor_init):
    ppid(motor_init.ppid),spid(motor_init.spid),method(motor_init.method),
    info(motor_init.info){

    intensity = 0;
    target_angle = 0;
    target_speed = 0;

    motor_data = (MotorData){.angle = 0.0,.ecd_angle = 0.0, .last_ecd_angle = 0.0,.rotate_speed = 0.0, .current = 0.0, .temp = 0.0};
    mode = STOP;
}

void Motor::reset() {
    motor_data = (MotorData){0.0,0.0,0.0,0.0,0.0,0.0};
    mode = STOP;
    intensity = 0;
    target_angle = 0;
    target_speed = 0;
}

void Motor::handle(){
    //calculateIntensity
    if(mode == WORKING)
    switch (method) {
        case POSITION_SPEED: {
            target_speed = ppid.calculate(target_angle,motor_data.angle);
            intensity = spid.calculate(target_speed, motor_data.rotate_speed);
            break;
        }
        case SPEED:{
            target_angle = motor_data.angle;
            intensity = spid.calculate(target_speed, motor_data.rotate_speed);
            break;
        }
    }
    else if(mode == STOP){
        intensity = 0;
        target_speed = 0;
    }
    else if(mode == POWER_OFF){
        intensity = 0;
        target_speed = 0;
        target_angle = 0;
    }
}

void Motor::setAngle(const float& target_angle_) {
    target_angle = target_angle_;
}

void Motor::setSpeed(const float& target_speed_){
    target_speed = target_speed_;
}
void Motor::setMode(const Motor::Mode& mode_) {
    mode = mode_;
}
