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

volatile u8 flagbuf_valid_instruction[(UART0_RXBUF_LEN) / (FRAME_MAX_LEN)] = {0}; // ����кϷ�ָ��ı�־λ����

// �����жϷ������У�����һ֡�ı�־λ��0--׼������һ֡�ĵ�һ���ֽڣ�1--���ڽ��ո�֡������
// static volatile bit recv_frame_flag = 0;
// �����жϷ������У�����һ֡�ı�־λ��0--׼������һ֡�ĵ�һ���ֽڣ�1--���ڽ��ո�֡������
static bit flag_is_recving_data = 0;
static volatile u8 frame_len = 0;    // ��¼�����жϷ������У���ǰҪ���յ�����֡���ֽ���
static volatile u8 cur_recv_len = 0; // ��¼�����жϷ������У���ǰ�ѽ��յ�����֡���ֽ���
volatile u8 recv_frame_cnt = 0;      // ���յ�������֡�ĸ���

static volatile u32 blank_index = 0; // ��¼��ǰ�������֡�Ļ������Ŀյĵط�(�������±�)��׼�����һ֡������

bit test_bit = 0;

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
    volatile u8 uart0_tmp_val = 0;

    // �����ж�����IP������ɾ��
    __IRQnIPnPush(UART0_IRQn);
    // ---------------- �û��������� -------------------
    // RX��������ж�
    if (UART0_STA & UART_RX_DONE(0x1))
    {
        // test_bit = 1;
        uart0_tmp_val = UART0_DATA;

        // if ((0 == flag_is_recving_data && UART0_DATA == 0xA5) ||
        //     (1 == flag_is_recving_data && UART0_DATA == 0xA5))
        if (uart0_tmp_val == 0xA5)
        {
            // 1. ������µ�һ֡���ݣ��Ը�ʽͷ0xA5��ʼ�����򿪶�ʱ����Ϊ��ʱ�ж���׼��
            // 2. ������ڽ���һ֡���ݣ�ȴ���յ���һ�θ�ʽͷ������֮ǰ�յ������ݣ����½�����һ֡
            flag_is_recving_data = 1;
            cur_recv_len = 0;
            frame_len = 0;
            tmr0_disable();
            tmr0_cnt = 0;
            tmr0_enable();
        }
        else if ((0 == flag_is_recving_data && uart0_tmp_val != 0xA5) ||
                 (recv_frame_cnt >= ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN))))
        {
            // 1. ������µ�һ֡���ݣ�ȴ������0xA5��ͷ��˵����һ֡������Ч
            // 2. ����������������治�¸��������֡
            // ֱ���˳��жϣ�����������
            tmr0_disable();
            tmr0_cnt = 0;
            __IRQnIPnPop(UART0_IRQn);
            return;
        }

        if (1 == cur_recv_len)
        {
            // ������յ�һ֡�ĵڶ����ֽڵ����ݣ���¼��Ҫ���յ����ݳ���
            frame_len = uart0_tmp_val;
        }

        // �������е����˵�����ڽ���һ������֡
        if (0 == recved_flagbuf[blank_index])
        {
            // �������������ʣ��λ�������һ��֡���ŷ�������
            uart0_recv_buf[blank_index][cur_recv_len++] = uart0_tmp_val;
            // uart0_recv_buf[0][cur_recv_len++] = UART0_DATA; // ������
            tmr0_cnt = 0; // ÿ���յ�һ���ֽ����ݣ���ճ�ʱ�ļ�ʱ

#if 1
            if ((cur_recv_len) == frame_len)
            {
                // �����������һ֡����
                uart0_recv_len[blank_index] = frame_len; // ���µ�ǰ����֡�ĳ���
                flag_is_recving_data = 0;                // ��־λ���㣬׼��������һ֡����
                cur_recv_len = 0;
                frame_len = 0;

                recved_flagbuf[blank_index] = 1; // ��Ӧ�Ľ�����ɱ�־λ��һ
                recv_frame_cnt++;                // ���յ�������һ֡������ֵ��һ
                test_bit = 1;
                blank_index++;
                if (blank_index >= ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)))
                {
                    blank_index = 0;
                }

                // if (recv_frame_cnt < ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)))
                // {
                //     // ����������п�λ�����յ�����֡���� < �������ܹ��ܽ��յ�����֡������

                //     // ƫ�Ƶ������пյĵط���׼��������һ������֡
                //     while (1)
                //     {
                //         blank_index++;

                //         if (blank_index >= ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)))
                //         {
                //             // ����±곬���˻����������ɵ�ָ�����
                //             blank_index = 0;
                //         }

                //         if (0 == recved_flagbuf[blank_index])
                //         {
                //             // ����ǿյ�һ�����������˳���׼������һ�ν�������֡
                //             break;
                //         }
                //     }
                // }

                {
                    tmr0_disable();
                    tmr0_cnt = 0;
                    __IRQnIPnPop(UART0_IRQn);
                    return;
                }
            } // if ((cur_recv_len) == frame_len)
#endif
        } // if (0 == recved_flagbuf[blank_index])
        // else // �����ǰ��������û��ʣ��λ�������һ��֡��
        // {
        // }

    } // if (UART0_STA & UART_RX_DONE(0x1))

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
    u8 i = 0;                         // ѭ������ֵ��ע��ѭ������Ҫ���ڵ��������ܴ�ŵ�ָ����Ŀ��
    volatile u8 checksum = 0;         // �����ʱ��У���
    volatile bit __flag_is_crc_or_len_err = 0; // ��־λ��У��� / ���ݳ��� �Ƿ����,0--δ����1--����

