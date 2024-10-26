#ifndef __MILEAGE_H
#define __MILEAGE_H

#include "include.h"   // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

// 定义存放里程的结构体
typedef struct 
{
    u32 total_mileage; // 总里程表（大计里程，0~99999.9）
    u32 subtotal_mileage; // 短距离里程表（小计里程，0~9999）
}mileage_t;

extern mileage_t mileage_info; // 存放里程表的结构体变量

void mileage_config(void);
void mileage_scan(void); // 里程扫描（大计里程扫描+小计里程扫描）

#endif