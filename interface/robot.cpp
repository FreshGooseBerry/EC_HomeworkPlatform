//
// Created by ASUS on 2023/11/18.
//

#include "robot.h"

#include "../app/control.h"
#include "../base/remote/remote.h"
#include "../hardware_config.h"

//遥控器
//remote control
RC rc(RC_UART);

//机器人总体初始化，在main.c中调用
//Initialize the robot, and be used in main.c
void robotInit() {
    //控制相关初始化
    controlInit();
    //遥控器初始化
    rc.init();
}

//机器人总控制循环，在TIM中断中以1000Hz调用
//Main control loop used in tim callback
void robotControlLoop() {
    controlLoop();
}

