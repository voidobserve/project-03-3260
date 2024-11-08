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
 * @brief  �û������ʼ�������ӿ�.
 * @param  None
 * @retval None
 */
void user_init(void)
{
    fun_info_init(); // ��ʼ�����ڴ����Ϣ�ı���


    tmr0_config(); // ���ڼ�����ݳ�ʱ��Ҫʹ�õ��Ķ�ʱ��
    uart0_config(); // ���ͺͽ���ָ��ʹ�õ��Ĵ���
    pin_level_scan_config(); // ɲ����ת��ơ���λ�ļ����������

    tmr1_config(); // ���һ��ʱ���ڵ������������Ķ�ʱ��
    // tmr2_pwm_config(); // �����õ�PWM
    speed_scan_config(); // ʱ��ɨ�������
    engine_speed_scan_config(); // ������ת��ɨ�������

    tmr3_config(); // ��ʱ�����д��flash����Ķ�ʱ��

    // mileage_init(); // ��flash�ж��������̺�С��������ݣ��Ѿ�����fun_info�ṹ������У�

    p03_output_config(); // ����ߵ�ƽ�õ����ţ���ʱ����ad����һ��ֵ������������ߵ�ƽ��������һ��ʼ���Ǹߵ�ƽ
    P03 = 1;

    adc_pin_config();
}

#if 0
#ifdef DEVELOPMENT_BOARD // �������϶�Ӧ������������ӿ�
/**
 * @brief  �û�����ѭ��ִ�к����ӿ�.����������ɨ�躯���ӿڣ�
 * @param  None
 * @retval None
 */
