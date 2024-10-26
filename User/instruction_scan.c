// ɨ���Ƿ���ָ�������ָ��Ͷ�Ӧ�����ݣ�
// �����ǵ�ĳһ�����ܵ�״̬�и��£����Ͷ�Ӧ������
#include "instruction_scan.h"

// ��bit���壬����ʡ�ռ�
volatile bit flag_get_all_status = 0;   // ��ȡ���й��ܵ�״̬
volatile bit get_gear = 0;        // ��ȡ��λ״̬ / �õ��˵�λ��״̬
volatile bit get_battery = 0;     // ��ȡ���״̬ / �õ��˵�ص�״̬����ص�������λ���ٷֱȣ�
volatile bit get_brake = 0;       // ��ȡɲ��״̬ / �õ���ɲ����״̬
volatile bit get_left_turn = 0;    // ��ȡ��ת��Ƶ�״̬ / �õ�����ת��Ƶ�״̬
volatile bit get_right_turn = 0;   // ��ȡ��ת��Ƶ�״̬ / �õ�����ת��Ƶ�״̬
volatile bit get_high_beam = 0;    // ��ȡԶ��Ƶ�״̬ / �õ���Զ��Ƶ�״̬
volatile bit get_engine_speed = 0; // ��ȡ��������ת�� / �õ��˷�������ת��
volatile bit flag_get_speed = 0;       // ��ȡʱ�� / �õ���ʱ��
volatile bit getFuel = 0;        // ��ȡ���� / �õ�����������λ���ٷֱȣ�
volatile bit getWaterTemp = 0;   // ��ȡˮ�� / �õ���ˮ��

volatile bit getTotalMileage = 0;    // ��ȡ������ / �õ��˴�����
volatile bit getSubTotalMileage = 0; // ��ȡС����� / �õ���С�����

void instruction_handle(void)
{
    if (flag_get_all_status)
    {
        // ���Ҫ��ȡ���й��ܵ�״̬
        flag_get_all_status = 0; // �����־λ

#if MY_DEBUG
        printf("get_all_status\n");
#endif
    }

    if (get_gear)
    {
        // ���Ҫ��ȡ��λ��״̬
        get_gear = 0; //

#if MY_DEBUG
        printf("get_gear\n");
#endif

        send_data(SEND_GEAR, fun_info.gear); // ���͵�ǰ��λ��״̬
    }

    if (get_battery)
    {
        // ���Ҫ��ȡ��ص�����״̬
        get_battery = 0;

        send_data(SEND_BATTERY, fun_info.battery); // ���͵�ص���

#if MY_DEBUG
        printf("get_battery\n");
#endif
    }

    if (get_brake)
    {
        // ���Ҫ��ȡ��ȡɲ����״̬
        get_brake = 0;

#if MY_DEBUG
        printf("get_brake\n");
#endif

        send_data(SEND_BARKE, fun_info.brake); // ���͵�ǰɲ����״̬
    }

    if (get_left_turn)
    {
        // ���Ҫ��ȡ��ת��Ƶ�״̬
        get_left_turn = 0;

#if MY_DEBUG
        printf("get_left_turn\n");
#endif

        send_data(SEND_LEFT_TURN, fun_info.left_turn); // ���͵�ǰ��ת��Ƶ�״̬
    }

    if (get_right_turn)
    {
        // ���Ҫ��ȡ��ת��Ƶ�״̬
        get_right_turn = 0;

        // uart0_sendstr("get_right_turn\n");

        send_data(SEND_RIGHT_TURN, fun_info.right_turn); // ���͵�ǰ��ת��Ƶ�״̬
    }

    if (get_high_beam)
    {
        // ���Ҫ��ȡԶ��Ƶ�״̬
        get_high_beam = 0;

        // uart0_sendstr("get_high_beam\n");

        send_data(SEND_HIGH_BEAM, fun_info.high_beam); // ���͵�ǰԶ��Ƶ�״̬
    }

    if (get_engine_speed)
    {
        // ���Ҫ��ȡ��������ת��
        get_engine_speed = 0;

        // uart0_sendstr("get_engine_speed\n");

        send_data(SEND_ENGINE_SPEED, fun_info.engine_speeed);
    }

    if (flag_get_speed)
    {
        // ���Ҫ��ȡʱ��
        flag_get_speed = 0;

        // uart0_sendstr("get_speed\n");

#ifdef INTERNATIONAL // ʹ�ù��Ƶ�λ

        send_data(SEND_SPEED, fun_info.speed); // ���͵�ǰ�ɼ����ĳ��٣�ʱ�٣�

#endif // INTERNATIONAL ʹ�ù��Ƶ�λ

#ifdef IMPERIAL // ʹ��Ӣ�Ƶ�λ

        send_data(SEND_SPEED, fun_info.speed * 621 / 1000);

#endif
    }

    if (getFuel)
    {
        // ���Ҫ��ȡ����
        getFuel = 0;

        // uart0_sendstr("getFuel\n");
    }

    if (getWaterTemp)
    {
        // ���Ҫ��ȡˮ��
        getWaterTemp = 0;

        // uart0_sendstr("getWaterTemp\n");
    }

    if (getTotalMileage)
    {
        // ���Ҫ��ȡ������ / �õ��˴������µ�����
        getTotalMileage = 0;

#ifdef INTERNATIONAL // ���Ƶ�λ

        // ֻ���Ͱ��׼����ϵ�����
        send_data(SEND_TOTAL_MILEAGE, mileage_info.total_mileage / 100);

#endif // INTERNATIONAL ���Ƶ�λ

#ifdef IMPERIAL // Ӣ�Ƶ�λ
#if MY_DEBUG
        printf("total mileage: %lu * 0.1 mile", mileage_info.total_mileage * 62137 / 10000000);
#endif // MY_DEBUG
       // ֻ����0.1Ӣ�Ｐ���ϵ�����
        send_data(SEND_TOTAL_MILEAGE, mileage_info.total_mileage * 62137 / 10000000);

#endif // IMPERIAL Ӣ�Ƶ�λ
    }

    if (getSubTotalMileage)
    {
        // ���Ҫ��ȡС����� / �õ���С������µ�����
        getSubTotalMileage = 0;

#ifdef INTERNATIONAL // ���Ƶ�λ

        // ֻ����ǧ�׼����ϵ�����
        send_data(SEND_SUBTOTAL_MILEAGE, mileage_info.subtotal_mileage / 1000);

#endif // INTERNATIONAL ���Ƶ�λ

#ifdef IMPERIAL // Ӣ�Ƶ�λ
#if MY_DEBUG
        printf("sub total mileage: %lu mile", mileage_info.subtotal_mileage * 62137 / 100000000);
#endif // MY_DEBUG
       // ֻ����Ӣ�Ｐ���ϵ�����
        send_data(SEND_SUBTOTAL_MILEAGE, mileage_info.subtotal_mileage * 62137 / 100000000);

#endif // IMPERIAL Ӣ�Ƶ�λ
    }
}
