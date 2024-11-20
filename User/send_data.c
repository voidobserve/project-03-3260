#include "send_data.h"

void send_data(u8 instruct, u32 send_data)
{
    u32 check_num = 0; // 存放校验和

    uart0_sendbyte(FORMAT_HEAD); // 先发送格式头

#if 0  // 使用swtich()语句的版本
    switch (instruct)
    {
    case SEND_GEAR:               // 发送挡位的状态
        uart0_sendbyte(0x05);     // 发送指令的总长度
        uart0_sendbyte(instruct); // 发送指令
        uart0_sendbyte(send_data);

        check_num = FORMAT_HEAD + 0x05 + SEND_GEAR + (u8)send_data;

        break;

    case SEND_BATTERY:             // 发送电池电量的状态
        uart0_sendbyte(0x05);      // 发送指令的总长度
        uart0_sendbyte(instruct);  // 发送指令
        uart0_sendbyte(send_data); // 发送电池电量信息

        check_num = FORMAT_HEAD + 0x05 + SEND_BATTERY + (u8)send_data;

        break;

    case SEND_BARKE:               // 发送刹车的状态
        uart0_sendbyte(0x05);      // 发送指令的总长度
        uart0_sendbyte(instruct);  // 发送指令
        uart0_sendbyte(send_data); // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_BARKE + (u8)send_data;

        break;

    case SEND_LEFT_TURN:           // 发送左转向灯的状态
        uart0_sendbyte(0x05);      // 发送指令的总长度
        uart0_sendbyte(instruct);  // 发送指令
        uart0_sendbyte(send_data); // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_LEFT_TURN + (u8)send_data;

        break;

    case SEND_RIGHT_TURN:          // 发送右转向灯的状态
        uart0_sendbyte(0x05);      // 发送指令的总长度
        uart0_sendbyte(instruct);  // 发送指令
        uart0_sendbyte(send_data); // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_RIGHT_TURN + (u8)send_data;

        break;

    case SEND_HIGH_BEAM:           // 发送远光灯的状态
        uart0_sendbyte(0x05);      // 发送指令的总长度
        uart0_sendbyte(instruct);  // 发送指令
        uart0_sendbyte(send_data); // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_HIGH_BEAM + (u8)send_data;

        break;

    case SEND_ENGINE_SPEED:             // 发送发动机的转速
        uart0_sendbyte(0x06);           // 发送指令的总长度
        uart0_sendbyte(instruct);       // 发送指令
        uart0_sendbyte(send_data >> 8); // 发送信息
        uart0_sendbyte(send_data);      // 发送信息

        check_num = FORMAT_HEAD + 0x06 + SEND_ENGINE_SPEED + (u8)(send_data >> 8) + (u8)(send_data);

        break;

    case SEND_SPEED:                    // 发送时速
        uart0_sendbyte(0x06);           // 发送指令的总长度
        uart0_sendbyte(instruct);       // 发送指令
        uart0_sendbyte(send_data >> 8); // 发送信息
        uart0_sendbyte(send_data);      // 发送信息

        check_num = FORMAT_HEAD + 0x06 + SEND_SPEED + (u8)(send_data >> 8) + (u8)(send_data);

        break;

    case SEND_FUEL:                // 发送油量
        uart0_sendbyte(0x05);      // 发送指令的总长度
        uart0_sendbyte(instruct);  // 发送指令
        uart0_sendbyte(send_data); // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_FUEL + (u8)send_data;

        break;

    case SEND_WATER_TEMP:          // 发送水温
        uart0_sendbyte(0x05);      // 发送指令的总长度
        uart0_sendbyte(instruct);  // 发送指令
        uart0_sendbyte(send_data); // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_WATER_TEMP + (u8)send_data;

        break;

    case SEND_TOTAL_MILEAGE:      // 发送大计里程
        uart0_sendbyte(0x07);     // 发送指令的总长度
        uart0_sendbyte(instruct); // 发送指令

        uart0_sendbyte(send_data >> 16); // 发送信息
        uart0_sendbyte(send_data >> 8);  // 发送信息
        uart0_sendbyte(send_data);       // 发送信息

        check_num = FORMAT_HEAD + 0x07 + SEND_TOTAL_MILEAGE + (u8)(send_data >> 16) + (u8)(send_data >> 8) + (u8)send_data;

        break;

    case SEND_SUBTOTAL_MILEAGE: // 发送小计里程

        uart0_sendbyte(0x06);     // 发送指令的总长度
        uart0_sendbyte(instruct); // 发送指令

        uart0_sendbyte(send_data >> 8); // 发送信息
        uart0_sendbyte(send_data);      // 发送信息

        check_num = FORMAT_HEAD + 0x06 + SEND_SUBTOTAL_MILEAGE + (u8)(send_data >> 8) + (u8)send_data;

        break;
    // case SEND_TOUCH_KEY_STATUS: // 发送触摸按键的状态

    //     uart0_sendbyte(0x06);     // 发送指令的总长度
    //     uart0_sendbyte(instruct); // 发送指令

    //     uart0_sendbyte(send_data >> 8); // 发送信息
    //     uart0_sendbyte(send_data);      // 发送信息

    //     check_num = FORMAT_HEAD + 0x06 + SEND_SUBTOTAL_MILEAGE + (u8)(send_data >> 8) + (u8)send_data;

    //     break;
    }
#endif // 使用swtich()语句的版本

#if 1 // 使用if()语句版本

    check_num += FORMAT_HEAD; // 使用if()语句时，才使用这一条程序

    if (SEND_GEAR == instruct ||             /* 发送挡位的状态 */
        SEND_BATTERY == instruct ||          /* 发送电池电量的状态 */
        SEND_BARKE == instruct ||            /* 发送刹车的状态 */
        SEND_LEFT_TURN == instruct ||        /* 发送左转向灯的状态 */
        SEND_RIGHT_TURN == instruct ||       /* 发送右转向灯的状态 */
        SEND_HIGH_BEAM == instruct ||        /* 发送远光灯的状态 */
        SEND_FUEL == instruct ||             /* 发送油量 */
        SEND_WATER_TEMP == instruct ||       /* 发送水温 */
        SEND_TEMP_OF_WATER_ALERT == instruct /* 发送水温报警 */
        )                                    // 如果指令的总长度只有5个字节
    {
        uart0_sendbyte(0x05);     // 发送指令的总长度
        uart0_sendbyte(instruct); // 发送指令
        uart0_sendbyte(send_data);

        check_num += 0x05 + (u8)instruct + (u8)send_data;
    }
    else if (SEND_ENGINE_SPEED == instruct ||     /* 发送发动机的转速 */
             SEND_SPEED == instruct ||            /* 发送时速 */
             SEND_SUBTOTAL_MILEAGE == instruct || /* 发送小计里程 */
             SEND_TOUCH_KEY_STATUS == instruct || /* 发送触摸按键的状态 */
             SEND_VOLTAGE_OF_BATTERY == instruct     /* 发送电池电压 */
             )                                    // 如果指令的总长度为6个字节
    {
        uart0_sendbyte(0x06);           // 发送指令的总长度
        uart0_sendbyte(instruct);       // 发送指令
        uart0_sendbyte(send_data >> 8); // 发送信息
        uart0_sendbyte(send_data);      // 发送信息

        check_num += 0x06 + (u8)instruct + (u8)(send_data >> 8) + (u8)(send_data);
    }
    else if (SEND_TOTAL_MILEAGE == instruct || /* 发送大计里程 */
             SEND_TIME == instruct             /* 发送时间 */
             )                                 // 如果指令的总长度为7个字节
    {

        uart0_sendbyte(0x07);     // 发送指令的总长度
        uart0_sendbyte(instruct); // 发送指令

        uart0_sendbyte(send_data >> 16); // 发送信息
        uart0_sendbyte(send_data >> 8);  // 发送信息
        uart0_sendbyte(send_data);       // 发送信息

        check_num += 0x07 + (u8)instruct + (u8)(send_data >> 16) + (u8)(send_data >> 8) + (u8)send_data;
    }
    else if (SEND_DATE == instruct /* 发送日期(年月日) */
             )                     // 如果指令的总长度为8个字节
    {
        uart0_sendbyte(0x08);     // 发送指令的总长度
        uart0_sendbyte(instruct); // 发送指令

        uart0_sendbyte(send_data >> 24); // 发送信息
        uart0_sendbyte(send_data >> 16); // 发送信息
        uart0_sendbyte(send_data >> 8);  // 发送信息
        uart0_sendbyte(send_data);       // 发送信息

        check_num += 0x08 + (u8)instruct + (u8)(send_data >> 24) + (u8)(send_data >> 16) + (u8)(send_data >> 8) + (u8)send_data;
    }

#endif // 使用if()语句版本

    // check_num &= 0x0F;         // 取前面的数字相加的低四位
    check_num &= 0xFF;         // 取前面的数字相加的低八位
    uart0_sendbyte(check_num); // 发送校验和

    delay_ms(10); // 每次发送完成后，延时10ms
}


//  如果待发送的有效数据超过了4个字节，使用以下函数
// void send_data(u8 instruction, u8 *data_buf, u8 len)
// {

// }

