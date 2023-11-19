//
// Created by ASUS on 2023/11/15.
//

#include "control.h"
#include "iwdg.h"

#include "../base/board/led.h"
#include "../base/common/math.h"
#include "../base/remote/remote.h"

extern RC rc;

BoardLed led;

void iwdgHandler(bool iwdg_refresh_flag){
    if(!iwdg_refresh_flag){

    }
    else{
        HAL_IWDG_Refresh(&hiwdg);
    }
}

void controlInit(){
    led.init();
}

void controlLoop(){
    iwdgHandler(true);
    if(rc.rcSwitch.s1 == RC::UP){
        led.setColor(200,0,0);
    }
    led.handle();
}