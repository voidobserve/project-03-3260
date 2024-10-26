// GPIO���õ�Դ�ļ�
#include "my_gpio.h"

// ������ų�ʼ���������ã�
// P1_1
void output_pin_init(void)
{
    P1_MD0 &= ~(GPIO_P11_MODE_SEL(0x1)); // ��Ӧ�ļĴ���������
    P1_MD0 |= GPIO_P11_MODE_SEL(0x1);    // ����Ϊ���ģʽ

    FOUT_S11 = GPIO_FOUT_AF_FUNC; // ѡ��AF�������

    // λ����
    P11 = 1;
}

/**
 * @brief P11����Ϊ���ģʽ
 */
void p11_output_config(void)
{
    P1_MD0 &= ~(GPIO_P11_MODE_SEL(0x1)); // ��Ӧ�ļĴ���������
    P1_MD0 |= GPIO_P11_MODE_SEL(0x1);    // ����Ϊ���ģʽ

    FOUT_S11 = GPIO_FOUT_AF_FUNC; // ѡ��AF�������

    P1_PU |= GPIO_P11_PULL_UP(0x01); // �ڲ�����������
    P1_PD |= GPIO_P11_PULL_PD(0x01); // �ڲ�����������
    // �����������������������ž��൱�ڸ���״̬��ͬʱ�ܹ�����һЩ��·�ĸ���

    // λ����
    P11 = 1;
}

/**
 * @brief P12����Ϊ���ģʽ
 */
void p12_output_config(void)
{
    P1_MD0 &= ~(GPIO_P12_MODE_SEL(0x1)); // ��Ӧ�ļĴ���������
    P1_MD0 |= GPIO_P12_MODE_SEL(0x1);    // ����Ϊ���ģʽ

    FOUT_S12 = GPIO_FOUT_AF_FUNC; // ѡ��AF�������

    P1_PU |= GPIO_P12_PULL_UP(0x01); // �ڲ�����������
    P1_PD |= GPIO_P12_PULL_PD(0x01); // �ڲ�����������
    // �����������������������ž��൱�ڸ���״̬��ͬʱ�ܹ�����һЩ��·�ĸ���

    // λ����
    P12 = 1;
}

/**
 * @brief P01����Ϊ���ģʽ
 *        P01�ڿ����������ӵ���LED6������
 */
void p01_output_config(void)
{
    P0_MD0 &= ~(GPIO_P01_MODE_SEL(0x1)); // ��Ӧ�ļĴ���������
    P0_MD0 |= GPIO_P01_MODE_SEL(0x1);    // ����Ϊ���ģʽ

    FOUT_S01 = GPIO_FOUT_AF_FUNC; // ѡ��AF�������

    P0_PU |= GPIO_P01_PULL_UP(0x01); // �ڲ�����������
    P0_PD |= GPIO_P01_PULL_PD(0x01); // �ڲ�����������
    // �����������������������ž��൱�ڸ���״̬��ͬʱ�ܹ�����һЩ��·�ĸ���

    // λ����
    P01 = 0;
}

/**
 * @brief P03����Ϊ���ģʽ
 */
void p03_output_config(void)
{
    P0_MD0 &= ~(GPIO_P03_MODE_SEL(0x1)); // ��Ӧ�ļĴ���������
    P0_MD0 |= GPIO_P03_MODE_SEL(0x1);    // ����Ϊ���ģʽ

    FOUT_S03 = GPIO_FOUT_AF_FUNC; // ѡ��AF�������
}

/**
 * @brief P26����Ϊ���ģʽ
 */
void p26_output_config(void)
{
    P2_MD1 &= ~(GPIO_P26_MODE_SEL(0x1)); // ��Ӧ�ļĴ���������
    P2_MD1 |= GPIO_P26_MODE_SEL(0x1);    // ����Ϊ���ģʽ

    FOUT_S26 = GPIO_FOUT_AF_FUNC; // ѡ��AF�������

    P2_PU |= GPIO_P26_PULL_UP(0x01); // �ڲ�����������
    P2_PD |= GPIO_P26_PULL_PD(0x01); // �ڲ�����������
    // �����������������������ž��൱�ڸ���״̬��ͬʱ�ܹ�����һЩ��·�ĸ���

    // λ����
    P26 = 0;
}

