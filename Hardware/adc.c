#include "adc.h"

// 用自定义的序号来标记测量adc的引脚，方便切换adc的引脚时进行判断
#define ADC_PIN_BATTERY 0x01 // 测量电池电压的引脚
#define ADC_PIN_TOUCH 0x02   // 测量触摸芯片发送过来的电压的引脚

u16 adc_val = 0; // adc值，0~4095
// bit adc_done_flag = 0; // adc转换完成标志

// adc引脚配置，使用adc时还需要切换到对应的引脚通道
void adc_pin_config(void)
{
    // 配置P23为模拟输入模式
    P2_MD0 |= GPIO_P23_MODE_SEL(0x3); // 设为模拟模式
    // P04--测量电池电压的引脚
    P0_MD0 |= GPIO_P04_MODE_SEL(0x3); // 模拟模式
    // P05--测量触摸IC传过来的电压的引脚
    P0_MD0 |= GPIO_P05_MODE_SEL(0x3); // 模拟模式

    // ADC配置
    ADC_ACON1 &= ~(ADC_VREF_SEL(0x7) | ADC_EXREF_SEL(0x1)); // 清除电压选择，关闭外部参考电压
    ADC_ACON1 &= ~(ADC_INREF_SEL(0x01));                    // 关闭内部参考电压

    // 不能使用VCCA，待修改，看看芯片最低的工作电压和实际提供的电压
    ADC_ACON1 |= ADC_VREF_SEL(0x06) | ADC_TEN_SEL(0x3); // 选择内部参考电压VCCA，关闭测试信号
    ADC_ACON0 = ADC_CMP_EN(0x1) |                       // 打开ADC中的CMP使能信号
                ADC_BIAS_EN(0x1) |                      // 打开ADC偏置电流能使信号
                ADC_BIAS_SEL(0x1);                      // 偏置电流选择：1x
}

// 切换adc扫描的引脚
// pin_index--引脚序号
void adc_sel_pin(u8 pin_index)
{
    switch (pin_index)
    {
    case ADC_PIN_BATTERY:                  // P04
        ADC_CHS0 = ADC_ANALOG_CHAN(0x04) | // P04通路
                   ADC_EXT_SEL(0x0);       // 选择外部通路
        ADC_CFG0 |= ADC_CHAN0_EN(0x1) |    // 使能通道0转换
                    ADC_EN(0x1);           // 使能A/D转换
        break;

    case ADC_PIN_TOUCH:                    // P05
        ADC_CHS0 = ADC_ANALOG_CHAN(0x05) | // P05通路
                   ADC_EXT_SEL(0x0);       // 选择外部通路
        ADC_CFG0 |= ADC_CHAN0_EN(0x1) |    // 使能通道0转换
                    ADC_EN(0x1);           // 使能A/D转换
        break;
    }
    delay_ms(1); // 等待ADC模块配置稳定，需要等待20us以上
}

// 获取adc值，存放到变量adc_val中(adc单次转换)
u16 adc_single_convert(void)
{
    ADC_CFG0 |= ADC_CHAN0_TRG(0x1); // 触发ADC0转换
    while (!(ADC_STA & ADC_CHAN0_DONE(0x1)))
        ;                                           // 等待转换完成
    ADC_STA = ADC_CHAN0_DONE(0x1);                  // 清除ADC0转换完成标志位
    return ((ADC_DATAH0 << 4) | (ADC_DATAL0 >> 4)); // 读取ADC0的值
}

// adc采集+滤波
void adc_sample_filter(void)
{
    u8 i = 0;
    u32 tmp;
    for (i = 0; i < 32; i++)
    {
        tmp += adc_single_convert();
        if (i >= 1)
        {
            tmp >>= 1; // 相当于tmp /= 2;
        }
    }

    adc_val = (u16)tmp;
}

void adc_showval(void)
{
    adc_sel_pin(ADC_PIN_BATTERY);
    adc_sample_filter();

// 连接串口查看ADC值
#if USE_MY_DEBUG
    // printf("ADC_PIN_BATTERY = %d\r\n", adc_val);
#endif
    delay_ms(500);

    adc_sel_pin(ADC_PIN_TOUCH);
    adc_sample_filter();

    // 连接串口查看ADC值
#if USE_MY_DEBUG
    // printf("ADC_PIN_TOUCH = %d\r\n", adc_val);
#endif
    delay_ms(500);
}

// 测试用，还未能真正检测，只能将检测到的ad值直接转换成对应的百分比
void adc_scan(void)
{
    adc_sel_pin(ADC_PIN_BATTERY);
    adc_sample_filter();                                // 滤波完成之后可能还会出现跳变，应该在后续加入死区判断
    fun_info.battery = (u8)((u32)adc_val * 100 / 4095); // u16的adc_val乘以100会直接溢出，这里要强转为u32

#if USE_MY_DEBUG
    // 测试用：
    // printf("ADC_PIN_BATTERY = %d\r\n", adc_val);
    // printf("battery : %d %%\n", (u16)fun_info.battery); // %d打印是两个字节，不强转会越界访问
#endif

    flag_get_battery = 1;

    delay_ms(10); // adc采样不能太频繁
}
