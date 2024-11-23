#ifndef __MILEAGE_H
#define __MILEAGE_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

extern volatile u32 distance;               // 存放每次扫描时走过的路程
extern volatile u32 mileage_scan_time_cnt; // 里程扫描所需的计数值,每隔一定时间将里程写入flash

void mileage_scan(void); // 里程扫描（大计里程扫描+小计里程扫描）

#endif