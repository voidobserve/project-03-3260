#ifndef __SPEED_SCAN_H
#define __SPEED_SCAN_H

#include "include.h" // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

extern volatile u32 distance; // ���ÿ��ɨ��ʱ�߹���·��

void speed_scan_config(void); 
void speed_scan(void);

#endif
