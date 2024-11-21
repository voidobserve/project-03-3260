// my_config.h -- 自定义的配置文件
#ifndef __MY_CONFIG_H
#define __MY_CONFIG_H

#include "include.h" // 使用芯片官方提供的头文件

#define ARRAY_SIZE(arry) (sizeof(arry) / sizeof(arry[0]))

#define USE_MY_DEBUG 1

// 串口0波特率
#define UART0_BAUDRATE (115200)
// 串口1波特率
// #define UART1_BAUDRATE 115200 // 这款芯片（TX3260）不能使用UART1的DMA来接收不定长的数据

// 串口0接收缓冲区的大小（单位：字节）
#define UART0_RXBUF_LEN (30) // 最大不能超过255，超过之后，串口接收会出错

// 长度单位
// USE_INTERNATIONAL -- 国际通用单位
// USE_IMPERIAL -- 英制单位
#define USE_INTERNATIONAL

// 扫描时间配置：

// ======================================================
// 触摸按键配置
// 使用5V参考电压，外部没有分压的情况下：
#define AD_KEY_INTERVAL (50)         // 判断区间，检测到ad值在(目标ad值-区间区 ~ 目标ad值+区间值,认为条件成立)
#define AD_KEY_ONE_LEFT_VAL (876)    // 从左往下数，第一个按键对应的ad值 (1.07V)
#define AD_KEY_TWO_LEFT_VAL (3604)   // 从左往下数，第二个按键对应的ad值 (4.40V)
#define AD_KEY_THREE_LEFT_VAL (2859) // 从左往下数，第三个按键对应的ad值 (3.49V)

#define AD_KEY_ONE_RIGHT_VAL (1499)   // 从右往下数，第一个按键对应的ad值 (1.83V)
#define AD_KEY_TWO_RIGHT_VAL (1744)   // 从右往下数，第二个按键对应的ad值 (2.13V)
#define AD_KEY_THREE_RIGHT_VAL (2359) // 从右往下数，第三个按键对应的ad值 (2.88V)

// 不能使用这种方式，ad值不在定义的按键对应的ad值区间时，都认为按键没有按下
// #define AD_KEY_NONE (4095) // 没有按键按下时，对应的ad值

// 触摸按键键值定义(检测到短按/持续时，要发送的键值)：
#define TOUCH_KEY_VAL_MUTE ((u16)0x0040)    // 左2 从左往下数，第二个按键
#define TOUCH_KEY_VAL_VOL_INC ((u16)0x0041) // 左1 从左往下数，第一个按键
#define TOUCH_KEY_VAL_VOL_DEC ((u16)0x0042) // 左3 从左往下数，第三个按键
#define TOUCH_KEY_VAL_MODE ((u16)0x0065)    // 右2 从右往下数，第二个按键
#define TOUCH_KEY_VAL_PRE ((u16)0x00E3)     // 右3 从右往下数，第三个按键
#define TOUCH_KEY_VAL_NEXT ((u16)0x00E4)    // 右1 从右往下数，第一个按键

// 检测到长按时要发送的键值：
#define TOUCH_KEY_VAL_POWER ((u16)0x0046) // 左2 从左往下数，第二个按键
#define TOUCH_KEY_VAL_MAIN ((u16)0x0011)  // 右2 从右往下数，第二个按键

// 注意：只有 TOUCH_KEY_VAL_VOL_INC 和 TOUCH_KEY_VAL_VOL_DEC 需要检测持续按下

// #define DETECT_DOUBLE_CLICK_INTERVAL (100) // 检测双击的时间间隔(单位：ms)
#define LONG_PRESS_TIME_THRESHOLD_MS (750) // 长按时间阈值（单位：ms）
#define HOLD_PRESS_TIME_THRESHOLD_MS (150) // 长按持续(不松手)的时间阈值(单位：ms)，每隔 xx 时间认为有一次长按持续事件
#define LOOSE_PRESS_CNT_MS (50)            // 松手计时，松开手多久，才认为是真的松手了
#define ONE_CYCLE_TIME_MS (1)              // 主函数完成一次循环所需的时间，单位：ms (0--说明每次调用该函数的时间很短，可以忽略不计)
// 触摸按键配置
// ======================================================

// #define USE_MY_DEBUG 1

#include <stdio.h>   // printf()
#include "my_gpio.h" // 自定义的、使用到的引脚

#include "uart0.h" // 接收 / 发送 指令 使用的串口

#include "tmr0.h" // 串口检测数据超时需要使用到的定时器
#include "tmr1.h" // 用于扫描发动机转速的定时器
#include "tmr2.h" // 用于扫描时速的定时器
#include "tmr3.h" // 定时将总里程写入flash所需的定时器

#include "tmr4.h" // 用于后台更新当前时间的定时器

#include "instruction.h" // 扫描接收到的数据（必须要在串口0的头文件后面，串口要先接收并验证，才轮到它扫描指令）
#include "fun_info.h"    // 存放各个功能的状态信息

#include "pin_level_scan.h" // 扫描引脚电平状态

#include "send_data.h" // 发送带有数据的指令

#include "adc.h" // adc

#include "speed_scan.h"        // 速度（时速）扫描
#include "engine_speed_scan.h" // 发动机转速扫描

#include "flash.h"   // 提供flash操作的接口
#include "mileage.h" // 里程表的相关接口

#include "ad_key.h"

#endif // end file
