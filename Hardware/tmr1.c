#include "tmr1.h"

// #define TMR1_CNT_TIME 152 // 152 * 0.65625us 约等于100us

// 定时器定时周期 (单位:Hz)
// 周期值 = 系统时钟 / 定时器分频 / 频率 - 1
#define TMR1_PERIOD (SYSCLK / 128 / 10000 - 1) // 10000Hz,100us

// volatile bit tmr1_flag = 0; // TMR1中断服务函数中会置位的标志位
volatile u32 tmr1_cnt = 0; // 定时器TMR1的计数值（每次在中断服务函数中会加一）

/**
 * @brief 配置定时器TMR1，配置完成后，定时器默认关闭
 */
void tmr1_config(void)
{
    __SetIRQnIP(TMR1_IRQn, TMR1_IQn_CFG); // 设置中断优先级（TMR1）
    __DisableIRQ(TMR1_IRQn);              // 禁用中断
    IE_EA = 1;                            // 打开总中断

    // 清除TMR1的计数值
    TMR1_CNTL = 0;
    TMR1_CNTH = 0;

    TMR1_CONL &= ~TMR_PRESCALE_SEL(0x03); // 清除TMR1的预分频配置寄存器
    TMR1_CONL |= TMR_PRESCALE_SEL(0x07);  // 定时器预分频
    TMR1_CONL &= ~TMR_MODE_SEL(0x03);     // 清除TMR1的模式配置寄存器
    TMR1_CONL |= TMR_MODE_SEL(0x01);      // 配置TMR1的模式为计数器模式，最后对系统时钟的脉冲进行计数

    TMR1_CONH &= ~TMR_PRD_PND(0x01); // 清除TMR1的计数标志位，表示未完成计数
    TMR1_CONH |= TMR_PRD_IRQ_EN(1);  // 使能TMR1的计数中断

    // 配置TMR1的计数周期
    TMR1_PRH = TMR_PERIOD_VAL_H((TMR1_PERIOD >> 8) & 0xFF); // 周期值
    TMR1_PRL = TMR_PERIOD_VAL_L((TMR1_PERIOD >> 0) & 0xFF);

    TMR1_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除TMR1的时钟源配置寄存器
    TMR1_CONL |= TMR_SOURCE_SEL(0x05); // 配置TMR1的时钟源，不用任何时钟
}

/**
 * @brief 开启定时器TMR1，开始计时
 */
void tmr1_enable(void)
{
    // 重新给TMR1配置时钟
    TMR1_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除定时器的时钟源配置寄存器
    TMR1_CONL |= TMR_SOURCE_SEL(0x06);    // 配置定时器的时钟源，使用系统时钟

    __EnableIRQ(TMR1_IRQn); // 使能中断
    IE_EA = 1;              // 打开总中断
}

/**
 * @brief 关闭定时器，清空计数值
 */
void tmr1_disable(void)
{
    // 不给定时器提供时钟，让它停止计数
    TMR1_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除定时器的时钟源配置寄存器
    TMR1_CONL |= TMR_SOURCE_SEL(0x05);    // 配置定时器的时钟源，不用任何时钟

    // 清除定时器的计数值
    TMR1_CNTL = 0;
    TMR1_CNTH = 0;

    __DisableIRQ(TMR1_IRQn); // 关闭中断（不使能中断）
}

// TMR1中断服务函数
void TIMR1_IRQHandler(void) interrupt TMR1_IRQn
{
    // 进入中断设置IP，不可删除
    __IRQnIPnPush(TMR1_IRQn);

    // ---------------- 用户函数处理 -------------------

    // 周期中断
    if (TMR1_CONH & TMR_PRD_PND(0x1))
    {
        TMR1_CONH |= TMR_PRD_PND(0x1); // 清除pending

        tmr1_cnt++;

#if 0
        // 如果到了5s
        if (TMR1_cnt == 1000)
        {
            TMR1_cnt = 0;
            TMR1_flag = 1; // 给对应的标志位置一
        }
#endif
    }

    // 退出中断设置IP，不可删除
    __IRQnIPnPop(TMR1_IRQn);
}