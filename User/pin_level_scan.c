#include "pin_level_scan.h"

u32 pin_level_scan_time_cnt = 0;

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
    // 如果到了扫描时间，再更新挡位、转向灯和刹车的状态
    if (pin_level_scan_time_cnt >= PIN_LEVEL_SCAN_TIME_MS)
    {
        pin_level_scan_time_cnt = 0;

        if (P23)
        {
            // 如果没有刹车
            fun_info.brake = OFF;
        }
        else
        {
            // 如果有刹车
            fun_info.brake = ON;
        }
        flag_get_brake = 1;

        if (P25)
        {
            // 如果左转向灯未开启
            fun_info.left_turn = OFF;
        }
        else
        {
            // 如果左转向灯开启
            fun_info.left_turn = ON;
        }
        flag_get_left_turn = 1;

        if (P22)
        {
            // 如果右转向灯未开启
            fun_info.right_turn = OFF;
        }
        else
        {
            // 如果右转向灯开启
            fun_info.right_turn = ON;
        }
        flag_get_right_turn = 1;

        if (P21)
        {
            // 如果远光灯未开启
            fun_info.high_beam = OFF;
        }
        else
        {
            // 如果远光灯开启
            fun_info.high_beam = ON;
        }
        flag_get_high_beam = 1;

        // 以最低挡位优先，当最低档有信号时，不管其他挡位的信号，直接以最低档的为主

        if (0 == P06)
        {
            // 空挡
            fun_info.gear = GEAR_NEUTRAL;
        }
        else if (0 == P07)
        {
            // 一档
            fun_info.gear = GEAR_FIRST;
        }
        else if (0 == P10)
        {
            // 二档
            fun_info.gear = GEAR_SECOND;
        }
        else if (0 == P13)
        {
            // 三档
            fun_info.gear = GEAR_THIRD;
        }
        else if (0 == P14)
        {
            // 四档
            fun_info.gear = GEAR_FOURTH;
        }
        else if (0 == P27)
        {
            // 五档
            fun_info.gear = GEAR_FIFTH;
        }
        else if (0 == P30)
        {
            // 六档
            fun_info.gear = GEAR_SIXTH;
        }
        flag_get_gear = 1;
    }

    // if () // ACC引脚检测，检测到高电平，P03也输出高电平，检测到低电平，让P03输出低电平
    if (P04)
    {
        // 检测到ACC为高电平,
        P03 = 1;
    }
    else
    {
        P03 = 0;
    }
}
