//
// Created by ASUS on 2023/11/15.
//

#ifndef EC_MATH_H
#define EC_MATH_H

#include <algorithm>

const float PI = 3.141592653589793238462;

namespace math{
//limit functions
float limit(const float& value, const float& inf, const float& sup);
float loopLimit(float val, const float& min, const float& max);
float DeadBand(float val, const float& min, const float& max);

//degree units conversion
float deg2rad(const float& deg);
float rad2deg(const float& rad);
float rpm2dps(const float& rpm);
float rpm2radps(const float& rpm);
float ecd2deg(const float& ecd, const float& ecd_range);

float degNormalize180(const float& angle);

}

#endif //EC_MATH_H
