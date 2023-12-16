//
// Created by ASUS on 2023/12/8.
//

#ifndef EC_HOMEWORKPLATFORM_IMU_H
#define EC_HOMEWORKPLATFORM_IMU_H

#include "stdint.h"
#include "spi.h"
#include "BMI088reg.h"

#define BMI088_ACC_SOFTRESET_ADDR 0x7E
#define BMI088_ACC_PWR_CTRL_ADDR 0x7D
#define BMI088_ACC_X_LSB_ADDR 0X12
#define BMI088_ACC_X_MSB_ADDR 0X13
#define BMI088_ACC_Y_LSB_ADDR 0x14
#define BMI088_ACC_Y_MSB_ADDR 0x15
#define BMI088_ACC_Z_LSB_ADDR 0x16
#define BMI088_ACC_Z_MSB_ADDR 0x17
#define BMI088_ACC_RANGE_ADDR 0x41

#define BMI088_GYRO_SOFTRESET 0x14
#define BMI088_GYRO_RANGE 0x0F
#define BMI088_RATE_X_LSB_ADDR 0x02
#define BMI088_RATE_X_MSB_ADDR 0x03
#define BMI088_RATE_Y_LSB_ADDR 0x04
#define BMI088_RATE_Y_MSB_ADDR 0x05
#define BMI088_RATE_Z_LSB_ADDR 0x06
#define BMI088_RATE_Z_MSB_ADDR 0x07

#define BMI088_TEMP_LSB 0x23
#define BMI088_TEMP_MSB 0x22

#define BMI088_ACC_NORMAL_MODE 0x04
#define BMI088_ENTER_SOFTRESET 0xB6

#define BMI088_TEMP_FACTOR 0.125f
#define BMI088_TEMP_OFFSET 23.0f

#define BMI088_WRITE_ACCEL_REG_NUM 6
#define BMI088_WRITE_GYRO_REG_NUM 6

#define BMI088_GYRO_DATA_READY_BIT 0
#define BMI088_ACCEL_DATA_READY_BIT 1
#define BMI088_ACCEL_TEMP_DATA_READY_BIT 2

#define BMI088_LONG_DELAY_TIME 80
#define BMI088_COM_WAIT_SENSOR_TIME 150


#define BMI088_ACCEL_IIC_ADDRESSE (0x18 << 1)
#define BMI088_GYRO_IIC_ADDRESSE (0x68 << 1)

#define BMI088_ACCEL_RANGE_3G
//#define BMI088_ACCEL_RANGE_6G
//#define BMI088_ACCEL_RANGE_12G
//#define BMI088_ACCEL_RANGE_24G

#define BMI088_GYRO_RANGE_2000
//#define BMI088_GYRO_RANGE_1000
//#define BMI088_GYRO_RANGE_500
//#define BMI088_GYRO_RANGE_250
//#define BMI088_GYRO_RANGE_125


#define BMI088_ACCEL_3G_SEN 0.0008974358974f
#define BMI088_ACCEL_6G_SEN 0.00179443359375f
#define BMI088_ACCEL_12G_SEN 0.0035888671875f
#define BMI088_ACCEL_24G_SEN 0.007177734375f


