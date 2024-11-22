#include "adc.h"


volatile u16 adc_val = 0; // adc值，0~4095
// bit adc_done_flag = 0; // adc转换完成标志

// adc配置，使用adc时还需要切换到对应的引脚通道
void adc_config(void)
{
    // // 配置P23为模拟输入模式
    // P2_MD0 |= GPIO_P23_MODE_SEL(0x3); // 设为模拟模式
    
    // P04--测量电池电压的引脚
    P0_MD0 |= GPIO_P04_MODE_SEL(0x3); // 模拟模式
    // P05--测量触摸IC传过来的电压的引脚
    P0_MD0 |= GPIO_P05_MODE_SEL(0x3); // 模拟模式

    // ADC配置
    ADC_ACON1 &= ~(ADC_VREF_SEL(0x7) | ADC_EXREF_SEL(0x1)); // 清除电压选择，关闭外部参考电压
    ADC_ACON1 &= ~(ADC_INREF_SEL(0x01));                    // 关闭内部参考电压

    // 不能使用VCCA，待修改，看看芯片最低的工作电压和实际提供的电压
    ADC_ACON1 |= ADC_VREF_SEL(0x06) | ADC_TEN_SEL(0x3); // 选择内部VCCA作为参考电压，关闭测试信号
    ADC_ACON0 = ADC_CMP_EN(0x1) |                       // 打开ADC中的CMP使能信号
                ADC_BIAS_EN(0x1) |                      // 打开ADC偏置电流能使信号
                ADC_BIAS_SEL(0x1);                      // 偏置电流选择：1x
}

// 切换adc扫描的引脚
// adc_pin--adc引脚，在对应的枚举类型中定义
void adc_sel_pin(u8 adc_pin)
{
    switch (adc_pin)
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
static u16 __adc_single_convert(void)
{
    ADC_CFG0 |= ADC_CHAN0_TRG(0x1); // 触发ADC0转换
    while (!(ADC_STA & ADC_CHAN0_DONE(0x1)))
        ;                                           // 等待转换完成
    ADC_STA = ADC_CHAN0_DONE(0x1);                  // 清除ADC0转换完成标志位
    return ((ADC_DATAH0 << 4) | (ADC_DATAL0 >> 4)); // 读取ADC0的值
}

// adc采集+滤波
u16 adc_getval(void)
{
    u16 __adc_val_tmp = 0; // 存放单次采集到的ad值
    u32 __adc_val_sum = 0; // 存放所有采集到的ad值的累加
    u16 __get_adcmax = 0; // 存放采集到的最大的ad值
    u16  __get_adcmin = 0xFFFF; // 存放采集到的最小的ad值(初始值为最大值)
    u8 i = 0;

    for (i = 0; i < 20; i++)
    {
        __adc_val_tmp = __adc_single_convert(); // 获取一次ad值
        if (i < 2)
            continue; // 丢弃前两次采样的
        if (__adc_val_tmp > __get_adcmax)
            __get_adcmax = __adc_val_tmp; // 更新当前采集到的最大值
        if (__adc_val_tmp < __get_adcmin)
            __get_adcmin = __adc_val_tmp; // 更新当前采集到的最小值
        __adc_val_sum += __adc_val_tmp;
    }

    __adc_val_sum -= __get_adcmax;          // 去掉一个最大
    __adc_val_sum -= __get_adcmin;          // 去掉一个最小
    __adc_val_tmp = (__adc_val_sum >> 4); // 除以16，取平均值

    return __adc_val_tmp;
}

// void adc_showval(void)
// {
//     adc_sel_pin(ADC_PIN_BATTERY);
//     adc_sample_filter();

// // 连接串口查看ADC值
// #if USE_MY_DEBUG
//     // printf("ADC_PIN_BATTERY = %d\r\n", adc_val);
// #endif
//     delay_ms(500);

//     adc_sel_pin(ADC_PIN_TOUCH);
//     adc_sample_filter();

//     // 连接串口查看ADC值
// #if USE_MY_DEBUG
//     // printf("ADC_PIN_TOUCH = %d\r\n", adc_val);
// #endif
//     delay_ms(500);
// }

// // 测试用，还未能真正检测，只能将检测到的ad值直接转换成对应的百分比
// void adc_scan(void)
// {
//     adc_sel_pin(ADC_PIN_BATTERY);
//     adc_sample_filter();                                // 滤波完成之后可能还会出现跳变，应该在后续加入死区判断
//     fun_info.battery = (u8)((u32)adc_val * 100 / 4095); // u16的adc_val乘以100会直接溢出，这里要强转为u32

// #if USE_MY_DEBUG
//     // 测试用：
//     // printf("ADC_PIN_BATTERY = %d\r\n", adc_val);
//     // printf("battery : %d %%\n", (u16)fun_info.battery); // %d打印是两个字节，不强转会越界访问
// #endif

//     flag_get_battery = 1;

//     delay_ms(10); // adc采样不能太频繁
// }
