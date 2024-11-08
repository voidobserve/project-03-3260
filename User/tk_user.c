/**
 ******************************************************************************
 * @file    User/tk_user.c
 * @author  HUGE-IC Application Team
 * @version V1.0.0
 * @date    05-20-2022
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2022 TAIXIN-IC</center></h2>
 *
 *
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "include.h"
#include "my_config.h"



/**
 * @brief  用户代码初始化函数接口.
 * @param  None
 * @retval None
 */
void user_init(void)
{
    fun_info_init(); // 初始化用于存放信息的变量


    tmr0_config(); // 串口检测数据超时需要使用到的定时器
    uart0_config(); // 发送和接收指令使用到的串口
    pin_level_scan_config(); // 刹车、转向灯、挡位的检测引脚配置

    tmr1_config(); // 检测一段时间内的脉冲个数所需的定时器
    // tmr2_pwm_config(); // 测试用的PWM
    speed_scan_config(); // 时速扫描的配置
    engine_speed_scan_config(); // 发动机转速扫描的配置

    tmr3_config(); // 定时将里程写入flash所需的定时器

    // mileage_init(); // 从flash中读出大计里程和小计里程数据（已经并入fun_info结构体变量中）

    p03_output_config(); // 输出高电平用的引脚，到时候检测ad到了一定值，再让它输出高电平，现在它一开始就是高电平
    P03 = 1;

    adc_pin_config();
}

#if 0
#ifdef DEVELOPMENT_BOARD // 开发板上对应额触摸按键函数接口
/**
 * @brief  用户代码循环执行函数接口.（触摸按键扫描函数接口）
 * @param  None
 * @retval None
 */
