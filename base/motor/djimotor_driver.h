//
// Created by ASUS on 2023/12/2.
//

#ifndef EC_HOMEWORKPLATFORM_DJIMOTOR_DRIVER_H
#define EC_HOMEWORKPLATFORM_DJIMOTOR_DRIVER_H

#include "motor.h"
#include "stdint.h"

class DjiMotorDriver{
public:
    enum CanIdRange {
        ID_1_4,
        ID_5_8,
        ID_9_11,
    };
    struct FeedbackData{
        int16_t ecd;            //encoder value(0-8191)
        int16_t rotate_speed;   //rpm
        int16_t torque_current;
        int8_t temp;            //temperture
    };
private:
    CAN_HandleTypeDef* hcan;

    //Transmit
    CAN_TxHeaderTypeDef tx_header;
    uint8_t transmit_data[8];
    uint32_t tx_mailbox;

    //Receive
    FeedbackData can1_fb_data[8];
    FeedbackData can2_fb_data[8];

public:
    //电机地址数组，在motor_monitor中定义
    Motor* can1_motor[8];
    Motor* can2_motor[8];

public:
    DjiMotorDriver(Motor* can1_motor_[], Motor* can2_motor_[]);
    bool isDjiMotorMessage(CAN_RxHeaderTypeDef* rx_header);
    void sendControlPackage(const uint8_t can_channel, const DjiMotorDriver::CanIdRange& can_id_range);
    void rxItHandle(CAN_HandleTypeDef* hcan_, CAN_RxHeaderTypeDef* rx_header_, uint8_t raw_data[8]);
    void handle();
    void init();
};

#endif //EC_HOMEWORKPLATFORM_DJIMOTOR_DRIVER_H
