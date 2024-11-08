#ifndef __UART0_H
#define __UART0_H

#include "include.h" // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�
#include "my_config.h" // �����Զ����ͷ�ļ�

// ����һ֡���ĳ���
#ifndef FRAME_MAX_LEN
#define FRAME_MAX_LEN 10
#endif // FRAME_MAX_LEN

// ������Ž��յ�����֡�Ļ�����
extern volatile u8 uart0_recv_buf[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)][FRAME_MAX_LEN];
extern volatile u8 uart0_recv_len[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)];
extern volatile u8 recved_flagbuf[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)];
extern volatile u8 recv_frame_cnt; // ���յ�������֡�ĸ���
extern volatile u8 flagbuf_valid_instruction[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)]; // ����кϷ�ָ��ı�־λ����

// ����0������
#ifndef UART0_BAUDRATE
#define UART0_BAUDRATE 115200
#endif // UART0_BAUDRATE

// ����0���ջ������Ĵ�С
#ifndef UART0_RXBUF_LEN
#define UART0_RXBUF_LEN 256
#endif

void uart0_config(void);
void uart0_sendbyte(u8 senddata);
void uart0_send_buff(u8* buf, u8 len); // ͨ��uart0������������
// void uart0_sendbytes(u8 *buf, u32 buf_len);
// void uart0_sendstr(u8 *str); // UART0�����ַ����ĺ���

void uart0_scan_handle(void); // ���鴮�ڽ��ջ������������Ƿ����ָ��ĺ���

#endif