void user_handle(void)
{
    volatile unsigned long int KeyOnOutput = __tk_key_flag; // __tk_key_flag单次按键标志

    static volatile unsigned long int KeyOld = 0; // 用于存放长按时，对应的按键

    // 是否长按的标志，0--否，1--是
    // 需要保存长按的状态，表示定时器一直在发送键值
    static unsigned char long_touch_flag = 0; // 上一次扫描到的按键是否为长按的标志

    static volatile u32 cnt = 0; // 长按计数值

    u32 i = 0; // 循环计数值

    u16 send_data = 0; // 要发送的带有键值的数据

    if (KeyOnOutput && 0 == long_touch_flag && cnt < TK_LONG_KEY_TIME) // 如果有按键按下（并且之前扫描到的不是长按）
    {
        // 通过循环判断是否为长按
        for (i = 0; i < 500000; i++) // 实际上用不到这么大的数字，到了设定的长按时间就会退出
        {
            // 按键扫描函数，使用了库里面的接口（闭源库）
            // 这个函数会更新__tk_key_flag的值
            delay_ms(10);
            __tk_scan();

            if (KeyOnOutput == __tk_key_flag)
            {
                // 如果按键键值一致，说明按键一直被按下，未松开
                cnt++;
            }
            else
            {
                // 如果按键键值不一致或是松开了按键
                // cnt = 0;
                break;
            }
            if (cnt > TK_LONG_KEY_TIME) // 如果长按超过了这里设置的时间
            {
                long_touch_flag = 1; // 标记为长按
                break;
            }
        }

        if (cnt > TK_LONG_KEY_TIME)
        {
            long_touch_flag = 1; // 标记为长按
        }
        else
        {
            // 如果是短按
            cnt = 0; // 清除长按的计数值

            // 判断当前触摸的键值，做对应的操作
            if (TK_CH0_VALIB == KeyOnOutput)
            {
                // 如果开发板的TK1被触摸
                display_data ^= 0x0100; // 对应的LED状态取反，后面会调用led_display()刷新
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH0_VALIB == KeyOnOutput)
                    {
                        // 如果检测到是同一个按键按下，说明是双击
                        // 等待手指松开按键，不然发送完带有双击状态的键值后会再发送一次单击状态的键值
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK1);
                        return;
                    }
                }

                // 如果不是同一个按键按下或是没有值，说明是短按
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK1); // 发送带有状态的键值
            }
            else if (TK_CH4_VALIB == KeyOnOutput)
            {
                // 如果开发板的TK2被触摸
                display_data ^= 0x0200; // 对应的LED状态取反，后面会调用led_display()刷新
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH4_VALIB == KeyOnOutput)
                    {
                        // 如果检测到是同一个按键按下，说明是双击
                        // 等待手指松开按键，不然发送完带有双击状态的键值后会再发送一次单击状态的键值
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK2);
                        return;
                    }
                }

                // 如果不是同一个按键按下或是没有值，说明是短按
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK2); // 发送带有状态的键值
            }
            else if (TK_CH5_VALIB == KeyOnOutput)
            {
                // 如果开发板的TK3被触摸
                display_data ^= 0x0400; // 对应的LED状态取反，后面会调用led_display()刷新
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH5_VALIB == KeyOnOutput)
                    {
                        // 如果检测到是同一个按键按下，说明是双击
                        // 等待手指松开按键，不然发送完带有双击状态的键值后会再发送一次单击状态的键值
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK3);
                        return;
                    }
                }

                // 如果不是同一个按键按下或是没有值，说明是短按
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK3); // 发送带有状态的键值
            }
            else if (TK_CH6_VALIB == KeyOnOutput)
            {
                // 如果开发板的TK4被触摸
                display_data ^= 0x0800; // 对应的LED状态取反，后面会调用led_display()刷新
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH6_VALIB == KeyOnOutput)
                    {
                        // 如果检测到是同一个按键按下，说明是双击
                        // 等待手指松开按键，不然发送完带有双击状态的键值后会再发送一次单击状态的键值
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK4);
                        return;
                    }
                }

                // 如果不是同一个按键按下或是没有值，说明是短按
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK4); // 发送带有状态的键值
            }
        }
    }

    if (1 == long_touch_flag && 0 != KeyOnOutput)
    {
        KeyOld = KeyOnOutput;

        // 如果之前是长按，现在还未松开
        // 判断当前触摸的键值，做对应的操作
        if (TK_CH0_VALIB == KeyOnOutput)
        {
            // 如果开发板的TK1被触摸
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                display_data ^= 0x0100; // 对应的LED状态取反，后面会调用led_display()刷新
                led_display();          // LED状态更新显示
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK1);
            }

            // 若还不松开手，则每隔150ms发送一次带有持续状态信息的键值信号
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // 原本是填90的，但是软件延时有误差，这里只能慢慢调试
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK1);
                }
            }

            // display_data ^= 0x0100; // 对应的LED状态取反，后面会调用led_display()刷新
            // led_display();          // LED状态更新显示
        }
        else if (TK_CH4_VALIB == KeyOnOutput)
        {
            // 如果开发板的TK2被触摸
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                display_data ^= 0x0200; // 对应的LED状态取反，后面会调用led_display()刷新
                led_display();          // LED状态更新显示
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK2);
            }

            // 若还不松开手，则每隔150ms发送一次带有持续状态信息的键值信号
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // 原本是填90的，但是软件延时有误差，这里只能慢慢调试
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK2);
                }
            }

            // display_data ^= 0x0200; // 对应的LED状态取反，后面会调用led_display()刷新
            // led_display();          // LED状态更新显示
        }
        else if (TK_CH5_VALIB == KeyOnOutput)
        {
            // 如果开发板的TK3被触摸
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                display_data ^= 0x0400; // 对应的LED状态取反，后面会调用led_display()刷新
                led_display();          // LED状态更新显示
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK3);
            }

            // 若还不松开手，则每隔150ms发送一次带有持续状态信息的键值信号
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // 原本是填90的，但是软件延时有误差，这里只能慢慢调试
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK3);
                }
            }

            // display_data ^= 0x0400; // 对应的LED状态取反，后面会调用led_display()刷新
            // led_display();          // LED状态更新显示
        }
        else if (TK_CH6_VALIB == KeyOnOutput)
        {
            // 如果开发板的TK4被触摸
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                display_data ^= 0x0800; // 对应的LED状态取反，后面会调用led_display()刷新
                led_display();          // LED状态更新显示
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK4);
            }

            // 若还不松开手，则每隔150ms发送一次带有持续状态信息的键值信号
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // 原本是填90的，但是软件延时有误差，这里只能慢慢调试
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK4);
                }
            }

            // display_data ^= 0x0800; // 对应的LED状态取反，后面会调用led_display()刷新
            // led_display();          // LED状态更新显示
        }
    }
    else if (1 == long_touch_flag && 0 == KeyOnOutput)
    {
        // 如果之前是长按，现在却松开了按键
        switch (KeyOld)
        {
        case TK_CH0_VALIB:
            // send_status_keyval(KEY_PRESS_LOOSE, KEY_TK1);
            break;

        case TK_CH4_VALIB:
            // send_status_keyval(KEY_PRESS_LOOSE, KEY_TK2);
            break;

        case TK_CH5_VALIB:
            // send_status_keyval(KEY_PRESS_LOOSE, KEY_TK3);
            break;

        case TK_CH6_VALIB:
            // send_status_keyval(KEY_PRESS_LOOSE, KEY_TK4);
            break;
        }

        long_touch_flag = 0; // 清除标志位
        cnt = 0;
        KeyOld = 0;
    }