void user_handle(void)
{
    volatile unsigned long int KeyOnOutput = __tk_key_flag; // __tk_key_flag���ΰ�����־

    static volatile unsigned long int KeyOld = 0; // ���ڴ�ų���ʱ����Ӧ�İ���

    // �Ƿ񳤰��ı�־��0--��1--��
    // ��Ҫ���泤����״̬����ʾ��ʱ��һֱ�ڷ��ͼ�ֵ
    static unsigned char long_touch_flag = 0; // ��һ��ɨ�赽�İ����Ƿ�Ϊ�����ı�־

    static volatile u32 cnt = 0; // ��������ֵ

    u32 i = 0; // ѭ������ֵ

    u16 send_data = 0; // Ҫ���͵Ĵ��м�ֵ������

    if (KeyOnOutput && 0 == long_touch_flag && cnt < TK_LONG_KEY_TIME) // ����а������£�����֮ǰɨ�赽�Ĳ��ǳ�����
    {
        // ͨ��ѭ���ж��Ƿ�Ϊ����
        for (i = 0; i < 500000; i++) // ʵ�����ò�����ô������֣������趨�ĳ���ʱ��ͻ��˳�
        {
            // ����ɨ�躯����ʹ���˿�����Ľӿڣ���Դ�⣩
            // ������������__tk_key_flag��ֵ
            delay_ms(10);
            __tk_scan();

            if (KeyOnOutput == __tk_key_flag)
            {
                // ���������ֵһ�£�˵������һֱ�����£�δ�ɿ�
                cnt++;
            }
            else
            {
                // ���������ֵ��һ�»����ɿ��˰���
                // cnt = 0;
                break;
            }
            if (cnt > TK_LONG_KEY_TIME) // ��������������������õ�ʱ��
            {
                long_touch_flag = 1; // ���Ϊ����
                break;
            }
        }

        if (cnt > TK_LONG_KEY_TIME)
        {
            long_touch_flag = 1; // ���Ϊ����
        }
        else
        {
            // ����Ƕ̰�
            cnt = 0; // ��������ļ���ֵ

            // �жϵ�ǰ�����ļ�ֵ������Ӧ�Ĳ���
            if (TK_CH0_VALIB == KeyOnOutput)
            {
                // ����������TK1������
                display_data ^= 0x0100; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH0_VALIB == KeyOnOutput)
                    {
                        // �����⵽��ͬһ���������£�˵����˫��
                        // �ȴ���ָ�ɿ���������Ȼ���������˫��״̬�ļ�ֵ����ٷ���һ�ε���״̬�ļ�ֵ
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK1);
                        return;
                    }
                }

                // �������ͬһ���������»���û��ֵ��˵���Ƕ̰�
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK1); // ���ʹ���״̬�ļ�ֵ
            }
            else if (TK_CH4_VALIB == KeyOnOutput)
            {
                // ����������TK2������
                display_data ^= 0x0200; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH4_VALIB == KeyOnOutput)
                    {
                        // �����⵽��ͬһ���������£�˵����˫��
                        // �ȴ���ָ�ɿ���������Ȼ���������˫��״̬�ļ�ֵ����ٷ���һ�ε���״̬�ļ�ֵ
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK2);
                        return;
                    }
                }

                // �������ͬһ���������»���û��ֵ��˵���Ƕ̰�
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK2); // ���ʹ���״̬�ļ�ֵ
            }
            else if (TK_CH5_VALIB == KeyOnOutput)
            {
                // ����������TK3������
                display_data ^= 0x0400; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH5_VALIB == KeyOnOutput)
                    {
                        // �����⵽��ͬһ���������£�˵����˫��
                        // �ȴ���ָ�ɿ���������Ȼ���������˫��״̬�ļ�ֵ����ٷ���һ�ε���״̬�ļ�ֵ
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK3);
                        return;
                    }
                }

                // �������ͬһ���������»���û��ֵ��˵���Ƕ̰�
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK3); // ���ʹ���״̬�ļ�ֵ
            }
            else if (TK_CH6_VALIB == KeyOnOutput)
            {
                // ����������TK4������
                display_data ^= 0x0800; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH6_VALIB == KeyOnOutput)
                    {
                        // �����⵽��ͬһ���������£�˵����˫��
                        // �ȴ���ָ�ɿ���������Ȼ���������˫��״̬�ļ�ֵ����ٷ���һ�ε���״̬�ļ�ֵ
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK4);
                        return;
                    }
                }

                // �������ͬһ���������»���û��ֵ��˵���Ƕ̰�
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK4); // ���ʹ���״̬�ļ�ֵ
            }
        }
    }

    if (1 == long_touch_flag && 0 != KeyOnOutput)
    {
        KeyOld = KeyOnOutput;

        // ���֮ǰ�ǳ��������ڻ�δ�ɿ�
        // �жϵ�ǰ�����ļ�ֵ������Ӧ�Ĳ���
        if (TK_CH0_VALIB == KeyOnOutput)
        {
            // ����������TK1������
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                display_data ^= 0x0100; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                led_display();          // LED״̬������ʾ
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK1);
            }

            // �������ɿ��֣���ÿ��150ms����һ�δ��г���״̬��Ϣ�ļ�ֵ�ź�
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // ԭ������90�ģ����������ʱ��������ֻ����������
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK1);
                }
            }

            // display_data ^= 0x0100; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
            // led_display();          // LED״̬������ʾ
        }
        else if (TK_CH4_VALIB == KeyOnOutput)
        {
            // ����������TK2������
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                display_data ^= 0x0200; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                led_display();          // LED״̬������ʾ
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK2);
            }

            // �������ɿ��֣���ÿ��150ms����һ�δ��г���״̬��Ϣ�ļ�ֵ�ź�
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // ԭ������90�ģ����������ʱ��������ֻ����������
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK2);
                }
            }

            // display_data ^= 0x0200; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
            // led_display();          // LED״̬������ʾ
        }
        else if (TK_CH5_VALIB == KeyOnOutput)
        {
            // ����������TK3������
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                display_data ^= 0x0400; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                led_display();          // LED״̬������ʾ
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK3);
            }

            // �������ɿ��֣���ÿ��150ms����һ�δ��г���״̬��Ϣ�ļ�ֵ�ź�
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // ԭ������90�ģ����������ʱ��������ֻ����������
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK3);
                }
            }

            // display_data ^= 0x0400; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
            // led_display();          // LED״̬������ʾ
        }
        else if (TK_CH6_VALIB == KeyOnOutput)
        {
            // ����������TK4������
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                display_data ^= 0x0800; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                led_display();          // LED״̬������ʾ
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK4);
            }

            // �������ɿ��֣���ÿ��150ms����һ�δ��г���״̬��Ϣ�ļ�ֵ�ź�
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // ԭ������90�ģ����������ʱ��������ֻ����������
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK4);
                }
            }

            // display_data ^= 0x0800; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
            // led_display();          // LED״̬������ʾ
        }
    }
    else if (1 == long_touch_flag && 0 == KeyOnOutput)
    {
        // ���֮ǰ�ǳ���������ȴ�ɿ��˰���
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

        long_touch_flag = 0; // �����־λ
        cnt = 0;
        KeyOld = 0;
    }

