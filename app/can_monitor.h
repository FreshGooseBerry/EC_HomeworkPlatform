//
// Created by ASUS on 2023/11/26.
//

#ifndef EC_HOMEWORKPLATFORM_CAN_MONITOR_H
#define EC_HOMEWORKPLATFORM_CAN_MONITOR_H

#include "can.h"

void canFilterConfig();
void canInit();
bool isDjiMotorMessage(CAN_RxHeaderTypeDef* rx_header);

#endif //EC_HOMEWORKPLATFORM_CAN_MONITOR_H
