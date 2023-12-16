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
    /*accelerator soft reset*/
    BMI088_accel_write_single_reg(BMI088_ACC_SOFTRESET_ADDR,BMI088_ENTER_SOFTRESET);
    HAL_Delay(100);//延迟1ms,初始化要求

    //set mode to NORMAL; 复位后默认是暂停模式，向0x7D写0x04改成正常模式
    BMI088_accel_write_single_reg(BMI088_ACC_PWR_CTRL_ADDR,BMI088_ACC_NORMAL_MODE);
    HAL_Delay(100);

    /*Gyro soft reset*/
    BMI088_gyro_write_single_reg(BMI088_GYRO_SOFTRESET,BMI088_ENTER_SOFTRESET);
    HAL_Delay(100);
}

void IMU::init() {
    /*
    uint8_t write_reg_num = 0;
    uint8_t res = 0;
    //set accel sonsor config and check
    for (write_reg_num = 0; write_reg_num < BMI088_WRITE_ACCEL_REG_NUM; write_reg_num++)
    {

        BMI088_accel_write_single_reg(write_BMI088_accel_reg_data_error[write_reg_num][0], write_BMI088_accel_reg_data_error[write_reg_num][1]);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        BMI088_accel_read_single_reg(write_BMI088_accel_reg_data_error[write_reg_num][0], res);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        if (res != write_BMI088_accel_reg_data_error[write_reg_num][1])
        {
            return write_BMI088_accel_reg_data_error[write_reg_num][2];
        }
    }



    //set gyro sonsor config and check
    for (write_reg_num = 0; write_reg_num < BMI088_WRITE_GYRO_REG_NUM; write_reg_num++)
    {

        BMI088_gyro_write_single_reg(write_BMI088_gyro_reg_data_error[write_reg_num][0], write_BMI088_gyro_reg_data_error[write_reg_num][1]);
        HAL_Delay(2);

        BMI088_gyro_read_single_reg(write_BMI088_gyro_reg_data_error[write_reg_num][0], &res);
        HAL_Delay(2);

        if (res != write_BMI088_gyro_reg_data_error[write_reg_num][1])
        {
            Error_Handler();
            //return write_BMI088_gyro_reg_data_error[write_reg_num][2];
        }
    }
    */
    reset();

    bmi088_accel_init();
    bmi088_gyro_init();



    //uint8_t txData;
    //uint16_t counter;
/*
    BMI088_ACCEL_NS_L();

    txData = 0x7D;
    readWriteByte(txData);
    txData = 0x04;
    readWriteByte(txData);

    HAL_Delay(1);

    BMI088_ACCEL_NS_H();
*/
    /*
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
     */
    //gyro_range = BMI088_GYRO_RANGE_2000;

    gyro_unit = BMI088_GYRO_2000_SEN;
    accel_unit = BMI088_ACCEL_3G_SEN;

}

void IMU::handle() {
    //read data
    BMI088_accel_read_multi_reg(BMI088_ACC_X_LSB_ADDR,rawdata_accel,6);

    BMI088_accel_read_multi_reg(BMI088_TEMP_M,rawdata_temp,2);

    //handle data
    acceleration.x = (float)((int16_t)(rawdata_accel[1] << 8) | rawdata_accel[0]) * accel_unit;
    acceleration.y = (float)((int16_t)(rawdata_accel[3] << 8) | rawdata_accel[2]) * accel_unit;
    acceleration.z = (float)((int16_t)(rawdata_accel[5] << 8) | rawdata_accel[4]) * accel_unit;

    BMI088_gyro_read_multi_reg(BMI088_ACC_CHIP_ID,rawdata_gyro,8);

    if(rawdata_gyro[0] == BMI088_GYRO_CHIP_ID_VALUE) {
        gyro_rate.x =
            (float)((int16_t)(rawdata_gyro[3] << 8) | rawdata_gyro[2]) * gyro_unit;//先移位再转int_16!
        gyro_rate.y =
            (float)((int16_t)(rawdata_gyro[5] << 8) | rawdata_gyro[4]) * gyro_unit;
        gyro_rate.z =
            (float)((int16_t)(rawdata_gyro[7] << 8) | rawdata_gyro[6]) * gyro_unit;
    }

    temp = (int16_t)((rawdata_temp[0] << 3) | (rawdata_temp[1] >> 5));
    if (temp > 1023)
    {
        temp -= 2048;
    }
    temp = temp * BMI088_TEMP_FACTOR + BMI088_TEMP_OFFSET;
}

uint8_t IMU::readWriteByte(uint8_t txData) {
    __attribute__ ((aligned (4)))uint8_t pxData;

    if(HAL_SPI_TransmitReceive(&hspi1,&txData,&pxData,1,1000)!=HAL_OK){
        Error_Handler();
    }

    return pxData;
}
/*
uint8_t IMU::readByte() {
    uint8_t pxData;
    //uint8_t counter = 0;
    HAL_SPI_Receive(&hspi1,&pxData,1,1000);

    //while((HAL_SPI_GetState(&hspi1)==HAL_SPI_STATE_BUSY_RX) && counter < 50000){//等待传输完成
    //    ++counter;
    //}
    //if(counter == 50000){//超时
    //    Error_Handler();
    //}

    return pxData;
}*/

void IMU::BMI088_read_single_reg(uint8_t reg, uint8_t *return_data)
{
    readWriteByte(reg | 0x80);
    *return_data = readWriteByte(0x55);//?
}

void IMU::BMI088_write_single_reg(uint8_t reg, uint8_t data){
    readWriteByte(reg);
    readWriteByte(data);
}

