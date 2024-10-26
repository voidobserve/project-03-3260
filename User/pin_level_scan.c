#include "pin_level_scan.h"

void pin_level_scan_config(void)
{
    p21_input_config(); // 远光灯状态对应的引脚
    p22_input_config(); // 右转向灯状态对应的引脚
    p23_input_config(); // 刹车状态对应的引脚
    p25_input_config(); // 左转向灯状态对应的引脚

    p30_input_config(); // 6档对应的引脚
    p27_input_config(); // 5档对应的引脚
    p14_input_config(); // 4档对应的引脚
    p13_input_config(); // 3档对应的引脚
    p10_input_config(); // 2档对应的引脚
    p07_input_config(); // 1档对应的引脚
    p06_input_config(); // 空挡对应的引脚
}

// 引脚电平扫描，都是低电平有效
void pin_level_scan(void)
{
    if (P23 == 0)
    {
        // 如果 刹车

        // printf("P23 == 0\n");
        if (OFF == fun_info.brake)
        {
            // 如果之前刹车是关闭的
            get_brake = 1;
            fun_info.brake = ON;
        }
        else
        {
            // 如果刹车在上一次扫描时就是开启的
            // 不用更新状态
        }
    }
    else
    {
        // 如果现在刹车是关闭的，看看上一次扫描时刹车是否开启，
        // 如果上一次扫描时是开启的，要发送刹车关闭的信息
        if (ON == fun_info.brake)
        {
            fun_info.brake = OFF;
            get_brake = 1;
        }
    }

    if (P25 == 0)
    {
        // 如果 左转向灯开启
        if (OFF == fun_info.left_turn)
        {
            // 如果上一次扫描时远光灯是关闭的
            fun_info.left_turn = ON;
            get_left_turn = 1;
        }
    }
    else
    {
        if (ON == fun_info.left_turn)
        {
            // 如果上一次扫描时远光灯是开启的
            fun_info.left_turn = OFF;
            get_left_turn = 1;
        }
    }

    if (P22 == 0)
    {
        // 如果 右转向灯开启
        // printf("P22 == 0\n");
        if (OFF == fun_info.right_turn)
        {
            // 如果上一次扫描时右转向灯是关闭的
            fun_info.right_turn = ON;
            get_right_turn = 1;
        }
    }
    else
    {
        if (ON == fun_info.right_turn)
        {
            // 如果上一次扫描时右转向灯是开启的
            fun_info.right_turn = OFF;
            get_right_turn = 1;
        }
    }

    if (P21 == 0)
    {
        // 如果 远光灯开启
        // printf("P21 == 0\n");

        if (OFF == fun_info.high_beam)
        {
            // 如果上一次扫描时远光灯是关闭的
            fun_info.high_beam = ON;
            get_high_beam = 1;
        }
    }
    else
    {
        if (ON == fun_info.high_beam)
        {
            // 如果上一次扫描时远光灯是开启的
            fun_info.high_beam = OFF;
            get_high_beam = 1;
        }
    }

    // 以最低挡位优先，当最低档有信号时，不管其他挡位的信号，直接以最低档的为主
    if (0 == P06)
    {
        if (GEAR_NEUTRAL != fun_info.gear)
        {
            // 如果上一次扫描时的挡位不是空挡
            fun_info.gear = GEAR_NEUTRAL;
            get_gear = 1;
        }
    }
    else if (0 == P07)
    {
        if (GEAR_FIRST != fun_info.gear)
        {
            // 如果上一次扫描时的挡位不是一档
            fun_info.gear = GEAR_FIRST;
            get_gear = 1;
        }
    }
    else if (0 == P10)
    {
        if (GEAR_SECOND != fun_info.gear)
        {
            // 如果上一次扫描时的挡位不是二档
            fun_info.gear = GEAR_SECOND;
            get_gear = 1;
        }
    }
    else if (0 == P13)
    {
        if (GEAR_THIRD != fun_info.gear)
        {
            // 如果上一次扫描时的挡位不是三档
            fun_info.gear = GEAR_THIRD;
            get_gear = 1;
        }
    }
    else if (0 == P14)
    {
        if (GEAR_FOURTH != fun_info.gear)
        {
            // 如果上一次扫描时的挡位不是四档
            fun_info.gear = GEAR_FOURTH;
            get_gear = 1;
        }
    }
    else if (0 == P27)
    {
        if (GEAR_FIFTH != fun_info.gear)
        {
            // 如果上一次扫描时的挡位不是五档
            fun_info.gear = GEAR_FIFTH;
            get_gear = 1;
        }
    }
    else if (0 == P30)
    {
        // printf("P30 == 0\n");
        if (GEAR_SIXTH != fun_info.gear)
        {
            // 如果上一次扫描时的挡位不是六档
            fun_info.gear = GEAR_SIXTH;
            get_gear = 1;
        }
    }
}
