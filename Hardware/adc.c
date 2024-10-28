#include "adc.h"

// ���Զ�����������ǲ���adc�����ţ������л�adc������ʱ�����ж�
#define ADC_PIN_BATTERY 0x01 // ������ص�ѹ������
#define ADC_PIN_TOUCH 0x02   // ��������оƬ���͹����ĵ�ѹ������

u16 adc_val = 0; // adcֵ��0~4095
// bit adc_done_flag = 0; // adcת����ɱ�־

// adc�������ã�ʹ��adcʱ����Ҫ�л�����Ӧ������ͨ��
void adc_pin_config(void)
{
    // ����P23Ϊģ������ģʽ
    P2_MD0 |= GPIO_P23_MODE_SEL(0x3); // ��Ϊģ��ģʽ
    // P04--������ص�ѹ������
    P0_MD0 |= GPIO_P04_MODE_SEL(0x3); // ģ��ģʽ
    // P05--��������IC�������ĵ�ѹ������
    P0_MD0 |= GPIO_P05_MODE_SEL(0x3); // ģ��ģʽ

    // ADC����
    ADC_ACON1 &= ~(ADC_VREF_SEL(0x7) | ADC_EXREF_SEL(0x1)); // �����ѹѡ�񣬹ر��ⲿ�ο���ѹ
    ADC_ACON1 &= ~(ADC_INREF_SEL(0x01));                    // �ر��ڲ��ο���ѹ

    // ����ʹ��VCCA�����޸ģ�����оƬ��͵Ĺ�����ѹ��ʵ���ṩ�ĵ�ѹ
    ADC_ACON1 |= ADC_VREF_SEL(0x06) | ADC_TEN_SEL(0x3); // ѡ���ڲ��ο���ѹVCCA���رղ����ź�
    ADC_ACON0 = ADC_CMP_EN(0x1) |                       // ��ADC�е�CMPʹ���ź�
                ADC_BIAS_EN(0x1) |                      // ��ADCƫ�õ�����ʹ�ź�
                ADC_BIAS_SEL(0x1);                      // ƫ�õ���ѡ��1x
}

// �л�adcɨ�������
// pin_index--�������
void adc_sel_pin(u8 pin_index)
{
    switch (pin_index)
    {
    case ADC_PIN_BATTERY:                  // P04
        ADC_CHS0 = ADC_ANALOG_CHAN(0x04) | // P04ͨ·
                   ADC_EXT_SEL(0x0);       // ѡ���ⲿͨ·
        ADC_CFG0 |= ADC_CHAN0_EN(0x1) |    // ʹ��ͨ��0ת��
                    ADC_EN(0x1);           // ʹ��A/Dת��
        break;

    case ADC_PIN_TOUCH:                    // P05
        ADC_CHS0 = ADC_ANALOG_CHAN(0x05) | // P05ͨ·
                   ADC_EXT_SEL(0x0);       // ѡ���ⲿͨ·
        ADC_CFG0 |= ADC_CHAN0_EN(0x1) |    // ʹ��ͨ��0ת��
                    ADC_EN(0x1);           // ʹ��A/Dת��
        break;
    }
    delay_ms(1); // �ȴ�ADCģ�������ȶ�����Ҫ�ȴ�20us����
}

// ��ȡadcֵ����ŵ�����adc_val��(adc����ת��)
u16 adc_single_convert(void)
{
    ADC_CFG0 |= ADC_CHAN0_TRG(0x1); // ����ADC0ת��
    while (!(ADC_STA & ADC_CHAN0_DONE(0x1)))
        ;                                           // �ȴ�ת�����
    ADC_STA = ADC_CHAN0_DONE(0x1);                  // ���ADC0ת����ɱ�־λ
    return ((ADC_DATAH0 << 4) | (ADC_DATAL0 >> 4)); // ��ȡADC0��ֵ
}

// adc�ɼ�+�˲�
void adc_sample_filter(void)
{
    u8 i = 0;
    u32 tmp;
    for (i = 0; i < 32; i++)
    {
        tmp += adc_single_convert();
        if (i >= 1)
        {
            tmp >>= 1; // �൱��tmp /= 2;
        }
    }

    adc_val = (u16)tmp;
}

void adc_showval(void)
{
    adc_sel_pin(ADC_PIN_BATTERY);
    adc_sample_filter();

// ���Ӵ��ڲ鿴ADCֵ
#if USE_MY_DEBUG
    // printf("ADC_PIN_BATTERY = %d\r\n", adc_val);
#endif
    delay_ms(500);

    adc_sel_pin(ADC_PIN_TOUCH);
    adc_sample_filter();

    // ���Ӵ��ڲ鿴ADCֵ
#if USE_MY_DEBUG
    // printf("ADC_PIN_TOUCH = %d\r\n", adc_val);
#endif
    delay_ms(500);
}

// �����ã���δ��������⣬ֻ�ܽ���⵽��adֱֵ��ת���ɶ�Ӧ�İٷֱ�
void adc_scan(void)
{
    adc_sel_pin(ADC_PIN_BATTERY);
    adc_sample_filter();                                // �˲����֮����ܻ���������䣬Ӧ���ں������������ж�
    fun_info.battery = (u8)((u32)adc_val * 100 / 4095); // u16��adc_val����100��ֱ�����������ҪǿתΪu32

#if USE_MY_DEBUG
    // �����ã�
    // printf("ADC_PIN_BATTERY = %d\r\n", adc_val);
    // printf("battery : %d %%\n", (u16)fun_info.battery); // %d��ӡ�������ֽڣ���ǿת��Խ�����
#endif

    flag_get_battery = 1;

    delay_ms(10); // adc��������̫Ƶ��
}