#define BMI088_GYRO_2000_SEN 0.00106526443603169529841533860381f
#define BMI088_GYRO_1000_SEN 0.00053263221801584764920766930190693f
#define BMI088_GYRO_500_SEN 0.00026631610900792382460383465095346f
#define BMI088_GYRO_250_SEN 0.00013315805450396191230191732547673f
#define BMI088_GYRO_125_SEN 0.000066579027251980956150958662738366f

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
    enum
    {
        BMI088_NO_ERROR = 0x00,
        BMI088_ACC_PWR_CTRL_ERROR = 0x01,
        BMI088_ACC_PWR_CONF_ERROR = 0x02,
        BMI088_ACC_CONF_ERROR = 0x03,
        BMI088_ACC_SELF_TEST_ERROR = 0x04,
        BMI088_ACC_RANGE_ERROR = 0x05,
        BMI088_INT1_IO_CTRL_ERROR = 0x06,
        BMI088_INT_MAP_DATA_ERROR = 0x07,
        BMI088_GYRO_RANGE_ERROR = 0x08,
        BMI088_GYRO_BANDWIDTH_ERROR = 0x09,
        BMI088_GYRO_LPM1_ERROR = 0x0A,
        BMI088_GYRO_CTRL_ERROR = 0x0B,
        BMI088_GYRO_INT3_INT4_IO_CONF_ERROR = 0x0C,
        BMI088_GYRO_INT3_INT4_IO_MAP_ERROR = 0x0D,

        BMI088_SELF_TEST_ACCEL_ERROR = 0x80,
        BMI088_SELF_TEST_GYRO_ERROR = 0x40,
        BMI088_NO_SENSOR = 0xFF,
    };
    uint8_t write_BMI088_accel_reg_data_error[BMI088_WRITE_ACCEL_REG_NUM][3] =
        {
            {BMI088_ACC_PWR_CTRL, BMI088_ACC_ENABLE_ACC_ON, BMI088_ACC_PWR_CTRL_ERROR},
            {BMI088_ACC_PWR_CONF, BMI088_ACC_PWR_ACTIVE_MODE, BMI088_ACC_PWR_CONF_ERROR},
            {BMI088_ACC_CONF,  BMI088_ACC_NORMAL| BMI088_ACC_800_HZ | BMI088_ACC_CONF_MUST_Set, BMI088_ACC_CONF_ERROR},
            {BMI088_ACC_RANGE, BMI088_ACC_RANGE_3G, BMI088_ACC_RANGE_ERROR},
            {BMI088_INT1_IO_CTRL, BMI088_ACC_INT1_IO_ENABLE | BMI088_ACC_INT1_GPIO_PP | BMI088_ACC_INT1_GPIO_LOW, BMI088_INT1_IO_CTRL_ERROR},
            {BMI088_INT_MAP_DATA, BMI088_ACC_INT1_DRDY_INTERRUPT, BMI088_INT_MAP_DATA_ERROR}

        };

    uint8_t write_BMI088_gyro_reg_data_error[BMI088_WRITE_GYRO_REG_NUM][3] =
        {
            {BMI088_GYRO_RANGE, BMI088_GYRO_2000, BMI088_GYRO_RANGE_ERROR},
            {BMI088_GYRO_BANDWIDTH, BMI088_GYRO_1000_116_HZ | BMI088_GYRO_BANDWIDTH_MUST_Set, BMI088_GYRO_BANDWIDTH_ERROR},
            {BMI088_GYRO_LPM1, BMI088_GYRO_NORMAL_MODE, BMI088_GYRO_LPM1_ERROR},
            {BMI088_GYRO_CTRL, BMI088_DRDY_ON, BMI088_GYRO_CTRL_ERROR},
            {BMI088_GYRO_INT3_INT4_IO_CONF, BMI088_GYRO_INT3_GPIO_PP | BMI088_GYRO_INT3_GPIO_LOW, BMI088_GYRO_INT3_INT4_IO_CONF_ERROR},
            {BMI088_GYRO_INT3_INT4_IO_MAP, BMI088_GYRO_DRDY_IO_INT3, BMI088_GYRO_INT3_INT4_IO_MAP_ERROR}

        };

public:
    float temp;
    GyroRate gyro_rate;
    Acceleration acceleration;

    uint16_t gyro_range;
    uint16_t accel_range;

    float gyro_unit;
    float accel_unit;

    __attribute__ ((aligned (4)))uint8_t rawdata_accel[6];
    __attribute__ ((aligned (4)))uint8_t rawdata_gyro[8];
    __attribute__ ((aligned (4)))uint8_t rawdata_temp[2];

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

    //HAL_TransmitReceive
    uint8_t readWriteByte(uint8_t txData);
    //uint8_t readByte();

    //gyro and accel read and write
    void BMI088_read_single_reg(uint8_t reg, uint8_t *return_data);
    void BMI088_write_single_reg(uint8_t reg, uint8_t data);
    void BMI088_read_multi_reg(uint8_t reg, uint8_t* buf, uint8_t len);

    void BMI088_accel_read_single_reg(uint8_t reg, uint8_t* return_data);
    void BMI088_accel_read_multi_reg(uint8_t reg, uint8_t* data, uint8_t len);
    void BMI088_accel_write_single_reg(uint8_t reg, uint8_t data);

    void BMI088_gyro_read_single_reg(uint8_t reg, uint8_t* return_data);
    void BMI088_gyro_read_multi_reg(uint8_t reg, uint8_t* data, uint8_t len);
    void BMI088_gyro_write_single_reg(uint8_t reg, uint8_t data);

    //accel and gyro init
    void bmi088_accel_init(void);
    void bmi088_gyro_init(void);

    void BMI088_delay_us(uint16_t us);
    void BMI088_delay_ms(uint16_t ms);
};

#endif //EC_HOMEWORKPLATFORM_IMU_H
