#include "engine_speed_scan.h"

// 发动机每转一圈，能检测到的脉冲个数
#ifndef ENGINE_SPEED_SCAN_PULSE_PER_TURN
#define ENGINE_SPEED_SCAN_PULSE_PER_TURN (16)
#endif

volatile u32 engine_speed_scan_time_cnt = 0; // 发动机转速扫描时，用到的时间计数值，会在定时器中断中累加

volatile u32 detect_engine_pulse_cnt = 0; // 检测发送机转速的脉冲计数值，会在IO中断内累加

// 发动机转速的相关配置
void engine_speed_scan_config(void)
{
    // 使用IO中断来对脉冲计数
    __SetIRQnIP(P1_IRQn, P1_IQn_CFG); // 设置中断优先级
    __EnableIRQ(P1_IRQn);             // 使能P1中断
    IE_EA = 1;                        // 使能总开关

    P1_MD1 &= ~GPIO_P15_MODE_SEL(0x3); // 输入模式
    P1_PU |= GPIO_P15_PULL_UP(0x1);    // 配置为上拉
    P1_IMK |= GPIO_P15_IRQ_MASK(0x1);  // 使能IO中断
    P1_TRG1 &= ~GPIO_P15_TRG_SEL(0x3);
    P1_TRG1 |= GPIO_P15_TRG_SEL(0x2); // 配置上升沿触发
}

// 发动机转速扫描
void engine_speed_scan(void)
{
#define CONVER_ONE_MINUTE_TO_MS (60000)
    u32 rpm = 0;

    if (engine_speed_scan_time_cnt >= ENGINE_SPEED_SCAN_TIME_MS) // 如果已经到了累计的时间
    {
        engine_speed_scan_time_cnt -= ENGINE_SPEED_SCAN_TIME_MS; // 为了准确，减去这部分时间对应的计数值，而不是直接清零
        // 下面是根据250ms检测到的脉冲个数来计算出每分钟的转速
        // 250ms * 4 * 60 == 1min
        // rpm = detect_engine_pulse_cnt * 240 / ENGINE_SPEED_SCAN_PULSE_PER_TURN; // 计算得出1min的转速
        
        // 计算得出1min的转速
        // 扫描时间内发动机转过的圈数 == 扫描时间内采集到的脉冲个数 / 发动机一圈对应的脉冲个数
        // 1min内发动机的转速 == 1min / 扫描时间 * 扫描时间内发动机转过的圈数
        // 换算成单片机能够计算的形式：
        // 1min内发动机的转速 == 扫描时间内发动机转过的圈数 * 1min / 扫描时间
        // 1min内发动机的转速 == 扫描时间内采集到的脉冲个数* 1min / 发动机一圈对应的脉冲个数 / 扫描时间
        rpm = detect_engine_pulse_cnt * (CONVER_ONE_MINUTE_TO_MS / ENGINE_SPEED_SCAN_TIME_MS) / ENGINE_SPEED_SCAN_PULSE_PER_TURN; 
        detect_engine_pulse_cnt = 0; // 计算完成后，清空计数值

        // 限制待发送的发动机转速
        if (rpm >= 65535)
        {
            rpm = 65535;
        }

        fun_info.engine_speeed = rpm; //
#if USE_MY_DEBUG
        // printf("engine speed %lu rpm\n", rpm);
#endif
        flag_get_engine_speed = 1; // 多久更新一次状态还未确定
    }
}

