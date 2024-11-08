#include "speed_scan.h"

// 多少个脉冲表示一圈
#ifndef PULSE_PER_TURN
#define PULSE_PER_TURN 16
#endif // 多少个脉冲表示一圈

// 车轮一圈表示多少毫米
#ifndef MM_PER_TURN
#define MM_PER_TURN 1795 // 一圈1795毫米
#endif                   // 车轮一圈表示多少毫米

static volatile u32 pulse_cnt = 0; // 脉冲计数值
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

// // 关闭速度扫描的相关中断，清空相关的计数
// static void speed_scan_disable(void)
// {

//     __DisableIRQ(P0_IRQn); // 关闭P0中断

//     pulse_cnt = 0; // 清空脉冲计数
//     tmr1_disable();
//     tmr1_cnt = 0;
// }

// // 开启速度扫描相关的中断
// static void speed_scan_enable(void)
// {
//     pulse_cnt = 0;
//     tmr1_cnt = 0;

//     __EnableIRQ(P0_IRQn); // 使能P0中断

//     tmr1_enable();
// }

// 获取每小时走过多少毫米
// static u32 get_speed_mm_per_hour(void)
// {
//     // u32 tmp = 0;
//     // speed_scan_enable();
//     // while (tmr1_cnt < 2500)
//     //     ;                                           // 等待250ms
//     // tmp = pulse_cnt * MM_PER_TURN / PULSE_PER_TURN; // 250ms走过来多少毫米
//     // distance += tmp;                                // 存放走过的距离，单位：毫米(因为没有使用浮点类型,需要累加,否则后续统计里程时就会丢失数据)
//     // speed_scan_disable();
//     // tmp *= 14400; // 计算得出1小时走过的毫米     // tmp = tmp * 4 * 3600;
//     // return tmp;

//     u32 tmp = 0;
//     if (tmr2_cnt >= 2500) // 如果经过了250ms
//     {
//         // 计算250ms内走过了多少毫米
//         tmp = pulse_cnt *MM_PER_TURN / PULSE_PER_TURN
//                                            distance += tmp; // 存放走过的距离，单位：毫米(因为没有使用浮点类型,需要累加,否则后续统计里程时就会丢失数据)
//         // 计算得出1小时能走过多少毫米
//         // tmp = tmp * 4 * 3600;
//         tmp *= 14400;

//         pulse_cnt = 0;
//         tmr2_cnt = 0;
//     }
// }

// 采集一次速度数据
static u32 get_speed(void)
{
#if 0
    u32 speed_km_per_hour = get_speed_mm_per_hour(); // 得到每小时走过的毫米数
    // speed_km_per_hour /= 1000000;                  // 换算成每小时走过的千米数
    return speed_km_per_hour / 1000000;
#endif

    return (get_speed_mm_per_hour() / 1000000); // 换算成 km
}

// 速度扫描函数，已经验证可以使用
void speed_scan(void)
{
    static u32 last_speed = 0; // 记录上一次采集到的速度

    static u8 scan_cnt = 0; // 扫描次数，用在多次检测速度的功能中，以确保速度确实在变化

    u32 cur_speed = 0;
    u32 tmp = 0;

    if (tmr2_cnt >= 1000) // 如果经过了100ms
    {
        // 计算1000ms内走过了多少毫米
        tmp = pulse_cnt * MM_PER_TURN / PULSE_PER_TURN;
        distance += tmp; // 存放走过的距离，单位：毫米(因为没有使用浮点类型,需要累加,否则后续统计里程时就会丢失数据)
        // 计算得出1小时能走过多少毫米
        // tmp = tmp * 4 * 3600;
        tmp *= 14400;

        cur_speed = tmp / 1000000; // 换算成 km/h的单位

        // u32 cur_speed = get_speed(); // 当前采集的速度
        // printf("--------%lu km/h\n", cur_speed);

        pulse_cnt = 0; // 清除脉冲计数
        tmr2_cnt = 0;  // 清除定时器计数

#if 0
        { // 比对多次检测到的速度，确认速度是在变化的，才更新信息
            if (((cur_speed > last_speed) && (cur_speed - last_speed > 2)) ||
                ((cur_speed < last_speed) && (last_speed - cur_speed > 2)))
            {
                // 如果本次采集到的数据与上一次采集到的数据差值超过了1，可以直接发送出去

                // 测试用：
                // printf("%lu km/h\n", cur_speed);

                last_speed = cur_speed;

                fun_info.speed = cur_speed;
                flag_get_speed = 1;
            }
            else if (((cur_speed > last_speed) && (cur_speed - last_speed < 2)) ||
                     ((cur_speed < last_speed) && (last_speed - cur_speed < 2)))
            {
                // 如果本次采集到的数据与上一次采集到的数据差值只有1，再采集几次
                u8 cnt = 0;

                if ((cur_speed > last_speed) && (cur_speed - last_speed < 2))
                {
                    // 如果测得速度是增加的，要确定它是真的在加速
                    u8 i = 0;

                    for (i = 0; i < 3; i++)
                    {
                        cur_speed = get_speed();

                        if ((cur_speed > last_speed) && (cur_speed - last_speed < 2))
                        {
                            cnt++;
                        }
                    }
                }
                else if ((cur_speed < last_speed) && (last_speed - cur_speed < 2))
                {
                    // 如果测得速度是减少的，要确定它是真的在减速
                    u8 i = 0;
                    for (i = 0; i < 3; i++)
                    {
                        cur_speed = get_speed();

                        if ((cur_speed < last_speed) && (last_speed - cur_speed < 2))
                        {
                            cnt++;
                        }
                    }
                }

                if (cnt >= 2)
                {
                    // 测试用：
                    // printf("%lu km/h\n", cur_speed);

                    last_speed = cur_speed;

                    fun_info.speed = cur_speed;
                    flag_get_speed = 1;
                }
            }
            else if (cur_speed == 0 && last_speed != 0)
            {
                // 测试用：
                // printf("%lu km/h\n", cur_speed);

                fun_info.speed = cur_speed;
                flag_get_speed = 1;
            }
        } // 比对多次检测到的速度，确认速度是在变化的，才更新信息
#endif
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
        pulse_cnt++;
    }
    P0_PND = p0_pnd; // 清P0中断标志位
    // -------------------------------------------------
    // 退出中断设置IP，不可删除
    __IRQnIPnPop(P0_IRQn);
}
