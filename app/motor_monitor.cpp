//
// Created by ASUS on 2023/12/2.
//

#include "motor_monitor.h"

PidInitTypedef spid_init = {
    .kp = 1,
    .ki = 1,
    .kd = 0,
    .i_max = 10,
    .out_max = 184 //3508 电机输出电流上限，可以调小，勿调大
};

PidInitTypedef ppid_init = {
    .kp = 0,
    .ki = 0,
    .kd = 0,
    .i_max = 0,
    .out_max = 0
};

const PID spid(spid_init);
const PID ppid(ppid_init);

MotorInitTypeDef motor_init = {
    .method = Motor::SPEED,
    .info = {
        .type = Motor::M3508,
        .ratio = 19.2032f
    },
    .spid = spid,
    .ppid = ppid,
};

Motor test_motor(motor_init);

//为兼容GM6020，id本应为1-11，由于暂时用不到，所以id只有1-8
//can1控制的电机地址数组
//GM6020必须在id大于等于5处定义
Motor* can1MotorList[8] = {
    &test_motor,    //id 1
    nullptr,        //id 2
    nullptr,        //id 3
    nullptr,        //id 4
    nullptr,        //id 5
    nullptr,        //id 6
    nullptr,        //id 7
    nullptr,        //id 8
};

//can2控制的电机地址数组
//GM6020必须在id大于等于5处定义
Motor* can2MotorList[8] = {
    nullptr,        //id 1
    nullptr,        //id 2
    nullptr,        //id 3
    nullptr,        //id 4
    nullptr,        //id 5
    nullptr,        //id 6
    nullptr,        //id 7
    nullptr,        //id 8
};

DjiMotorDriver dji_motor_driver(can1MotorList,can2MotorList);