void IMU::BMI088_read_multi_reg(uint8_t reg, uint8_t* buf, uint8_t len) {
    readWriteByte(reg | 0x80);

    while(len > 0){
        *buf = readWriteByte(0x55);
        ++buf;
        --len;
    }
}

void IMU::BMI088_accel_read_single_reg(uint8_t reg, uint8_t* return_data) {
    BMI088_ACCEL_NS_L();
    readWriteByte(reg | 0x80);
    readWriteByte(0x55);//when receiving accelerator data, the first receive byte must be discarded
    *return_data = readWriteByte(0x55);
    BMI088_ACCEL_NS_H();
}

void IMU::BMI088_accel_read_multi_reg(uint8_t reg, uint8_t* data, uint8_t len) {
    BMI088_ACCEL_NS_L();
    readWriteByte(reg | 0x80);
    BMI088_read_multi_reg(reg,data,len);//the first receive byte discarded in function
    BMI088_ACCEL_NS_H();
}

void IMU::BMI088_gyro_read_single_reg(uint8_t reg, uint8_t* return_data) {
    BMI088_GYRO_NS_L();
    BMI088_read_single_reg(reg,return_data);
    BMI088_GYRO_NS_H();
}

void IMU::BMI088_gyro_read_multi_reg(uint8_t reg, uint8_t* data, uint8_t len) {
    BMI088_GYRO_NS_L();
    BMI088_read_multi_reg(reg,data,len);
    BMI088_GYRO_NS_H();
}

void IMU::BMI088_gyro_write_single_reg(uint8_t reg, uint8_t data) {
    BMI088_GYRO_NS_L();
    BMI088_write_single_reg(reg,data);
    //HAL_Delay(50);
    BMI088_GYRO_NS_H();
}

void IMU::BMI088_accel_write_single_reg(uint8_t reg, uint8_t data) {
    BMI088_ACCEL_NS_L();
    BMI088_write_single_reg(reg,data);
    //HAL_Delay(50);
    BMI088_ACCEL_NS_H();
}

void IMU::bmi088_accel_init(void)
{
    uint8_t res = 0;
    uint8_t write_reg_num = 0;

    //check commiunication
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID, &res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID, &res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    //accel software reset
    BMI088_accel_write_single_reg(BMI088_ACC_SOFTRESET, BMI088_ACC_SOFTRESET_VALUE);
    BMI088_delay_ms(BMI088_LONG_DELAY_TIME);

    //check commiunication is normal after reset
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID, &res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
    BMI088_accel_read_single_reg(BMI088_ACC_CHIP_ID, &res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    // check the "who am I"
    if (res != BMI088_ACC_CHIP_ID_VALUE)
    {
        Error_Handler();
        //return BMI088_NO_SENSOR;
    }

    //set accel sonsor config and check
    for (write_reg_num = 0; write_reg_num < BMI088_WRITE_ACCEL_REG_NUM; write_reg_num++)
    {

        BMI088_accel_write_single_reg(write_BMI088_accel_reg_data_error[write_reg_num][0], write_BMI088_accel_reg_data_error[write_reg_num][1]);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        BMI088_accel_read_single_reg(write_BMI088_accel_reg_data_error[write_reg_num][0], &res);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        if (res != write_BMI088_accel_reg_data_error[write_reg_num][1])
        {
            Error_Handler();
            //return write_BMI088_accel_reg_data_error[write_reg_num][2];
        }
    }
    //return BMI088_NO_ERROR;
}

void IMU::bmi088_gyro_init(void)
{
    uint8_t write_reg_num = 0;
    uint8_t res = 0;

    //check commiunication
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID, &res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID, &res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    //reset the gyro sensor
    BMI088_gyro_write_single_reg(BMI088_GYRO_SOFTRESET, BMI088_GYRO_SOFTRESET_VALUE);
    BMI088_delay_ms(BMI088_LONG_DELAY_TIME);
    //check commiunication is normal after reset
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID, &res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);
    BMI088_gyro_read_single_reg(BMI088_GYRO_CHIP_ID, &res);
    BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

    // check the "who am I"
    if (res != BMI088_GYRO_CHIP_ID_VALUE)
    {
        Error_Handler();
        //return BMI088_NO_SENSOR;
    }

    //set gyro sonsor config and check
    for (write_reg_num = 0; write_reg_num < BMI088_WRITE_GYRO_REG_NUM; write_reg_num++)
    {

        BMI088_gyro_write_single_reg(write_BMI088_gyro_reg_data_error[write_reg_num][0], write_BMI088_gyro_reg_data_error[write_reg_num][1]);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        BMI088_gyro_read_single_reg(write_BMI088_gyro_reg_data_error[write_reg_num][0], &res);
        BMI088_delay_us(BMI088_COM_WAIT_SENSOR_TIME);

        if (res != write_BMI088_gyro_reg_data_error[write_reg_num][1])
        {
            Error_Handler();
            //return write_BMI088_gyro_reg_data_error[write_reg_num][2];
        }
    }

    //return BMI088_NO_ERROR;
}

void IMU::BMI088_delay_ms(uint16_t ms)
{
    while(ms--)
    {
        BMI088_delay_us(1000);
    }
}

void IMU::BMI088_delay_us(uint16_t us)
{

    uint32_t ticks = 0;
    uint32_t told = 0;
    uint32_t tnow = 0;
    uint32_t tcnt = 0;
    uint32_t reload = 0;
    reload = SysTick->LOAD;
    ticks = us * 168;
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break;
            }
        }
    }


}