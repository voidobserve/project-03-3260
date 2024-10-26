#include "tmr0.h"

#define TMR0_CNT_TIME 152 // 152 * 0.65625us 约等于100us

// volatile bit tmr0_flag = 0; // tmr0中断服务函数中会置位的标志位
volatile u32 tmr0_cnt = 0;            // 定时器TMR0的计数值（每次在中断服务函数中会加一）

/**
 * @brief 配置定时器TMR0
 */
void tmr0_config(void)
{
    __SetIRQnIP(TMR0_IRQn, TMR0_IQn_CFG); // 设置中断优先级（TMR0）

    TMR0_CONL &= ~TMR_PRESCALE_SEL(0x03); // 清除TMR0的预分频配置寄存器
    // 配置TMR0的预分频，为32分频，即21MHz / 32 = 0.65625MHz，约0.67us计数一次
    // （实际测试和计算得出这个系统时钟是21MHz的，但是还是有些误差，不是准确的21MHz）
    TMR0_CONL |= TMR_PRESCALE_SEL(0x05);
    TMR0_CONL &= ~TMR_MODE_SEL(0x03); // 清除TMR0的模式配置寄存器
    TMR0_CONL |= TMR_MODE_SEL(0x01);  // 配置TMR0的模式为计数器模式，最后对系统时钟的脉冲进行计数

    TMR0_CONH &= ~TMR_PRD_PND(0x01); // 清除TMR0的计数标志位，表示未完成计数
    TMR0_CONH |= TMR_PRD_IRQ_EN(1);  // 使能TMR0的计数中断

    // 配置TMR0的计数周期
    TMR0_PRL = (unsigned char)(TMR0_CNT_TIME % 255);
    TMR0_PRH = (unsigned char)(TMR0_CNT_TIME / 255);

    // 清除TMR0的计数值
    TMR0_CNTL = 0;
    TMR0_CNTH = 0;

    TMR0_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除TMR0的时钟源配置寄存器
    // TMR0_CONL |= TMR_SOURCE_SEL(0x07); // 配置TMR0的时钟源，使用系统时钟
    TMR0_CONL |= TMR_SOURCE_SEL(0x05); // 配置TMR0的时钟源，不用任何时钟
                                       // __EnableIRQ(TMR0_IRQn);			   // 使能中断

    __DisableIRQ(TMR0_IRQn); // 禁用中断
    IE_EA = 1;               // 打开总中断
}

/**
 * @brief 开启定时器TMR0，开始计时
 */
void tmr0_enable(void)
{
    // 重新给TMR0配置时钟
    TMR0_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除定时器的时钟源配置寄存器
    TMR0_CONL |= TMR_SOURCE_SEL(0x06);    // 配置定时器的时钟源，使用系统时钟（约21MHz）

    __EnableIRQ(TMR0_IRQn); // 使能中断
    IE_EA = 1;              // 打开总中断
}

/**
 * @brief 关闭定时器0，清空计数值
 */
void tmr0_disable(void)
{
    // 不给定时器提供时钟，让它停止计数
    TMR0_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除定时器的时钟源配置寄存器
    TMR0_CONL |= TMR_SOURCE_SEL(0x05);    // 配置定时器的时钟源，不用任何时钟

    // 清除定时器的计数值
    TMR0_CNTL = 0;
    TMR0_CNTH = 0;

    __DisableIRQ(TMR0_IRQn); // 关闭中断（不使能中断）
}

// TMR0中断服务函数
void TIMR0_IRQHandler(void) interrupt TMR0_IRQn
{
    // 进入中断设置IP，不可删除
    __IRQnIPnPush(TMR0_IRQn);

    // ---------------- 用户函数处理 -------------------

    // 周期中断
    if (TMR0_CONH & TMR_PRD_PND(0x1))
    {
        TMR0_CONH |= TMR_PRD_PND(0x1); // 清除pending

        tmr0_cnt++;

        // P11 = ~P11; // 测试用，看看中断触发周期是否正确

#if 0
        // 如果到了5s
        if (TMR0_cnt == 1000)
        {
            TMR0_cnt = 0;
            TMR0_flag = 1; // 给对应的标志位置一
        }
#endif
    }

    // 退出中断设置IP，不可删除
    __IRQnIPnPop(TMR0_IRQn);
}