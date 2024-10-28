#include "speed_scan.h"

// ���ٸ������ʾһȦ
#ifndef PULSE_PER_TURN
#define PULSE_PER_TURN 16
#endif // ���ٸ������ʾһȦ

// ����һȦ��ʾ���ٺ���
#ifndef MM_PER_TURN
#define MM_PER_TURN 1795 // һȦ1795����
#endif                   // ����һȦ��ʾ���ٺ���

static volatile u32 pulse_cnt = 0; // �������ֵ
volatile u32 distance = 0;         // ���ÿ��ɨ��ʱ�߹���·�̣���λ�����ף�-->������̱�ļ���

// ʱ��ɨ�������
void speed_scan_config(void)
{
#ifdef DEVELOPMENT_BOARD
    // ʹ��IO�ж������������
    __SetIRQnIP(P1_IRQn, P1_IQn_CFG); // �����ж����ȼ�
    __EnableIRQ(P1_IRQn);             // ʹ��P1�ж�
    IE_EA = 1;                        // ʹ���ܿ���

    P1_MD0 &= ~GPIO_P13_MODE_SEL(0x3); // ����ģʽ
    P1_PD |= GPIO_P13_PULL_PD(0x1);    // ����Ϊ����
    P1_IMK |= GPIO_P13_IRQ_MASK(0x1);  // ʹ��IO�ж�
    P1_TRG0 &= ~GPIO_P13_TRG_SEL(0x3);
    P1_TRG0 |= GPIO_P13_TRG_SEL(0x2); // ���������ش���
#endif

#ifdef CIRCUIT_BOARD
    // ʹ��IO�ж������������
    __SetIRQnIP(P0_IRQn, P0_IQn_CFG); // �����ж����ȼ�
    __EnableIRQ(P0_IRQn);             // ʹ��P1�ж�
    IE_EA = 1;                        // ʹ���ܿ���

    P0_MD0 &= ~GPIO_P02_MODE_SEL(0x3); // ����ģʽ
    P0_PD |= GPIO_P02_PULL_PD(0x1);    // ����Ϊ����
    P0_IMK |= GPIO_P02_IRQ_MASK(0x1);  // ʹ��IO�ж�
    P0_TRG0 &= ~GPIO_P02_TRG_SEL(0x3);
    P0_TRG0 |= GPIO_P02_TRG_SEL(0x2); // ���������ش���
#endif
}

// �ر��ٶ�ɨ�������жϣ������صļ���
static void speed_scan_disable(void)
{
#ifdef DEVELOPMENT_BOARD
    __DisableIRQ(P1_IRQn); // �ر�P1�ж�
#endif
#ifdef CIRCUIT_BOARD
    __DisableIRQ(P0_IRQn); // �ر�P0�ж�
#endif
    pulse_cnt = 0; // ����������
    tmr1_disable();
    tmr1_cnt = 0;
}

// �����ٶ�ɨ����ص��ж�
static void speed_scan_enable(void)
{
    pulse_cnt = 0;
    tmr1_cnt = 0;
#ifdef DEVELOPMENT_BOARD
    __EnableIRQ(P1_IRQn); // ʹ��P1�ж�
#endif
#ifdef CIRCUIT_BOARD
    __EnableIRQ(P0_IRQn); // ʹ��P0�ж�
#endif

    tmr1_enable();
}

// ��ȡÿСʱ�߹����ٺ���
static u32 get_speed_mm_per_hour(void)
{
    u32 tmp = 0;
    speed_scan_enable();
    while (tmr1_cnt < 2500)
        ;                                           // �ȴ�250ms
    tmp = pulse_cnt * MM_PER_TURN / PULSE_PER_TURN; // 250ms�߹������ٺ���
    distance += tmp;                                // ����߹��ľ��룬��λ������(��Ϊû��ʹ�ø�������,��Ҫ�ۼ�,�������ͳ�����ʱ�ͻᶪʧ����)
    speed_scan_disable();
    tmp *= 14400; // ����ó�1Сʱ�߹��ĺ���     // tmp = tmp * 4 * 3600;
    return tmp;
}

