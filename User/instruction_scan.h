#ifndef __INSTRUCTION_SCAN_H
#define __INSTRUCTION_SCAN_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

extern volatile bit flag_get_all_status;   // 获取所有功能的状态
extern volatile bit get_gear;        // 获取挡位状态 / 得到了挡位的状态
extern volatile bit get_battery;     // 获取电池电量的状态/ 得到了电池的状态（电池电量，单位：百分比）
extern volatile bit get_brake;       // 获取刹车状态 / 得到了刹车的状态
extern volatile bit get_left_turn;    // 获取左转向灯的状态 / 得到了左转向灯的状态
extern volatile bit get_right_turn;   // 获取右转向灯的状态 / 得到了右转向灯的状态
extern volatile bit get_high_beam;    // 获取远光灯的状态 / 得到了远光灯的状态
extern volatile bit get_engine_speed; // 获取发动机的转速 / 得到了发动机的转速
extern volatile bit flag_get_speed;       // 获取时速 / 得到了时速
extern volatile bit getFuel;        // 获取油量 / 得到了油量（单位：百分比）
extern volatile bit getWaterTemp;   // 获取水温 / 得到了水温

extern volatile bit getTotalMileage; // 获取大计里程 / 得到了大计里程
extern volatile bit getSubTotalMileage; // 获取小计里程 / 得到了小计里程

void instruction_handle(void); // 扫描是否有对应的指令

#endif
