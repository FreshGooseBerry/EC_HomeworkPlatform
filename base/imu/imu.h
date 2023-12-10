//
// Created by ASUS on 2023/12/8.
//

#ifndef EC_HOMEWORKPLATFORM_IMU_H
#define EC_HOMEWORKPLATFORM_IMU_H

#include "stdint.h"
#include "spi.h"

class IMU{
public:
    struct GyroRate{
        float x;
        float y;
        float z;
    };
    struct Acceleration{
        float x;
        float y;
        float z;
    };

public:
    float temp;
    GyroRate gyro_rate;
    Acceleration acceleration;

    uint8_t gyro_range;
    uint8_t accel_range;

    float gyro_unit;
    float accel_unit;

    __attribute__ ((aligned (4)))int8_t rawdata_accel[6];
    __attribute__ ((aligned (4)))int8_t rawdata_gyro[6];

public:
    IMU();
    void init();
    void handle();
    void reset();
    //NSS select slave
    void BMI088_ACCEL_NS_L();
    void BMI088_ACCEL_NS_H();
    void BMI088_GYRO_NS_L();
    void BMI088_GYRO_NS_H();

    uint8_t readWriteByte(uint8_t txData);
    uint8_t readByte();
    void BMI088_read_single_reg(uint8_t reg, uint8_t *return_data);
};

#endif //EC_HOMEWORKPLATFORM_IMU_H
