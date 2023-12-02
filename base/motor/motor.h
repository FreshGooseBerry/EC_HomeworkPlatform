//
// Created by ASUS on 2023/11/26.
//

#ifndef EC_HOMEWORKPLATFORM_MOTOR_H
#define EC_HOMEWORKPLATFORM_MOTOR_H

#include "pid.h"
#include "stdint.h"
#include "can.h"

class Motor {
public:
    enum CanIdRange {
        ID_1_4,
        ID_5_8,
        ID_9_11,
    };
    enum MotorType {
        M3508,  //C620
        M2006,  //C610
        GM6020,
    };
    enum ControlMethod {
        POSITION_SPEED, // 位置，速度双环 PID 控制
        SPEED,          // 速度单环 PID 控制
    };
    enum Mode {
        POWER_OFF,      // 断电，控制量置零
        STOP,           // 将目标速度置零，计算得出控制量，使得电机停转
        WORKING,        // 电机正常工作
    };

    struct MotorData {
        float angle;            // 减速后的输出端角度
        float ecd_angle;        // 编码器角度
        float last_ecd_angle;
        float rotate_speed;     // 减速后的输出端转速
        float current;          // 转矩电流
        float temp;             // 电机温度
    };
    struct MotorInfo {
        MotorType type;
        float ratio;// 减速比
        CanIdRange can_id_range; //ID范围
        uint8_t id; //电机ID
    };

public:
    MotorInfo  info;
    ControlMethod method;
    Mode mode;

    int16_t intensity;                 // 控制量
    float target_angle, target_speed;   // 期望角度、速度
    MotorData  motor_data;
    PID ppid, spid;

public:
    Motor(const MotorType& type_, const float& ratio_, const ControlMethod& method_,
          const PID& ppid_, const PID& spid_, const uint8_t& id_);
    void reset(void);       // 重置电机所有状态
    void handle(void);      // 根据当前 mode_ 计算控制量
    void setAngle(const float& target_angle_); // 设置目标角度
    void setSpeed(const float& target_speed_); // 设置目标速度

    bool isRightMessage(CAN_RxHeaderTypeDef* rx_header);
    void CanMessageUnpack(CAN_HandleTypeDef* hcan, CAN_RxHeaderTypeDef* rx_header, uint8_t* rx_data);
    void CanMessageTransmit(CAN_HandleTypeDef* hcan, CanIdRange id_range);

};

#endif //EC_HOMEWORKPLATFORM_MOTOR_H
