#ifndef __ENGINE_SPEED_SCAN_H
#define __ENGINE_SPEED_SCAN_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

void engine_speed_scan_config(void); // 发动机转速扫描的配置
void engine_speed_scan(void); // 发动机转速扫描

#endif