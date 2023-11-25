//
// Created by ASUS on 2023/11/14.
//

#include "remote.h"
#include <string.h>

const uint16_t rc_ch_offset = 1024;

RC::RC(UART_HandleTypeDef* huart):huart_rc(huart){
    rcSwitch.s1 = DOWN;
    rcSwitch.s2 = DOWN;
    rx_len = 0;
};

void RC::reset() {
    rawData.ch[0] = 0;
    rawData.ch[1] = 0;
    rawData.ch[2] = 0;
    rawData.ch[3] = 0;
    rawData.s[0] = 0;
    rawData.s[1] = 0;

    rcChannel.lc = 0;
    rcChannel.lr = 0;
    rcChannel.rc = 0;
    rcChannel.rr = 0;

    mouse.x = 0;
    mouse.y = 0;
    mouse.z = 0;
    mouse.press_l = false;
    mouse.press_r = false;

    rcSwitch.s1 = DOWN;
    rcSwitch.s1 = DOWN;

    rcKey = 0;
}

void RC::init() {
    rx_len = 0;
    reset();
    if(huart_rc != nullptr) {
        __HAL_UART_ENABLE_IT(huart_rc,UART_IT_IDLE);
        HAL_UART_Receive_DMA(huart_rc,rx_buffer,1);
    }
}

void RC::handle() {
    //unpacked raw data
    rawData.ch[0] = ((int16_t)rx_data[0] | ((int16_t)rx_data[1] << 8)) & 0x07FF;          //rr
    rawData.ch[1] = (((int16_t)rx_data[1] >> 3) | ((int16_t)rx_data[2] << 5)) & 0x07FF;   //rc
    rawData.ch[2] = (((int16_t)rx_data[2] >> 6) | ((int16_t)rx_data[3] << 2)              //lr
                     | ((int16_t)rx_data[4] << 10)) & 0x07FF;
    rawData.ch[3] = (((int16_t)rx_data[4] >> 1) | ((int16_t)rx_data[5] << 7)) & 0x07FF;   //lc

    rawData.s[0] = ((rx_data[5] >> 4) & 0x000C) >> 2;   //switch left
    rawData.s[1] = (rx_data[5] >> 4) & 0x0003;          //switch right

    mouse.x = (int16_t)rx_data[6] | ((int16_t)rx_data[7] << 8);
    mouse.y = (int16_t)rx_data[8] | ((int16_t)rx_data[9] << 8);
    mouse.y = (int16_t)rx_data[10] | ((int16_t)rx_data[11] << 8);
    mouse.press_l = (rx_data[12] != 0);
    mouse.press_r = (rx_data[13] != 0);

    rcKey = (uint16_t)rx_data[14] | ((uint16_t)rx_data[15] << 8);

    //process raw data
    //channel
    rcChannel.rr = rawData.ch[0] - rc_ch_offset;
    rcChannel.rc = rawData.ch[1] - rc_ch_offset;
    rcChannel.lr = rawData.ch[2] - rc_ch_offset;
    rcChannel.lc = rawData.ch[3] - rc_ch_offset;
    //s1
    if(rawData.s[0] == 1){
        rcSwitch.s1 = UP;
    }
    else if(rawData.s[0] == 2){
        rcSwitch.s1 = MID;
    }
    else if(rawData.s[0] == 3){
        rcSwitch.s1 = DOWN;
    }
    //s2
    if(rawData.s[1] == 1){
        rcSwitch.s2 = UP;
    }
    else if(rawData.s[1] == 2){
        rcSwitch.s2 = MID;
    }
    else if(rawData.s[1] == 3){
        rcSwitch.s2 = DOWN;
    }
}

//UART接受中断
void RC::rxCallback() {
    ++rx_len;
    if(huart_rc != nullptr){
        HAL_UART_Receive_DMA(huart_rc,rx_buffer + rx_len,1); //why?
    }
}

//UART空闲中断，意味着一次传输已完成
void RC::idleCallback() {
    if(rx_len >=RC_FRAME_LEN){
        memcpy((uint8_t*) rx_data, rx_buffer + rx_len - RC_FRAME_LEN, RC_FRAME_LEN); //这里不太明白为什么不直接等
    }
    rx_len = 0;
    if(huart_rc != nullptr){
        HAL_UART_AbortReceive(huart_rc);    //why?
        HAL_UART_Receive_DMA(huart_rc,rx_buffer,1);
    }
}

bool RC::uartCheck(UART_HandleTypeDef* huart){
    if(huart == huart_rc){
        return true;
    }
    return false;
}