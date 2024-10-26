#include "send_data.h"

void send_data(u32 instruct, u32 send_data)
{
    u32 check_num = 0; // ���У���

    uart0_sendbyte(FORMAT_HEAD); // �ȷ��͸�ʽͷ

    switch (instruct)
    {
    case SEND_GEAR:                // ���͵�λ��״̬
        uart0_sendbyte(0x05);      // ����ָ����ܳ���
        uart0_sendbyte(SEND_GEAR); // ����ָ��
        uart0_sendbyte(send_data);

        check_num = FORMAT_HEAD + 0x05 + SEND_GEAR + (u8)send_data;
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ

        break;

    case SEND_BATTERY:                // ���͵�ص�����״̬
        uart0_sendbyte(0x05);         // ����ָ����ܳ���
        uart0_sendbyte(SEND_BATTERY); // ����ָ��
        uart0_sendbyte(send_data);    // ���͵�ص�����Ϣ

        check_num = FORMAT_HEAD + 0x05 + SEND_BATTERY + (u8)send_data;
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ
        break;

    case SEND_BARKE:                // ����ɲ����״̬
        uart0_sendbyte(0x05);       // ����ָ����ܳ���
        uart0_sendbyte(SEND_BARKE); // ����ָ��
        uart0_sendbyte(send_data);  // ������Ϣ

        check_num = FORMAT_HEAD + 0x05 + SEND_BARKE + (u8)send_data;
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ
        break;

    case SEND_LEFT_TURN:                // ������ת��Ƶ�״̬
        uart0_sendbyte(0x05);           // ����ָ����ܳ���
        uart0_sendbyte(SEND_LEFT_TURN); // ����ָ��
        uart0_sendbyte(send_data);      // ������Ϣ

        check_num = FORMAT_HEAD + 0x05 + SEND_LEFT_TURN + (u8)send_data;
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ
        break;

    case SEND_RIGHT_TURN:                // ������ת��Ƶ�״̬
        uart0_sendbyte(0x05);            // ����ָ����ܳ���
        uart0_sendbyte(SEND_RIGHT_TURN); // ����ָ��
        uart0_sendbyte(send_data);       // ������Ϣ

        check_num = FORMAT_HEAD + 0x05 + SEND_RIGHT_TURN + (u8)send_data;
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ
        break;

    case SEND_HIGH_BEAM:                // ����Զ��Ƶ�״̬
        uart0_sendbyte(0x05);           // ����ָ����ܳ���
        uart0_sendbyte(SEND_HIGH_BEAM); // ����ָ��
        uart0_sendbyte(send_data);      // ������Ϣ

        check_num = FORMAT_HEAD + 0x05 + SEND_HIGH_BEAM + (u8)send_data;
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ
        break;

    case SEND_ENGINE_SPEED:                // ���ͷ�������ת��
        uart0_sendbyte(0x06);              // ����ָ����ܳ���
        uart0_sendbyte(SEND_ENGINE_SPEED); // ����ָ��
        uart0_sendbyte(send_data >> 8);    // ������Ϣ
        uart0_sendbyte(send_data);         // ������Ϣ

        check_num = FORMAT_HEAD + 0x06 + SEND_ENGINE_SPEED + (u8)(send_data >> 8) + (u8)(send_data);
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ
        break;

    case SEND_SPEED:                    // ����ʱ��
        uart0_sendbyte(0x06);           // ����ָ����ܳ���
        uart0_sendbyte(SEND_SPEED);     // ����ָ��
        uart0_sendbyte(send_data >> 8); // ������Ϣ
        uart0_sendbyte(send_data);      // ������Ϣ

        check_num = FORMAT_HEAD + 0x06 + SEND_SPEED + (u8)(send_data >> 8) + (u8)(send_data);
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ
        break;

    case SEND_FUEL:                // ��������
        uart0_sendbyte(0x05);      // ����ָ����ܳ���
        uart0_sendbyte(SEND_FUEL); // ����ָ��
        uart0_sendbyte(send_data); // ������Ϣ

        check_num = FORMAT_HEAD + 0x05 + SEND_FUEL + (u8)send_data;
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ
        break;

    case SEND_WATER_TEMP:                // ����ˮ��
        uart0_sendbyte(0x05);            // ����ָ����ܳ���
        uart0_sendbyte(SEND_WATER_TEMP); // ����ָ��
        uart0_sendbyte(send_data);       // ������Ϣ

        check_num = FORMAT_HEAD + 0x05 + SEND_WATER_TEMP + (u8)send_data;
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ
        break;

    case SEND_TOTAL_MILEAGE:                // ���ʹ�����
        uart0_sendbyte(0x07);               // ����ָ����ܳ���
        uart0_sendbyte(SEND_TOTAL_MILEAGE); // ����ָ��

        uart0_sendbyte(send_data >> 16); // ������Ϣ
        uart0_sendbyte(send_data >> 8);  // ������Ϣ
        uart0_sendbyte(send_data);       // ������Ϣ

        check_num = FORMAT_HEAD + 0x07 + SEND_TOTAL_MILEAGE + (u8)(send_data >> 16) + (u8)(send_data >> 8) + (u8)send_data;
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ
        break;

    case SEND_SUBTOTAL_MILEAGE: // ����С�����

        uart0_sendbyte(0x06);               // ����ָ����ܳ���
        uart0_sendbyte(SEND_SUBTOTAL_MILEAGE); // ����ָ��

        uart0_sendbyte(send_data >> 8);  // ������Ϣ
        uart0_sendbyte(send_data);       // ������Ϣ

        check_num = FORMAT_HEAD + 0x06 + SEND_SUBTOTAL_MILEAGE + (u8)(send_data >> 8) + (u8)send_data;
        check_num &= 0x0F; // ȡǰ���������ӵĵ���λ

        break;
    }

    uart0_sendbyte(check_num); // ����У���

    delay_ms(10); // ÿ�η�����ɺ���ʱ10ms
}