#if 0
    if (1 == __tk_long_key_flag) // 如果长按按键（这个功能测试发现不行，不能用）
    {
        __tk_long_key_flag = 0;

        // 这里可以自己处理，添加自己需要的功能

        P11 = 0;
    }
#endif

    led_display(); // LED状态更新显示
}
#endif // end ifdef DEVELOPMENT_BOARD

#ifdef CIRCUIT_BOARD // 目标电路板对应的触摸按键函数接口
/**
 * @brief  用户代码循环执行函数接口.（触摸按键扫描函数接口）
 * @param  None
 * @retval None
 */
void user_handle(void)
{
    volatile unsigned long int KeyOnOutput = __tk_key_flag; // __tk_key_flag单次按键标志

    static volatile unsigned long int KeyOld = 0; // 用于存放长按时，对应的按键

    // 是否长按的标志，0--否，1--是
    // 需要保存长按的状态，表示定时器一直在发送键值
    static unsigned char long_touch_flag = 0; // 上一次扫描到的按键是否为长按的标志

    static volatile u32 cnt = 0; // 长按计数值

    u32 i = 0; // 循环计数值

    u16 send_data = 0; // 要发送的带有键值的数据

    if (KeyOnOutput && 0 == long_touch_flag && cnt < TK_LONG_KEY_TIME) // 如果有按键按下（并且之前扫描到的不是长按）
    {
        // 通过循环判断是否为长按
        for (i = 0; i < 500000; i++) // 实际上用不到这么大的数字，到了设定的长按时间就会退出
        {
            // 按键扫描函数，使用了库里面的接口（闭源库）
            // 这个函数会更新__tk_key_flag的值
            delay_ms(10);
            __tk_scan();

            if (KeyOnOutput == __tk_key_flag)
            {
                // 如果按键键值一致，说明按键一直被按下，未松开
                cnt++;
            }
            else
            {
                // 如果按键键值不一致或是松开了按键
                // cnt = 0;
                break;
            }
            if (cnt > TK_LONG_KEY_TIME) // 如果长按超过了这里设置的时间
            {
                long_touch_flag = 1; // 标记为长按
                break;
            }
        }

        if (cnt > TK_LONG_KEY_TIME)
        {
            long_touch_flag = 1; // 标记为长按
        }
        else
        {
            // 如果是短按
            cnt = 0; // 清除长按的计数值

            // 判断当前触摸的键值，做对应的操作
            if (TK_CH25_VALIB == KeyOnOutput)
            {
                // 如果KEY1被触摸
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH25_VALIB == KeyOnOutput)
                    {
                        // 如果检测到是同一个按键按下，说明是双击
                        // 等待手指松开按键，不然发送完带有双击状态的键值后会再发送一次单击状态的键值
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK1);
                        return;
                    }
                }

                // 如果不是同一个按键按下或是没有值，说明是短按
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK1); // 发送带有状态的键值
            }
            else if (TK_CH3_VALIB == KeyOnOutput)
            {
                // 如果KEY2被触摸
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH3_VALIB == KeyOnOutput)
                    {
                        // 如果检测到是同一个按键按下，说明是双击
                        // 等待手指松开按键，不然发送完带有双击状态的键值后会再发送一次单击状态的键值
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK2);
                        return;
                    }
                }

                // 如果不是同一个按键按下或是没有值，说明是短按
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK2); // 发送带有状态的键值
            }
            else if (TK_CH2_VALIB == KeyOnOutput)
            {
                // 如果KEY3被触摸
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH2_VALIB == KeyOnOutput)
                    {
                        // 如果检测到是同一个按键按下，说明是双击
                        // 等待手指松开按键，不然发送完带有双击状态的键值后会再发送一次单击状态的键值
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK3);
                        return;
                    }
                }

                // 如果不是同一个按键按下或是没有值，说明是短按
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK3); // 发送带有状态的键值
            }
            else if (TK_CH0_VALIB == KeyOnOutput)
            {
                // 如果KEY4被触摸
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH0_VALIB == KeyOnOutput)
                    {
                        // 如果检测到是同一个按键按下，说明是双击
                        // 等待手指松开按键，不然发送完带有双击状态的键值后会再发送一次单击状态的键值
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK4);
                        return;
                    }
                }

                // 如果不是同一个按键按下或是没有值，说明是短按
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK4); // 发送带有状态的键值
            }
            else if (TK_CH9_VALIB == KeyOnOutput)
            {
                // 如果KEY5被触摸
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH9_VALIB == KeyOnOutput)
                    {
                        // 如果检测到是同一个按键按下，说明是双击
                        // 等待手指松开按键，不然发送完带有双击状态的键值后会再发送一次单击状态的键值
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK5);
                        return;
                    }
                }

                // 如果不是同一个按键按下或是没有值，说明是短按
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK5); // 发送带有状态的键值
            }
            else if (TK_CH10_VALIB == KeyOnOutput)
            {
                // 如果KEY6被触摸
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH10_VALIB == KeyOnOutput)
                    {
                        // 如果检测到是同一个按键按下，说明是双击
                        // 等待手指松开按键，不然发送完带有双击状态的键值后会再发送一次单击状态的键值
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK6);
                        return;
                    }
                }

                // 如果不是同一个按键按下或是没有值，说明是短按
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK6); // 发送带有状态的键值
            }
        }
    }

    if (1 == long_touch_flag && 0 != KeyOnOutput)
    {
        KeyOld = KeyOnOutput;

        // 如果之前是长按，现在还未松开
        // 判断当前触摸的键值，做对应的操作
        if (TK_CH25_VALIB == KeyOnOutput)
        {
            // 如果KEY1被触摸
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK1);
            }

            // 若还不松开手，则每隔150ms发送一次带有持续状态信息的键值信号
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // 原本是填90的，但是软件延时有误差，这里只能慢慢调试
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK1);
                }
            }
        }
        else if (TK_CH3_VALIB == KeyOnOutput)
        {
            // 如果KEY2被触摸
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK2);
            }

            // 若还不松开手，则每隔150ms发送一次带有持续状态信息的键值信号
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // 原本是填90的，但是软件延时有误差，这里只能慢慢调试
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK2);
                }
            }

            // display_data ^= 0x0200; // 对应的LED状态取反，后面会调用led_display()刷新
            // led_display();          // LED状态更新显示
        }
        else if (TK_CH2_VALIB == KeyOnOutput)
        {
            // 如果KEY3被触摸
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // display_data ^= 0x0400; // 对应的LED状态取反，后面会调用led_display()刷新
                // led_display();          // LED状态更新显示
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK3);
            }

            // 若还不松开手，则每隔150ms发送一次带有持续状态信息的键值信号
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // 原本是填90的，但是软件延时有误差，这里只能慢慢调试
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK3);
                }
            }

            // display_data ^= 0x0400; // 对应的LED状态取反，后面会调用led_display()刷新
            // led_display();          // LED状态更新显示
        }
        else if (TK_CH0_VALIB == KeyOnOutput)
        {
            // 如果KEY4被触摸
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // display_data ^= 0x0800; // 对应的LED状态取反，后面会调用led_display()刷新
                // led_display();          // LED状态更新显示
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK4);
            }

            // 若还不松开手，则每隔150ms发送一次带有持续状态信息的键值信号
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // 原本是填90的，但是软件延时有误差，这里只能慢慢调试
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK4);
                }
            }

            // display_data ^= 0x0800; // 对应的LED状态取反，后面会调用led_display()刷新
            // led_display();          // LED状态更新显示
        }
        else if (TK_CH9_VALIB == KeyOnOutput)
        {
            // 如果KEY5被触摸
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // display_data ^= 0x0800; // 对应的LED状态取反，后面会调用led_display()刷新
                // led_display();          // LED状态更新显示
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK5);
            }

            // 若还不松开手，则每隔150ms发送一次带有持续状态信息的键值信号
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // 原本是填90的，但是软件延时有误差，这里只能慢慢调试
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK5);
                }
            }

            // display_data ^= 0x0800; // 对应的LED状态取反，后面会调用led_display()刷新
            // led_display();          // LED状态更新显示
        }

        else if (TK_CH10_VALIB == KeyOnOutput)
        {
            // 如果KEY6被触摸
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // display_data ^= 0x0800; // 对应的LED状态取反，后面会调用led_display()刷新
                // led_display();          // LED状态更新显示
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK6);
            }

            // 若还不松开手，则每隔150ms发送一次带有持续状态信息的键值信号
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // 原本是填90的，但是软件延时有误差，这里只能慢慢调试
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK6);
                }
            }

            // display_data ^= 0x0800; // 对应的LED状态取反，后面会调用led_display()刷新
            // led_display();          // LED状态更新显示
        }
    }
    else if (1 == long_touch_flag && 0 == KeyOnOutput)
    {
        // 如果之前是长按，现在却松开了按键
        switch (KeyOld)
        {
        case TK_CH25_VALIB:
            // send_status_keyval(KEY_PRESS_LOOSE, KEY_TK1);
            break;

        case TK_CH3_VALIB:
            // send_status_keyval(KEY_PRESS_LOOSE, KEY_TK2);
            break;

        case TK_CH2_VALIB:
            // send_status_keyval(KEY_PRESS_LOOSE, KEY_TK3);
            break;

        case TK_CH0_VALIB:
            // send_status_keyval(KEY_PRESS_LOOSE, KEY_TK4);
            break;

        case TK_CH9_VALIB:
            // send_status_keyval(KEY_PRESS_LOOSE, KEY_TK5);
            break;

        case TK_CH10_VALIB:
            // send_status_keyval(KEY_PRESS_LOOSE, KEY_TK6);
            break;
        }

        long_touch_flag = 0; // 清除标志位
        cnt = 0;
        KeyOld = 0;
    }

#if 0
    if (1 == __tk_long_key_flag) // 如果长按按键（这个功能测试发现不行，不能用）
    {
        __tk_long_key_flag = 0;

        // 这里可以自己处理，添加自己需要的功能

        P11 = 0;
    }
#endif

    // led_display(); // LED状态更新显示
}
#endif // end of #ifdef CIRCUIT_BOARD
/*************************** (C) COPYRIGHT 2022 TAIXIN-IC ***** END OF FILE *****/
#endif