#include "tmr1.h"

// #define TMR1_CNT_TIME 152 // 152 * 0.65625us Լ����100us

// ��ʱ����ʱ���� (��λ:Hz)
// ����ֵ = ϵͳʱ�� / ��ʱ����Ƶ / Ƶ�� - 1
#define TMR1_PERIOD (SYSCLK / 128 / 10000 - 1) // 10000Hz,100us

// volatile bit tmr1_flag = 0; // TMR1�жϷ������л���λ�ı�־λ
volatile u32 tmr1_cnt = 0; // ��ʱ��TMR1�ļ���ֵ��ÿ�����жϷ������л��һ��

/**
 * @brief ���ö�ʱ��TMR1��������ɺ󣬶�ʱ��Ĭ�Ϲر�
 */
void tmr1_config(void)
{
    __SetIRQnIP(TMR1_IRQn, TMR1_IQn_CFG); // �����ж����ȼ���TMR1��
    __DisableIRQ(TMR1_IRQn);              // �����ж�
    IE_EA = 1;                            // �����ж�

    // ���TMR1�ļ���ֵ
    TMR1_CNTL = 0;
    TMR1_CNTH = 0;

    TMR1_CONL &= ~TMR_PRESCALE_SEL(0x03); // ���TMR1��Ԥ��Ƶ���üĴ���
    TMR1_CONL |= TMR_PRESCALE_SEL(0x07);  // ��ʱ��Ԥ��Ƶ
    TMR1_CONL &= ~TMR_MODE_SEL(0x03);     // ���TMR1��ģʽ���üĴ���
    TMR1_CONL |= TMR_MODE_SEL(0x01);      // ����TMR1��ģʽΪ������ģʽ������ϵͳʱ�ӵ�������м���

    TMR1_CONH &= ~TMR_PRD_PND(0x01); // ���TMR1�ļ�����־λ����ʾδ��ɼ���
    TMR1_CONH |= TMR_PRD_IRQ_EN(1);  // ʹ��TMR1�ļ����ж�

    // ����TMR1�ļ�������
    TMR1_PRH = TMR_PERIOD_VAL_H((TMR1_PERIOD >> 8) & 0xFF); // ����ֵ
    TMR1_PRL = TMR_PERIOD_VAL_L((TMR1_PERIOD >> 0) & 0xFF);

    TMR1_CONL &= ~(TMR_SOURCE_SEL(0x07)); // ���TMR1��ʱ��Դ���üĴ���
    TMR1_CONL |= TMR_SOURCE_SEL(0x05); // ����TMR1��ʱ��Դ�������κ�ʱ��
}

/**
 * @brief ������ʱ��TMR1����ʼ��ʱ
 */
void tmr1_enable(void)
{
    // ���¸�TMR1����ʱ��
    TMR1_CONL &= ~(TMR_SOURCE_SEL(0x07)); // �����ʱ����ʱ��Դ���üĴ���
    TMR1_CONL |= TMR_SOURCE_SEL(0x06);    // ���ö�ʱ����ʱ��Դ��ʹ��ϵͳʱ��

    __EnableIRQ(TMR1_IRQn); // ʹ���ж�
    IE_EA = 1;              // �����ж�
}

/**
 * @brief �رն�ʱ������ռ���ֵ
 */
void tmr1_disable(void)
{
    // ������ʱ���ṩʱ�ӣ�����ֹͣ����
    TMR1_CONL &= ~(TMR_SOURCE_SEL(0x07)); // �����ʱ����ʱ��Դ���üĴ���
    TMR1_CONL |= TMR_SOURCE_SEL(0x05);    // ���ö�ʱ����ʱ��Դ�������κ�ʱ��

    // �����ʱ���ļ���ֵ
    TMR1_CNTL = 0;
    TMR1_CNTH = 0;

    __DisableIRQ(TMR1_IRQn); // �ر��жϣ���ʹ���жϣ�
}

// TMR1�жϷ�����
void TIMR1_IRQHandler(void) interrupt TMR1_IRQn
{
    // �����ж�����IP������ɾ��
    __IRQnIPnPush(TMR1_IRQn);

    // ---------------- �û��������� -------------------

    // �����ж�
    if (TMR1_CONH & TMR_PRD_PND(0x1))
    {
        TMR1_CONH |= TMR_PRD_PND(0x1); // ���pending

        tmr1_cnt++;

#if 0
        // �������5s
        if (TMR1_cnt == 1000)
        {
            TMR1_cnt = 0;
            TMR1_flag = 1; // ����Ӧ�ı�־λ��һ
        }
#endif
    }

    // �˳��ж�����IP������ɾ��
    __IRQnIPnPop(TMR1_IRQn);
}