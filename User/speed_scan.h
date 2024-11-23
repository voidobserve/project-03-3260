#ifndef __SPEED_SCAN_H
#define __SPEED_SCAN_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

extern volatile u32 speed_scan_time_cnt; // 速度扫描时，用到的时间计数值，会在定时器中断中累加

extern volatile u32 detect_speed_pulse_cnt; // 检测时速的脉冲计数值

void speed_scan_config(void);
void speed_scan(void);

#endif
