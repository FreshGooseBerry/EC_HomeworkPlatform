//
// Created by ASUS on 2023/11/14.
//

#include "led.h"

TIM_HandleTypeDef* led_htim_ = &htim5;
const uint32_t red_channel_ = TIM_CHANNEL_3;
const uint32_t green_channel_ = TIM_CHANNEL_2;
const uint32_t blue_channel_ = TIM_CHANNEL_1;

BoardLed::BoardLed():led_htim(led_htim_), red_channel(red_channel_),
    blue_channel(blue_channel_), green_channel(green_channel_), red(0), blue(0), green(0) {};

void BoardLed::init() {
    HAL_TIM_PWM_Start(led_htim,red_channel);
    HAL_TIM_PWM_Start(led_htim,blue_channel);
    HAL_TIM_PWM_Start(led_htim,green_channel);
}

void BoardLed::setColor(uint8_t r, uint8_t b, uint8_t g){
    red = r;
    blue = b;
    green = g;
}

void BoardLed::setModeOn() {
    mode = ON;
}

void BoardLed::setModeOff() {
    mode = OFF;
}

void BoardLed::setModeBreath(uint16_t period) {
    breath_period = period;
}

void BoardLed::setModeBlink(uint16_t off_time, uint16_t on_time) {
    blink_on_time = on_time;
    blink_off_time = off_time;
}

void BoardLed::handle() {
    switch (mode) {
        case ON:{
            alpha = 255;
            break;
        }
        case OFF:{
            alpha = 0;
            break;
        }
        case BREATH:{

        }
    }

    uint16_t r_pwm, g_pwm, b_pwm;
    r_pwm = red * alpha;
    g_pwm = green * alpha;
    b_pwm = blue * alpha;
    __HAL_TIM_SetCompare(led_htim, red_channel, r_pwm);
    __HAL_TIM_SetCompare(led_htim, green_channel, g_pwm);
    __HAL_TIM_SetCompare(led_htim, blue_channel, b_pwm);
}

//Refer
void BoardLed::handle(void) {
    if (mode_ == OFF) {
        // 常暗
        a_ = 0;
    } else if (mode_ == ON) {
        // 常亮
        a_ = 255;
    } else if (mode_ == BREATH) {
        // 呼吸灯
        float t = math::loopLimit(HAL_GetTick(), 0, breath_period_);
        if (t < breath_period_ / 2) {
            a_ = (uint8_t)(t / breath_period_ * 2 * 255);
        } else {
            a_ = (uint8_t)(255 - t / breath_period_ * 2 * 255);
        }
    } else if (mode_ == BLINK) {
        // 闪烁
        if (HAL_GetTick() - blink_param_.period_start_tick > blink_param_.period) {
            blink_param_.period_start_tick = HAL_GetTick();
        } else if (HAL_GetTick() - blink_param_.period_start_tick >
                   2 * blink_param_.times * blink_param_.dt[0]) {
            a_ = 0;
        } else if ((HAL_GetTick() - blink_param_.period_start_tick) %
                       (2 * blink_param_.dt[0]) >
                   blink_param_.dt[0]) {
            a_ = 255;
        } else {
            a_ = 0;
        }
    }

    // 设置PWM占空比
    uint16_t r_pwm, g_pwm, b_pwm;
    r_pwm = r_ * a_;
    g_pwm = g_ * a_;
    b_pwm = b_ * a_;
    __HAL_TIM_SetCompare(htim_, r_ch_, r_pwm);
    __HAL_TIM_SetCompare(htim_, g_ch_, g_pwm);
    __HAL_TIM_SetCompare(htim_, b_ch_, b_pwm);
}