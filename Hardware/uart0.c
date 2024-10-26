#include "uart0.h"

#include <string.h> // memset()

// 设置的波特率需要适配单片机的时钟，这里直接使用了官方的代码
#define USER_UART0_BAUD ((SYSCLK - UART0_BAUDRATE) / (UART0_BAUDRATE))

// 用来存放接收的数据帧的缓冲区
volatile u8 uart0_recv_buf[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)][FRAME_MAX_LEN] = {0};
// 用于记录接收缓冲区的对应的数据帧的长度
volatile u8 uart0_recv_len[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)] = {0};
// 记录缓冲区中对应的位置是否有数据的标志位（数组）
// 例如，缓冲区下标0的位置有指令，标志位数组下标0的元素值为1，没有指令，元素的值为0
volatile u8 recved_flagbuf[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)] = {0};

// 串口中断服务函数中，接收一帧的标志位，0--准备接收一帧的第一个字节，1--正在接收该帧的数据
static volatile bit recv_frame_flag = 0;
static volatile u8 frame_len = 0;    // 记录串口中断服务函数中，当前要接收的数据帧的字节数
static volatile u8 cur_recv_len = 0; // 记录串口中断服务函数中，当前已接收的数据帧的字节数
volatile u8 recv_frame_cnt = 0;      // 接收到的数据帧的个数

static volatile u32 blank_index = 0; // 记录当前存放数据帧的缓冲区的空的地方(缓冲区下标)，准备存放一帧的数据

// 重写putchar()函数
char putchar(char c)
{
    uart0_sendbyte(c);
    return c;
}

#ifdef DEVELOPMENT_BOARD
// uart0初始化
// 波特率由宏 UART0_BAUDRATE 来决定
void uart0_init(void)
{
    // 开发板上，P11为发送引脚，P12为接收引脚
    P1_MD0 &= (~GPIO_P11_MODE_SEL(0x3) | ~GPIO_P12_MODE_SEL(0x3));
    P1_MD0 |= GPIO_P11_MODE_SEL(0x1); // 输出模式
    FOUT_S11 |= GPIO_FOUT_UART0_TX;   // 配置P11为UART0_TX
    FIN_S7 |= GPIO_FIN_SEL_P12;       // 配置P12为UART0_RX

    __EnableIRQ(UART0_IRQn); // 打开UART模块中断
    IE_EA = 1;               // 打开总中断

    UART0_BAUD1 = (USER_UART0_BAUD >> 8) & 0xFF; // 配置波特率高八位
    UART0_BAUD0 = USER_UART0_BAUD & 0xFF;        // 配置波特率低八位
    UART0_CON0 = UART_STOP_BIT(0x0) |
                 UART_RX_IRQ_EN(0x1) |
                 UART_EN(0x1); // 8bit数据，1bit停止位，使能RX中断
}
#endif // DEVELOPMENT_BOARD

#ifdef CIRCUIT_BOARD
// uart0初始化
// 波特率由宏 UART0_BAUDRATE 来决定
void uart0_init(void)
{
    // 电路板上，P11为发送引脚，P12为接收引脚
    P1_MD0 &= (~GPIO_P11_MODE_SEL(0x3) | ~GPIO_P12_MODE_SEL(0x3));
    P1_MD0 |= GPIO_P11_MODE_SEL(0x1); // 输出模式
    FOUT_S11 |= GPIO_FOUT_UART0_TX;   // 配置P11为UART0_TX
    FIN_S7 |= GPIO_FIN_SEL_P12;       // 配置P12为UART0_RX

    __EnableIRQ(UART0_IRQn); // 打开UART模块中断
    IE_EA = 1;               // 打开总中断

    UART0_BAUD1 = (USER_UART0_BAUD >> 8) & 0xFF; // 配置波特率高八位
    UART0_BAUD0 = USER_UART0_BAUD & 0xFF;        // 配置波特率低八位
    UART0_CON0 = UART_STOP_BIT(0x0) |
                 UART_RX_IRQ_EN(0x1) |
                 UART_EN(0x1); // 8bit数据，1bit停止位，使能RX中断

    // memset(uart0_recv_buf, 0, sizeof(uart0_recv_buf));
}
#endif // DEVELOPMENT_BOARD

