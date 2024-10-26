#ifndef __TMR1_H
#define __TMR1_H

#include "include.h"   // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

// extern volatile bit tmr1_flag; // TMR1�жϷ������л���λ�ı�־λ
extern volatile u32 tmr1_cnt;  // ��ʱ��TMR1�ļ���ֵ��ÿ�����жϷ������л��һ��

void tmr1_config(void);  // ���ö�ʱ������ʼ����
void tmr1_enable(void);  // ������ʱ��
void tmr1_disable(void); // �رն�ʱ�������Ӳ���ļ���ֵ

#endif
