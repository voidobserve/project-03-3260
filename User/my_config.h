// my_config.h -- 自定义的配置文件
#ifndef __MY_CONFIG_H
#define __MY_CONFIG_H

#include "include.h" // 使用芯片官方提供的头文件

// 开发平台
// DEVELOPMENT_BOARD -- 开发板
// CIRCUIT_BOARD -- 电路板
#define CIRCUIT_BOARD

// #define MY_DEBUG 1

// 串口0波特率
#define UART0_BAUDRATE      115200
// 串口1波特率
// #define UART1_BAUDRATE 115200 // 这款芯片（TX3260）不能使用UART1的DMA来接收不定长的数据

// 串口0接收缓冲区的大小（单位：字节）
#define UART0_RXBUF_LEN     30 // 最大不能超过255，超过之后，串口接收会出错

// 长度单位
// INTERNATIONAL -- 国际通用单位
// IMPERIAL -- 英制单位
#define  INTERNATIONAL

// 是否开启自定义的打印调试功能
#define MY_DEBUG 0

#define USE_TMR2 0 // 是否使用定时器2

#include <stdio.h>
#include "my_gpio.h" // 自定义的、使用到的引脚
#include "uart0.h"
#include "tmr0.h" // 串口检测数据超时需要使用到的定时器
#include "tmr1.h" // 扫描霍尔传感器的IO口的脉冲需要用到的定时器

#include "tmr2.h" // 测试用，P24+定时器PWM输出，给检测霍尔的引脚提供信号

#include "tmr3.h" // 定时将总里程写入flash所需的定时器

#include "instruction.h" // 扫描接收到的数据（必须要在串口0的头文件后面，串口要先接收并验证，才轮到它扫描指令）
#include "instruction_scan.h" // 扫描是否有收到符合格式的指令
#include "fun_info.h"         // 存放各个功能的状态信息

#include "pin_level_scan.h" // 扫描引脚电平状态

#include "send_data.h" // 发送带有数据的指令

#include "adc.h" // adc

#include "speed_scan.h" // 速度（时速）扫描
#include "engine_speed_scan.h" // 发动机转速扫描

#include "flash.h" // 用于将总里程、小计里程存入flash中
#include "mileage.h" // 里程表的相关接口

#endif // end file
