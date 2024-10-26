#ifndef __UART1_H
#define __UART1_H

#include "include.h" // ʹ��оƬ�ٷ��ṩ��ͷ�ļ�

#include "my_config.h" // �����Զ����ͷ�ļ�

// ����1������
#ifndef UART1_BAUDRATE
#define UART1_BAUDRATE  460800
#endif // UART1_BAUDRATE

void uart1_init(void);
void uart1_sendbyte(u8 senddata);
void uart1_sendbytes(u8 *buf, u32 buf_len);

// ����ʹ��UART1+DMA�����ղ����������ݣ�����Ĵ��벻����
void uart1_dma_recv_scan(void);

#endif
