#ifndef __ADC_H
#define __ADC_H

#include "include.h" // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

extern u16 adc_val;
// extern bit adc_done_flag;

void adc_pin_config(void);
void adc_showval(void);
void adc_scan(void); 

#endif
