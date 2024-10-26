#include "pin_level_scan.h"

void pin_level_scan_config(void)
{
    p21_input_config(); // Զ���״̬��Ӧ������
    p22_input_config(); // ��ת���״̬��Ӧ������
    p23_input_config(); // ɲ��״̬��Ӧ������
    p25_input_config(); // ��ת���״̬��Ӧ������

    p30_input_config(); // 6����Ӧ������
    p27_input_config(); // 5����Ӧ������
    p14_input_config(); // 4����Ӧ������
    p13_input_config(); // 3����Ӧ������
    p10_input_config(); // 2����Ӧ������
    p07_input_config(); // 1����Ӧ������
    p06_input_config(); // �յ���Ӧ������
}

// ���ŵ�ƽɨ�裬���ǵ͵�ƽ��Ч
void pin_level_scan(void)
{
    if (P23 == 0)
    {
        // ��� ɲ��

        // printf("P23 == 0\n");
        if (OFF == fun_info.brake)
        {
            // ���֮ǰɲ���ǹرյ�
            get_brake = 1;
            fun_info.brake = ON;
        }
        else
        {
            // ���ɲ������һ��ɨ��ʱ���ǿ�����
            // ���ø���״̬
        }
    }
    else
    {
        // �������ɲ���ǹرյģ�������һ��ɨ��ʱɲ���Ƿ�����
        // �����һ��ɨ��ʱ�ǿ����ģ�Ҫ����ɲ���رյ���Ϣ
        if (ON == fun_info.brake)
        {
            fun_info.brake = OFF;
            get_brake = 1;
        }
    }

    if (P25 == 0)
    {
        // ��� ��ת��ƿ���
        if (OFF == fun_info.left_turn)
        {
            // �����һ��ɨ��ʱԶ����ǹرյ�
            fun_info.left_turn = ON;
            get_left_turn = 1;
        }
    }
    else
    {
        if (ON == fun_info.left_turn)
        {
            // �����һ��ɨ��ʱԶ����ǿ�����
            fun_info.left_turn = OFF;
            get_left_turn = 1;
        }
    }

    if (P22 == 0)
    {
        // ��� ��ת��ƿ���
        // printf("P22 == 0\n");
        if (OFF == fun_info.right_turn)
        {
            // �����һ��ɨ��ʱ��ת����ǹرյ�
            fun_info.right_turn = ON;
            get_right_turn = 1;
        }
    }
    else
    {
        if (ON == fun_info.right_turn)
        {
            // �����һ��ɨ��ʱ��ת����ǿ�����
            fun_info.right_turn = OFF;
            get_right_turn = 1;
        }
    }

    if (P21 == 0)
    {
        // ��� Զ��ƿ���
        // printf("P21 == 0\n");

        if (OFF == fun_info.high_beam)
        {
            // �����һ��ɨ��ʱԶ����ǹرյ�
            fun_info.high_beam = ON;
            get_high_beam = 1;
        }
    }
    else
    {
        if (ON == fun_info.high_beam)
        {
            // �����һ��ɨ��ʱԶ����ǿ�����
            fun_info.high_beam = OFF;
            get_high_beam = 1;
        }
    }

    // ����͵�λ���ȣ�����͵����ź�ʱ������������λ���źţ�ֱ������͵���Ϊ��
    if (0 == P06)
    {
        if (GEAR_NEUTRAL != fun_info.gear)
        {
            // �����һ��ɨ��ʱ�ĵ�λ���ǿյ�
            fun_info.gear = GEAR_NEUTRAL;
            get_gear = 1;
        }
    }
    else if (0 == P07)
    {
        if (GEAR_FIRST != fun_info.gear)
        {
            // �����һ��ɨ��ʱ�ĵ�λ����һ��
            fun_info.gear = GEAR_FIRST;
            get_gear = 1;
        }
    }
    else if (0 == P10)
    {
        if (GEAR_SECOND != fun_info.gear)
        {
            // �����һ��ɨ��ʱ�ĵ�λ���Ƕ���
            fun_info.gear = GEAR_SECOND;
            get_gear = 1;
        }
    }
    else if (0 == P13)
    {
        if (GEAR_THIRD != fun_info.gear)
        {
            // �����һ��ɨ��ʱ�ĵ�λ��������
            fun_info.gear = GEAR_THIRD;
            get_gear = 1;
        }
    }
    else if (0 == P14)
    {
        if (GEAR_FOURTH != fun_info.gear)
        {
            // �����һ��ɨ��ʱ�ĵ�λ�����ĵ�
            fun_info.gear = GEAR_FOURTH;
            get_gear = 1;
        }
    }
    else if (0 == P27)
    {
        if (GEAR_FIFTH != fun_info.gear)
        {
            // �����һ��ɨ��ʱ�ĵ�λ�����嵵
            fun_info.gear = GEAR_FIFTH;
            get_gear = 1;
        }
    }
    else if (0 == P30)
    {
        // printf("P30 == 0\n");
        if (GEAR_SIXTH != fun_info.gear)
        {
            // �����һ��ɨ��ʱ�ĵ�λ��������
            fun_info.gear = GEAR_SIXTH;
            get_gear = 1;
        }
    }
}
