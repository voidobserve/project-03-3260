#include "send_data.h"

void send_data(u32 instruct, u32 send_data)
{
    u32 check_num = 0; // 存放校验和

    uart0_sendbyte(FORMAT_HEAD); // 先发送格式头

    switch (instruct)
    {
    case SEND_GEAR:                // 发送挡位的状态
        uart0_sendbyte(0x05);      // 发送指令的总长度
        uart0_sendbyte(SEND_GEAR); // 发送指令
        uart0_sendbyte(send_data);

        check_num = FORMAT_HEAD + 0x05 + SEND_GEAR + (u8)send_data;
        check_num &= 0x0F; // 取前面的数字相加的低四位

        break;

    case SEND_BATTERY:                // 发送电池电量的状态
        uart0_sendbyte(0x05);         // 发送指令的总长度
        uart0_sendbyte(SEND_BATTERY); // 发送指令
        uart0_sendbyte(send_data);    // 发送电池电量信息

        check_num = FORMAT_HEAD + 0x05 + SEND_BATTERY + (u8)send_data;
        check_num &= 0x0F; // 取前面的数字相加的低四位
        break;

    case SEND_BARKE:                // 发送刹车的状态
        uart0_sendbyte(0x05);       // 发送指令的总长度
        uart0_sendbyte(SEND_BARKE); // 发送指令
        uart0_sendbyte(send_data);  // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_BARKE + (u8)send_data;
        check_num &= 0x0F; // 取前面的数字相加的低四位
        break;

    case SEND_LEFT_TURN:                // 发送左转向灯的状态
        uart0_sendbyte(0x05);           // 发送指令的总长度
        uart0_sendbyte(SEND_LEFT_TURN); // 发送指令
        uart0_sendbyte(send_data);      // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_LEFT_TURN + (u8)send_data;
        check_num &= 0x0F; // 取前面的数字相加的低四位
        break;

    case SEND_RIGHT_TURN:                // 发送右转向灯的状态
        uart0_sendbyte(0x05);            // 发送指令的总长度
        uart0_sendbyte(SEND_RIGHT_TURN); // 发送指令
        uart0_sendbyte(send_data);       // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_RIGHT_TURN + (u8)send_data;
        check_num &= 0x0F; // 取前面的数字相加的低四位
        break;

    case SEND_HIGH_BEAM:                // 发送远光灯的状态
        uart0_sendbyte(0x05);           // 发送指令的总长度
        uart0_sendbyte(SEND_HIGH_BEAM); // 发送指令
        uart0_sendbyte(send_data);      // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_HIGH_BEAM + (u8)send_data;
        check_num &= 0x0F; // 取前面的数字相加的低四位
        break;

    case SEND_ENGINE_SPEED:                // 发送发动机的转速
        uart0_sendbyte(0x06);              // 发送指令的总长度
        uart0_sendbyte(SEND_ENGINE_SPEED); // 发送指令
        uart0_sendbyte(send_data >> 8);    // 发送信息
        uart0_sendbyte(send_data);         // 发送信息

        check_num = FORMAT_HEAD + 0x06 + SEND_ENGINE_SPEED + (u8)(send_data >> 8) + (u8)(send_data);
        check_num &= 0x0F; // 取前面的数字相加的低四位
        break;

    case SEND_SPEED:                    // 发送时速
        uart0_sendbyte(0x06);           // 发送指令的总长度
        uart0_sendbyte(SEND_SPEED);     // 发送指令
        uart0_sendbyte(send_data >> 8); // 发送信息
        uart0_sendbyte(send_data);      // 发送信息

        check_num = FORMAT_HEAD + 0x06 + SEND_SPEED + (u8)(send_data >> 8) + (u8)(send_data);
        check_num &= 0x0F; // 取前面的数字相加的低四位
        break;

    case SEND_FUEL:                // 发送油量
        uart0_sendbyte(0x05);      // 发送指令的总长度
        uart0_sendbyte(SEND_FUEL); // 发送指令
        uart0_sendbyte(send_data); // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_FUEL + (u8)send_data;
        check_num &= 0x0F; // 取前面的数字相加的低四位
        break;

    case SEND_WATER_TEMP:                // 发送水温
        uart0_sendbyte(0x05);            // 发送指令的总长度
        uart0_sendbyte(SEND_WATER_TEMP); // 发送指令
        uart0_sendbyte(send_data);       // 发送信息

        check_num = FORMAT_HEAD + 0x05 + SEND_WATER_TEMP + (u8)send_data;
        check_num &= 0x0F; // 取前面的数字相加的低四位
        break;

    case SEND_TOTAL_MILEAGE:                // 发送大计里程
        uart0_sendbyte(0x07);               // 发送指令的总长度
        uart0_sendbyte(SEND_TOTAL_MILEAGE); // 发送指令

        uart0_sendbyte(send_data >> 16); // 发送信息
        uart0_sendbyte(send_data >> 8);  // 发送信息
        uart0_sendbyte(send_data);       // 发送信息

        check_num = FORMAT_HEAD + 0x07 + SEND_TOTAL_MILEAGE + (u8)(send_data >> 16) + (u8)(send_data >> 8) + (u8)send_data;
        check_num &= 0x0F; // 取前面的数字相加的低四位
        break;

    case SEND_SUBTOTAL_MILEAGE: // 发送小计里程

        uart0_sendbyte(0x06);               // 发送指令的总长度
        uart0_sendbyte(SEND_SUBTOTAL_MILEAGE); // 发送指令

        uart0_sendbyte(send_data >> 8);  // 发送信息
        uart0_sendbyte(send_data);       // 发送信息

        check_num = FORMAT_HEAD + 0x06 + SEND_SUBTOTAL_MILEAGE + (u8)(send_data >> 8) + (u8)send_data;
        check_num &= 0x0F; // 取前面的数字相加的低四位

        break;
    }

    uart0_sendbyte(check_num); // 发送校验和

    delay_ms(10); // 每次发送完成后，延时10ms
}

