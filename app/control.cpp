//
// Created by ASUS on 2023/11/15.
//

#include "control.h"
//#include "iwdg.h"

#include "../base/board/led.h"
#include "../base/common/math.h"
#include "../base/remote/remote.h"

#include "motor_monitor.h"
#include "can_monitor.h"

extern RC rc;

BoardLed led;
/*
void iwdgHandler(bool iwdg_refresh_flag){
    if(!iwdg_refresh_flag){

    }
    else{
        HAL_IWDG_Refresh(&hiwdg);
    }
}
*/

void controlInit(){
    led.init();
    led.setColor(0,0,255);
    led.setModeOn();

    dji_motor_driver.init();
}

void controlLoop(){
    //iwdgHandler(true);
    //if(rc.rcSwitch.s2 == RC::UP){
    //    led.setColor(0,255,0);
    //}
    led.handle();
    dji_motor_driver.handle();
    dji_motor_driver.can1_motor[0]->setSpeed(rc.rcChannel.rc);
    canSendPackage();
}