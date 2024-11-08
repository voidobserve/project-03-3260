// ��̱��Դ����
#include "mileage.h"


// �����ɨ��
void mileage_scan(void)
{
#ifdef INTERNATIONAL // ���Ƶ�λ

    static u32 old_total_mileage = 0;    // ������¼�ɵĴ����̵ı���
    static u32 old_subtotal_mileage = 0; // ������¼�ɵ�С����̵ı���

    static bit tmr_is_open = 0; // ��ʱ���Ƿ�򿪵ı�־λ

    if (0 == tmr_is_open)
    {
        tmr_is_open = 1;
        tmr3_cnt = 0;
        tmr3_enable();
    }

    if (tmr3_cnt >= 3000)
    {
        // ÿ30s�������̺�С�����д��flash
        tmr3_disable();
        tmr3_cnt = 0;
        tmr_is_open = 0;

        flash_write(0x00, (u8 *)&fun_info, sizeof(fun_info));
    }

    if (distance >= 1000)
    {
        // ����߹��ľ��볬����1m���ٽ��б���
        fun_info.total_mileage += distance / 1000;    // ������ת�����ף��ٱ���
        fun_info.subtotal_mileage += distance / 1000; // ������ת�����ף��ٱ���
        distance %= 1000;                                 // ʣ�µġ�δ����ġ�����1m������������һ�εı���
    }

    if ((fun_info.total_mileage - old_total_mileage) > 100)
    {
        // ����������б仯�ҳ�����100m
        old_total_mileage = fun_info.total_mileage; // ��¼�ɵ����

        // printf("total mileage: %lum\n", fun_info.total_mileage);

        {
            // �������ݵĲ������������ñ�־λ
            // Ҫע�ⷢ�͵�����λ��������һλ��С������λΪKM������
            flag_get_total_mileage = 1;
        }
    }

    if ((fun_info.subtotal_mileage - old_subtotal_mileage) > 1000)
    {
        // ���С������б仯�ҳ�����1000m
        old_subtotal_mileage = fun_info.subtotal_mileage; // ��¼�ɵ����

        // printf("subtotal mileage: %lum\n", fun_info.subtotal_mileage);

        {
            // �������ݵĲ������������ñ�־λ
            // Ҫע�ⷢ�͵�����λ�ġ���λΪKM������
            flag_get_sub_total_mileage = 1;
        }
    }

#endif // INTERNATIONAL ���Ƶ�λ

#ifdef IMPERIAL // Ӣ�Ƶ�λ

    static u32 old_total_mileage = 0;    // ������¼�ɵĴ����̵ı���
    static u32 old_subtotal_mileage = 0; // ������¼�ɵ�С����̵ı���

    static bit tmr_is_open = 0; // ��ʱ���Ƿ�򿪵ı�־λ

    if (0 == tmr_is_open)
    {
        tmr_is_open = 1;
        tmr3_cnt = 0;
        tmr3_enable();
    }

    if (tmr3_cnt >= 3000)
    {
        // ÿ30s�������̺�С�����д��flash
        tmr3_disable();
        tmr3_cnt = 0;
        tmr_is_open = 0;

        flash_write(0x00, (u8 *)&fun_info, sizeof(fun_info));
    }

    if (distance >= 1000)
    {
        // ����߹��ľ��볬����1m���ٽ��б���
        fun_info.total_mileage += distance / 1000;    // ������ת�����ף��ٱ���
        fun_info.subtotal_mileage += distance / 1000; // ������ת�����ף��ٱ���
        distance %= 1000;                                 // ʣ�µġ�δ����ġ�����1m������������һ�εı���
    }

    if ((fun_info.total_mileage - old_total_mileage) > 161)
    {
        // ����������б仯�ҳ�����161m��Լ0.1Ӣ��
        old_total_mileage = fun_info.total_mileage; // ��¼�ɵ����

        // printf("total mileage: %lum\n", fun_info.total_mileage);

        {
            // �������ݵĲ������������ñ�־λ
            // Ҫע�ⷢ�͵�����λ��������һλ��С������λΪӢ�������

            flag_get_total_mileage = 1;
        }
    }

    if ((fun_info.subtotal_mileage - old_subtotal_mileage) > 1610)
    {
        // ���С������б仯�ҳ�����1610m��Լ1Ӣ��
        old_subtotal_mileage = fun_info.subtotal_mileage; // ��¼�ɵ����

        // printf("subtotal mileage: %lum\n", fun_info.subtotal_mileage);

        {
            // �������ݵĲ������������ñ�־λ
            // Ҫע�ⷢ�͵�����λ�ġ���λΪӢ�������
            flag_get_sub_total_mileage = 1;
        }
    }

#endif // IMPERIAL Ӣ�Ƶ�λ
}
