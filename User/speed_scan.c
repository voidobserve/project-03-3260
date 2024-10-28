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
#ifdef DEVELOPMENT_BOARD
    // 使用IO中断来对脉冲计数
    __SetIRQnIP(P1_IRQn, P1_IQn_CFG); // 设置中断优先级
    __EnableIRQ(P1_IRQn);             // 使能P1中断
    IE_EA = 1;                        // 使能总开关

    P1_MD0 &= ~GPIO_P13_MODE_SEL(0x3); // 输入模式
    P1_PD |= GPIO_P13_PULL_PD(0x1);    // 配置为下拉
    P1_IMK |= GPIO_P13_IRQ_MASK(0x1);  // 使能IO中断
    P1_TRG0 &= ~GPIO_P13_TRG_SEL(0x3);
    P1_TRG0 |= GPIO_P13_TRG_SEL(0x2); // 配置上升沿触发
#endif

#ifdef CIRCUIT_BOARD
    // 使用IO中断来对脉冲计数
    __SetIRQnIP(P0_IRQn, P0_IQn_CFG); // 设置中断优先级
    __EnableIRQ(P0_IRQn);             // 使能P1中断
    IE_EA = 1;                        // 使能总开关

    P0_MD0 &= ~GPIO_P02_MODE_SEL(0x3); // 输入模式
    P0_PD |= GPIO_P02_PULL_PD(0x1);    // 配置为下拉
    P0_IMK |= GPIO_P02_IRQ_MASK(0x1);  // 使能IO中断
    P0_TRG0 &= ~GPIO_P02_TRG_SEL(0x3);
    P0_TRG0 |= GPIO_P02_TRG_SEL(0x2); // 配置上升沿触发
#endif
}

// 关闭速度扫描的相关中断，清空相关的计数
static void speed_scan_disable(void)
{
#ifdef DEVELOPMENT_BOARD
    __DisableIRQ(P1_IRQn); // 关闭P1中断
#endif
#ifdef CIRCUIT_BOARD
    __DisableIRQ(P0_IRQn); // 关闭P0中断
#endif
    pulse_cnt = 0; // 清空脉冲计数
    tmr1_disable();
    tmr1_cnt = 0;
}

// 开启速度扫描相关的中断
static void speed_scan_enable(void)
{
    pulse_cnt = 0;
    tmr1_cnt = 0;
#ifdef DEVELOPMENT_BOARD
    __EnableIRQ(P1_IRQn); // 使能P1中断
#endif
#ifdef CIRCUIT_BOARD
    __EnableIRQ(P0_IRQn); // 使能P0中断
#endif

    tmr1_enable();
}

// 获取每小时走过多少毫米
static u32 get_speed_mm_per_hour(void)
{
    u32 tmp = 0;
    speed_scan_enable();
    while (tmr1_cnt < 2500)
        ;                                           // 等待250ms
    tmp = pulse_cnt * MM_PER_TURN / PULSE_PER_TURN; // 250ms走过来多少毫米
    distance += tmp;                                // 存放走过的距离，单位：毫米(因为没有使用浮点类型,需要累加,否则后续统计里程时就会丢失数据)
    speed_scan_disable();
    tmp *= 14400; // 计算得出1小时走过的毫米     // tmp = tmp * 4 * 3600;
    return tmp;
}

// 采集一次速度数据
static u32 get_speed(void)
{
#if 0
    u32 speed_km_per_hour = get_speed_mm_per_hour(); // 得到每小时走过的毫米数
    // speed_km_per_hour /= 1000000;                  // 换算成每小时走过的千米数
    return speed_km_per_hour / 1000000;
#endif

    return (get_speed_mm_per_hour() / 1000000);
}

// 速度扫描函数，已经验证可以使用
void speed_scan(void)
{
    static u32 last_speed = 0;   // 记录上一次采集到的速度
    u32 cur_speed = get_speed(); // 当前采集的速度
    // printf("--------%lu km/h\n", cur_speed);

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
}

#ifdef DEVELOPMENT_BOARD
// P1中断服务函数
void P1_IRQHandler(void) interrupt P1_IRQn
{
    // Px_PND寄存器写任何值都会清标志位
    u8 p1_pnd = P1_PND;

    // 进入中断设置IP，不可删除
    __IRQnIPnPush(P1_IRQn);
    // ---------------- 用户函数处理 -------------------
    if (p1_pnd & GPIO_P13_IRQ_PNG(0x1))
    {
        pulse_cnt++;
    }
    P1_PND = p1_pnd; // 清P1中断标志位
    // -------------------------------------------------
    // 退出中断设置IP，不可删除
    __IRQnIPnPop(P1_IRQn);
}
#endif

#ifdef CIRCUIT_BOARD
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
#endif
