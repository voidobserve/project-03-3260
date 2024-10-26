#include "uart0.h"

#include <string.h> // memset()

// ���õĲ�������Ҫ���䵥Ƭ����ʱ�ӣ�����ֱ��ʹ���˹ٷ��Ĵ���
#define USER_UART0_BAUD ((SYSCLK - UART0_BAUDRATE) / (UART0_BAUDRATE))

// ������Ž��յ�����֡�Ļ�����
volatile u8 uart0_recv_buf[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)][FRAME_MAX_LEN] = {0};
// ���ڼ�¼���ջ������Ķ�Ӧ������֡�ĳ���
volatile u8 uart0_recv_len[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)] = {0};
// ��¼�������ж�Ӧ��λ���Ƿ������ݵı�־λ�����飩
// ���磬�������±�0��λ����ָ���־λ�����±�0��Ԫ��ֵΪ1��û��ָ�Ԫ�ص�ֵΪ0
volatile u8 recved_flagbuf[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)] = {0};

// �����жϷ������У�����һ֡�ı�־λ��0--׼������һ֡�ĵ�һ���ֽڣ�1--���ڽ��ո�֡������
static volatile bit recv_frame_flag = 0;
static volatile u8 frame_len = 0;    // ��¼�����жϷ������У���ǰҪ���յ�����֡���ֽ���
static volatile u8 cur_recv_len = 0; // ��¼�����жϷ������У���ǰ�ѽ��յ�����֡���ֽ���
volatile u8 recv_frame_cnt = 0;      // ���յ�������֡�ĸ���

static volatile u32 blank_index = 0; // ��¼��ǰ�������֡�Ļ������Ŀյĵط�(�������±�)��׼�����һ֡������

// ��дputchar()����
char putchar(char c)
{
    uart0_sendbyte(c);
    return c;
}

#ifdef DEVELOPMENT_BOARD
// uart0��ʼ��
// �������ɺ� UART0_BAUDRATE ������
void uart0_init(void)
{
    // �������ϣ�P11Ϊ�������ţ�P12Ϊ��������
    P1_MD0 &= (~GPIO_P11_MODE_SEL(0x3) | ~GPIO_P12_MODE_SEL(0x3));
    P1_MD0 |= GPIO_P11_MODE_SEL(0x1); // ���ģʽ
    FOUT_S11 |= GPIO_FOUT_UART0_TX;   // ����P11ΪUART0_TX
    FIN_S7 |= GPIO_FIN_SEL_P12;       // ����P12ΪUART0_RX

    __EnableIRQ(UART0_IRQn); // ��UARTģ���ж�
    IE_EA = 1;               // �����ж�

    UART0_BAUD1 = (USER_UART0_BAUD >> 8) & 0xFF; // ���ò����ʸ߰�λ
    UART0_BAUD0 = USER_UART0_BAUD & 0xFF;        // ���ò����ʵͰ�λ
    UART0_CON0 = UART_STOP_BIT(0x0) |
                 UART_RX_IRQ_EN(0x1) |
                 UART_EN(0x1); // 8bit���ݣ�1bitֹͣλ��ʹ��RX�ж�
}
#endif // DEVELOPMENT_BOARD

#ifdef CIRCUIT_BOARD
// uart0��ʼ��
// �������ɺ� UART0_BAUDRATE ������
void uart0_init(void)
{
    // ��·���ϣ�P11Ϊ�������ţ�P12Ϊ��������
    P1_MD0 &= (~GPIO_P11_MODE_SEL(0x3) | ~GPIO_P12_MODE_SEL(0x3));
    P1_MD0 |= GPIO_P11_MODE_SEL(0x1); // ���ģʽ
    FOUT_S11 |= GPIO_FOUT_UART0_TX;   // ����P11ΪUART0_TX
    FIN_S7 |= GPIO_FIN_SEL_P12;       // ����P12ΪUART0_RX

    __EnableIRQ(UART0_IRQn); // ��UARTģ���ж�
    IE_EA = 1;               // �����ж�

    UART0_BAUD1 = (USER_UART0_BAUD >> 8) & 0xFF; // ���ò����ʸ߰�λ
    UART0_BAUD0 = USER_UART0_BAUD & 0xFF;        // ���ò����ʵͰ�λ
    UART0_CON0 = UART_STOP_BIT(0x0) |
                 UART_RX_IRQ_EN(0x1) |
                 UART_EN(0x1); // 8bit���ݣ�1bitֹͣλ��ʹ��RX�ж�

    // memset(uart0_recv_buf, 0, sizeof(uart0_recv_buf));
}
#endif // DEVELOPMENT_BOARD

