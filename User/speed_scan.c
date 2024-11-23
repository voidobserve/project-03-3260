#include "speed_scan.h"

// 多少个脉冲表示一圈
#ifndef SPEED_SCAN_PULSE_PER_TURN
#define SPEED_SCAN_PULSE_PER_TURN (16)
#endif // 多少个脉冲表示一圈

// 车轮一圈表示多少毫米
#ifndef SPEED_SCAN_MM_PER_TURN
#define SPEED_SCAN_MM_PER_TURN (1795) // 一圈1795毫米
#endif                                // 车轮一圈表示多少毫米

volatile u32 speed_scan_time_cnt = 0; // 速度扫描时，用到的时间计数值，会在定时器中断中累加

volatile u32 detect_speed_pulse_cnt = 0; // 检测时速的脉冲计数值

// 时速扫描的配置
void speed_scan_config(void)
{
    // 使用IO中断来对脉冲计数
    __SetIRQnIP(P0_IRQn, P0_IQn_CFG); // 设置中断优先级
    __EnableIRQ(P0_IRQn);             // 使能P1中断
    IE_EA = 1;                        // 使能总开关

    P0_MD0 &= ~GPIO_P02_MODE_SEL(0x3); // 输入模式
    P0_PU |= GPIO_P02_PULL_UP(0x1);    // 配置为上拉
    // P0_PD |= GPIO_P02_PULL_PD(0x01); // 下拉
    P0_IMK |= GPIO_P02_IRQ_MASK(0x1);  // 使能IO中断
    P0_TRG0 &= ~GPIO_P02_TRG_SEL(0x3); // 清空触发沿配置的寄存器
    P0_TRG0 |= GPIO_P02_TRG_SEL(0x2);  // 配置上升沿触发
    // P0_TRG0 |= GPIO_P02_TRG_SEL(0x1); // 配置下降沿触发
}

// 速度扫描函数
void speed_scan(void)
{
#if 0
    // 以下这种方法，万一速度保持不变，只会发送第一次检测到的数据，
    // 如果这次检测到的数据没有被MP5正确接收，MP5就显示不了该数据 
    static u32 last_speed = 0; // 记录上一次采集到的速度
    static u8 speed_increases_cnt = 0; // 检测速度是否在增加的计数值
    static u8 speed_decreases_cnt = 0; // 检测速度是否在减少的计数值
#endif

    u32 cur_speed = 0;                    // 记录当前采集到的速度
    static u32 cur_speed_average_val = 0; // 存放当前速度的平均值(单位：km/h)

    static u8 speed_scan_cnt = 0;

    if (speed_scan_time_cnt >= SPEED_SCAN_TIME_MS) // 如果经过了 xx ms
    {
        speed_scan_time_cnt -= SPEED_SCAN_TIME_MS; // 为了准确，减去这部分时间对应的计数值，而不是直接清零

        /*
            计算 xx ms内走过了多少毫米
            xx ms内走过了多少毫米 == 当前扫描时间内检测到的脉冲个数 / 车轮一圈对应多少个脉冲 * 一圈对应 xx 毫米
            换成单片机可以计算的形式：
            xx ms内走过了多少毫米 == 当前扫描时间内检测到的脉冲个数 * 一圈对应 xx 毫米 / 车轮一圈对应多少个脉冲
        */
        cur_speed = detect_speed_pulse_cnt * SPEED_SCAN_MM_PER_TURN / SPEED_SCAN_PULSE_PER_TURN;

        // printf("cur pulse cnt %lu \n", detect_speed_pulse_cnt); // 临时测试用

        detect_speed_pulse_cnt = 0; // 清空脉冲计数
        distance += cur_speed;      // 存放走过的距离，单位：毫米

        /*
            已知在扫描时间内走过了xx mm
            时速的计算公式:
            扫描时间内走过的距离 / 1000 * (1 / 扫描时间对1s的占比) * 3.6
                扫描时间内走过的距离 / 1000，转换成 m/扫描时间 的单位
                * (1 / 扫描时间对1s的占比)，转换成以s为单位的速度
                * 3.6，因为 1m/s == 3.6km/h，最后转换成 以km/h的单位
            转换成单片机可以计算的形式：
            时速 == 扫描时间内走过的距离 * 36 * (1 / 扫描时间对1s的占比) / 10000；

            逐渐变换成单片机可以计算的形式：
            cur_speed = cur_speed * 36 * (1 / (SPEED_SCAN_TIME_MS / 1000)) / 10000;
            cur_speed = cur_speed * 36 * 1000 / SPEED_SCAN_TIME_MS / 10000;
            cur_speed = cur_speed * 36 / SPEED_SCAN_TIME_MS / 10;
        */
        cur_speed = (cur_speed * 36) / SPEED_SCAN_TIME_MS / 10;

        // printf("cur speed %lu \n", cur_speed);

#if 0
        // 以下这种方法，万一速度保持不变，只会发送第一次检测到的数据，
        // 如果这次检测到的数据没有被MP5正确接收，MP5就显示不了该数据
        if (cur_speed > last_speed)
        {
            speed_decreases_cnt = 0;
            speed_increases_cnt++;

            if (speed_increases_cnt >= SPEED_SCAN_FILTER_CNT) //  
            {
                last_speed = cur_speed;

                // 对准备发送的时速做限制
                if (cur_speed >= 999) // 999km/h
                {
                    cur_speed = 999;
                }

                fun_info.speed = cur_speed;
                flag_get_speed = 1;
            }
        }
        else if (cur_speed < last_speed)
        {
            speed_increases_cnt = 0;
            speed_decreases_cnt++;
            if (speed_decreases_cnt >= SPEED_SCAN_FILTER_CNT) //  
            {
                // 对准备发送的时速做限制
                if (cur_speed >= 999) // 999km/h
                {
                    cur_speed = 999;
                }

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
#endif

        if (speed_scan_cnt < SPEED_SCAN_FILTER_CNT)
        {
            // 如果未达到重复检测的次数
            speed_scan_cnt++;
            cur_speed_average_val += cur_speed; // 累加当前得到的时速(单位：km/h)
        }
        else
        {
            // 如果达到了重复检测的次数
            speed_scan_cnt = 0;
            cur_speed_average_val /= SPEED_SCAN_FILTER_CNT; // 时速取平均值
            fun_info.speed = cur_speed_average_val;         // 存放得到的时速
            cur_speed_average_val = 0;                      // 清空变量的值
#if USE_MY_DEBUG

            if (fun_info.speed != 0)
            {
                printf("cur speed %lu km/h\n", fun_info.speed);
            }

#endif

            // 限制要发送的时速:
            if (fun_info.speed > 999)
            {
                fun_info.speed = 0;
            }
            else
            {
                // 如果时速未超出能够显示的范围
                flag_get_speed = 1; //
            }
        }
    }
}
