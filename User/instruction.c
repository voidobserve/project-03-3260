#include "instruction.h"

#include <string.h>

// 用bit定义，来节省空间
volatile bit flag_get_all_status = 0;    // 获取所有功能的状态
volatile bit flag_get_gear = 0;          // 获取挡位状态 / 得到了挡位的状态
volatile bit flag_get_battery = 0;       // 获取电池状态 / 得到了电池的状态（电池电量，单位：百分比）
volatile bit flag_get_brake = 0;         // 获取刹车状态 / 得到了刹车的状态
volatile bit flag_get_left_turn = 0;     // 获取左转向灯的状态 / 得到了左转向灯的状态
volatile bit flag_get_right_turn = 0;    // 获取右转向灯的状态 / 得到了右转向灯的状态
volatile bit flag_get_high_beam = 0;     // 获取远光灯的状态 / 得到了远光灯的状态
volatile bit flag_get_engine_speed = 0;  // 获取发动机的转速 / 得到了发动机的转速
volatile bit flag_get_speed = 0;         // 获取时速 / 得到了时速
volatile bit flag_get_fuel = 0;          // 获取油量 / 得到了油量（单位：百分比）
volatile bit flag_get_temp_of_water = 0; // 获取水温 / 得到了水温

volatile bit flag_get_total_mileage = 0;     // 获取大计里程 / 得到了大计里程
volatile bit flag_get_sub_total_mileage = 0; // 获取小计里程 / 得到了小计里程

volatile bit flag_get_touch_key_status = 0; // 获取触摸按键的状态
volatile bit flag_alter_date = 0;           // 修改日期
volatile bit flag_alter_time = 0;           // 修改时间

volatile bit flag_get_bat_val = 0;               // 获取电池电压
volatile bit flag_set_temp_of_water_warning = 0; // 设置水温报警

volatile bit flag_clear_total_mileage = 0;     // 清除大计里程
volatile bit flag_clear_sub_total_mileage = 0; // 清除小计里程

// // 存放接收到的设置水温报警的指令中，对应的操作，默认为无操作
// volatile u8 operation_set_temp_of_water_warning = OPERATION_SET_TEMP_OF_WATER_WARNING_NONE;

