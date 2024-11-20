#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

// 定义 MP5向单片机发送的的指令
enum
{
    INSTRUCTION_GET_ALL_STATUS = 0x20,       // 获取所有功能的状态
    INSTRUCTION_GET_GEAR = 0x21,             // 获取挡位的状态
    INSTRUCTION_GET_BATTERY = 0x22,          // 获取电池电量的状态
    INSTRUCTION_GET_BARKE = 0x23,            // 获取刹车的状态
    INSTRUCTION_GET_LEFT_TURN = 0x24,        // 获取左转向灯的状态
    INSTRUCTION_GET_RIGHT_TURN = 0x25,       // 获取右转向灯的状态
    INSTRUCTION_GET_HIGH_BEAM = 0x26,        // 获取远光灯的状态
    INSTRUCTION_GET_ENGINE_SPEED = 0x27,     // 获取发动机转速
    INSTRUCTION_GET_SPEED = 0x28,            // 获取时速
    INSTRUCTION_GET_FUEL = 0x29,             // 获取油量
    INSTRUCTION_GET_TEMP_OF_WATER = 0x2A,    // 获取水温
    INSTRUCTION_GET_TOTAL_MILEAGE = 0x2B,    // 获取大计里程
    INSTRUCTION_GET_SUBTOTAL_MILEAGE = 0x2C, // 获取小计里程

    // INSTRUCTION_GET_TOUCH_KEY_STATUS = 0x2D,      // 获取触摸按键状态
    INSTRUCTION_ALTER_DATE = 0x2E,                // 修改日期
    INSTRUCTION_ALTER_TIME = 0x2F,                // 修改时间
    INSTRUCTION_GET_BATTERY_VAL = 0x30,           // 获取电池电压
    INSTRUCTION_SET_TEMP_OF_WATER_WARNING = 0x31, // 设置水温报警
    INSTRUCTION_CLEAR_TOTAL_MILEAGE = 0x32,       // 清除大计里程
    INSTRUCTION_CLEAR_SUBTOTAL_MILEAGE = 0x33,    // 清除小计里程
};

// // 定义 水温报警的相关操作
// enum
// {
//     OPERATION_SET_TEMP_OF_WATER_WARNING_NONE = 0, // 无操作
//     OPERATION_SET_TEMP_OF_WATER_WARNING_ON,       // 打开水温报警
//     OPERATION_SET_TEMP_OF_WATER_WARNING_OFF,      // 关闭水温报警
// };

extern volatile bit flag_get_all_status;    // 获取所有功能的状态
extern volatile bit flag_get_gear;          // 获取挡位状态 / 得到了挡位的状态
extern volatile bit flag_get_battery;       // 获取电池电量的状态/ 得到了电池的状态（电池电量，单位：百分比）
extern volatile bit flag_get_brake;         // 获取刹车状态 / 得到了刹车的状态
extern volatile bit flag_get_left_turn;     // 获取左转向灯的状态 / 得到了左转向灯的状态
extern volatile bit flag_get_right_turn;    // 获取右转向灯的状态 / 得到了右转向灯的状态
extern volatile bit flag_get_high_beam;     // 获取远光灯的状态 / 得到了远光灯的状态
extern volatile bit flag_get_engine_speed;  // 获取发动机的转速 / 得到了发动机的转速
extern volatile bit flag_get_speed;         // 获取时速 / 得到了时速
extern volatile bit flag_get_fuel;          // 获取油量 / 得到了油量（单位：百分比）
extern volatile bit flag_get_temp_of_water; // 获取水温 / 得到了水温

extern volatile bit flag_get_total_mileage;     // 获取大计里程 / 得到了大计里程
extern volatile bit flag_get_sub_total_mileage; // 获取小计里程 / 得到了小计里程

// extern volatile bit flag_get_touch_key_status;      // 获取触摸按键的状态
extern volatile bit flag_alter_date;                // 修改日期
extern volatile bit flag_alter_time;                // 修改时间
extern volatile bit flag_get_voltage_of_battery;               // 获取电池电压
extern volatile bit flag_set_temp_of_water_warning; // 设置水温报警
extern volatile bit flag_clear_total_mileage;       // 清除大计里程
extern volatile bit flag_clear_sub_total_mileage;   // 清除小计里程

extern void instruction_scan(void);   // 扫描指令缓冲区中是否有对应指令
extern void instruction_handle(void); // 扫描是否有对应的指令,如果有，则执行对应操作

#endif