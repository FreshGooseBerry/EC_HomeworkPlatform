//
// Created by ASUS on 2023/12/2.
//

#include "djimotor_driver.h"
#include "..\common\math.h"

DjiMotorDriver::DjiMotorDriver(Motor* can1_motor_[], Motor* can2_motor_[]) {
    for(uint8_t i = 0; i < 8; ++i){
        can1_motor[i] = can1_motor_[i];
        can2_motor[i] = can2_motor_[i];
    }
}

/**
 * @brief send control package to motor controller
 * @param can_channel 1 or 2
 * @param can_id_range 1-4 or 5-8 or 9-11(unused)
 * @attention be called in can_monitor.cpp
 */
void DjiMotorDriver::sendControlPackage(const uint8_t can_channel, const DjiMotorDriver::CanIdRange& can_id_range){
    tx_header.IDE=CAN_ID_STD;
    tx_header.RTR=CAN_RTR_DATA;
    tx_header.DLC=8;
    tx_header.TransmitGlobalTime=DISABLE;
    if(can_channel == 1){
        hcan = &hcan1;
    }
    else if(can_channel == 2){
        hcan = &hcan2;
    }
    else{
        //channel id Error
        Error_Handler();
    }
    switch (can_id_range) {
        case ID_1_4:{
            //M3508 or M2006
            tx_header.StdId = 0x200;
            transmit_data[0] = (int8_t )((can1_motor[0]->intensity) >> 8);
            transmit_data[1] = (int8_t)((can1_motor[0]->intensity) & 0x00FF);
            transmit_data[2] = (can1_motor[1]->intensity) >> 8;
            transmit_data[3] = (int8_t)((can1_motor[1]->intensity) & 0x00FF);
            transmit_data[4] = (can1_motor[2]->intensity) >> 8;
            transmit_data[5] = (int8_t)((can1_motor[2]->intensity) & 0x00FF);
            transmit_data[6] = (can1_motor[3]->intensity) >> 8;
            transmit_data[7] = (int8_t)((can1_motor[3]->intensity) & 0x00FF);
            break ;
        }
        case ID_5_8:{
            //M3508 or M2006 -from 5 to 8 | GM6020 -from 1 to 4
            tx_header.StdId = 0x1FF;
            transmit_data[0] = (can1_motor[4]->intensity) >> 8;
            transmit_data[1] = (int8_t)((can1_motor[4]->intensity) & 0x00FF);
            transmit_data[2] = (can1_motor[5]->intensity) >> 8;
            transmit_data[3] = (int8_t)((can1_motor[5]->intensity) & 0x00FF);
            transmit_data[4] = (can1_motor[6]->intensity) >> 8;
            transmit_data[5] = (int8_t)((can1_motor[6]->intensity) & 0x00FF);
            transmit_data[6] = (can1_motor[7]->intensity) >> 8;
            transmit_data[7] = (int8_t)((can1_motor[7]->intensity) & 0x00FF);
            break ;
        }
        case ID_9_11:{
            //GM6020 5-7
            /*
                tx_header.StdId = 0x2FF;
                transmit_data[0] = (can1_motor[8]->intensity) >> 8;
                transmit_data[1] = (int8_t)((can1_motor[8]->intensity) & 0x00FF);
                transmit_data[2] = (can1_motor[9]->intensity) >> 8;
                transmit_data[3] = (int8_t)((can1_motor[9]->intensity) & 0x00FF);
                transmit_data[4] = (can1_motor[10]->intensity) >> 8;
                transmit_data[5] = (int8_t)((can1_motor[10]->intensity) & 0x00FF);
                transmit_data[6] = 0;
                transmit_data[7] = 0;
                break ;
                 */
        }
    }
    HAL_CAN_AddTxMessage(hcan, &tx_header, transmit_data, &tx_mailbox);
}

bool DjiMotorDriver::isDjiMotorMessage(CAN_RxHeaderTypeDef* rx_header){
    return (rx_header->StdId >= 0x200 && rx_header->StdId <= 0x20b);
}

void DjiMotorDriver::rxItHandle(CAN_HandleTypeDef* hcan_, CAN_RxHeaderTypeDef* rx_header_, uint8_t raw_data[8]){
    uint8_t id = rx_header_->StdId - 0x200;
    Motor* motor;
    FeedbackData rawData;
    if(hcan_ == &hcan1){
        //注意减1
        rawData = can1_fb_data[id - 1];
        motor = can1_motor[id - 1];
    }
    else{
        rawData = can2_fb_data[id - 1];
        motor = can2_motor[id - 1];
    }
    rawData.ecd = ((uint16_t)raw_data[0] << 8) | (uint16_t)raw_data[1];
    rawData.rotate_speed = ((uint16_t)raw_data[2] << 8) | (uint16_t)raw_data[3];
    rawData.torque_current = ((uint16_t)raw_data[4] << 8) | (uint16_t)raw_data[5];
    rawData.temp = raw_data[6];//C610不返回温度，但依然兼容

    //电机数据更新
    //angle
    motor->motor_data.last_ecd_angle = motor->motor_data.ecd_angle;
    motor->motor_data.ecd_angle = rawData.ecd;
    float delta = math::ecd2deg(rawData.ecd - motor->motor_data.last_ecd_angle, 8192);//8192对于360度
    delta = math::degNormalize180(delta) / motor->info.ratio;
    motor->motor_data.angle = motor->motor_data.angle + delta;

    //rotate speed
    motor->motor_data.rotate_speed = math::rpm2radps(rawData.rotate_speed);
    //torge current
    motor->motor_data.current = rawData.torque_current;
    //temperture
    motor->motor_data.temp = rawData.temp;
}

void DjiMotorDriver::handle() {
    for(uint8_t i = 0; i < 8; ++i){
        if(can1_motor[i] != nullptr) {
            can1_motor[i]->handle();
        }
        if(can2_motor[i] != nullptr) {
            can2_motor[i]->handle();
        }
    }

}

void DjiMotorDriver::init() {
    for(uint8_t i = 0; i < 8; ++i) {
        if(can1_motor[i] != nullptr) {
            can1_motor[i]->reset();
        }
        if(can2_motor[i] != nullptr) {
            can2_motor[i]->reset();
        }
    }
    can1_motor[0]->setMode(Motor::WORKING);
}