#if 0
    if (1 == __tk_long_key_flag) // �������������������ܲ��Է��ֲ��У������ã�
    {
        __tk_long_key_flag = 0;

        // ��������Լ���������Լ���Ҫ�Ĺ���

        P11 = 0;
    }
#endif

    led_display(); // LED״̬������ʾ
}
#endif // end ifdef DEVELOPMENT_BOARD

#ifdef CIRCUIT_BOARD // Ŀ���·���Ӧ�Ĵ������������ӿ�
/**
 * @brief  �û�����ѭ��ִ�к����ӿ�.����������ɨ�躯���ӿڣ�
 * @param  None
 * @retval None
 */
void user_handle(void)
{
    volatile unsigned long int KeyOnOutput = __tk_key_flag; // __tk_key_flag���ΰ�����־

    static volatile unsigned long int KeyOld = 0; // ���ڴ�ų���ʱ����Ӧ�İ���

    // �Ƿ񳤰��ı�־��0--��1--��
    // ��Ҫ���泤����״̬����ʾ��ʱ��һֱ�ڷ��ͼ�ֵ
    static unsigned char long_touch_flag = 0; // ��һ��ɨ�赽�İ����Ƿ�Ϊ�����ı�־

    static volatile u32 cnt = 0; // ��������ֵ

    u32 i = 0; // ѭ������ֵ

    u16 send_data = 0; // Ҫ���͵Ĵ��м�ֵ������

    if (KeyOnOutput && 0 == long_touch_flag && cnt < TK_LONG_KEY_TIME) // ����а������£�����֮ǰɨ�赽�Ĳ��ǳ�����
    {
        // ͨ��ѭ���ж��Ƿ�Ϊ����
        for (i = 0; i < 500000; i++) // ʵ�����ò�����ô������֣������趨�ĳ���ʱ��ͻ��˳�
        {
            // ����ɨ�躯����ʹ���˿�����Ľӿڣ���Դ�⣩
            // ������������__tk_key_flag��ֵ
            delay_ms(10);
            __tk_scan();

            if (KeyOnOutput == __tk_key_flag)
            {
                // ���������ֵһ�£�˵������һֱ�����£�δ�ɿ�
                cnt++;
            }
            else
            {
                // ���������ֵ��һ�»����ɿ��˰���
                // cnt = 0;
                break;
            }
            if (cnt > TK_LONG_KEY_TIME) // ��������������������õ�ʱ��
            {
                long_touch_flag = 1; // ���Ϊ����
                break;
            }
        }

        if (cnt > TK_LONG_KEY_TIME)
        {
            long_touch_flag = 1; // ���Ϊ����
        }
        else
        {
            // ����Ƕ̰�
            cnt = 0; // ��������ļ���ֵ

            // �жϵ�ǰ�����ļ�ֵ������Ӧ�Ĳ���
            if (TK_CH25_VALIB == KeyOnOutput)
            {
                // ���KEY1������
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH25_VALIB == KeyOnOutput)
                    {
                        // �����⵽��ͬһ���������£�˵����˫��
                        // �ȴ���ָ�ɿ���������Ȼ���������˫��״̬�ļ�ֵ����ٷ���һ�ε���״̬�ļ�ֵ
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK1);
                        return;
                    }
                }

                // �������ͬһ���������»���û��ֵ��˵���Ƕ̰�
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK1); // ���ʹ���״̬�ļ�ֵ
            }
            else if (TK_CH3_VALIB == KeyOnOutput)
            {
                // ���KEY2������
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH3_VALIB == KeyOnOutput)
                    {
                        // �����⵽��ͬһ���������£�˵����˫��
                        // �ȴ���ָ�ɿ���������Ȼ���������˫��״̬�ļ�ֵ����ٷ���һ�ε���״̬�ļ�ֵ
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK2);
                        return;
                    }
                }

                // �������ͬһ���������»���û��ֵ��˵���Ƕ̰�
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK2); // ���ʹ���״̬�ļ�ֵ
            }
            else if (TK_CH2_VALIB == KeyOnOutput)
            {
                // ���KEY3������
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH2_VALIB == KeyOnOutput)
                    {
                        // �����⵽��ͬһ���������£�˵����˫��
                        // �ȴ���ָ�ɿ���������Ȼ���������˫��״̬�ļ�ֵ����ٷ���һ�ε���״̬�ļ�ֵ
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK3);
                        return;
                    }
                }

                // �������ͬһ���������»���û��ֵ��˵���Ƕ̰�
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK3); // ���ʹ���״̬�ļ�ֵ
            }
            else if (TK_CH0_VALIB == KeyOnOutput)
            {
                // ���KEY4������
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH0_VALIB == KeyOnOutput)
                    {
                        // �����⵽��ͬһ���������£�˵����˫��
                        // �ȴ���ָ�ɿ���������Ȼ���������˫��״̬�ļ�ֵ����ٷ���һ�ε���״̬�ļ�ֵ
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK4);
                        return;
                    }
                }

                // �������ͬһ���������»���û��ֵ��˵���Ƕ̰�
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK4); // ���ʹ���״̬�ļ�ֵ
            }
            else if (TK_CH9_VALIB == KeyOnOutput)
            {
                // ���KEY5������
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH9_VALIB == KeyOnOutput)
                    {
                        // �����⵽��ͬһ���������£�˵����˫��
                        // �ȴ���ָ�ɿ���������Ȼ���������˫��״̬�ļ�ֵ����ٷ���һ�ε���״̬�ļ�ֵ
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK5);
                        return;
                    }
                }

                // �������ͬһ���������»���û��ֵ��˵���Ƕ̰�
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK5); // ���ʹ���״̬�ļ�ֵ
            }
            else if (TK_CH10_VALIB == KeyOnOutput)
            {
                // ���KEY6������
                for (i = 0; i < 300; i++)
                {
                    delay_ms(1);
                    __tk_scan();
                    KeyOnOutput = __tk_key_flag;

                    if (TK_CH10_VALIB == KeyOnOutput)
                    {
                        // �����⵽��ͬһ���������£�˵����˫��
                        // �ȴ���ָ�ɿ���������Ȼ���������˫��״̬�ļ�ֵ����ٷ���һ�ε���״̬�ļ�ֵ
                        while (0 != KeyOnOutput)
                        {
                            __tk_scan();
                            KeyOnOutput = __tk_key_flag;
                        }

                        // send_status_keyval(KEY_PRESS_DOUBLECLICK, KEY_TK6);
                        return;
                    }
                }

                // �������ͬһ���������»���û��ֵ��˵���Ƕ̰�
                // send_status_keyval(KEY_PRESS_SHORT, KEY_TK6); // ���ʹ���״̬�ļ�ֵ
            }
        }
    }

    if (1 == long_touch_flag && 0 != KeyOnOutput)
    {
        KeyOld = KeyOnOutput;

        // ���֮ǰ�ǳ��������ڻ�δ�ɿ�
        // �жϵ�ǰ�����ļ�ֵ������Ӧ�Ĳ���
        if (TK_CH25_VALIB == KeyOnOutput)
        {
            // ���KEY1������
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK1);
            }

            // �������ɿ��֣���ÿ��150ms����һ�δ��г���״̬��Ϣ�ļ�ֵ�ź�
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // ԭ������90�ģ����������ʱ��������ֻ����������
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK1);
                }
            }
        }
        else if (TK_CH3_VALIB == KeyOnOutput)
        {
            // ���KEY2������
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK2);
            }

            // �������ɿ��֣���ÿ��150ms����һ�δ��г���״̬��Ϣ�ļ�ֵ�ź�
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // ԭ������90�ģ����������ʱ��������ֻ����������
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK2);
                }
            }

            // display_data ^= 0x0200; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
            // led_display();          // LED״̬������ʾ
        }
        else if (TK_CH2_VALIB == KeyOnOutput)
        {
            // ���KEY3������
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // display_data ^= 0x0400; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                // led_display();          // LED״̬������ʾ
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK3);
            }

            // �������ɿ��֣���ÿ��150ms����һ�δ��г���״̬��Ϣ�ļ�ֵ�ź�
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // ԭ������90�ģ����������ʱ��������ֻ����������
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK3);
                }
            }

            // display_data ^= 0x0400; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
            // led_display();          // LED״̬������ʾ
        }
        else if (TK_CH0_VALIB == KeyOnOutput)
        {
            // ���KEY4������
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // display_data ^= 0x0800; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                // led_display();          // LED״̬������ʾ
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK4);
            }

            // �������ɿ��֣���ÿ��150ms����һ�δ��г���״̬��Ϣ�ļ�ֵ�ź�
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // ԭ������90�ģ����������ʱ��������ֻ����������
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK4);
                }
            }

            // display_data ^= 0x0800; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
            // led_display();          // LED״̬������ʾ
        }
        else if (TK_CH9_VALIB == KeyOnOutput)
        {
            // ���KEY5������
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // display_data ^= 0x0800; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                // led_display();          // LED״̬������ʾ
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK5);
            }

            // �������ɿ��֣���ÿ��150ms����һ�δ��г���״̬��Ϣ�ļ�ֵ�ź�
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // ԭ������90�ģ����������ʱ��������ֻ����������
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK5);
                }
            }

            // display_data ^= 0x0800; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
            // led_display();          // LED״̬������ʾ
        }

        else if (TK_CH10_VALIB == KeyOnOutput)
        {
            // ���KEY6������
            if (cnt >= TK_LONG_KEY_TIME && cnt < 90)
            {
                // display_data ^= 0x0800; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
                // led_display();          // LED״̬������ʾ
                // send_status_keyval(KEY_PRESS_LONG, KEY_TK6);
            }

            // �������ɿ��֣���ÿ��150ms����һ�δ��г���״̬��Ϣ�ļ�ֵ�ź�
            while (0 != KeyOnOutput)
            {
                delay_ms(10);
                cnt++;
                __tk_scan();
                KeyOnOutput = __tk_key_flag;

                if (cnt >= 87) // ԭ������90�ģ����������ʱ��������ֻ����������
                {
                    cnt = TK_LONG_KEY_TIME;
                    // send_status_keyval(KEY_PRESS_CONTINUE, KEY_TK6);
                }
            }

            // display_data ^= 0x0800; // ��Ӧ��LED״̬ȡ������������led_display()ˢ��
            // led_display();          // LED״̬������ʾ
        }
    }
    else if (1 == long_touch_flag && 0 == KeyOnOutput)
    {
        // ���֮ǰ�ǳ���������ȴ�ɿ��˰���
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

        long_touch_flag = 0; // �����־λ
        cnt = 0;
        KeyOld = 0;
    }

#if 0
    if (1 == __tk_long_key_flag) // �������������������ܲ��Է��ֲ��У������ã�
    {
        __tk_long_key_flag = 0;

        // ��������Լ���������Լ���Ҫ�Ĺ���

        P11 = 0;
    }
#endif

    // led_display(); // LED״̬������ʾ
}
#endif // end of #ifdef CIRCUIT_BOARD
/*************************** (C) COPYRIGHT 2022 TAIXIN-IC ***** END OF FILE *****/
#endif