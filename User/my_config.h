// my_config.h -- �Զ���������ļ�
#ifndef __MY_CONFIG_H
#define __MY_CONFIG_H

#include "include.h" // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�

// ����ƽ̨
// DEVELOPMENT_BOARD -- ������
// CIRCUIT_BOARD -- ��·��
#define CIRCUIT_BOARD

// #define MY_DEBUG 1

// ����0������
#define UART0_BAUDRATE      115200
// ����1������
// #define UART1_BAUDRATE 115200 // ���оƬ��TX3260������ʹ��UART1��DMA�����ղ�����������

// ����0���ջ������Ĵ�С����λ���ֽڣ�
#define UART0_RXBUF_LEN     30 // ����ܳ���255������֮�󣬴��ڽ��ջ����

// ���ȵ�λ
// INTERNATIONAL -- ����ͨ�õ�λ
// IMPERIAL -- Ӣ�Ƶ�λ
#define  INTERNATIONAL

// �Ƿ����Զ���Ĵ�ӡ���Թ���
#define MY_DEBUG 0

#define USE_TMR2 0 // �Ƿ�ʹ�ö�ʱ��2

#include <stdio.h>
#include "my_gpio.h" // �Զ���ġ�ʹ�õ�������
#include "uart0.h"
#include "tmr0.h" // ���ڼ�����ݳ�ʱ��Ҫʹ�õ��Ķ�ʱ��
#include "tmr1.h" // ɨ�������������IO�ڵ�������Ҫ�õ��Ķ�ʱ��

#include "tmr2.h" // �����ã�P24+��ʱ��PWM��������������������ṩ�ź�

#include "tmr3.h" // ��ʱ�������д��flash����Ķ�ʱ��

#include "instruction.h" // ɨ����յ������ݣ�����Ҫ�ڴ���0��ͷ�ļ����棬����Ҫ�Ƚ��ղ���֤�����ֵ���ɨ��ָ�
#include "instruction_scan.h" // ɨ���Ƿ����յ����ϸ�ʽ��ָ��
#include "fun_info.h"         // ��Ÿ������ܵ�״̬��Ϣ

#include "pin_level_scan.h" // ɨ�����ŵ�ƽ״̬

#include "send_data.h" // ���ʹ������ݵ�ָ��

#include "adc.h" // adc

#include "speed_scan.h" // �ٶȣ�ʱ�٣�ɨ��
#include "engine_speed_scan.h" // ������ת��ɨ��

#include "flash.h" // ���ڽ�����̡�С����̴���flash��
#include "mileage.h" // ��̱����ؽӿ�

#endif // end file
