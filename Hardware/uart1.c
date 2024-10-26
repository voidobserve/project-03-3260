// ����ʹ��UART1+DMA�����ղ����������ݣ�����Ĵ��벻����
#include "uart1.h"

// ���õĲ�������Ҫ���䵥Ƭ����ʱ�ӣ�����ֱ��ʹ���˹ٷ��Ĵ���
#define USER_UART1_BAUD ((SYSCLK - UART1_BAUDRATE) / (UART1_BAUDRATE))

// ���ջ������Ĵ�С
#define UART1_RXBUF_LEN 30

#define RX_DMA_LEN 30


volatile u8 uart1_rx_buf[UART1_RXBUF_LEN] = {0};

void uart1_init(void)
{
    // ��·���ϣ�P11Ϊ�������ţ�P12Ϊ��������
    P1_MD0 &= (~GPIO_P11_MODE_SEL(0x3) | ~GPIO_P12_MODE_SEL(0x3));
    P1_MD0 |= GPIO_P11_MODE_SEL(0x1); // ���ģʽ
    FOUT_S11 |= GPIO_FOUT_UART1_TX;   // ����P11ΪUART1_TX
    FIN_S8 |= GPIO_FIN_SEL_P12;       // ����P12ΪUART1_RX

    __EnableIRQ(UART1_IRQn); // ��UARTģ���ж�
    IE_EA = 1;               // �����ж�

    UART1_BAUD1 = (USER_UART1_BAUD >> 8) & 0xFF; // ���ò����ʸ߰�λ
    UART1_BAUD0 = USER_UART1_BAUD & 0xFF;        // ���ò����ʵͰ�λ
    UART1_CON1 = UART_DMA_IRQ(0x1);              // DMA�ж�ʹ��
    UART1_CON0 = UART_STOP_BIT(0x0) |            // 8bit���ݣ�1bitֹͣλ
                 UART_EN(0x1);

// DMA ���ý���
#if 0
    UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf_1) >> 8) & 0xFF); // DMA��ַ�߰�λ
    UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf_1) >> 0) & 0xFF); // DMA��ַ�Ͱ�λ
    UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA����
    UART1_STA = UART_DMA_DONE(0x1);                                          // ��DMA״̬��־
    UART1_DMACON = UART_RXDMA_KEY(0x1) | UART_RXDMA_EN(0x1);                 // RXDMAʹ��
#endif

    UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf) >> 8) & 0xFF); // DMA��ַ�߰�λ
    UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf) >> 0) & 0xFF); // DMA��ַ�Ͱ�λ
    UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                          // DMA����
    UART1_STA = UART_DMA_DONE(0x1);                                        // ��DMA״̬��־
    UART1_DMACON = UART_RXDMA_KEY(0x1) | UART_RXDMA_EN(0x1);               // RXDMAʹ��
}

// UART1����һ���ֽ����ݵĺ���
void uart1_sendbyte(u8 senddata)
{
    UART1_CON0 |= UART_EN(0x1);               // ʹ�ܴ��ڽ��з���
    while (!(UART1_STA & UART_TX_DONE(0x01))) // �ȴ�ǰһ�η������
        ;
    UART1_DATA = senddata;
    while (!(UART1_STA & UART_TX_DONE(0x01))) // �ȴ���η������
        ;
    UART1_CON0 &= ~UART_EN(0x1); // �رմ���ֹͣ����
}

// UART1���������ֽ����ݵĺ���
void uart1_sendbytes(u8 *buf, u32 buf_len)
{
    u32 i = 0;
    for (i = 0; i < buf_len; i++)
    {
        uart1_sendbyte(buf[i]);
    }
}

// UART1�жϷ�������DMA�������Żᴥ�����жϣ�
void UART1_IRQHandler(void) interrupt UART1_IRQn
{
    // �����ж�����IP������ɾ��
    __IRQnIPnPush(UART1_IRQn);

    // ---------------- �û��������� -------------------

    // DMA����ж�,DMA���ڿ���
    if (UART1_STA & UART_DMA_DONE(0x1))
    {
#if 0
        if (switch_buf_flag == 0)
        {
            switch_buf_flag = 1;
            // ʹ��DMA����
            UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf_1) >> 8) & 0xFF); // DMA��ַ�߰�λ
            UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf_1) >> 0) & 0xFF); // DMA��ַ�Ͱ�λ
            UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA����
            UART1_STA |= UART_DMA_DONE(0x1);                                         // ��DMA״̬��־
            UART1_DMACON = UART_TXDMA_KEY(0x1) | UART_TXDMA_EN(0x1);                 // TXDMAʹ��
            while (!(UART1_STA & UART_DMA_DONE(0x1)))
                ; // �ȴ��������

            // ���ͺ���ʹ��DMA����
            UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf_2) >> 8) & 0xFF); // DMA��ַ�߰�λ
            UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf_2) >> 0) & 0xFF); // DMA��ַ�Ͱ�λ
            UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA����
            UART1_STA |= UART_DMA_DONE(0x1);                                         // ��DMA״̬��־
            UART1_DMACON = UART_RXDMA_KEY(0x1) | UART_RXDMA_EN(0x1);                 // RXDMAʹ��
        }
        else
        {
            switch_buf_flag = 0;
            // ʹ��DMA����
            UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf_2) >> 8) & 0xFF); // DMA��ַ�߰�λ
            UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf_2) >> 0) & 0xFF); // DMA��ַ�Ͱ�λ
            UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA����
            UART1_STA |= UART_DMA_DONE(0x1);                                         // ��DMA״̬��־
            UART1_DMACON = UART_TXDMA_KEY(0x1) | UART_TXDMA_EN(0x1);                 // TXDMAʹ��
            while (!(UART1_STA & UART_DMA_DONE(0x1)))
                ; // �ȴ��������

            // ���ͺ���ʹ��DMA����
            UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf_1) >> 8) & 0xFF); // DMA��ַ�߰�λ
            UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf_1) >> 0) & 0xFF); // DMA��ַ�Ͱ�λ
            UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA����
            UART1_STA |= UART_DMA_DONE(0x1);                                         // ��DMA״̬��־
            UART1_DMACON = UART_RXDMA_KEY(0x1) | UART_RXDMA_EN(0x1);                 // RXDMAʹ��
        }
