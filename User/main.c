/**
 ******************************************************************************
 * @file    main.c
 * @author  HUGE-IC Application Team
 * @version V1.0.0
 * @date    05-11-2022
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2021 HUGE-IC</center></h2>
 *
 * 版权说明后续补上
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "include.h"

// 更新时间
void time_update(void)
{
    if (tmr4_cnt >= 1000) // 如果计时满1s(1000ms)
    {
        tmr4_cnt -= 1000; // 为了尽量让时间准确，这里是 -=1000 ，而不是清零

        if (fun_info.save_info.time_sec < 59) // 最大加到59s，测试通过
        {
            fun_info.save_info.time_sec++;
        }
        else
        {
            fun_info.save_info.time_sec = 0;
            fun_info_save(); // 每分钟保存一次时间到flash

            // printf("cur min %d \n", (u16)fun_info.time_min);

            if (fun_info.save_info.time_min < 59) // 最大加到59min，测试通过
            {
                fun_info.save_info.time_min++;
            }
            else
            {
                fun_info.save_info.time_min = 0;
                // printf("cur hour %d \n", (u16)fun_info.time_hour);

                if (fun_info.save_info.time_hour < 23) // 最大加到23h，测试通过
                {
                    fun_info.save_info.time_hour++;
                }
                else
                {
                    fun_info.save_info.time_hour = 0;
                    fun_info.save_info.time_min = 0;
                    fun_info.save_info.time_sec = 0;
                }
            }
        }

#if USE_MY_DEBUG // 测试单片机发送的数据
        // printf("cur sec %d \n", (u16)fun_info.time_sec);

        // 测试用，可以在这里修改数据，然后发送
        {
            /*
                #define TEST_SEND_BATTERY // 测试通过
                #define TEST_SEND_BRAKE // MP5上面没有显示，串口有发送对应的数据
                #define TEST_SEND_LEFT_TURN // 测试通过
                #define TEST_SEND_RIGHT_TURN // 测试通过
                #define TEST_SEND_HIGH_BEAM // 测试通过

                #define TEST_SEND_SPEED // 测试通过，但是要注意时速的显示范围
                #define TEST_SEND_ENGINE_SPEED // 测试通过
                #define TEST_SEND_FUEL // 测试通过

                #define TEST_SEND_TEMP_OF_WATER // 没有显示，改用水温报警
                #define TEST_SEND_TOTAL_MILEAGE // 测试通过
                #define TEST_SEND_SUBTOTAL_MILEAGE // 测试通过

                #define TEST_SEND_TOUCH_KEY
                #define TEST_SEND_TEMP_OF_WATER_WARNING // 测试通过
                #define TEST_SEND_VOLTAGE_OF_BATTERY // 测试通过
            */

            // #define TEST_SEND_VOLTAGE_OF_BATTERY

#ifdef TEST_SEND_BATTERY
            // =============================================
            // 测试发送电池电量--单位：百分比
            if (fun_info.battery < 100)
            {
                fun_info.battery++;
            }
            else
            {
                fun_info.battery = 0;
            }

            // printf("cur battery %d \%\n", (u16)fun_info.battery);
            flag_get_battery = 1;
#endif

#ifdef TEST_SEND_BRAKE
            // =============================================
            // 测试发送 刹车状态
            if (fun_info.brake)
            {
                fun_info.brake = 0;
            }
            else
            {
                fun_info.brake = 1;
            }

            flag_get_brake = 1;
#endif

#ifdef TEST_SEND_LEFT_TURN
            // =============================================
            // 测试发送 左转向灯
            if (fun_info.left_turn)
            {
                fun_info.left_turn = 0;
            }
            else
            {
                fun_info.left_turn = 1;
            }

            flag_get_left_turn = 1;
#endif

#ifdef TEST_SEND_RIGHT_TURN
            // =============================================
            // 测试发送 右转向灯
            if (fun_info.right_turn)
            {
                fun_info.right_turn = 0;
            }
            else
            {
                fun_info.right_turn = 1;
            }

            flag_get_right_turn = 1;
#endif