// �ɼ�һ���ٶ�����
static u32 get_speed(void)
{
#if 0
    u32 speed_km_per_hour = get_speed_mm_per_hour(); // �õ�ÿСʱ�߹��ĺ�����
    // speed_km_per_hour /= 1000000;                  // �����ÿСʱ�߹���ǧ����
    return speed_km_per_hour / 1000000;
#endif

    return (get_speed_mm_per_hour() / 1000000);
}

// �ٶ�ɨ�躯�����Ѿ���֤����ʹ��
void speed_scan(void)
{
    static u32 last_speed = 0;   // ��¼��һ�βɼ������ٶ�
    u32 cur_speed = get_speed(); // ��ǰ�ɼ����ٶ�
    // printf("--------%lu km/h\n", cur_speed);

    if (((cur_speed > last_speed) && (cur_speed - last_speed > 2)) ||
        ((cur_speed < last_speed) && (last_speed - cur_speed > 2)))
    {
// ������βɼ�������������һ�βɼ��������ݲ�ֵ������1������ֱ�ӷ��ͳ�ȥ

        // �����ã�
        // printf("%lu km/h\n", cur_speed);

        last_speed = cur_speed;

        fun_info.speed = cur_speed;
        flag_get_speed = 1;
    }
    else if (((cur_speed > last_speed) && (cur_speed - last_speed < 2)) ||
             ((cur_speed < last_speed) && (last_speed - cur_speed < 2)))
    {
        // ������βɼ�������������һ�βɼ��������ݲ�ֵֻ��1���ٲɼ�����
        u8 cnt = 0;

        if ((cur_speed > last_speed) && (cur_speed - last_speed < 2))
        {
            // �������ٶ������ӵģ�Ҫȷ����������ڼ���
            u8 i = 0;

            for (i = 0; i < 3; i++)
            {
                cur_speed = get_speed();

                if ((cur_speed > last_speed) && (cur_speed - last_speed < 2))
                {
                    cnt++;
                }
            }
        }
        else if ((cur_speed < last_speed) && (last_speed - cur_speed < 2))
        {
            // �������ٶ��Ǽ��ٵģ�Ҫȷ����������ڼ���
            u8 i = 0;
            for (i = 0; i < 3; i++)
            {
                cur_speed = get_speed();

                if ((cur_speed < last_speed) && (last_speed - cur_speed < 2))
                {
                    cnt++;
                }
            }
        }

        if (cnt >= 2)
        {
            // �����ã�
            // printf("%lu km/h\n", cur_speed);

            last_speed = cur_speed;

            fun_info.speed = cur_speed;
            flag_get_speed = 1;
        }
    }
    else if (cur_speed == 0 && last_speed != 0)
    {
        // �����ã�
        // printf("%lu km/h\n", cur_speed);


        fun_info.speed = cur_speed;
        flag_get_speed = 1;
    }
}

#ifdef DEVELOPMENT_BOARD
// P1�жϷ�����
void P1_IRQHandler(void) interrupt P1_IRQn
{
    // Px_PND�Ĵ���д�κ�ֵ�������־λ
    u8 p1_pnd = P1_PND;

    // �����ж�����IP������ɾ��
    __IRQnIPnPush(P1_IRQn);
    // ---------------- �û��������� -------------------
    if (p1_pnd & GPIO_P13_IRQ_PNG(0x1))
    {
        pulse_cnt++;
    }
    P1_PND = p1_pnd; // ��P1�жϱ�־λ
    // -------------------------------------------------
    // �˳��ж�����IP������ɾ��
    __IRQnIPnPop(P1_IRQn);
}
#endif

#ifdef CIRCUIT_BOARD
// P0�жϷ�����
void P0_IRQHandler(void) interrupt P0_IRQn
{
    // Px_PND�Ĵ���д�κ�ֵ�������־λ
    u8 p0_pnd = P0_PND;

    // �����ж�����IP������ɾ��
    __IRQnIPnPush(P0_IRQn);
    // ---------------- �û��������� -------------------
    if (p0_pnd & GPIO_P02_IRQ_PNG(0x1))
    {
        pulse_cnt++;
    }
    P0_PND = p0_pnd; // ��P0�жϱ�־λ
    // -------------------------------------------------
    // �˳��ж�����IP������ɾ��
    __IRQnIPnPop(P0_IRQn);
}
#endif
