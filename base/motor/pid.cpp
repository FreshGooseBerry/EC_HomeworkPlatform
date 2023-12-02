//
// Created by ASUS on 2023/11/26.
//

#include "pid.h"
#include "../common/math.h"

PID::PID(float kp, float ki, float kd, float i_max, float out_max)
    :kp_(kp),ki_(ki),kd_(kd),i_max_(i_max),out_max_(out_max){}

float PID::calculate(float ref, float fdb) {
    ref_=ref;
    fdb_=fdb;

    err_=ref-fdb;
    //比例项
    pout_=kp_*err_;
    //积分项
    err_sum_=math::limit(err_+err_sum_,-i_max_,i_max_);
    iout_=ki_*err_sum_;
    //微分项
    dout_=kd_*(err_-last_err_);

    output_=std::max(pout_+iout_+dout_,out_max_);
    last_err_=err_;
    return output_;
}


