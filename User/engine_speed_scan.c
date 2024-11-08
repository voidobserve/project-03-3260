#include "engine_speed_scan.h"

// ������ÿתһȦ���ܼ�⵽���������
#ifndef ENGINE_PULSE_PER_TURN
#define ENGINE_PULSE_PER_TURN 16
#endif

static volatile u32 pulse_cnt = 0; // �������ֵ

// ������ת�ٵ��������
void engine_speed_scan_config(void)
{
    // ʹ��IO�ж������������
    __SetIRQnIP(P1_IRQn, P1_IQn_CFG); // �����ж����ȼ�
    __EnableIRQ(P1_IRQn);             // ʹ��P1�ж�
    IE_EA = 1;                        // ʹ���ܿ���

    P1_MD0 &= ~GPIO_P12_MODE_SEL(0x3); // ����ģʽ
    P1_PD |= GPIO_P12_PULL_PD(0x1);    // ����Ϊ����
    P1_IMK |= GPIO_P12_IRQ_MASK(0x1);  // ʹ��IO�ж�
    P1_TRG0 &= ~GPIO_P12_TRG_SEL(0x3);
    P1_TRG0 |= GPIO_P12_TRG_SEL(0x2); // ���������ش���
}

// ��ȡ������ÿ����ת��
// static u32 get_engine_speed_rpm(void)
// {
//     // return (get_turns_per_250ms() * 4 * 60);

//     u32 tmp = 0;
//     pulse_cnt = 0;
//     tmr1_cnt = 0;
//     tmr1_enable();
//     __EnableIRQ(P1_IRQn); // ʹ���ж�


//     while (tmr1_cnt < 2500)
//     {
//     }; // �ȴ�250ms

//     // �����ÿmin(250ms * 4 * 60 == 1min)ת�˶���Ȧ
//     tmp = pulse_cnt * 240 / ENGINE_PULSE_PER_TURN;

//     tmr1_disable();


//     __DisableIRQ(P1_IRQn); // �����ж�

//     tmr1_cnt = 0;
//     pulse_cnt = 0;

//     return tmp;
// }

// ������ת��ɨ��
void engine_speed_scan(void)
{
    // u32 rpm = get_engine_speed_rpm();
    // // printf("rpm: %lu\n", rpm);

    // fun_info.engine_speeed = rpm;
    // flag_get_engine_speed = 1;
    
    u32 rpm = 0;
    
    if (tmr1_cnt >= 2500) // ����Ѿ�����250ms
    {
        // 250ms * 4 * 60 == 1min
        // �����Ǹ���250ms��⵽����������������ÿ���ӵ�ת��
        rpm = pulse_cnt * 240 / ENGINE_PULSE_PER_TURN; 

        tmr1_cnt = 0;
        pulse_cnt = 0;
        fun_info.engine_speeed = rpm;
        printf("engine speed %lu rpm\n", rpm);
        // flag_get_engine_speed = 1; // ��ø���һ��״̬��δȷ��
    }

}


// P1�жϷ�����
void P1_IRQHandler(void) interrupt P1_IRQn
{
    // Px_PND�Ĵ���д�κ�ֵ�������־λ
    u8 p1_pnd = P1_PND;

    // �����ж�����IP������ɾ��
    __IRQnIPnPush(P1_IRQn);
    // ---------------- �û��������� -------------------

    if (p1_pnd & GPIO_P12_IRQ_PNG(0x1))
    {
        pulse_cnt++;
    }
    P1_PND = p1_pnd; // ��P1�жϱ�־λ

    // -------------------------------------------------
    // �˳��ж�����IP������ɾ��
    __IRQnIPnPop(P1_IRQn);
}
