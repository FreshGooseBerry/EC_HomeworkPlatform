//
// Created by ASUS on 2023/11/18.
//

#ifndef EC_HOMEWORKPLATFORM_CALLBACK_H
#define EC_HOMEWORKPLATFORM_CALLBACK_H

#ifdef __cplusplus
extern "C"{
#endif

#include "usart.h"

void User_UART_IdleHandler(UART_HandleTypeDef* huart);



#ifdef __cplusplus
};
#endif


#endif //EC_HOMEWORKPLATFORM_CALLBACK_H
