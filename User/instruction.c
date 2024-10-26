#include "instruction.h"

// 检查接收是否正确的函数，如果接收正确，
// 根据接收到的数据中的指令，给对应的指令的标志位置一
void instruction_scan(void)
{
    u32 i = 0;

    for (i = 0; i < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); i++)
    {
        if (recved_flagbuf[i]) // 如果对应的位置有合法的数据帧
        {
            // 根据接收到数据中的指令，给对应的指令的标志位置一
            if (4 == uart0_recv_buf[i][1]) // 如果是四位长度的指令
            {
                switch (uart0_recv_buf[i][2]) // 根据不同的指令来给对应的标志位置一
                {
                case INSTRUCTION_GET_ALL_STATUS: // 获取所有功能的状态
                    flag_get_all_status = 1;
                    break;

                case INSTRUCTION_GET_GEAR: // 获取挡位的状态
                    get_gear = 1;
                    break;

                case INSTRUCTION_GET_BATTERY: // 获取电池电量的状态
                    get_battery = 1;
                    break;

                case INSTRUCTION_GET_BARKE: // 获取刹车的状态
                    get_brake = 1;
                    break;

                case INSTRUCTION_GET_LEFT_TURN: // 获取左转向灯的状态
                    get_left_turn = 1;
                    break;

                case INSTRUCTION_GET_RIGHT_TURN: // 获取右转向灯的状态
                    get_right_turn = 1;
                    break;

                case INSTRUCTION_GET_HIGH_BEAM: // 获取远光灯的状态
                    get_high_beam = 1;
                    break;

                case INSTRUCTION_GET_ENGINE_SPEED: // 获取发动机转速
                    get_engine_speed = 1;
                    break;

                case INSTRUCTION_GET_SPEED: // 获取时速
                    flag_get_speed = 1;
                    break;

                case INSTRUCTION_GET_FUEL: // 获取油量
                    getFuel = 1;
                    break;

                case INSTRUCTION_GET_WATER_TEMP: // 获取水温
                    getWaterTemp = 1;
                    break;

                case INSTRUCTION_GET_TOTAL_MILEAGE: // 获取大计里程
                    getTotalMileage = 1;
                    break;

                case INSTRUCTION_GET_SUBTOTAL_MILEAGE: // 获取小计里程
                    getSubTotalMileage = 1;
                    break;



                case INSTRUCTION_GET_TOUCH_KEY_STATUS: // 获取触摸按键的状态

                break;
                
                }

                if (recv_frame_cnt > 0) //
                {
                    recv_frame_cnt--; // 从串口接收的数据帧数目减一，表示指令已经从缓冲区取出
                }

                recved_flagbuf[i] = 0;
            } // if (4 == uart0_recv_buf[i][1])
        } 
    }
}
