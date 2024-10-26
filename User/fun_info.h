#ifndef __FUN_INFO_H
#define __FUN_INFO_H

#include "include.h"   // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

// ��λ�Ķ���
enum
{
    GEAR_NEUTRAL = 0x00, // �յ�
    GEAR_FIRST = 0x01,   // һ��
    GEAR_SECOND = 0x02,  // ����
    GEAR_THIRD = 0x03,   // ����
    GEAR_FOURTH = 0x04,  // �ĵ�
    GEAR_FIFTH = 0x05,   // �嵵
    GEAR_SIXTH = 0x06    // ����
};

// ���壬�ƵĿ���״̬
enum
{
    ON = 0x01,
    OFF = 0x00
};

// ���壬�洢���й��ܵ�״̬�Ľṹ��
typedef struct
{
    u32 engine_speeed; // ��������ת��
    u32 speed;         // ʱ��

    u8 gear;    // ��λ
    u8 battery; // ��ص���
    u8 brake;   // ɲ����״̬

    // �Ƶ�״̬��ON �� OFF��־
    u8 left_turn;  // ��ת��Ƶ�״̬
    u8 right_turn; // ��ת��Ƶ�״̬
    u8 high_beam;  // Զ��Ƶ�״̬

    u8 fuel;       // ����
    u8 water_temp; // ˮ��
} fun_info_t;

extern volatile fun_info_t fun_info; // ������й���״̬�Ľṹ�����

#endif
