#ifndef __SPEED_SCAN_H
#define __SPEED_SCAN_H

#include "include.h" // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

extern volatile u32 distance; // 存放每次扫描时走过的路程

void speed_scan_config(void); 
void speed_scan(void);

#endif
