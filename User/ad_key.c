#include "ad_key.h"

enum
{
    AD_KEY_SCAN_STATUS_NONE,                  // 空状态，检测是否有按键按下
    AD_KEY_SCAN_STATUS_IS_DETECT_LOOG_PRESS,  // 正在检测是否为长按（要跟检测长按作区分）
    AD_KEY_SCAN_STATUS_IS_HANDLE_LONG_PRESS,  // 正在处理长按
    AD_KEY_SCAN_STATUS_IS_HANDLE_HOLD_PRESS,  // 正在处理长按持续（不松手）
    AD_KEY_SCAN_STATUS_IS_HANDLE_SHORT_PRESS, // 正在处理短按

    AD_KEY_SCAN_STATUS_IS_WAIT_SHORT_PRESS_RELEASE, // 正在等待短按松开
    AD_KEY_SCAN_STATUS_IS_WAIT_LONG_PRESS_RELEASE,  // 正在等待长按松开
    AD_KEY_SCAN_STATUS_IS_END,                      // 收尾处理
};
static volatile u8 ad_key_scan_status = 0; // 非阻塞的按键扫描函数中，使用的状态机

// 存放按键对应的ad值:
const u16 ad_key_scan_table[] = {
    AD_KEY_ONE_LEFT_VAL,
    AD_KEY_TWO_LEFT_VAL,
    AD_KEY_THREE_LEFT_VAL,
    AD_KEY_ONE_RIGHT_VAL,
    AD_KEY_TWO_RIGHT_VAL,
    AD_KEY_THREE_RIGHT_VAL,
};

static u8 __detect_press(u16 cur_ad_key)
{
    u8 flag_is_detect_press = 0;
    u8 i = 0;
    for (i = 0; i < ARRAY_SIZE(ad_key_scan_table); i++)
    {
        if (cur_ad_key > (ad_key_scan_table[i] - AD_KEY_INTERVAL) &&
            cur_ad_key < (ad_key_scan_table[i] + AD_KEY_INTERVAL))
        {
            // 如果之前未检测到按键，现在检测到按键按下
            flag_is_detect_press = 1;
            break;
        }
    }

    return flag_is_detect_press;
}

