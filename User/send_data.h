#ifndef __SEND_DATA_H
#define __SEND_DATA_H

#include "include.h" // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

#ifndef FORMAT_HEAD // �����ʽͷ
#define FORMAT_HEAD 0xA5
#endif // FORMAT_HEAD

// ���嵥Ƭ����MP5���͵Ĵ������ݵ�ָ��
enum
{
    SEND_GEAR = 0x01, // ���͵�λ��״̬
    SEND_BATTERY = 0x02, // ���͵�ص�����״̬
    SEND_BARKE = 0x03, // ����ɲ����״̬
    SEND_LEFT_TURN = 0x04, // ������ת��Ƶ�״̬
    SEND_RIGHT_TURN = 0x05, // ������ת��Ƶ�״̬
    SEND_HIGH_BEAM = 0x06, // ����Զ��Ƶ�״̬
    SEND_ENGINE_SPEED = 0x07, // ���ͷ�������ת��
    SEND_SPEED = 0x08, // ����ʱ��
    SEND_FUEL = 0x09, // ��������
    SEND_WATER_TEMP = 0x0A, // ����ˮ��
    SEND_TOTAL_MILEAGE = 0x0B, // ���ʹ�����
    SEND_SUBTOTAL_MILEAGE = 0x0C // ����С�����
};

void send_data(u32 instruct, u32 send_data);

#endif