#ifdef TEST_SEND_HIGH_BEAM
            // #if 1
            // =============================================
            // 测试发送 远光灯 状态
            if (fun_info.high_beam)
            {
                fun_info.high_beam = 0;
            }
            else
            {
                fun_info.high_beam = 1;
            }

            flag_get_high_beam = 1;
#endif

#ifdef TEST_SEND_SPEED
            // =============================================
            // 测试发送 时速
            if (fun_info.speed < 0xFFFF)
            {
                fun_info.speed++;
            }
            else
            {
                fun_info.speed = 0;
            }

            flag_get_speed = 1;
#endif

#ifdef TEST_SEND_ENGINE_SPEED
            // =============================================
            // 测试发送 发动机转速
            if (fun_info.engine_speeed < (0xFFFF - 1000))
            {
                fun_info.engine_speeed += 1000;
            }
            else
            {
                fun_info.engine_speeed = 1000;
            }

            flag_get_engine_speed = 1;
#endif

#ifdef TEST_SEND_FUEL
            // =============================================
            // 测试发送 油量
            if (fun_info.fuel < 0xFF)
            {
                fun_info.fuel++;
            }
            else
            {
                fun_info.fuel = 0;
            }

            flag_get_fuel = 1;

#endif

#ifdef TEST_SEND_TEMP_OF_WATER
            // =============================================
            // 测试发送 水温
            if (fun_info.temp_of_water < 0xFF)
            {
                fun_info.temp_of_water++;
            }
            else
            {
                fun_info.temp_of_water = 0;
            }

            flag_get_temp_of_water = 1;
#endif

#ifdef TEST_SEND_TOTAL_MILEAGE
            // =============================================
            // 测试发送 大计里程
            // fun_info.save_info.total_mileage = 90000;
            if (fun_info.save_info.total_mileage < (0xFFFFFFFF - 1000))
            {
                fun_info.save_info.total_mileage += 1000;
            }
            else
            {
                fun_info.save_info.total_mileage = 0xFFFF0000;
            }

            flag_get_total_mileage = 1;

#endif

#ifdef TEST_SEND_SUBTOTAL_MILEAGE
            // =============================================
            // 测试发送 小计里程
            if (fun_info.save_info.subtotal_mileage < 1000)
            {
                fun_info.save_info.subtotal_mileage++;
            }
            else
            {
                fun_info.save_info.subtotal_mileage = 0;
            }

            flag_get_sub_total_mileage = 1;
#endif

#ifdef TEST_SEND_TEMP_OF_WATER_WARNING
            // =============================================
            // 测试发送 水温报警

            if (fun_info.flag_is_in_water_temp_warning)
            {
                fun_info.flag_is_in_water_temp_warning = 0;
            }
            else
            {
                fun_info.flag_is_in_water_temp_warning = 1;
            }

            flag_set_temp_of_water_warning = 1;
#endif

#ifdef TEST_SEND_VOLTAGE_OF_BATTERY
            // 测试 发送电池电压
            if (fun_info.voltage_of_battery < (0xFFFF - 10))
            {
                fun_info.voltage_of_battery += 10;
            }
            else
            {
                fun_info.voltage_of_battery = 10000;
            }

            flag_get_voltage_of_battery = 1;
#endif
        }

#endif // #if USE_MY_DEBUG // 测试单片机发送的数据
    }
}

