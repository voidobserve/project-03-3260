#include "tmr4.h"

void tmr4_pwm_config(void)
{
    // 配置TIMER4的PWM端口：P21--TMR4_PWM
    P2_MD0 &= ~GPIO_P21_MODE_SEL(0x03);
    P2_MD0 |= GPIO_P21_MODE_SEL(0x01);
    FOUT_S21 = GPIO_FOUT_TMR4_PWMOUT; // 选择tmr4_pwm_o

#define PEROID_VAL (SYSCLK / 1 / 1000 - 1) // 周期值=系统时钟/分频/频率 - 1
    // TIMER4配置1kHz频率PWM
    __EnableIRQ(TMR4_IRQn);                                      // 打开TIMER4模块中断
    TMR_ALLCON = TMR4_CNT_CLR(0x1);                              // Timer4 计数清零
    TMR4_CAP10 = TMR4_PRD_VAL_L((PEROID_VAL >> 0) & 0xFF);       // 周期低八位寄存器
    TMR4_CAP11 = TMR4_PRD_VAL_H((PEROID_VAL >> 8) & 0xFF);       // 周期高八位寄存器
    TMR4_CAP20 = TMR4_PWM_VAL_L(((PEROID_VAL / 2) >> 0) & 0xFF); // 比较值高八位寄存器
    TMR4_CAP21 = TMR4_PWM_VAL_H(((PEROID_VAL / 2) >> 8) & 0xFF); // 比较值低八位寄存器
    TMR4_CON0 = TMR4_PRESCALE_SEL(0x0) |                         // 不分频
                TMR4_SOURCE_SEL(0x7) |                           // 计数源选择系统时钟
                TMR4_MODE_SEL(0x1);                              // PWM输出模式
    TMR4_EN = TMR4_EN(0x1);                                      // 使能定时器

    IE_EA = 1; // 使能总中断
}
