#ifndef __ADC_H
#define __ADC_H

#include "include.h" // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

extern u16 adc_val;
// extern bit adc_done_flag;

void adc_pin_config(void);
void adc_showval(void);
void adc_scan(void); 

#endif