// UART0�жϷ������������жϣ�
void UART0_IRQHandler(void) interrupt UART0_IRQn
{
    // �����ж�����IP������ɾ��
    __IRQnIPnPush(UART0_IRQn);
    // ---------------- �û��������� -------------------
    // RX��������ж�
    if (UART0_STA & UART_RX_DONE(0x1))
    {
        if (0 == recv_frame_flag && UART0_DATA == 0xA5)
        {
            // ������µ�һ֡���ݣ��Ը�ʽͷ0xA5��ʼ�����򿪶�ʱ����Ϊ��ʱ�ж���׼��
            recv_frame_flag = 1;

            {
                // ���¸�TMR0����ʱ��
                TMR0_CONL &= ~(TMR_SOURCE_SEL(0x07)); // �����ʱ����ʱ��Դ���üĴ���
                TMR0_CONL |= TMR_SOURCE_SEL(0x06);    // ���ö�ʱ����ʱ��Դ��ʹ��ϵͳʱ�ӣ�Լ21MHz��
                __EnableIRQ(TMR0_IRQn);               // ʹ���ж�
                IE_EA = 1;                            // �����ж�
            }
        }
        else if (0 == recv_frame_flag && UART0_DATA != 0xA5)
        {
            // ������µ�һ֡���ݣ�ȴ������0xA5��ͷ��˵����һ֡������Ч

            // �˳��ж�����IP������ɾ��
            __IRQnIPnPop(UART0_IRQn);
            return;
        }
        else if (1 == recv_frame_flag && UART0_DATA == 0xA5)
        {
            // ������ڽ���һ֡���ݣ�ȴ���յ���һ�θ�ʽͷ������֮ǰ�յ������ݣ����½�����һ֡
            cur_recv_len = 0;
            frame_len = 0;
        }
        else if (recv_frame_cnt > ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)))
        {
            // ����������������治�¸��������֡

            // �˳��ж�����IP������ɾ��
            __IRQnIPnPop(UART0_IRQn);
            return;
        }

        if (1 == cur_recv_len)
        {
            // ������յ�һ֡�ĵڶ����ֽڵ����ݣ���¼��Ҫ���յ����ݳ���
            frame_len = UART0_DATA;
        }

        if (0 == recved_flagbuf[blank_index])
        {
            // �������������ʣ��λ�������һ��֡���ŷ�������
            uart0_recv_buf[blank_index][cur_recv_len++] = UART0_DATA;
            uart0_recv_len[blank_index] = cur_recv_len; // ���µ�ǰ����֡�ĳ���
            tmr0_cnt = 0;                               // ÿ���յ�һ���ֽ����ݣ���ճ�ʱ�ļ�ʱ
        }

        // ������ͷ����������֡�ĳ��ȣ�������жϾͻ�Ӱ����һ֡���ݵĽ���
        // ������жϿ�ͷ���˸�ʽͷ��ʶ����ɨ�躯������֡���ȵ���֤���������ݹ�������̵����
        if (cur_recv_len == frame_len)
        {
            // �����������һ֡����
            recv_frame_flag = 0; // ��־λ���㣬׼��������һ֡����
            cur_recv_len = 0;
            frame_len = 0;

            recved_flagbuf[blank_index] = 1; // ��Ӧ�Ľ�����ɱ�־λ��һ
            recv_frame_cnt++;                // ���յ�������һ֡������ֵ��һ

            if (recv_frame_cnt < ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)))
            {
                // ����������п�λ�����յ�����֡���� < �������ܹ��ܽ��յ�����֡������

                // ƫ�Ƶ������пյĵط���׼��������һ������֡
                while (1)
                {
                    blank_index++;

                    if (blank_index >= ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)))
                    {
                        // ����±곬���˻����������ɵ�ָ�����
                        blank_index = 0;
                    }

                    if (0 == recved_flagbuf[blank_index])
                    {
                        // ����ǿյ�һ�����������˳���׼������һ�ν�������֡
                        break;
                    }
                }
            }

            {
                // ������ʱ���ṩʱ�ӣ�����ֹͣ����
                TMR0_CONL &= ~(TMR_SOURCE_SEL(0x07)); // �����ʱ����ʱ��Դ���üĴ���
                TMR0_CONL |= TMR_SOURCE_SEL(0x05);    // ���ö�ʱ����ʱ��Դ�������κ�ʱ��
                // �����ʱ���ļ���ֵ
                TMR0_CNTL = 0;
                TMR0_CNTH = 0;
                __DisableIRQ(TMR0_IRQn); // �ر��жϣ���ʹ���жϣ�
                tmr0_cnt = 0;
            }
        }
    }

    // �˳��ж�����IP������ɾ��
    __IRQnIPnPop(UART0_IRQn);
}

