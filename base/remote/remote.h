//
// Created by ASUS on 2023/11/14.
//

#ifndef EC_HOMEWORKPLATFORM_REMOTE_H
#define EC_HOMEWORKPLATFORM_REMOTE_H

#include "usart.h"
#include "stdint.h"

#define RC_RX_BUF_SIZE 36u
#define RC_FRAME_LEN 18u

typedef uint16_t RCKey;

class RC{
private:
    UART_HandleTypeDef* huart_rc;

    //遥控器原始数据包 data frame of remote controller
    struct __packed RCRawData{
        int16_t ch[4];
        uint8_t s[2];
    }rawData;

    volatile uint8_t rx_len;
    uint8_t rx_buffer[RC_RX_BUF_SIZE];
    volatile uint8_t rx_data[RC_FRAME_LEN];      //这里用 unsigned 但是实际上数据有负

public:
    //遥控器拨档 remote switch
    enum RCSwitchState{
        UP,
        MID,
        DOWN
    };

    //摇杆
    struct __packed RCChannel{
        int16_t lr;    //left row
        int16_t lc;    //left col
        int16_t rr;    //right row
        int16_t rc;    //right col
    }rcChannel;
    //拨档
    struct __packed RCSwitch{
        RCSwitchState s1;   //left switch
        RCSwitchState s2;   //right switch
    }rcSwitch;
    //鼠标
    struct __packed Mouse{
        int16_t x; //position
        int16_t y;
        int16_t z;
        bool press_l;
        bool press_r;
    }mouse;
    //键盘按键
    RCKey rcKey;

public:
    RC(UART_HandleTypeDef* huart);
    void init();
    void reset();
    void handle();

    void rxCallback();
    void idleCallback();

    bool uartCheck(UART_HandleTypeDef* huart);

};

#endif //EC_HOMEWORKPLATFORM_REMOTE_H
