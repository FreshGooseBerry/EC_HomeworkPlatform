//
// Created by ASUS on 2023/11/14.
//
#include <stdint.h>
#include "tim.h"

#ifndef EC_LED_H
#define EC_LED_H

class BoardLed{
private:
    enum LedMode{
        OFF,
        ON,
        BREATH,
        BLINK
    } mode;

    uint8_t alpha, red, blue, green;
    uint32_t red_channel, blue_channel, green_channel;
    TIM_HandleTypeDef* led_htim = &htim5;

    uint16_t breath_period;
    uint16_t blink_on_time, blink_off_time;


public:
    BoardLed();

    void init();

    void setColor(uint8_t r, uint8_t b, uint8_t g);
    void setModeOn();
    void setModeOff();
    void setModeBreath(uint16_t period);
    void setModeBlink(uint16_t off_time,uint16_t on_time);

    void handle();

};

void LED_RGBa_Show(uint32_t aRGB){
    uint32_t alpha;
    uint32_t red, blue, green;

    alpha = ((aRGB & 0xFF000000) >> 24);
    red = ((aRGB & 0x00FF0000) >> 16) * alpha;
    blue = ((aRGB & 0x0000FF00) >> 8) * alpha;
    green = ((aRGB & 0x000000FF) >> 0) * alpha;

    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,blue);
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_2,green);
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_3,red);
}

void LED_Show(uint8_t red, uint8_t blue, uint8_t green){
    //from 0-255
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_1,blue);
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_2,green);
    __HAL_TIM_SET_COMPARE(&htim5,TIM_CHANNEL_3,red);
}

void LED_Breath(uint8_t red, uint8_t blue, uint8_t green){
    while(red>0){
        LED_Show(red, blue, green);
        HAL_Delay(1);
        --red;
        ++blue;
    }
    while(blue>0){
        LED_Show(red, blue, green);
        HAL_Delay(1);
        --blue;
        ++green;
    }
    while(green>0){
        LED_Show(red, blue, green);
        HAL_Delay(1);
        --green;
        ++red;
    }
}

#endif //EC_LED_H
