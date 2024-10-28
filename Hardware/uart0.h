#ifndef __UART0_H
#define __UART0_H

#include "include.h" // 使用芯片官方提供的头文件
#include "my_config.h" // 包含自定义的头文件

// 定义一帧最大的长度
#ifndef FRAME_MAX_LEN
#define FRAME_MAX_LEN 10
#endif // FRAME_MAX_LEN

// 用来存放接收的数据帧的缓冲区
extern volatile u8 uart0_recv_buf[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)][FRAME_MAX_LEN];
extern volatile u8 recved_flagbuf[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)];
extern volatile u8 recv_frame_cnt; // 接收到的数据帧的个数
extern volatile u8 flagbuf_valid_instruction[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)]; // 存放有合法指令的标志位数组

// 串口0波特率
#ifndef UART0_BAUDRATE
#define UART0_BAUDRATE 115200
#endif // UART0_BAUDRATE

// 串口0接收缓冲区的大小
#ifndef UART0_RXBUF_LEN
#define UART0_RXBUF_LEN 256
#endif

void uart0_init(void);
void uart0_sendbyte(u8 senddata);
// void uart0_sendbytes(u8 *buf, u32 buf_len);
// void uart0_sendstr(u8 *str); // UART0发送字符串的函数

void uart0_scan_handle(void); // 检验串口接收缓冲区的数据是否符合指令的函数

#endif
