#ifndef __FUN_INFO_H
#define __FUN_INFO_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

// 挡位的定义
enum
{
    GEAR_NEUTRAL = 0x00, // 空挡
    GEAR_FIRST = 0x01,   // 一档
    GEAR_SECOND = 0x02,  // 二档
    GEAR_THIRD = 0x03,   // 三档
    GEAR_FOURTH = 0x04,  // 四档
    GEAR_FIFTH = 0x05,   // 五档
    GEAR_SIXTH = 0x06    // 六档
};

// 定义，灯的开关状态
enum
{
    ON = 0x01,
    OFF = 0x00
};

// 定义，存储所有功能的状态的结构体
typedef struct
{
    u32 engine_speeed; // 发动机的转速
    u32 speed;         // 时速

    u8 gear;    // 挡位
    u8 battery; // 电池电量
    u8 brake;   // 刹车的状态

    // 灯的状态用ON 或 OFF标志
    u8 left_turn;  // 左转向灯的状态
    u8 right_turn; // 右转向灯的状态
    u8 high_beam;  // 远光灯的状态

    u8 fuel;       // 油量
    u8 water_temp; // 水温
} fun_info_t;

extern volatile fun_info_t fun_info; // 存放所有功能状态的结构体变量

#endif
