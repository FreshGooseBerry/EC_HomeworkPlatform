//
// Created by ASUS on 2023/11/26.
//

#include "can_monitor.h"

void canFilterConfig(){
    CAN_FilterTypeDef sFilterConfig;
    //filter id range 0-13
    sFilterConfig.FilterBank=0;
    sFilterConfig.FilterMode=CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale=CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh=0x0000;
    sFilterConfig.FilterIdLow=0x0000;
    sFilterConfig.FilterMaskIdHigh=0x0000;
    sFilterConfig.FilterMaskIdLow=0x0000;
    sFilterConfig.FilterFIFOAssignment=CAN_RX_FIFO0;
    sFilterConfig.FilterActivation=ENABLE;
    sFilterConfig.SlaveStartFilterBank=14;
    if(HAL_CAN_ConfigFilter(&hcan1,&sFilterConfig)!=HAL_OK){
        Error_Handler();
    }

    //filter id range 14-27
    sFilterConfig.FilterBank=14;
    if(HAL_CAN_ConfigFilter(&hcan2,&sFilterConfig)!=HAL_OK){
        Error_Handler();
    }
}

void canInit(){
    HAL_CAN_Start(&hcan1);
    HAL_CAN_Start(&hcan2);
    HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
    HAL_CAN_ActivateNotification(&hcan2,CAN_IT_RX_FIFO0_MSG_PENDING);
    canFilterConfig();
}

bool isDjiMotorMessage(CAN_RxHeaderTypeDef* rx_header){
    if(info.type == M2006 || info.type == M3508){
        if(info.can_id_range == ID_1_4) {
            return (!(rx_header->StdId ^ 0x200))
        }
        if(info.can_id_range == ID_5_8) {
            return (!(rx_header->StdId ^ 0x1FF))
        }
        return false;
    }
    if(info.type == GM6020){
        if(info.can_id_range == ID_1_4) {
            return (!(rx_header->StdId ^ 0x1FF))
        }
        if(info.can_id_range == ID_5_8) {
            return (!(rx_header->StdId ^ 0x2FF))
        }
        return false;
    }
}
