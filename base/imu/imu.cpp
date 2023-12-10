//
// Created by ASUS on 2023/12/8.
//

#include "imu.h"

IMU::IMU() {
    temp = 0;
    gyro_rate.x = 0;
    gyro_rate.y = 0;
    gyro_rate.z = 0;
    acceleration.x = 0;
    acceleration.y = 0;
    acceleration.z = 0;

    gyro_range = 0;
    accel_range = 0;

}

void IMU::BMI088_ACCEL_NS_L(void)
{
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_RESET);
}
void IMU::BMI088_ACCEL_NS_H(void)
{
    HAL_GPIO_WritePin(CS1_Accel_GPIO_Port, CS1_Accel_Pin, GPIO_PIN_SET);
}
void IMU::BMI088_GYRO_NS_L(void)
{
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_RESET);
}
void IMU::BMI088_GYRO_NS_H(void)
{
    HAL_GPIO_WritePin(CS1_Gyro_GPIO_Port, CS1_Gyro_Pin, GPIO_PIN_SET);
}

void IMU::reset() {
    uint8_t txData;

    HAL_Delay(1);

    /*accelerator soft reset*/
    BMI088_ACCEL_NS_L();

    txData = 0x7E;
    readWriteByte(txData);

    txData = 0xB6;
    readWriteByte(txData);

    HAL_Delay(1);//延迟1ms,初始化要求

    //set mode to NORMAL; 复位后默认是暂停模式，向0x7D写0x04改成正常模式
    txData = 0x7D;
    readWriteByte(txData);

    txData = 0x04;
    readWriteByte(txData);

    HAL_Delay(1);

    BMI088_ACCEL_NS_H();

    /*Gyro soft reset*/
    BMI088_GYRO_NS_L();

    txData = 0x14;
    readWriteByte(txData);

    txData = 0xB6;
    readWriteByte(txData);

    HAL_Delay(1);//延迟1ms,初始化要求

    BMI088_GYRO_NS_H();
}

void IMU::init() {
    reset();

    uint8_t txData;
    uint16_t counter;
/*
    BMI088_ACCEL_NS_L();

    txData = 0x7D;
    readWriteByte(txData);
    txData = 0x04;
    readWriteByte(txData);

    HAL_Delay(1);

    BMI088_ACCEL_NS_H();
*/
    //read range
    BMI088_GYRO_NS_L();
    txData = (0x0F | 0x80);
    gyro_range = readWriteByte(txData);
    BMI088_GYRO_NS_H();

    BMI088_ACCEL_NS_L();
    txData = (0x41 | 0x80);
    accel_range = readWriteByte(txData);
    BMI088_ACCEL_NS_H();

    gyro_unit = (float)gyro_range / (float)65536;
    accel_unit = (float)accel_range / (float)65536;
}

void IMU::handle() {
    __attribute__ ((aligned (4)))uint8_t rxData;
    __attribute__ ((aligned (4)))uint8_t txData;
    uint16_t counter = 0;

    //read gyro
    BMI088_GYRO_NS_L();


    txData = (0x02 | 0x80);//0x02 is RATE_X_LSB
    rawdata_gyro[0] = readWriteByte(txData);
    for(counter = 1; counter <= 6; ++counter){
        rawdata_gyro[counter] = readByte();
    }

    BMI088_GYRO_NS_H();

    //read accelerator
    BMI088_ACCEL_NS_L();

    txData = (0x12 | 0x80);
    readWriteByte(txData);
    for(counter = 0; counter < 6; ++counter){
        rawdata_gyro[counter] = readByte();
    }

    BMI088_ACCEL_NS_H();

    //handle data
    acceleration.x = (float)(((int16_t)rawdata_accel[1] << 8) | (int16_t)rawdata_accel[0]) * accel_unit;
    acceleration.y = (float)(((int16_t)rawdata_accel[3] << 8) | (int16_t)rawdata_accel[2]) * accel_unit;
    acceleration.z = (float)(((int16_t)rawdata_accel[5] << 8) | (int16_t)rawdata_accel[4]) * accel_unit;

    gyro_rate.x = (float)(((int16_t)rawdata_gyro[1] << 8) | (int16_t)rawdata_gyro[0]) * gyro_unit;
    gyro_rate.y = (float)(((int16_t)rawdata_gyro[3] << 8) | (int16_t)rawdata_gyro[2]) * gyro_unit;
    gyro_rate.z = (float)(((int16_t)rawdata_gyro[5] << 8) | (int16_t)rawdata_gyro[4]) * gyro_unit;
}

uint8_t IMU::readWriteByte(uint8_t txData) {
    __attribute__ ((aligned (4)))uint8_t pxData;
    uint16_t counter = 0;
    if(HAL_SPI_TransmitReceive(&hspi1,&txData,&pxData,1,1000)!=HAL_OK){
        Error_Handler();
    }

    while((HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_TX_RX) && counter < 50000){//等待传输完成
        ++counter;
    }
    if(counter == 50000){//超时
        Error_Handler();
    }

    return pxData;
}

uint8_t IMU::readByte() {
    uint8_t pxData;
    uint16_t counter = 0;
    HAL_SPI_Receive(&hspi1,&pxData,1,1000);

    while((HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_RX) && counter < 50000){//等待传输完成
        ++counter;
    }
    if(counter == 50000){//超时
        Error_Handler();
    }

    return pxData;
}

void IMU::BMI088_read_single_reg(uint8_t reg, uint8_t *return_data)
{
    readWriteByte(reg | 0x80);
    *return_data = readWriteByte(0x55);
}