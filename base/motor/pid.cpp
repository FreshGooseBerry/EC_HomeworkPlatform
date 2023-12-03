//
// Created by ASUS on 2023/11/26.
//

#include "pid.h"
#include "../common/math.h"

PID::PID(const PidInitTypedef& pid_init){
    kp_ = pid_init.kp;
    kd_ = pid_init.kd;
    ki_ = pid_init.ki;
    i_max_ = pid_init.i_max;
    out_max_ = pid_init.out_max;
}

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

    output_=math::limit(pout_+iout_+dout_,-out_max_,out_max_);
    last_err_=err_;
    return output_;
}


