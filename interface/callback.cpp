//
// Created by ASUS on 2023/11/18.
//

#include "callback.h"
//#include "../app/control.h"

#include "usart.h"
#include "tim.h"

#include "../base/remote/remote.h"

extern RC rc;

//UART Transmit callback
void HAL_UART_TxCpltCallback(UART_HandleTypeDef* huart) {}
/*
//UART Receive callback
void HAL_UART_RxCpltCallback(UART_HandleTypeDef* huart){
    if(rc.uartCheck(huart)){
        rc.rxCallback();
    }
}
*/
/*
//自定义空闲中断处理 UART idle callback
//在stm32f4xx_it.c中的USART3_IRQHandler函数里调用 called in USART3_IRQHandler() in stm32f4xx_it.c
void User_UART_IdleHandler(UART_HandleTypeDef* huart){
    //判断空闲中断是否使能
    //judge if idle enabled
    if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET){
        //清除空闲标志（需调用函数清除）
        //clear idle flag
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        if(rc.uartCheck(huart)){
            rc.idleCallback();
        }
    }
}
*/
//TIM Period callback
//以1000Hz频率触发调用主循环
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
    if (htim->Instance == htim6.Instance) {
        // tim6触发的中断
        HAL_NVIC_DisableIRQ(TIM6_DAC_IRQn);
        robotControlLoop();
        HAL_NVIC_EnableIRQ(TIM6_DAC_IRQn);
    }
    if(htim->Instance == htim7.Instance) {
        // tim7触发的中断
        rc.handle();
    }
}

//IDLE callback
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size){
    if(huart == rc.huart_rc){
        rc.handle();
        HAL_UARTEx_ReceiveToIdle_DMA(&huart3,rc.rx_buffer,RC_RX_BUF_SIZE);
    }
}