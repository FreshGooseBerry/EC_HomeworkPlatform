//
// Created by ASUS on 2023/11/26.
//

#ifndef EC_HOMEWORKPLATFORM_PID_H
#define EC_HOMEWORKPLATFORM_PID_H

#include <algorithm>

struct PidInitTypedef{
    float kp;
    float ki;
    float kd;
    float i_max;
    float out_max;
};

class PID {
public:
    PID(const PidInitTypedef& pid_init);

    float calculate(float ref, float fdb);
    struct HHH{
        int a;
        char g;
    };

private:
    float kp_, ki_, kd_;        //每项系数
    float i_max_, out_max_;     //积分项限幅和总输出限幅
    float output_;

    float ref_, fdb_;           //目标reference，当前的返回值feedback
    float err_, err_sum_, last_err_;
    float pout_, iout_, dout_;
};

#endif //EC_HOMEWORKPLATFORM_PID_H
