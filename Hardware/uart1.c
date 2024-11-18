// 不能使用UART1+DMA来接收不定长的数据，这里的代码不能用
#include "uart1.h"

// 设置的波特率需要适配单片机的时钟，这里直接使用了官方的代码
#define USER_UART1_BAUD ((SYSCLK - UART1_BAUDRATE) / (UART1_BAUDRATE))

// 接收缓冲区的大小
#define UART1_RXBUF_LEN 30

#define RX_DMA_LEN 30


volatile u8 uart1_rx_buf[UART1_RXBUF_LEN] = {0};

void uart1_init(void)
{
    // 电路板上，P11为发送引脚，P12为接收引脚
    P1_MD0 &= (~GPIO_P11_MODE_SEL(0x3) | ~GPIO_P12_MODE_SEL(0x3));
    P1_MD0 |= GPIO_P11_MODE_SEL(0x1); // 输出模式
    FOUT_S11 |= GPIO_FOUT_UART1_TX;   // 配置P11为UART1_TX
    FIN_S8 |= GPIO_FIN_SEL_P12;       // 配置P12为UART1_RX

    __EnableIRQ(UART1_IRQn); // 打开UART模块中断
    IE_EA = 1;               // 打开总中断

    UART1_BAUD1 = (USER_UART1_BAUD >> 8) & 0xFF; // 配置波特率高八位
    UART1_BAUD0 = USER_UART1_BAUD & 0xFF;        // 配置波特率低八位
    UART1_CON1 = UART_DMA_IRQ(0x1);              // DMA中断使能
    UART1_CON0 = UART_STOP_BIT(0x0) |            // 8bit数据，1bit停止位
                 UART_EN(0x1);

// DMA 配置接收
#if 0
    UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf_1) >> 8) & 0xFF); // DMA地址高八位
    UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf_1) >> 0) & 0xFF); // DMA地址低八位
    UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA长度
    UART1_STA = UART_DMA_DONE(0x1);                                          // 清DMA状态标志
    UART1_DMACON = UART_RXDMA_KEY(0x1) | UART_RXDMA_EN(0x1);                 // RXDMA使能
#endif

    UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf) >> 8) & 0xFF); // DMA地址高八位
    UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf) >> 0) & 0xFF); // DMA地址低八位
    UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                          // DMA长度
    UART1_STA = UART_DMA_DONE(0x1);                                        // 清DMA状态标志
    UART1_DMACON = UART_RXDMA_KEY(0x1) | UART_RXDMA_EN(0x1);               // RXDMA使能
}

// UART1发送一个字节数据的函数
void uart1_sendbyte(u8 senddata)
{
    UART1_CON0 |= UART_EN(0x1);               // 使能串口进行发送
    while (!(UART1_STA & UART_TX_DONE(0x01))) // 等待前一次发送完成
        ;
    UART1_DATA = senddata;
    while (!(UART1_STA & UART_TX_DONE(0x01))) // 等待这次发送完成
        ;
    UART1_CON0 &= ~UART_EN(0x1); // 关闭串口停止发送
}

// UART1发送若干字节数据的函数
void uart1_sendbytes(u8 *buf, u32 buf_len)
{
    u32 i = 0;
    for (i = 0; i < buf_len; i++)
    {
        uart1_sendbyte(buf[i]);
    }
}