// UART0中断服务函数（接收中断）
void UART0_IRQHandler(void) interrupt UART0_IRQn
{
    // 进入中断设置IP，不可删除
    __IRQnIPnPush(UART0_IRQn);
    // ---------------- 用户函数处理 -------------------
    // RX接收完成中断
    if (UART0_STA & UART_RX_DONE(0x1))
    {
        if (0 == recv_frame_flag && UART0_DATA == 0xA5)
        {
            // 如果是新的一帧数据（以格式头0xA5开始），打开定时器，为超时判断做准备
            recv_frame_flag = 1;

            {
                // 重新给TMR0配置时钟
                TMR0_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除定时器的时钟源配置寄存器
                TMR0_CONL |= TMR_SOURCE_SEL(0x06);    // 配置定时器的时钟源，使用系统时钟（约21MHz）
                __EnableIRQ(TMR0_IRQn);               // 使能中断
                IE_EA = 1;                            // 打开总中断
            }
        }
        else if (0 == recv_frame_flag && UART0_DATA != 0xA5)
        {
            // 如果是新的一帧数据，却不是以0xA5开头，说明这一帧数据无效

            // 退出中断设置IP，不可删除
            __IRQnIPnPop(UART0_IRQn);
            return;
        }
        else if (1 == recv_frame_flag && UART0_DATA == 0xA5)
        {
            // 如果正在接收一帧数据，却又收到了一次格式头，舍弃之前收到的数据，重新接收这一帧
            cur_recv_len = 0;
            frame_len = 0;
        }
        else if (recv_frame_cnt > ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)))
        {
            // 如果缓冲区已满，存不下更多的数据帧

            // 退出中断设置IP，不可删除
            __IRQnIPnPop(UART0_IRQn);
            return;
        }

        if (1 == cur_recv_len)
        {
            // 如果接收到一帧的第二个字节的数据，记录下要接收的数据长度
            frame_len = UART0_DATA;
        }

        if (0 == recved_flagbuf[blank_index])
        {
            // 如果缓冲区中有剩余位置来存放一个帧，才放入数据
            uart0_recv_buf[blank_index][cur_recv_len++] = UART0_DATA;
            uart0_recv_len[blank_index] = cur_recv_len; // 更新当前数据帧的长度
            tmr0_cnt = 0;                               // 每接收到一个字节数据，清空超时的计时
        }

        // 如果发送方填错了数据帧的长度，这里的判断就会影响下一帧数据的接收
        // 因此在中断开头加了格式头的识别，在扫描函数加了帧长度的验证，避免数据过长或过短的情况
        if (cur_recv_len == frame_len)
        {
            // 如果接收完了一帧数据
            recv_frame_flag = 0; // 标志位清零，准备接收下一帧数据
            cur_recv_len = 0;
            frame_len = 0;

            recved_flagbuf[blank_index] = 1; // 对应的接收完成标志位置一
            recv_frame_cnt++;                // 接收到完整的一帧，计数值加一

            if (recv_frame_cnt < ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)))
            {
                // 如果数组中有空位（接收的数据帧个数 < 缓冲区总共能接收的数据帧个数）

                // 偏移到数组中空的地方，准备填入下一个数据帧
                while (1)
                {
                    blank_index++;

                    if (blank_index >= ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)))
                    {
                        // 如果下标超出了缓冲区能容纳的指令个数
                        blank_index = 0;
                    }

                    if (0 == recved_flagbuf[blank_index])
                    {
                        // 如果是空的一处缓冲区，退出，准备给下一次接收数据帧
                        break;
                    }
                }
            }

            {
                // 不给定时器提供时钟，让它停止计数
                TMR0_CONL &= ~(TMR_SOURCE_SEL(0x07)); // 清除定时器的时钟源配置寄存器
                TMR0_CONL |= TMR_SOURCE_SEL(0x05);    // 配置定时器的时钟源，不用任何时钟
                // 清除定时器的计数值
                TMR0_CNTL = 0;
                TMR0_CNTH = 0;
                __DisableIRQ(TMR0_IRQn); // 关闭中断（不使能中断）
                tmr0_cnt = 0;
            }
        }
    }

    // 退出中断设置IP，不可删除
    __IRQnIPnPop(UART0_IRQn);
}

// UART0发送一个字节数据的函数
void uart0_sendbyte(u8 senddata)
{
    while (!(UART0_STA & UART_TX_DONE(0x01)))
        ;
    UART0_DATA = senddata;
    while (!(UART0_STA & UART_TX_DONE(0x01))) // 等待这次发送完成
        ;
}

