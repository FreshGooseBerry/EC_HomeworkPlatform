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

#endif //EC_LED_H