// UART0����һ���ֽ����ݵĺ���
void uart0_sendbyte(u8 senddata)
{
    while (!(UART0_STA & UART_TX_DONE(0x01)))
        ;
    UART0_DATA = senddata;
    while (!(UART0_STA & UART_TX_DONE(0x01))) // �ȴ���η������
        ;
}

// ��UART0���ջ����������ݽ�����֤����ʱ��֤��������֤��У��ͼ��㣩
void uart0_scan_handle(void)
{
    u16 i = 0; // ѭ������ֵ

    u32 checksum = 0; // �����ʱ��У���

    // u16 j = 0; // �����õ�ѭ������ֵ

#if 0  // ������
    static volatile u8 tmp1 = 0;
    static volatile u8 tmp2 = 0;

    tmp2 = recv_frame_cnt;

    if (tmp1 != tmp2) // ������յ�֡�ĸ�����һ��
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
#endif // ������

#if 1 // ���ճ�ʱ����

    if (tmr0_cnt > 100) // һ֡�ڣ�����10msû���յ�����
    {
        // �����ʱ
        // uart0_sendstr("Time out!\n");

        tmr0_disable(); // �رն�ʱ��
        tmr0_cnt = 0;   // ��ն�ʱ������ֵ

        // ��ǰ������֡����
        cur_recv_len = 0;    // ��ǰ���յ���֡�ĳ�������
        frame_len = 0;       // Ҫ���յ�֡�ĳ��ȣ�����
        recv_frame_flag = 0; // ���¿�ʼ��������

#if 0  // �򴮿ڷ�������������������
        for (i = 0; i < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); i++)
        {
            uart0_sendbyte(i + '0');
            uart0_sendbyte('\n');
            uart0_sendnums(uart0_recv_buf[i], FRAME_MAX_LEN);
            uart0_sendstr("\n==========================\n");
        }
#endif // �򴮿ڷ�������������������

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
#endif // ���ճ�ʱ����

    // ���յ�������֡������֤
    for (i = 0; i < ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)); i++)
    {
        if (recved_flagbuf[i])
        {
            // �����Ӧ��λ��������������֡

            if (uart0_recv_len[i] != uart0_recv_buf[i][1])
            {
                // �������֡�ĳ��Ȳ���ȷ
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

                // ������ĸ��ֽڵ�����
                checksum = (uart0_recv_buf[i][0] + uart0_recv_buf[i][1] + uart0_recv_buf[i][2]) & 0x0F;

                if (checksum != uart0_recv_buf[i][3])
                {
                    // �������ó���У���������֡�е�У��Ͳ�һ��
                    // ˵���������������
                    // uart0_sendstr("checknum err_4Bytes\n");
                    recved_flagbuf[i] = 0;
                    recv_frame_cnt--;
                    // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);
                }

                break;

            case 5:

                // ���������ֽڵ�����
                checksum = (uart0_recv_buf[i][0] + uart0_recv_buf[i][1] + uart0_recv_buf[i][2] + uart0_recv_buf[i][3]) & 0x0F;

                if (checksum != uart0_recv_buf[i][4])
                {
                    // �������ó���У���������֡�е�У��Ͳ�һ��
                    // ˵���������������
                    // uart0_sendstr("checknum err_5Bytes\n");
                    recved_flagbuf[i] = 0;
                    recv_frame_cnt--;
                    // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);
                }

                break;
            case 6:

                // ����������ֽڵ�����
                checksum = (uart0_recv_buf[i][0] + uart0_recv_buf[i][1] + uart0_recv_buf[i][2] + uart0_recv_buf[i][3] + uart0_recv_buf[i][4]) & 0x0F;

                if (checksum != uart0_recv_buf[i][5])
                {
                    // �������ó���У���������֡�е�У��Ͳ�һ��
                    // ˵���������������
                    // uart0_sendstr("checknum err_6Bytes\n");
                    recved_flagbuf[i] = 0;
                    recv_frame_cnt--;
                    // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);
                }

                break;

            default:
                // ��������ġ�����������ֽڵ����ݣ�˵����������ֱ��������һ֡����
                // uart0_sendstr("recv cnt err\n");
                recved_flagbuf[i] = 0;
                recv_frame_cnt--;
                // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);

                break;
            }

            // ������е�������ݶ�����
        }
    }
}