// 检查接收是否正确的函数，如果接收正确，
// 根据接收到的数据中的指令，给对应的指令的标志位置一
void instruction_scan(void)
{
    u8 i = 0; // 注意要大于等于缓冲区能存放的指令数目

    for (i = 0; i < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); i++)
    {
        if (flagbuf_valid_instruction[i]) // 如果对应的位置有合法的数据帧
        {
            // 根据接收到数据中的指令，给对应的指令的标志位置一
            // if (4 == uart0_recv_buf[i][1]) // 如果是四位长度的指令
            {
                switch (uart0_recv_buf[i][2]) // 根据不同的指令来给对应的标志位置一
                {
                case INSTRUCTION_GET_ALL_STATUS: // 获取所有功能的状态
                    flag_get_all_status = 1;
                    break;

                case INSTRUCTION_GET_GEAR: // 获取挡位的状态
                    flag_get_gear = 1;
                    break;

                case INSTRUCTION_GET_BATTERY: // 获取电池电量的状态
                    flag_get_battery = 1;
                    break;

                case INSTRUCTION_GET_BARKE: // 获取刹车的状态
                    flag_get_brake = 1;
                    break;

                case INSTRUCTION_GET_LEFT_TURN: // 获取左转向灯的状态
                    flag_get_left_turn = 1;
                    break;

                case INSTRUCTION_GET_RIGHT_TURN: // 获取右转向灯的状态
                    flag_get_right_turn = 1;
                    break;

                case INSTRUCTION_GET_HIGH_BEAM: // 获取远光灯的状态
                    flag_get_high_beam = 1;
                    break;

                case INSTRUCTION_GET_ENGINE_SPEED: // 获取发动机转速
                    flag_get_engine_speed = 1;
                    break;

                case INSTRUCTION_GET_SPEED: // 获取时速
                    flag_get_speed = 1;
                    break;

                case INSTRUCTION_GET_FUEL: // 获取油量
                    flag_get_fuel = 1;
                    break;

                case INSTRUCTION_GET_TEMP_OF_WATER: // 获取水温
                    flag_get_temp_of_water = 1;
                    break;

                case INSTRUCTION_GET_TOTAL_MILEAGE: // 获取大计里程
                    flag_get_total_mileage = 1;
                    break;

                case INSTRUCTION_GET_SUBTOTAL_MILEAGE: // 获取小计里程
                    flag_get_sub_total_mileage = 1;
                    break;

                case INSTRUCTION_GET_TOUCH_KEY_STATUS: // 获取触摸按键的状态
                    flag_get_touch_key_status = 1;
                    break;

                case INSTRUCTION_ALTER_DATE: // 修改日期
                    flag_alter_date = 1;
                    // fun_info.date = ((u32)uart0_recv_buf[i][3] << 24) + /* 年 */
                    //                 ((u32)uart0_recv_buf[i][4] << 16) +
                    //                 ((u32)uart0_recv_buf[i][5] << 8) + /* 月 */
                    //                 ((u32)uart0_recv_buf[i][6] << 0);  /* 日 */
                    fun_info.year = ((u16)uart0_recv_buf[i][3] << 8) +
                                    (u16)uart0_recv_buf[i][4];
                    fun_info.month = uart0_recv_buf[i][5];
                    fun_info.day = uart0_recv_buf[i][6];

                    break;

                case INSTRUCTION_ALTER_TIME: // 修改时间
                    flag_alter_time = 1;
                    // printf("buff:\n");
                    // printf("%x %x %x %x %x %x %x \n", (u32)uart0_recv_buf[i][0], (u32)uart0_recv_buf[i][1], (u32)uart0_recv_buf[i][2], (u32)uart0_recv_buf[i][3], (u32)uart0_recv_buf[i][4], (u32)uart0_recv_buf[i][5], (u32)uart0_recv_buf[i][6]);

#if 0 // 测试用的程序
                    {
                        u8 j = 0;
                        for (j = 0; j < (FRAME_MAX_LEN); j++)
                        {
                            printf("%2x ", (u16)uart0_recv_buf[i][j]);
                        }
                        printf("\n==========================\n");
                    }

                    { // 测试校验和通不通过
                        u8 check_sum = 0;
                        check_sum += (uart0_recv_buf[i][0] +
                                      uart0_recv_buf[i][1] +
                                      uart0_recv_buf[i][2] +
                                      uart0_recv_buf[i][3] +
                                      uart0_recv_buf[i][4] +
                                      uart0_recv_buf[i][5]);
                        check_sum &= 0x0F;
                        if (check_sum == uart0_recv_buf[i][6])
                        {
                            printf("check sum pass\n");
                        }
                        else
                        {
                            printf("check sum err \n");
                        }
                    }

                    if (0)
                    {
                        u8 i = 0;
                        u8 j = 0;
                        u8 k = 0;
                        for (i = 0; i < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); i++)
                        {
                            for (j = 0; j < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); j++)
                            {
                                uart0_sendbyte(j + '0');
                                uart0_sendbyte('\n');
                                for (k = 0; k < FRAME_MAX_LEN; k++)
                                {
                                    printf("%2x ", (u16)uart0_recv_buf[j][k]);
                                }

                                printf("\n==========================\n");
                            }
                        }
                    }

#endif // 向串口发送整个缓冲区的数据
                    break;

                case INSTRUCTION_GET_BATTERY_VAL: // 获取电池电压
                    flag_get_bat_val = 1;
                    break;

                case INSTRUCTION_SET_TEMP_OF_WATER_WARNING: // 设置水温报警
                    flag_set_temp_of_water_warning = 1;

                    break;

                case INSTRUCTION_CLEAR_TOTAL_MILEAGE: // 清除大计里程
                    flag_clear_total_mileage = 1;
                    break;

                case INSTRUCTION_CLEAR_SUBTOTAL_MILEAGE: // 清除小计里程
                    flag_clear_sub_total_mileage = 1;
                    break;
                }

                if (recv_frame_cnt > 0) //
                {
                    recv_frame_cnt--; // 从串口接收的数据帧数目减一，表示指令已经从缓冲区取出
                }

                flagbuf_valid_instruction[i] = 0; // 清空缓冲区对应的元素，表示该下标的指令已经处理
                uart0_recv_len[i] = 0;
                recved_flagbuf[i] = 0;
                memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN); // 清空缓冲区对应的元素
            } // if (4 == uart0_recv_buf[i][1])
        }
    }
}

