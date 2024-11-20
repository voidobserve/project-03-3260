#include "speed_scan.h"

// 多少个脉冲表示一圈
#ifndef PULSE_PER_TURN
#define PULSE_PER_TURN (16)
#endif // 多少个脉冲表示一圈

// 车轮一圈表示多少毫米
#ifndef MM_PER_TURN
#define MM_PER_TURN (1795) // 一圈1795毫米
#endif                   // 车轮一圈表示多少毫米

static volatile u32 detect_speed_pulse_cnt = 0; // 脉冲计数值
volatile u32 distance = 0;         // 存放每次扫描时走过的路程（单位：毫米）-->用于里程表的计数

// 时速扫描的配置
void speed_scan_config(void)
{
    // 使用IO中断来对脉冲计数
    __SetIRQnIP(P0_IRQn, P0_IQn_CFG); // 设置中断优先级
    __EnableIRQ(P0_IRQn);             // 使能P1中断
    IE_EA = 1;                        // 使能总开关

    P0_MD0 &= ~GPIO_P02_MODE_SEL(0x3); // 输入模式
    P0_PD |= GPIO_P02_PULL_PD(0x1);    // 配置为下拉
    P0_IMK |= GPIO_P02_IRQ_MASK(0x1);  // 使能IO中断
    P0_TRG0 &= ~GPIO_P02_TRG_SEL(0x3);
    P0_TRG0 |= GPIO_P02_TRG_SEL(0x2); // 配置上升沿触发
}

// 速度扫描函数
void speed_scan(void)
{
    static u32 last_speed = 0;                  // 记录上一次采集到的速度

    static u8 speed_increases_cnt = 0; // 检测速度是否在增加的计数值
    static u8 speed_decreases_cnt = 0; // 检测速度是否在减少的计数值

    u32 cur_speed = 0; // 记录当前采集到的速度

    if (tmr2_cnt >= 1000) // 如果经过了100ms
    {
        tmr2_cnt = 0; // 清除定时器计数
        // 计算1000ms内走过了多少毫米
        // 脉冲个数 / 一圈有多少个脉冲 * 一圈表示走过了多少mm == 该段时间内走过的距离(单位:mm)
        cur_speed = detect_speed_pulse_cnt * MM_PER_TURN / PULSE_PER_TURN;
        distance += cur_speed; // 存放走过的距离，单位：毫米(因为没有使用浮点类型,需要累加,否则后续统计里程时就会丢失数据)
        // 计算得出1小时能走过多少毫米
        // tmp = tmp * 4 * 3600;
        cur_speed *= 14400;
        cur_speed /= 1000000; // 换算成 km/h的单位
        detect_speed_pulse_cnt = 0; // 清除脉冲计数

        if (cur_speed > last_speed)
        {
            speed_decreases_cnt = 0;
            speed_increases_cnt++;

            if (speed_increases_cnt >= 10)
            {
                last_speed = cur_speed;

                fun_info.speed = cur_speed;
                flag_get_speed = 1;
            }
        }
        else if (cur_speed < last_speed)
        {
            speed_increases_cnt = 0;
            speed_decreases_cnt++;
            if (speed_decreases_cnt >= 10)
            {
                last_speed = cur_speed;

                fun_info.speed = cur_speed;
                flag_get_speed = 1;
            }
        }
        else
        {
            // 如果速度未发生变化
            speed_increases_cnt = 0;
            speed_decreases_cnt = 0;
        }
    }
}

// P0中断服务函数
void P0_IRQHandler(void) interrupt P0_IRQn
{
    // Px_PND寄存器写任何值都会清标志位
    u8 p0_pnd = P0_PND;

    // 进入中断设置IP，不可删除
    __IRQnIPnPush(P0_IRQn);
    // ---------------- 用户函数处理 -------------------
    if (p0_pnd & GPIO_P02_IRQ_PNG(0x1))
    {
        detect_speed_pulse_cnt++;
    }
    P0_PND = p0_pnd; // 清P0中断标志位
    // -------------------------------------------------
    // 退出中断设置IP，不可删除
    __IRQnIPnPop(P0_IRQn);
}
