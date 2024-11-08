#include "engine_speed_scan.h"

// 发动机每转一圈，能检测到的脉冲个数
#ifndef ENGINE_PULSE_PER_TURN
#define ENGINE_PULSE_PER_TURN 16
#endif

static volatile u32 pulse_cnt = 0; // 脉冲计数值

// 发动机转速的相关配置
void engine_speed_scan_config(void)
{
    // 使用IO中断来对脉冲计数
    __SetIRQnIP(P1_IRQn, P1_IQn_CFG); // 设置中断优先级
    __EnableIRQ(P1_IRQn);             // 使能P1中断
    IE_EA = 1;                        // 使能总开关

    P1_MD0 &= ~GPIO_P12_MODE_SEL(0x3); // 输入模式
    P1_PD |= GPIO_P12_PULL_PD(0x1);    // 配置为下拉
    P1_IMK |= GPIO_P12_IRQ_MASK(0x1);  // 使能IO中断
    P1_TRG0 &= ~GPIO_P12_TRG_SEL(0x3);
    P1_TRG0 |= GPIO_P12_TRG_SEL(0x2); // 配置上升沿触发
}

// 获取发动机每分钟转速
// static u32 get_engine_speed_rpm(void)
// {
//     // return (get_turns_per_250ms() * 4 * 60);

//     u32 tmp = 0;
//     pulse_cnt = 0;
//     tmr1_cnt = 0;
//     tmr1_enable();
//     __EnableIRQ(P1_IRQn); // 使能中断


//     while (tmr1_cnt < 2500)
//     {
//     }; // 等待250ms

//     // 计算出每min(250ms * 4 * 60 == 1min)转了多少圈
//     tmp = pulse_cnt * 240 / ENGINE_PULSE_PER_TURN;

//     tmr1_disable();


//     __DisableIRQ(P1_IRQn); // 禁用中断

//     tmr1_cnt = 0;
//     pulse_cnt = 0;

//     return tmp;
// }

// 发动机转速扫描
void engine_speed_scan(void)
{
    // u32 rpm = get_engine_speed_rpm();
    // // printf("rpm: %lu\n", rpm);

    // fun_info.engine_speeed = rpm;
    // flag_get_engine_speed = 1;
    
    u32 rpm = 0;
    
    if (tmr1_cnt >= 2500) // 如果已经过了250ms
    {
        // 250ms * 4 * 60 == 1min
        // 下面是根据250ms检测到的脉冲个数来计算出每分钟的转速
        rpm = pulse_cnt * 240 / ENGINE_PULSE_PER_TURN; 

        tmr1_cnt = 0;
        pulse_cnt = 0;
        fun_info.engine_speeed = rpm;
        printf("engine speed %lu rpm\n", rpm);
        // flag_get_engine_speed = 1; // 多久更新一次状态还未确定
    }

}


// P1中断服务函数
void P1_IRQHandler(void) interrupt P1_IRQn
{
    // Px_PND寄存器写任何值都会清标志位
    u8 p1_pnd = P1_PND;

    // 进入中断设置IP，不可删除
    __IRQnIPnPush(P1_IRQn);
    // ---------------- 用户函数处理 -------------------

    if (p1_pnd & GPIO_P12_IRQ_PNG(0x1))
    {
        pulse_cnt++;
    }
    P1_PND = p1_pnd; // 清P1中断标志位

    // -------------------------------------------------
    // 退出中断设置IP，不可删除
    __IRQnIPnPop(P1_IRQn);
}
