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

// 
enum
{
    ON = 0x01,
    OFF = 0x00
};

// 定义存储在flash中的数据
typedef struct
{
    // 总里程表（单位：m，使用英制单位时，只需要再发送时进行转换）
    // （大计里程，范围：0~ 99999.9 KM）
    u32 total_mileage;
    // 短距离里程表(单位：m，使用英制单位时，只需要再发送时进行转换)
    // （小计里程，范围：0~999.9 KM）
    u32 subtotal_mileage;

    // 日期 4byte 年 月 日
    u16 year; // 年份
    u8 month; // 月份
    u8 day;   // 日期

    // 时间 3byte 时 分 秒，24H制存放
    u8 time_hour;
    u8 time_min;
    u8 time_sec;

} save_info_t;

// 定义，存储所有功能的状态的结构体
typedef struct
{
    save_info_t save_info; // 存储在flash中的结构体变量

    u32 engine_speeed; // 发动机的转速（单位：rpm）
    u32 speed;         // 时速(单位：km/h，使用英制单位时，只需要在发送时进行转换)

    // u16 touch_key_val; // 触摸按键键值
    u16 voltage_of_battery; // 电池电压

    u8 gear;    // 挡位
    u8 battery; // 电池电量(单位：百分比)
    u8 brake;   // 刹车的状态

    // 灯的状态用ON 或 OFF标志
    u8 left_turn;  // 左转向灯的状态
    u8 right_turn; // 右转向灯的状态
    u8 high_beam;  // 远光灯的状态

    u8 fuel;          // 油量
    // u8 temp_of_water; // 水温

    u8 flag_is_in_water_temp_warning; // 标志位，是否处于水温报警（1--水温报警，0--解除水温报警）

    // 在结构体中使用一位的数据：
    // u8 bit0 : 1;
    // u8 bit1 : 1;
    // u8 bit2 : 1;
    // u8 bit3 : 1;
    // u8 bit4 : 1;
    // u8 bit5 : 1;
    // u8 bit6 : 1;
    // u8 bit7 : 1;

} fun_info_t;

extern volatile fun_info_t fun_info; // 存放所有功能状态的结构体变量

extern void fun_info_init(void); // 初始化存放所有信息的结构体变量
extern void fun_info_save(void); // 将信息写回flash

#endif
