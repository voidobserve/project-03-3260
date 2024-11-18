// 里程表的源程序
#include "mileage.h"

// 总里程扫描
void mileage_scan(void)
{
#ifdef USE_INTERNATIONAL // 公制单位

    static u32 old_total_mileage = 0;    // 用来记录旧的大计里程的变量
    static u32 old_subtotal_mileage = 0; // 用来记录旧的小计里程的变量

    static bit flag_tmr_is_open = 0; // 定时器是否打开的标志位

    // 打开定时器
    if (0 == flag_tmr_is_open)
    {
        flag_tmr_is_open = 1;
        tmr3_cnt = 0;
        tmr3_enable();
    }

    if (tmr3_cnt >= 30000) // 30000 ms -- 30s
    {
        // 每30s将大计里程和小计里程写入flash
        // tmr3_disable();
        tmr3_cnt = 0;
        // flag_tmr_is_open = 0;

        // flash_write(0x00, (u8 *)&fun_info, sizeof(fun_info));
        fun_info_save(); // 将 fun_info 写回flash
    }

    if (distance >= 1000)
    {
        // 如果走过的距离超过了1m，再进行保存
        fun_info.total_mileage += distance / 1000;    // 将毫米转换成米，再保存
        fun_info.subtotal_mileage += distance / 1000; // 将毫米转换成米，再保存
        distance -= 1000;                             // 剩下的、未保存的、不满1m的数据留到下一次再保存
    }

    if ((fun_info.total_mileage - old_total_mileage) > 100)
    {
        // 如果大计里程有变化且超过了100m
        old_total_mileage = fun_info.total_mileage; // 记录旧的里程

        // printf("total mileage: %lum\n", fun_info.total_mileage);

        {
            // 发送数据的操作，可以先置标志位
            // 要注意发送的是五位的整数、一位的小数、单位为KM的数据
            flag_get_total_mileage = 1;
        }
    }

    if ((fun_info.subtotal_mileage - old_subtotal_mileage) > 1000)
    {
        // 如果小计里程有变化且超过了1000m
        old_subtotal_mileage = fun_info.subtotal_mileage; // 记录旧的里程

        // printf("subtotal mileage: %lum\n", fun_info.subtotal_mileage);

        {
            // 发送数据的操作，可以先置标志位
            // 要注意发送的是四位的、一位小数、单位为KM的数据
            flag_get_sub_total_mileage = 1;
        }
    }

#endif // INTERNATIONAL 公制单位

#ifdef USE_IMPERIAL // 英制单位

    static u32 old_total_mileage = 0;    // 用来记录旧的大计里程的变量
    static u32 old_subtotal_mileage = 0; // 用来记录旧的小计里程的变量

    static bit flag_tmr_is_open = 0; // 定时器是否打开的标志位

    if (0 == flag_tmr_is_open)
    {
        flag_tmr_is_open = 1;
        tmr3_cnt = 0;
        tmr3_enable();
    }

    if (tmr3_cnt >= 30000) // 30000 ms
    {
        // 每30s将大计里程和小计里程写入flash
        tmr3_disable();
        tmr3_cnt = 0;
        flag_tmr_is_open = 0;

        // flash_write(0x00, (u8 *)&fun_info, sizeof(fun_info));
        fun_info_save();
    }

    if (distance >= 1000)
    {
        // 如果走过的距离超过了1m，再进行保存
        fun_info.total_mileage += distance / 1000;    // 将毫米转换成米，再保存
        fun_info.subtotal_mileage += distance / 1000; // 将毫米转换成米，再保存
        distance -= 1000;                             // 剩下的、未保存的、不满1m的数据留到下一次的保存
    }

    if ((fun_info.total_mileage - old_total_mileage) > 161)
    {
        // 如果大计里程有变化且超过了161m，约0.1英里
        old_total_mileage = fun_info.total_mileage; // 记录旧的里程

        // printf("total mileage: %lum\n", fun_info.total_mileage);

        {
            // 发送数据的操作，可以先置标志位
            // 要注意发送的是五位的整数、一位的小数、单位为英里的数据

            flag_get_total_mileage = 1;
        }
    }

    if ((fun_info.subtotal_mileage - old_subtotal_mileage) > 1610)
    {
        // 如果小计里程有变化且超过了1610m，约1英里
        old_subtotal_mileage = fun_info.subtotal_mileage; // 记录旧的里程

        // printf("subtotal mileage: %lum\n", fun_info.subtotal_mileage);

        {
            // 发送数据的操作，可以先置标志位
            // 要注意发送的是四位的、单位为英里的数据
            flag_get_sub_total_mileage = 1;
        }
    }

#endif // USE_IMPERIAL 英制单位
}