#endif
        // ʹ��DMA����
        UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf) >> 8) & 0xFF); // DMA��ַ�߰�λ
        UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf) >> 0) & 0xFF); // DMA��ַ�Ͱ�λ
        UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA����
        UART1_STA |= UART_DMA_DONE(0x1);                                         // ��DMA״̬��־
        UART1_DMACON = UART_TXDMA_KEY(0x1) | UART_TXDMA_EN(0x1);                 // TXDMAʹ��
        while (!(UART1_STA & UART_DMA_DONE(0x1)))
            ; // �ȴ��������

        // ���ͺ���ʹ��DMA����
        UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf) >> 8) & 0xFF); // DMA��ַ�߰�λ
        UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf) >> 0) & 0xFF); // DMA��ַ�Ͱ�λ
        UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA����
        UART1_STA |= UART_DMA_DONE(0x1);                                         // ��DMA״̬��־
        UART1_DMACON = UART_RXDMA_KEY(0x1) | UART_RXDMA_EN(0x1);                 // RXDMAʹ��
    }

    // �˳��ж�����IP������ɾ��
    __IRQnIPnPop(UART1_IRQn);
}

// ��UART1��DMA���ջ���������ɨ��
void uart1_dma_recv_scan(void)
{
#if 0
    static u8 tmropen_flag = 0; // ��ʱ���Ƿ�򿪵ı�־λ

    // UART_DMA_DONE(n)             ((n) << 2)  // DMA״̬��־λ  0x0:����æ  0x1:����
    // ���DMA����æµ
    if (0 == (UART1_STA & UART_DMA_DONE(0x1)))
    {

        if (0 == tmropen_flag)
        {
            tmr0_enable(); // �򿪶�ʱ��0
            tmropen_flag = 1;
        }

        while (0 == (UART1_STA & UART_DMA_DONE(0x01))) // �ȴ�DMA��æµ���˳�
        {
            // ���������װ��һ�λ�������ʱ�䣬DMA��δ��æµ���˳�
            if (tmr0_cnt > RX_DMA_LEN + 5)
            {
                UART1_STA |= UART_DMA_DONE(0x1);                         // ��DMA״̬��־
                UART1_DMACON = UART_RXDMA_KEY(0x1) & UART_RXDMA_EN(0x0); // RXDMAʧ��

                if (switch_buf_flag == 0)
                {
                    switch_buf_flag = 1;

                    uart1_sendbytes(uart1_rx_buf_1, sizeof(uart1_rx_buf_1));
                    uart1_sendbytes("uart1_dma_recv_scan", sizeof("uart1_dma_recv_scan"));
                }
                else
                {
                    switch_buf_flag = 0;

                    uart1_sendbytes(uart1_rx_buf_2, sizeof(uart1_rx_buf_2));
                    uart1_sendbytes("uart1_dma_recv_scan", sizeof("uart1_dma_recv_scan"));
                }
            }
        }

        if (tmropen_flag)
        {
            tmr0_disable(); // �رն�ʱ��0
            tmr0_cnt = 0;   // �������ֵ
            tmropen_flag = 0; 
        }
    }
#endif

    if (0 == (UART1_STA & UART_DMA_DONE(0x1))) // æµ
    {
            // uart1_sendbytes("0 == (UART1_STA & UART_DMA_DONE(0x1))", sizeof("0 == (UART1_STA & UART_DMA_DONE(0x1))"));
        LED6 = 1;
    }
    else if (1 == (UART1_STA & UART_DMA_DONE(0x1))) // ����
    {
        // uart1_sendbytes("1 == (UART1_STA & UART_DMA_DONE(0x1))", sizeof("1 == (UART1_STA & UART_DMA_DONE(0x1))"));

        LED6 = 0;
    }
}