// UART1中断服务函数（DMA接收满才会触发该中断）
void UART1_IRQHandler(void) interrupt UART1_IRQn
{
    // 进入中断设置IP，不可删除
    __IRQnIPnPush(UART1_IRQn);

    // ---------------- 用户函数处理 -------------------

    // DMA完成中断,DMA处于空闲
    if (UART1_STA & UART_DMA_DONE(0x1))
    {
#if 0
        if (switch_buf_flag == 0)
        {
            switch_buf_flag = 1;
            // 使能DMA发送
            UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf_1) >> 8) & 0xFF); // DMA地址高八位
            UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf_1) >> 0) & 0xFF); // DMA地址低八位
            UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA长度
            UART1_STA |= UART_DMA_DONE(0x1);                                         // 清DMA状态标志
            UART1_DMACON = UART_TXDMA_KEY(0x1) | UART_TXDMA_EN(0x1);                 // TXDMA使能
            while (!(UART1_STA & UART_DMA_DONE(0x1)))
                ; // 等待发送完成

            // 发送后，再使能DMA接收
            UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf_2) >> 8) & 0xFF); // DMA地址高八位
            UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf_2) >> 0) & 0xFF); // DMA地址低八位
            UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA长度
            UART1_STA |= UART_DMA_DONE(0x1);                                         // 清DMA状态标志
            UART1_DMACON = UART_RXDMA_KEY(0x1) | UART_RXDMA_EN(0x1);                 // RXDMA使能
        }
        else
        {
            switch_buf_flag = 0;
            // 使能DMA发送
            UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf_2) >> 8) & 0xFF); // DMA地址高八位
            UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf_2) >> 0) & 0xFF); // DMA地址低八位
            UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA长度
            UART1_STA |= UART_DMA_DONE(0x1);                                         // 清DMA状态标志
            UART1_DMACON = UART_TXDMA_KEY(0x1) | UART_TXDMA_EN(0x1);                 // TXDMA使能
            while (!(UART1_STA & UART_DMA_DONE(0x1)))
                ; // 等待发送完成

            // 发送后，再使能DMA接收
            UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf_1) >> 8) & 0xFF); // DMA地址高八位
            UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf_1) >> 0) & 0xFF); // DMA地址低八位
            UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA长度
            UART1_STA |= UART_DMA_DONE(0x1);                                         // 清DMA状态标志
            UART1_DMACON = UART_RXDMA_KEY(0x1) | UART_RXDMA_EN(0x1);                 // RXDMA使能
        }
#endif
        // 使能DMA发送
        UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf) >> 8) & 0xFF); // DMA地址高八位
        UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf) >> 0) & 0xFF); // DMA地址低八位
        UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA长度
        UART1_STA |= UART_DMA_DONE(0x1);                                         // 清DMA状态标志
        UART1_DMACON = UART_TXDMA_KEY(0x1) | UART_TXDMA_EN(0x1);                 // TXDMA使能
        while (!(UART1_STA & UART_DMA_DONE(0x1)))
            ; // 等待发送完成

        // 发送后，再使能DMA接收
        UART1_DMAADRH = UART1_DMA_ADRH_VAL((((u16)uart1_rx_buf) >> 8) & 0xFF); // DMA地址高八位
        UART1_DMAADRL = UART1_DMA_ADRL_VAL((((u16)uart1_rx_buf) >> 0) & 0xFF); // DMA地址低八位
        UART1_DMALEN = UART1_DMA_LEN_VAL(RX_DMA_LEN);                            // DMA长度
        UART1_STA |= UART_DMA_DONE(0x1);                                         // 清DMA状态标志
        UART1_DMACON = UART_RXDMA_KEY(0x1) | UART_RXDMA_EN(0x1);                 // RXDMA使能
    }

    // 退出中断设置IP，不可删除
    __IRQnIPnPop(UART1_IRQn);
}

// 对UART1的DMA接收缓冲区进行扫描
void uart1_dma_recv_scan(void)
{
#if 0
    static u8 tmropen_flag = 0; // 定时器是否打开的标志位

    // UART_DMA_DONE(n)             ((n) << 2)  // DMA状态标志位  0x0:正在忙  0x1:空闲
    // 如果DMA处于忙碌
    if (0 == (UART1_STA & UART_DMA_DONE(0x1)))
    {

        if (0 == tmropen_flag)
        {
            tmr0_enable(); // 打开定时器0
            tmropen_flag = 1;
        }

        while (0 == (UART1_STA & UART_DMA_DONE(0x01))) // 等待DMA从忙碌中退出
        {
            // 如果超过了装满一次缓冲区的时间，DMA还未从忙碌中退出
            if (tmr0_cnt > RX_DMA_LEN + 5)
            {
                UART1_STA |= UART_DMA_DONE(0x1);                         // 清DMA状态标志
                UART1_DMACON = UART_RXDMA_KEY(0x1) & UART_RXDMA_EN(0x0); // RXDMA失能

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
            tmr0_disable(); // 关闭定时器0
            tmr0_cnt = 0;   // 清除计数值
            tmropen_flag = 0; 
        }
    }
#endif

    if (0 == (UART1_STA & UART_DMA_DONE(0x1))) // 忙碌
    {
            // uart1_sendbytes("0 == (UART1_STA & UART_DMA_DONE(0x1))", sizeof("0 == (UART1_STA & UART_DMA_DONE(0x1))"));
        LED6 = 1;
    }
    else if (1 == (UART1_STA & UART_DMA_DONE(0x1))) // 空闲
    {
        // uart1_sendbytes("1 == (UART1_STA & UART_DMA_DONE(0x1))", sizeof("1 == (UART1_STA & UART_DMA_DONE(0x1))"));

        LED6 = 0;
    }
}
