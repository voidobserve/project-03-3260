#ifndef __PIN_LEVEL_SCAN_H
#define __PIN_LEVEL_SCAN_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

void pin_level_scan_config(void); // 扫描引脚的配置（初始化）
void pin_level_scan(void); // 扫描引脚的电平


#endif