void main(void)
{
    // 看门狗默认打开, 复位时间2s
    system_init();

    WDT_KEY = WDT_KEY_VAL(0xDD); //  关闭看门狗

    // 关闭HCK和HDA的调试功能
    WDT_KEY = 0x55;  // 解除写保护
    IO_MAP &= ~0x01; // 清除这个寄存器的值，实现关闭HCK和HDA引脚的调试功能（解除映射）
    WDT_KEY = 0xBB;

    /* 用户代码初始化接口 */
    // user_init();
    {                    // user_init();
        fun_info_init(); // 初始化用于存放信息的变量

        tmr0_config();           // 串口检测数据超时需要使用到的定时器
        uart0_config();          // 发送和接收指令使用到的串口
        pin_level_scan_config(); // 刹车、转向灯、挡位的检测引脚配置

        tmr1_config(); // 检测一段时间内的脉冲个数所需的定时器
        // tmr2_config(); // 时速扫描使用到的定时器
        speed_scan_config();        // 时速扫描的配置
        engine_speed_scan_config(); // 发动机转速扫描的配置

        // tmr3_config();     // 定时将里程写入flash所需的定时器
        tmr4_cnt_config(); // 配置用于后台更新时间的定时器（时钟默认打开）

        p03_output_config(); // 输出高电平用的引脚，到时候检测acc,才输出高电平，现在它一开始就是高电平
        P03 = 1;             // 输出高电平，使能屏幕显示

        adc_config();
    } // user_init();

#if USE_MY_DEBUG
    // 测试用，一上电打印flash中存放的信息
    printf("year %d month %d day %d \n", (u16)fun_info.save_info.year, (u16)fun_info.save_info.month, (u16)fun_info.save_info.day);
#endif

#if 0 // 测试发送日期和时间
//     // 当前日期
//     fun_info.save_info.year = 2024;
//     fun_info.save_info.month = 0x0B;
//     fun_info.save_info.day = 0x0A;

// #if USE_MY_DEBUG
//     printf("year %d month %d day %d \n", (u16)fun_info.save_info.year, (u16)fun_info.save_info.month, (u16)fun_info.save_info.day);
// #endif

    // // 当前时间
    // fun_info.save_info.time_hour = 21;
    // fun_info.save_info.time_min = 45;
    // fun_info.save_info.time_sec = 55;
#endif

    tmr1_enable(); // 打开 检测时速、发动机转速、更新里程、定时检测油量 使用的定时器

    // 测试用到的配置：
    // P1_MD0 &= (~GPIO_P11_MODE_SEL(0x3));
    // P1_MD0 |= GPIO_P11_MODE_SEL(0x1); // 输出模式
    // FOUT_S11 |= GPIO_FOUT_AF_FUNC;

    /* 系统主循环 */
    while (1)
    {
        // 扫描状态是否变化，如果变化则更新标志位，更新状态的信息到结构体中
        // pin_level_scan();
        // speed_scan();        // 扫描当前时速
        // engine_speed_scan(); // 扫描当前发动机转速
        // adc_scan(); // adc扫描，并转换成百分比的形式发送
        // mileage_scan(); // 里程扫描（大计里程扫描+小计里程扫描）

        adc_sel_pin(ADC_PIN_TOUCH); // 内部至少占用1ms
        ad_key_scan();              // 检测触摸按键，内部会发送检测到的按键状态

        // 1. 测试 从串口接收指令，是否能够正确识别
        // {
        //     uart0_scan_handle();  // 检查串口接收缓冲区的数据是否符合协议,如果有正确的指令，会存到另一个缓冲区中
        //     instruction_scan();   // 扫描是否有合法的指令
        //     instruction_handle(); // 扫描是否有对应的获取/状态更新操作
        // }

        // 2. 测试 时速和发动机的转速的扫描，并且不会阻塞主循环
        // 注意要先打开对应的定时器
        {
            // engine_speed_scan(); // 检测发动机转速
            // speed_scan();        // 检测时速
            // mileage_scan();      // 检测大计里程和小计里程
        }

        // 油量检测：
        adc_sel_pin(ADC_PIN_FUEL); // 内部至少占用1ms
        fuel_capacity_scan();

        // 水温检测：
        adc_sel_pin(ADC_PIN_TEMP_OF_WATER);
        temp_of_water_scan();

        // 电池电量检测：
        adc_sel_pin(ADC_PIN_BATTERY);
        battery_scan();

        uart0_scan_handle();  // 检查串口接收缓冲区的数据是否符合协议,如果有正确的指令，会存到另一个缓冲区中
        instruction_scan();   // 扫描是否有合法的指令
        instruction_handle(); // 扫描是否有对应的获取/状态更新操作

        // delay_ms(500);

        // 注意要先打开对应的定时器:
        time_update(); // 更新时间
    }
}

/**
 * @}
 */

/*************************** (C) COPYRIGHT 2022 HUGE-IC ***** END OF FILE *****/