void instruction_handle(void)
{
    if (flag_get_all_status)
    {
        // 如果要获取所有功能的状态
        flag_get_all_status = 0; // 清除标志位

#if USE_MY_DEBUG
        printf(" flag_get_all_status\n");
#endif

        // 获取所有功能的状态，需要把这些功能对应的状态都发送出去
    }

    if (flag_get_gear)
    {
        // 如果要获取挡位的状态
        flag_get_gear = 0; //

#if USE_MY_DEBUG
        printf(" flag_get_gear\n");
#endif

        send_data(SEND_GEAR, fun_info.gear); // 发送当前挡位的状态
    }

    if (flag_get_battery)
    {
        // 如果要获取电池电量的状态
        flag_get_battery = 0;
#if USE_MY_DEBUG
        printf(" flag_get_battery\n");
#endif

        send_data(SEND_BATTERY, fun_info.battery); // 发送电池电量
    }

    if (flag_get_brake)
    {
        // 如果要获取获取刹车的状态
        flag_get_brake = 0;

#if USE_MY_DEBUG
        printf(" flag_get_brake\n");
#endif

        send_data(SEND_BARKE, fun_info.brake); // 发送当前刹车的状态
    }

    if (flag_get_left_turn)
    {
        // 如果要获取左转向灯的状态
        flag_get_left_turn = 0;

#if USE_MY_DEBUG
        printf(" flag_get_left_turn\n");
#endif

        send_data(SEND_LEFT_TURN, fun_info.left_turn); // 发送当前左转向灯的状态
    }

    if (flag_get_right_turn)
    {
        // 如果要获取右转向灯的状态
        flag_get_right_turn = 0;

        printf(" flag_get_right_turn\n");

        send_data(SEND_RIGHT_TURN, fun_info.right_turn); // 发送当前右转向灯的状态
    }

    if (flag_get_high_beam)
    {
        // 如果要获取远光灯的状态
        flag_get_high_beam = 0;

        printf(" flag_get_high_beam\n");

        send_data(SEND_HIGH_BEAM, fun_info.high_beam); // 发送当前远光灯的状态
    }

    if (flag_get_engine_speed)
    {
        // 如果要获取发动机的转速
        flag_get_engine_speed = 0;

        printf(" flag_get_engine_speed \n");

        send_data(SEND_ENGINE_SPEED, fun_info.engine_speeed);
    }

    if (flag_get_speed)
    {
        // 如果要获取时速
        flag_get_speed = 0;

        printf(" flag_get_speed \n");

#ifdef INTERNATIONAL // 使用公制单位

        send_data(SEND_SPEED, fun_info.speed); // 发送当前采集到的车速（时速）

#endif // INTERNATIONAL 使用公制单位

#ifdef IMPERIAL // 使用英制单位

        send_data(SEND_SPEED, fun_info.speed * 621 / 1000);

#endif
    }

    if (flag_get_fuel)
    {
        // 如果要获取油量
        flag_get_fuel = 0;

        printf(" flag_get_fuel \n");
    }

    if (flag_get_temp_of_water)
    {
        // 如果要获取水温
        flag_get_temp_of_water = 0;

        printf(" flag_get_temp_of_water \n");
    }

    if (flag_get_total_mileage)
    {
        // 如果要获取大计里程 / 得到了大计里程新的数据
        flag_get_total_mileage = 0;
        printf(" flag_get_total_mileage \n");

#ifdef INTERNATIONAL // 公制单位

        // 只发送百米及以上的数据
        send_data(SEND_TOTAL_MILEAGE, fun_info.total_mileage / 100);

#endif // INTERNATIONAL 公制单位

#ifdef IMPERIAL // 英制单位
#if USE_MY_DEBUG
        printf("total mileage: %lu * 0.1 mile", fun_info.total_mileage * 62137 / 10000000);
#endif // USE_MY_DEBUG
       // 只发送0.1英里及以上的数据
        send_data(SEND_TOTAL_MILEAGE, fun_info.total_mileage * 62137 / 10000000);

#endif // IMPERIAL 英制单位
    }

    if (flag_get_sub_total_mileage)
    {
        // 如果要获取小计里程 / 得到了小计里程新的数据
        flag_get_sub_total_mileage = 0;
        printf(" flag_get_sub_total_mileage \n");
#ifdef INTERNATIONAL // 公制单位

        // 只发送千米及以上的数据
        send_data(SEND_SUBTOTAL_MILEAGE, fun_info.subtotal_mileage / 1000);

#endif // INTERNATIONAL 公制单位

#ifdef IMPERIAL // 英制单位
#if USE_MY_DEBUG
        printf("sub total mileage: %lu mile", fun_info.subtotal_mileage * 62137 / 100000000);
#endif // USE_MY_DEBUG
       // 只发送英里及以上的数据
        send_data(SEND_SUBTOTAL_MILEAGE, fun_info.subtotal_mileage * 62137 / 100000000);

#endif // IMPERIAL 英制单位
    }

    if (flag_get_touch_key_status)
    {
        // 如果要获取触摸按键的状态 / 得到了触摸按键的状态
        flag_get_touch_key_status = 0;
        printf(" flag_get_touch_key_status \n");
    }

    if (flag_alter_date)
    {
        // 如果要修改日期
        flag_alter_date = 0;
        printf(" flag_alter_date \n");

        // printf("year %d month %d day %d \n", fun_info.date >> 16, ((fun_info.date >> 8) & 0xFF), (fun_info.date & 0xFF));
        printf("year %d month %d day %d \n", (u16)fun_info.year, (u16)fun_info.month, (u16)fun_info.day);

        fun_info_save(); // 将日期信息写回flash
    }

    if (flag_alter_time)
    {
        // 如果要修改时间
        flag_alter_time = 0;
        printf(" flag_alter_time \n");
    }

    if (flag_get_bat_val)
    {
        // 如果要获取电池电压
        flag_get_bat_val = 0;
        printf(" flag_get_bat_val \n");
    }

    if (flag_set_temp_of_water_warning)
    {
        // 如果要设置水温报警
        flag_set_temp_of_water_warning = 0;
        printf(" flag_set_temp_of_water_warning \n");

        //  发送当前水温报警的状态
    }

    if (flag_clear_total_mileage)
    {
        // 如果要清除大计里程
        flag_clear_total_mileage = 0;
        printf(" flag_clear_total_mileage \n");
    }

    if (flag_clear_sub_total_mileage)
    {
        // 如果要清除小计里程
        flag_clear_sub_total_mileage = 0;
        printf(" flag_clear_sub_total_mileage \n");
    }
}
