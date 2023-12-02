//
// Created by ASUS on 2023/11/26.
//

#include "motor.h"

Motor::Motor(const MotorType& type_, const float& ratio_, const ControlMethod& method_,
      const PID& ppid_, const PID& spid_,const uint8_t& id_):ppid(ppid_),spid(spid_),method(method_){
    info.ratio = ratio_;
    info.type = type_;
    info.id = id_;
    if(id_ <= 4 && id_ >= 1){
        info.can_id_range = ID_1_4;
    }
    else if(id_ <= 8 && id_ >= 5){
        info.can_id_range = ID_5_8;
    }
    else if(id_ <= 11 && id_ >= 9){
        info.can_id_range = ID_5_8;
    }

    intensity = 0;
    target_angle = 0;
    target_speed = 0;

    motor_data = {0.0,0.0,0.0,0.0,0.0,0.0};
    mode = STOP;
}

void Motor::reset() {
    motor_data = {0.0,0.0,0.0,0.0,0.0,0.0};
    mode = STOP;
    intensity = 0;
    target_angle = 0;
    target_speed = 0;
}

void Motor::handle(){

}

void Motor::setAngle(const float& target_angle_) {
    target_angle = target_angle_;
}

void Motor::setSpeed(const float& target_speed_){
    target_speed = target_speed_;
}

bool Motor::isRightMessage(CAN_RxHeaderTypeDef* rx_header){
    if(info.type == M2006 || info.type == M3508){
        if(info.can_id_range == ID_1_4) {
            return (!(rx_header->StdId ^ 0x200))
        }
        if(info.can_id_range == ID_5_8) {
            return (!(rx_header->StdId ^ 0x1FF))
        }
        return false;
    }
    if(info.type == GM6020){
        if(info.can_id_range == ID_1_4) {
            return (!(rx_header->StdId ^ 0x1FF))
        }
        if(info.can_id_range == ID_5_8) {
            return (!(rx_header->StdId ^ 0x2FF))
        }
        return false;
    }
}

void Motor::CanMessageUnpack(CAN_HandleTypeDef* hcan,CAN_RxHeaderTypeDef* rx_header,uint8_t* rx_data) {

}
