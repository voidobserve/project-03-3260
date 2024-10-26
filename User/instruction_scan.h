#ifndef __INSTRUCTION_SCAN_H
#define __INSTRUCTION_SCAN_H

#include "include.h"   // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

extern volatile bit flag_get_all_status;   // ��ȡ���й��ܵ�״̬
extern volatile bit get_gear;        // ��ȡ��λ״̬ / �õ��˵�λ��״̬
extern volatile bit get_battery;     // ��ȡ��ص�����״̬/ �õ��˵�ص�״̬����ص�������λ���ٷֱȣ�
extern volatile bit get_brake;       // ��ȡɲ��״̬ / �õ���ɲ����״̬
extern volatile bit get_left_turn;    // ��ȡ��ת��Ƶ�״̬ / �õ�����ת��Ƶ�״̬
extern volatile bit get_right_turn;   // ��ȡ��ת��Ƶ�״̬ / �õ�����ת��Ƶ�״̬
extern volatile bit get_high_beam;    // ��ȡԶ��Ƶ�״̬ / �õ���Զ��Ƶ�״̬
extern volatile bit get_engine_speed; // ��ȡ��������ת�� / �õ��˷�������ת��
extern volatile bit flag_get_speed;       // ��ȡʱ�� / �õ���ʱ��
extern volatile bit getFuel;        // ��ȡ���� / �õ�����������λ���ٷֱȣ�
extern volatile bit getWaterTemp;   // ��ȡˮ�� / �õ���ˮ��

extern volatile bit getTotalMileage; // ��ȡ������ / �õ��˴�����
extern volatile bit getSubTotalMileage; // ��ȡС����� / �õ���С�����

void instruction_handle(void); // ɨ���Ƿ��ж�Ӧ��ָ��

#endif