// 对UART0接收缓冲区的数据进行验证（超时验证、长度验证、校验和计算）
void uart0_scan_handle(void)
{
    u16 i = 0; // 循环计数值

    u32 checksum = 0; // 存放临时的校验和

    // u16 j = 0; // 测试用的循环计数值

#if 0  // 测试用
    static volatile u8 tmp1 = 0;
    static volatile u8 tmp2 = 0;

    tmp2 = recv_frame_cnt;

    if (tmp1 != tmp2) // 如果接收的帧的个数不一样
    {
        for (i = 0; i < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); i++)
        {
            printf("%d :\t", i);
            for (j = 0; j < FRAME_MAX_LEN; j++)
            {
                printf("%x ", (int16)uart0_recv_buf[i][j]);
            }
            printf("\n");
        }
    }
#endif // 测试用

#if 1 // 接收超时处理

    if (tmr0_cnt > 100) // 一帧内，超过10ms没有收到数据
    {
        // 如果超时
        // uart0_sendstr("Time out!\n");

        tmr0_disable(); // 关闭定时器
        tmr0_cnt = 0;   // 清空定时器计数值

        // 当前的数据帧作废
        cur_recv_len = 0;    // 当前接收到的帧的长度清零
        frame_len = 0;       // 要接收的帧的长度，清零
        recv_frame_flag = 0; // 重新开始接收数据

#if 0  // 向串口发送整个缓冲区的数据
        for (i = 0; i < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); i++)
        {
            uart0_sendbyte(i + '0');
            uart0_sendbyte('\n');
            uart0_sendnums(uart0_recv_buf[i], FRAME_MAX_LEN);
            uart0_sendstr("\n==========================\n");
        }
#endif // 向串口发送整个缓冲区的数据

#if 0
        for (i = 0; i < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); i++)
        {
            printf("%d :\t", i);
            for (j = 0; j < FRAME_MAX_LEN; j++)
            {
                printf("%x ", uart0_recv_buf[i][j]);
            }
            printf("\n");
        }
#endif // MY_DEBUG

        return;
    }
#endif // 接收超时处理

    // 对收到的数据帧进行验证
    for (i = 0; i < ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)); i++)
    {
        if (recved_flagbuf[i])
        {
            // 如果对应的位置有完整的数据帧

            if (uart0_recv_len[i] != uart0_recv_buf[i][1])
            {
                // 如果数据帧的长度不正确
                // uart0_sendstr("format len invalid!\n");
                recved_flagbuf[i] = 0;
                recv_frame_cnt--;
                // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);

                // for (i = 0; i < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); i++)
                // {
                //     uart0_sendbyte(i + '0');
                //     uart0_sendbyte('\n');
                //     uart0_sendnums(uart0_recv_buf[i], FRAME_MAX_LEN);
                //     uart0_sendstr("\n==========================\n");
                // }

                return;
            }

            switch (uart0_recv_len[i])
            {
            case 4:

                // 如果是四个字节的数据
                checksum = (uart0_recv_buf[i][0] + uart0_recv_buf[i][1] + uart0_recv_buf[i][2]) & 0x0F;

                if (checksum != uart0_recv_buf[i][3])
                {
                    // 如果计算得出的校验和与数据帧中的校验和不一致
                    // 说明传输的数据有误
                    // uart0_sendstr("checknum err_4Bytes\n");
                    recved_flagbuf[i] = 0;
                    recv_frame_cnt--;
                    // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);
                }

                break;

            case 5:

                // 如果是五个字节的数据
                checksum = (uart0_recv_buf[i][0] + uart0_recv_buf[i][1] + uart0_recv_buf[i][2] + uart0_recv_buf[i][3]) & 0x0F;

                if (checksum != uart0_recv_buf[i][4])
                {
                    // 如果计算得出的校验和与数据帧中的校验和不一致
                    // 说明传输的数据有误
                    // uart0_sendstr("checknum err_5Bytes\n");
                    recved_flagbuf[i] = 0;
                    recv_frame_cnt--;
                    // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);
                }

                break;
            case 6:

                // 如果是六个字节的数据
                checksum = (uart0_recv_buf[i][0] + uart0_recv_buf[i][1] + uart0_recv_buf[i][2] + uart0_recv_buf[i][3] + uart0_recv_buf[i][4]) & 0x0F;

                if (checksum != uart0_recv_buf[i][5])
                {
                    // 如果计算得出的校验和与数据帧中的校验和不一致
                    // 说明传输的数据有误
                    // uart0_sendstr("checknum err_6Bytes\n");
                    recved_flagbuf[i] = 0;
                    recv_frame_cnt--;
                    // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);
                }

                break;

            default:
                // 如果不是四、五或者六个字节的数据，说明接收有误，直接抛弃这一帧数据
                // uart0_sendstr("recv cnt err\n");
                recved_flagbuf[i] = 0;
                recv_frame_cnt--;
                // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);

                break;
            }

            // 如果运行到这里，数据都正常
        }
    }
}