// 非阻塞的按键扫描函数
void ad_key_scan(void)
{
    volatile u16 ad_key = 0;                   // __tk_key_flag 单次按键标志
    static u16 ad_key_last = 0;                // 用于存放长按时，对应的按键
    static volatile u32 touch_time_cnt = 0;    // 长按计数值
    static u8 flag_is_detect_ad_key_press = 0; // 标志位，检测按键是否按下

    /* 按键扫描函数 */
    ad_key = adc_getval(); // 每次调用约占用370us，可以忽略不计
    // printf("ad_key val %u\n", ad_key);

    if (AD_KEY_SCAN_STATUS_NONE == ad_key_scan_status) // 未检测到按键时
    {
        // 如果有按键按下（并且之前扫描到的不是长按）
        // 判断是否为长按：

        if (0 == flag_is_detect_ad_key_press)
        {
            flag_is_detect_ad_key_press = __detect_press(ad_key);
            if (flag_is_detect_ad_key_press)
            {
                // 如果之前未检测到按键，现在检测到按键按下:
                ad_key_last = ad_key;
            }
        }

        if (0 == flag_is_detect_ad_key_press)
        {
            // 未检测到按键按下
            return;
        }

        // if (flag_is_detect_ad_key_press)
        // {
        //     // 如果之前未检测到按键，现在检测到按键按下
        //     ad_key_last = ad_key;
        // }
        // else if (ad_key_last > (ad_key - AD_KEY_INTERVAL) &&
        //          ad_key_last < (ad_key + AD_KEY_INTERVAL))
        if (ad_key_last > (ad_key - AD_KEY_INTERVAL) &&
            ad_key_last < (ad_key + AD_KEY_INTERVAL))
        {
            // 如果上次检测到的按键与此次的按键相等，说明按键还未松开
            touch_time_cnt += (1 + ONE_CYCLE_TIME_MS);

            if (touch_time_cnt >= LONG_PRESS_TIME_THRESHOLD_MS)
            {
                // 如果长按超过了设置的长按时间
                // 跳转到长按处理
                touch_time_cnt = 0;     // 清除长按时间计数
                printf("long press\n"); // 测试用
                ad_key_scan_status = AD_KEY_SCAN_STATUS_IS_HANDLE_LONG_PRESS;
            }
        }
        else
        {
            // 如果上次检测到的按键与此次的按键不相等，并且上次检测到的按键不等于0
            // touch_time_cnt = 0; // 清除长按时间计数（可以留到收尾处理）

            // 跳转到短按处理
            ad_key_scan_status = AD_KEY_SCAN_STATUS_IS_HANDLE_SHORT_PRESS;
            printf("short press\n"); // 测试用
        }

        delay_ms(1); //
    }

    if (AD_KEY_SCAN_STATUS_IS_HANDLE_SHORT_PRESS == ad_key_scan_status)
    {
        // 如果是正在处理短按
        // flag_get_touch_key_status = 1; // 默认获取到了按键状态

        if (ad_key_last > (AD_KEY_ONE_LEFT_VAL - AD_KEY_INTERVAL) &&
            ad_key_last < (AD_KEY_ONE_LEFT_VAL + AD_KEY_INTERVAL))
        {
            // 如果  被触摸

            // 如果没有按键双击，说明是短按
#if USE_MY_DEBUG
            printf("one left press\n");
#endif
            // fun_info.touch_key_val = TOUCH_KEY_VAL_VOL_INC;
            send_data(SEND_TOUCH_KEY_STATUS, TOUCH_KEY_VAL_VOL_INC);
        }
        else if (ad_key_last > (AD_KEY_TWO_LEFT_VAL - AD_KEY_INTERVAL) &&
                 ad_key_last < (AD_KEY_TWO_LEFT_VAL + AD_KEY_INTERVAL))
        {
            // 如果  被触摸

            // 如果没有按键双击，说明是短按
#if USE_MY_DEBUG
            printf("two left press\n");
#endif
            // fun_info.touch_key_val = TOUCH_KEY_VAL_MUTE;
            send_data(SEND_TOUCH_KEY_STATUS, TOUCH_KEY_VAL_MUTE);
        }
        else if (ad_key_last > (AD_KEY_THREE_LEFT_VAL - AD_KEY_INTERVAL) &&
                 ad_key_last < (AD_KEY_THREE_LEFT_VAL + AD_KEY_INTERVAL))
        {
            // 如果  被触摸

#if USE_MY_DEBUG
            printf("three left press\n");
#endif
            // fun_info.touch_key_val = TOUCH_KEY_VAL_VOL_DEC;
            send_data(SEND_TOUCH_KEY_STATUS, TOUCH_KEY_VAL_VOL_DEC);
        }
        else if (ad_key_last > (AD_KEY_ONE_RIGHT_VAL - AD_KEY_INTERVAL) &&
                 ad_key_last < (AD_KEY_ONE_RIGHT_VAL + AD_KEY_INTERVAL))
        {
            // 如果  被触摸

#if USE_MY_DEBUG
            printf("one right press\n");
#endif
            // fun_info.touch_key_val = TOUCH_KEY_VAL_NEXT;
            send_data(SEND_TOUCH_KEY_STATUS, TOUCH_KEY_VAL_NEXT);
        }
        else if (ad_key_last > (AD_KEY_TWO_RIGHT_VAL - AD_KEY_INTERVAL) &&
                 ad_key_last < (AD_KEY_TWO_RIGHT_VAL + AD_KEY_INTERVAL))
        {
            // 如果  被触摸

#if USE_MY_DEBUG
            printf("two right press\n");
#endif
            // fun_info.touch_key_val = TOUCH_KEY_VAL_MODE;
            send_data(SEND_TOUCH_KEY_STATUS, TOUCH_KEY_VAL_MODE);
        }
        else if (ad_key_last > (AD_KEY_THREE_RIGHT_VAL - AD_KEY_INTERVAL) &&
                 ad_key_last < (AD_KEY_THREE_RIGHT_VAL + AD_KEY_INTERVAL))
        {
            // 如果  被触摸

#if USE_MY_DEBUG
            printf("three right press\n");
#endif
            // fun_info.touch_key_val = TOUCH_KEY_VAL_PRE;
            send_data(SEND_TOUCH_KEY_STATUS, TOUCH_KEY_VAL_PRE);
        }
        else
        {
            // 如果没有按键被触摸
            // flag_get_touch_key_status = 0; // 清空该标志位，不发送按键状态
        }

        ad_key_scan_status = AD_KEY_SCAN_STATUS_IS_WAIT_SHORT_PRESS_RELEASE;
    }

    if (AD_KEY_SCAN_STATUS_IS_HANDLE_LONG_PRESS == ad_key_scan_status)
    {
        // flag_get_touch_key_status = 1; // 默认获取到了按键状态
        // 处理长按
        if (ad_key_last > (AD_KEY_ONE_LEFT_VAL - AD_KEY_INTERVAL) &&
            ad_key_last < (AD_KEY_ONE_LEFT_VAL + AD_KEY_INTERVAL))
        {
            // 如果  被触摸

#if USE_MY_DEBUG
            // printf("one left long press\n");
#endif
            // flag_get_touch_key_status = 0; // 这个按键没有长按对应的操作
        }
        else if (ad_key_last > (AD_KEY_TWO_LEFT_VAL - AD_KEY_INTERVAL) &&
                 ad_key_last < (AD_KEY_TWO_LEFT_VAL + AD_KEY_INTERVAL))
        {
            // 如果KEY2被触摸

#if USE_MY_DEBUG
            printf("two left long press\n");
#endif
            // fun_info.touch_key_val = TOUCH_KEY_VAL_POWER;
            send_data(SEND_TOUCH_KEY_STATUS, TOUCH_KEY_VAL_POWER);
        }
        else if (ad_key_last > (AD_KEY_THREE_LEFT_VAL - AD_KEY_INTERVAL) &&
                 ad_key_last < (AD_KEY_THREE_LEFT_VAL + AD_KEY_INTERVAL))
        {
            // 如果  被触摸

#if USE_MY_DEBUG
            // printf("three left long press\n");
#endif
            // flag_get_touch_key_status = 0; // 这个按键没有长按对应的操作
        }
        else if (ad_key_last > (AD_KEY_ONE_RIGHT_VAL - AD_KEY_INTERVAL) &&
                 ad_key_last < (AD_KEY_ONE_RIGHT_VAL + AD_KEY_INTERVAL))
        {
            // 如果  被触摸

#if USE_MY_DEBUG
            // printf("one right long press\n");
#endif
            // flag_get_touch_key_status = 0; // 这个按键没有长按对应的操作
        }
        else if (ad_key_last > (AD_KEY_TWO_RIGHT_VAL - AD_KEY_INTERVAL) &&
                 ad_key_last < (AD_KEY_TWO_RIGHT_VAL + AD_KEY_INTERVAL))
        {
            // 如果  被触摸

#if USE_MY_DEBUG
            printf("two right long press\n");
#endif
            // fun_info.touch_key_val = TOUCH_KEY_VAL_MAIN;
            send_data(SEND_TOUCH_KEY_STATUS, TOUCH_KEY_VAL_MAIN);
        }
        else if (ad_key_last > (AD_KEY_THREE_RIGHT_VAL - AD_KEY_INTERVAL) &&
                 ad_key_last < (AD_KEY_THREE_RIGHT_VAL + AD_KEY_INTERVAL))
        {
            // 如果  被触摸

#if USE_MY_DEBUG
            // printf("three right long press\n");
#endif
            // flag_get_touch_key_status = 0; // 这个按键没有长按对应的操作
        }

        ad_key_scan_status = AD_KEY_SCAN_STATUS_IS_HANDLE_HOLD_PRESS; // 跳转到长按持续
    }

    if (AD_KEY_SCAN_STATUS_IS_HANDLE_HOLD_PRESS == ad_key_scan_status)
    {
        // 处理长按持续
        // flag_get_touch_key_status = 1; // 默认获取到了按键状态

        if (ad_key_last > (ad_key - AD_KEY_INTERVAL) &&
            ad_key_last < (ad_key + AD_KEY_INTERVAL))
        {
            touch_time_cnt++;
            if (ad_key_last > (AD_KEY_ONE_LEFT_VAL - AD_KEY_INTERVAL) &&
                ad_key_last < (AD_KEY_ONE_LEFT_VAL + AD_KEY_INTERVAL))
            {
                if (touch_time_cnt >= HOLD_PRESS_TIME_THRESHOLD_MS)
                {
                    touch_time_cnt = 0;
#if USE_MY_DEBUG
                    printf("one left continue press\n");
#endif
                    // fun_info.touch_key_val = TOUCH_KEY_VAL_VOL_INC;
                    send_data(SEND_TOUCH_KEY_STATUS, TOUCH_KEY_VAL_VOL_INC);
                }
            }
            else if (ad_key_last > (AD_KEY_TWO_LEFT_VAL - AD_KEY_INTERVAL) &&
                     ad_key_last < (AD_KEY_TWO_LEFT_VAL + AD_KEY_INTERVAL))
            {
                if (touch_time_cnt >= HOLD_PRESS_TIME_THRESHOLD_MS)
                {
                    touch_time_cnt = 0;
#if USE_MY_DEBUG
                    // printf("two left continue press\n");
#endif
                    // flag_get_touch_key_status = 0; // 这个按键没有长按持续对应的操作
                }
            }
            else if (ad_key_last > (AD_KEY_THREE_LEFT_VAL - AD_KEY_INTERVAL) &&
                     ad_key_last < (AD_KEY_THREE_LEFT_VAL + AD_KEY_INTERVAL))
            {
                if (touch_time_cnt >= HOLD_PRESS_TIME_THRESHOLD_MS)
                {
                    touch_time_cnt = 0;
#if USE_MY_DEBUG
                    // printf("three left continue press\n");
#endif
                    // fun_info.touch_key_val = TOUCH_KEY_VAL_VOL_DEC;
                    send_data(SEND_TOUCH_KEY_STATUS, TOUCH_KEY_VAL_VOL_DEC);
                }
            }
            else if (ad_key_last > (AD_KEY_ONE_RIGHT_VAL - AD_KEY_INTERVAL) &&
                     ad_key_last < (AD_KEY_ONE_RIGHT_VAL + AD_KEY_INTERVAL))
            {
                if (touch_time_cnt >= HOLD_PRESS_TIME_THRESHOLD_MS)
                {
                    touch_time_cnt = 0;
#if USE_MY_DEBUG
                    // printf("one right continue press\n");
#endif
                    // flag_get_touch_key_status = 0; // 这个按键没有长按持续对应的操作
                }
            }
            else if (ad_key_last > (AD_KEY_TWO_RIGHT_VAL - AD_KEY_INTERVAL) &&
                     ad_key_last < (AD_KEY_TWO_RIGHT_VAL + AD_KEY_INTERVAL))
            {
                if (touch_time_cnt >= HOLD_PRESS_TIME_THRESHOLD_MS)
                {
                    touch_time_cnt = 0;
#if USE_MY_DEBUG
                    // printf("two right continue press\n");
#endif
                    // flag_get_touch_key_status = 0; // 这个按键没有长按持续对应的操作
                }
            }
            else if (ad_key_last > (AD_KEY_THREE_RIGHT_VAL - AD_KEY_INTERVAL) &&
                     ad_key_last < (AD_KEY_THREE_RIGHT_VAL + AD_KEY_INTERVAL))
            {
                if (touch_time_cnt >= HOLD_PRESS_TIME_THRESHOLD_MS)
                {
                    touch_time_cnt = 0;
#if USE_MY_DEBUG
                    // printf("three right continue press\n");
#endif
                    // flag_get_touch_key_status = 0; // 这个按键没有长按持续对应的操作
                }
            }

            delay_ms(1);
        }
        else
        {
            // 如果之前检测到的按键与当前检测到的按键不一致，
            // 说明 可能松开了手 或是 一只手按住了原来的按键另一只手按了其他按键

            // 跳转到等待长按松开
            ad_key_scan_status = AD_KEY_SCAN_STATUS_IS_WAIT_LONG_PRESS_RELEASE;
            // flag_get_touch_key_status = 0; // 清空语句块刚开始默认置位的标志位
        }
    }

    if ((AD_KEY_SCAN_STATUS_IS_WAIT_LONG_PRESS_RELEASE == ad_key_scan_status) ||
        (AD_KEY_SCAN_STATUS_IS_WAIT_SHORT_PRESS_RELEASE == ad_key_scan_status))
    {
        // 如果是等待按键松开
        static volatile u16 loose_cnt = 0; // 存放松手计数值

        flag_is_detect_ad_key_press = __detect_press(ad_key);

        if (0 == flag_is_detect_ad_key_press)
        {
            loose_cnt++;
            delay_ms(1);
        }
        else
        {
            // 只要有一次检测到按键，说明没有松开手
            loose_cnt = 0;
        }

        if (loose_cnt >= LOOSE_PRESS_CNT_MS) // 这里的比较值需要注意，不能大于变量类型对应的最大值
        {
            loose_cnt = 0; // 清空松手计数
            // 如果 xx ms内没有检测到按键按下，说明已经松开手

            if (AD_KEY_SCAN_STATUS_IS_WAIT_LONG_PRESS_RELEASE == ad_key_scan_status)
            {
                if (ad_key_last > (AD_KEY_ONE_LEFT_VAL - AD_KEY_INTERVAL) &&
                    ad_key_last < (AD_KEY_ONE_LEFT_VAL + AD_KEY_INTERVAL))
                {
#if USE_MY_DEBUG
                    // printf("one left long press loose\n");
#endif
                    // send_status_keyval(KEY_PRESS_LOOSE, ad_key1);
                }
                else if (ad_key_last > (AD_KEY_TWO_LEFT_VAL - AD_KEY_INTERVAL) &&
                         ad_key_last < (AD_KEY_TWO_LEFT_VAL + AD_KEY_INTERVAL))
                {
#if USE_MY_DEBUG
                    // printf("two left long press loose\n");
#endif
                    // send_status_keyval(KEY_PRESS_LOOSE, ad_key2);
                }
                else if (ad_key_last > (AD_KEY_THREE_LEFT_VAL - AD_KEY_INTERVAL) &&
                         ad_key_last < (AD_KEY_THREE_LEFT_VAL + AD_KEY_INTERVAL))
                {
#if USE_MY_DEBUG
                    // printf("three left long press loose\n");
#endif
                    // send_status_keyval(KEY_PRESS_LOOSE, ad_key3);
                }
                else if (ad_key_last > (AD_KEY_ONE_RIGHT_VAL - AD_KEY_INTERVAL) &&
                         ad_key_last < (AD_KEY_ONE_RIGHT_VAL + AD_KEY_INTERVAL))
                {
#if USE_MY_DEBUG
                    // printf("one right long press loose\n");
#endif
                    // send_status_keyval(KEY_PRESS_LOOSE, ad_key4);
                }
                else if (ad_key_last > (AD_KEY_TWO_RIGHT_VAL - AD_KEY_INTERVAL) &&
                         ad_key_last < (AD_KEY_TWO_RIGHT_VAL + AD_KEY_INTERVAL))
                {
#if USE_MY_DEBUG
                    // printf("two right long press loose\n");
#endif
                    // send_status_keyval(KEY_PRESS_LOOSE, ad_key5);
                }
                else if (ad_key_last > (AD_KEY_THREE_RIGHT_VAL - AD_KEY_INTERVAL) &&
                         ad_key_last < (AD_KEY_THREE_RIGHT_VAL + AD_KEY_INTERVAL))
                {
#if USE_MY_DEBUG
                    // printf("three right long press loose\n");
#endif
                    // send_status_keyval(KEY_PRESS_LOOSE, ad_key5);
                }
            }

            ad_key_scan_status = AD_KEY_SCAN_STATUS_IS_END; // 跳转到收尾处理
        }
    }

    if (AD_KEY_SCAN_STATUS_IS_END == ad_key_scan_status)
    {
        // 收尾处理
        ad_key = 0;
        ad_key_last = 0;
        touch_time_cnt = 0;
        flag_is_detect_ad_key_press = 0;

        // fun_info.touch_key_val = 0;

        ad_key_scan_status = AD_KEY_SCAN_STATUS_NONE;
    }
}
