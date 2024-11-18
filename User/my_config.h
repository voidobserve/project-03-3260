// my_config.h -- 自定义的配置文件
#ifndef __MY_CONFIG_H
#define __MY_CONFIG_H

#include "include.h" // 使用芯片官方提供的头文件

// 开发平台
// DEVELOPMENT_BOARD -- 开发板
// CIRCUIT_BOARD -- 电路板
// #define CIRCUIT_BOARD

#define USE_MY_DEBUG 1

// 串口0波特率
#define UART0_BAUDRATE 115200
// 串口1波特率
// #define UART1_BAUDRATE 115200 // 这款芯片（TX3260）不能使用UART1的DMA来接收不定长的数据

// 串口0接收缓冲区的大小（单位：字节）
#define UART0_RXBUF_LEN 30 // 最大不能超过255，超过之后，串口接收会出错

// 长度单位
// USE_INTERNATIONAL -- 国际通用单位
// USE_IMPERIAL -- 英制单位
#define USE_INTERNATIONAL

// #define USE_MY_DEBUG 1

#include <stdio.h>   // printf()
#include "my_gpio.h" // 自定义的、使用到的引脚
#include "uart0.h" // 接收 / 发送 指令 使用的串口
#include "tmr0.h" // 串口检测数据超时需要使用到的定时器
#include "tmr1.h" // 用于扫描发动机转速的定时器
#include "tmr2.h" // 用于扫描时速的定时器
#include "tmr3.h" // 定时将总里程写入flash所需的定时器

#include "instruction.h" // 扫描接收到的数据（必须要在串口0的头文件后面，串口要先接收并验证，才轮到它扫描指令）
#include "fun_info.h"    // 存放各个功能的状态信息

#include "pin_level_scan.h" // 扫描引脚电平状态

#include "send_data.h" // 发送带有数据的指令

#include "adc.h" // adc

#include "speed_scan.h"        // 速度（时速）扫描
#include "engine_speed_scan.h" // 发动机转速扫描

#include "flash.h"   // 提供flash操作的接口
#include "mileage.h" // 里程表的相关接口

#endif // end file