// p06���ó���������ģʽ
void p06_input_config(void)
{
    // ����Ϊ����ģʽ
    P0_MD1 &= ~(GPIO_P06_MODE_SEL(0x3));
    // ����Ϊ����
    P0_PU |= (GPIO_P06_PULL_UP(0x1));
}

// p07���ó���������ģʽ
void p07_input_config(void)
{
    // ����Ϊ����ģʽ
    P0_MD1 &= ~(GPIO_P07_MODE_SEL(0x3));
    // ����Ϊ����
    P0_PU |= (GPIO_P07_PULL_UP(0x1));
}

// p10���ó���������ģʽ
void p10_input_config(void)
{
    // ����Ϊ����ģʽ
    P1_MD0 &= ~(GPIO_P10_MODE_SEL(0x3));
    // ����Ϊ����
    P1_PU |= (GPIO_P10_PULL_UP(0x1));
}

// p13���ó���������ģʽ
void p13_input_config(void)
{
    // ����Ϊ����ģʽ
    P1_MD0 &= ~(GPIO_P13_MODE_SEL(0x3));
    // ����Ϊ����
    P1_PU |= (GPIO_P13_PULL_UP(0x1));
}

// p14���ó���������ģʽ
void p14_input_config(void)
{
    // ����Ϊ����ģʽ
    P1_MD1 &= ~(GPIO_P14_MODE_SEL(0x3));
    // ����Ϊ����
    P1_PU |= (GPIO_P14_PULL_UP(0x1));
}

// // p17���ó���������ģʽ
// void p17_input_config(void)
// {
//     // ����Ϊ����ģʽ
//     P1_MD1 &= ~(GPIO_P17_MODE_SEL(0x3));
//     // ����Ϊ����
//     P1_PU |= (GPIO_P17_PULL_UP(0x1));
// }

// p21���ó���������ģʽ
void p21_input_config(void)
{
    // ����Ϊ����ģʽ
    P2_MD0 &= ~(GPIO_P21_MODE_SEL(0x3));
    // ����Ϊ����
    P2_PU |= (GPIO_P21_PULL_UP(0x1));
}

// p22���ó���������ģʽ
void p22_input_config(void)
{
    // ����Ϊ����ģʽ
    P2_MD0 &= ~(GPIO_P22_MODE_SEL(0x3));
    // ����Ϊ����
    P2_PU |= (GPIO_P22_PULL_UP(0x1));
}

// p23���ó���������ģʽ
void p23_input_config(void)
{
    // ����Ϊ����ģʽ
    P2_MD0 &= ~(GPIO_P23_MODE_SEL(0x3));
    // ����Ϊ����
    P2_PU |= (GPIO_P23_PULL_UP(0x1));
}

// p25���ó���������ģʽ
void p25_input_config(void)
{
    // ����Ϊ����ģʽ
    P2_MD1 &= ~(GPIO_P25_MODE_SEL(0x3));
    // ����Ϊ����
    P2_PU |= (GPIO_P25_PULL_UP(0x1));
}

// p27���ó���������ģʽ
void p27_input_config(void)
{
    // ����Ϊ����ģʽ
    P2_MD1 &= ~(GPIO_P27_MODE_SEL(0x3));
    // ����Ϊ����
    P2_PU |= (GPIO_P27_PULL_UP(0x1));
}

// P30���ó���������ģʽ
void p30_input_config(void)
{
    // ����Ϊ����ģʽ
    P3_MD0 &= ~(GPIO_P30_MODE_SEL(0x3));
    // ����Ϊ����
    P3_PU |= (GPIO_P30_PULL_UP(0x1));
}

// // P31���ó���������ģʽ
// void p31_input_config(void)
// {
//     // ����Ϊ����ģʽ
//     P3_MD0 &= ~(GPIO_P31_MODE_SEL(0x3));
//     // ����Ϊ����
//     P3_PU |= (GPIO_P31_PULL_UP(0x1));
// }
