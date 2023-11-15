//
// Created by ASUS on 2023/11/15.
//

#include "math.h"

/**
 * @brief limit value, functions map values to a certain interval
 * 限幅函数
 */
//将变量直接映射到区间
//limit
float math::limit(const float& value, const float& inf, const float& sup){
    if(inf >= sup){
        return value;
    }
    if(value > sup){
        return sup;
    }
    else if(value < inf){
        return inf;
    }
    return value;
}

//循环限幅，将max-min记为一个周期，将变量加减一定周期使其在某个范围以内
//loop limit
float math::loopLimit(float val, const float& min, const float& max) {
    if(min > max){
        return val;
    }
    if(val > max){
        while(val > max){
            val -= max - min;
        }
    }
    else if(val < min){
        while(val < min){
            val += max - min;
        }
    }
    return val;
}

//死区，若输入在上限与下限之间，输出零，否则不进行操作
//Dead band
float math::DeadBand(float val, const float& min, const float& max) {
    if(val < max && val > min){
        return 0;
    }
    return val;
}

/**
* @brief degree units conversion
* 单位转换函数
*/
//degree convert functions
float deg2rad(const float& deg){
    return (deg * PI / 180.0);
};

float rad2deg(const float& rad){
    return (rad * 180.0 / PI);
}

//angular velocity convert functions
float rpm2dps(const float& rpm){
    return rpm * 6.0;
}

//TODO: other units convert functions