#if 1 // ���ճ�ʱ����

    if (tmr0_cnt > 10) // һ֡�ڣ�����10msû���յ�����
    {
        // �����ʱ
        // uart0_sendstr("Time out!\n");

        tmr0_disable(); // �رն�ʱ��
        tmr0_cnt = 0;   // ��ն�ʱ������ֵ

        // ��ǰ������֡����
        cur_recv_len = 0;         // ��ǰ���յ���֡�ĳ�������
        frame_len = 0;            // Ҫ���յ�֡�ĳ��ȣ�����
        flag_is_recving_data = 0; // ���¿�ʼ��������

        // if (0 == recved_flagbuf[blank_index])
        // {
        //     memset(uart0_recv_buf[blank_index], 0, FRAME_MAX_LEN); // ��ճ�ʱ��ָ���Ӧ�Ľ��ջ�����
        // }

#if USE_MY_DEBUG
        printf("recv time out! \n");
#endif

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
#if USE_MY_DEBUG
                printf("format len invalid!\n");
#endif
                // flag_is_recving_data = 0;
                recved_flagbuf[i] = 0;
                recv_frame_cnt--;
                memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);

                // for (i = 0; i < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); i++)
                // {
                //     uart0_sendbyte(i + '0');
                //     uart0_sendbyte('\n');
                //     uart0_sendnums(uart0_recv_buf[i], FRAME_MAX_LEN);
                //     uart0_sendstr("\n==========================\n");
                // }

                return;
            }

            {                          // ����У���
                u8 __loop_crc_cnt = 0; // ���ڼ���У��͵�ѭ������ֵ
                checksum = 0;
                for (__loop_crc_cnt = 0; __loop_crc_cnt < (uart0_recv_len[i] - 1); __loop_crc_cnt++)
                {
                    checksum += uart0_recv_buf[i][__loop_crc_cnt];
                }

                checksum &= 0x0F;
                if (checksum != uart0_recv_buf[i][uart0_recv_len[i] - 1])
                {
                    // ��������У������յ���У��Ͳ�һ��
                    __flag_is_crc_or_len_err = 1;
                }
            } // ����У���

            if (__flag_is_crc_or_len_err)
            {
#if USE_MY_DEBUG
                printf("recv crc or len err! \n");
#endif
                // flag_is_recving_data = 0;
                __flag_is_crc_or_len_err = 0; //
                recved_flagbuf[i] = 0;
                recv_frame_cnt--;
                memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN); // ���У��ʹ����ָ���Ӧ�Ļ�����
            }

            // ������е�������ݶ�����
            flagbuf_valid_instruction[i] = 1; // ��Ӧ�ı�־λ��һ����ʾ�յ��˺Ϸ���ָ��
        } // if (recved_flagbuf[i])
    } // for (i = 0; i < ((UART0_RXBUF_LEN) / (FRAME_MAX_LEN)); i++)
}


#if 0
// �����õĳ���
void __uart_buff_check(void)
{
    u8 i = 0;
    u8 j = 0;
    u8 k = 0;
    if (test_bit)
    {
        test_bit = 0;
        for (i = 0; i < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); i++)
        {
            for (j = 0; j < (UART0_RXBUF_LEN) / (FRAME_MAX_LEN); j++)
            {
                uart0_sendbyte(j + '0');
                uart0_sendbyte('\n');
                for (k = 0; k < FRAME_MAX_LEN; k++)
                {
                    printf("%2x ", (u16)uart0_recv_buf[j][k]);
                }

                printf("\n==========================\n");
            }
        }
    }
}
#endif

#if 0
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
                    // recved_flagbuf[i] = 0;
                    // recv_frame_cnt--;
                    // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);
                    __flag_is_crc_or_len_err = 1;
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
                    // recved_flagbuf[i] = 0;
                    // recv_frame_cnt--;
                    // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);
                    __flag_is_crc_or_len_err = 1;
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
                    // recved_flagbuf[i] = 0;
                    // recv_frame_cnt--;
                    // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);
                    __flag_is_crc_or_len_err = 1;
                }

                break;
            case 7: // �����7���ֽڵ�����
                checksum = (uart0_recv_buf[i][0] + uart0_recv_buf[i][1] + uart0_recv_buf[i][2] + uart0_recv_buf[i][3] + uart0_recv_buf[i][4] + uart0_recv_buf[i][5]) & 0x0F;
                if (checksum != uart0_recv_buf[i][6])
                {
                    // �������ó���У���������֡�е�У��Ͳ�һ��
                    // ˵���������������
                    // recved_flagbuf[i] = 0;
                    // recv_frame_cnt--;

                    __flag_is_crc_or_len_err = 1;
                }
                break;
            case 8: // �����8���ֽڵ�����
                checksum = (uart0_recv_buf[i][0] + uart0_recv_buf[i][1] + uart0_recv_buf[i][2] + uart0_recv_buf[i][3] + uart0_recv_buf[i][4] + uart0_recv_buf[i][5] + uart0_recv_buf[i][6]) & 0x0F;
                if (checksum != uart0_recv_buf[i][7])
                {
                    // �������ó���У���������֡�е�У��Ͳ�һ��
                    // ˵���������������
                    // recved_flagbuf[i] = 0;
                    // recv_frame_cnt--;
                    __flag_is_crc_or_len_err = 1;
                }
                break;

            default:
                // ��������ġ��塢�����ߡ��˸��ֽڵ����ݣ�˵����������ֱ��������һ֡����
#if USE_MY_DEBUG
                // printf("recv cnt err\n");
                __flag_is_crc_or_len_err = 1;
#endif
                // recved_flagbuf[i] = 0;
                // recv_frame_cnt--;
                // memset(uart0_recv_buf[i], 0, FRAME_MAX_LEN);

                break;
            } // switch (uart0_recv_len[i])
#endif