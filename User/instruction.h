#ifndef __INSTRUCTION_H
#define __INSTRUCTION_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件


// 定义 MP5向单片机发送的的指令
enum
{
    INSTRUCTION_GET_ALL_STATUS = 0x20,   // 获取所有功能的状态
    INSTRUCTION_GET_GEAR = 0x21,         // 获取挡位的状态
    INSTRUCTION_GET_BATTERY = 0x22,      // 获取电池电量的状态
    INSTRUCTION_GET_BARKE = 0x23,        // 获取刹车的状态
    INSTRUCTION_GET_LEFT_TURN = 0x24,    // 获取左转向灯的状态
    INSTRUCTION_GET_RIGHT_TURN = 0x25,   // 获取右转向灯的状态
    INSTRUCTION_GET_HIGH_BEAM = 0x26,    // 获取远光灯的状态
    INSTRUCTION_GET_ENGINE_SPEED = 0x27, // 获取发动机转速
    INSTRUCTION_GET_SPEED = 0x28,        // 获取时速
    INSTRUCTION_GET_FUEL = 0x29,         // 获取油量
    INSTRUCTION_GET_WATER_TEMP = 0x2A,   // 获取水温
    INSTRUCTION_GET_TOTAL_MILEAGE = 0x2B,   // 获取大计里程
    INSTRUCTION_GET_SUBTOTAL_MILEAGE = 0x2C, // 获取小计里程

    INSTRUCTION_GET_TOUCH_KEY_STATUS = 0x2D, // 获取触摸按键状态
    INSTRUCTION_ALTER_DATE = 0x2E, // 修改日期
    INSTRUCTION_ALTER_TIME = 0x2F, // 修改时间
    INSTRUCTION_GET_BATTERY_VAL = 0x30, // 获取电池电压
    // INSTRUCTION_GET_TEMP_OF_WATER // 水温报警
    INSTRUCTION_CLEAR_TOTAL_MILEAGE = 0x32, // 清除大计里程
    INSTRUCTION_CLEAR_SUBTOTAL_MILEAGE = 0x33, // 清除小计里程
};

extern void instruction_scan(void);

#endif