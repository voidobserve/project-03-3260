#ifndef __MILEAGE_H
#define __MILEAGE_H

#include "include.h"   // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

// ��������̵Ľṹ��
typedef struct 
{
    u32 total_mileage; // ����̱������̣�0~99999.9��
    u32 subtotal_mileage; // �̾�����̱�С����̣�0~9999��
}mileage_t;

extern mileage_t mileage_info; // �����̱�Ľṹ�����

void mileage_config(void);
void mileage_scan(void); // ���ɨ�裨������ɨ��+С�����ɨ�裩

#endif