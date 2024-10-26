// ��̱��Դ����
#include "mileage.h"

mileage_t mileage_info = {0}; // �����̱�Ľṹ�����

// ��̱����ã���ʼ����
void mileage_config(void)
{
    // �ȴ�flash�ж������
    flash_read(0x00, (u8*)&mileage_info, sizeof(mileage_info));
}

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

        flash_write(0x00, (u8 *)&mileage_info, sizeof(mileage_info));
    }

    if (distance >= 1000)
    {
        // ����߹��ľ��볬����1m���ٽ��б���
        mileage_info.total_mileage += distance / 1000;    // ������ת�����ף��ٱ���
        mileage_info.subtotal_mileage += distance / 1000; // ������ת�����ף��ٱ���
        distance %= 1000;                                 // ʣ�µġ�δ����ġ�����1m������������һ�εı���
    }

    if ((mileage_info.total_mileage - old_total_mileage) > 100)
    {
        // ����������б仯�ҳ�����100m
        old_total_mileage = mileage_info.total_mileage; // ��¼�ɵ����

        // printf("total mileage: %lum\n", mileage_info.total_mileage);

        {
            // �������ݵĲ������������ñ�־λ
            // Ҫע�ⷢ�͵�����λ��������һλ��С������λΪKM������
            getTotalMileage = 1;
        }
    }

    if ((mileage_info.subtotal_mileage - old_subtotal_mileage) > 1000)
    {
        // ���С������б仯�ҳ�����1000m
        old_subtotal_mileage = mileage_info.subtotal_mileage; // ��¼�ɵ����

        // printf("subtotal mileage: %lum\n", mileage_info.subtotal_mileage);

        {
            // �������ݵĲ������������ñ�־λ
            // Ҫע�ⷢ�͵�����λ�ġ���λΪKM������
            getSubTotalMileage = 1;
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

        flash_write(0x00, (u8 *)&mileage_info, sizeof(mileage_info));
    }

    if (distance >= 1000)
    {
        // ����߹��ľ��볬����1m���ٽ��б���
        mileage_info.total_mileage += distance / 1000;    // ������ת�����ף��ٱ���
        mileage_info.subtotal_mileage += distance / 1000; // ������ת�����ף��ٱ���
        distance %= 1000;                                 // ʣ�µġ�δ����ġ�����1m������������һ�εı���
    }

    if ((mileage_info.total_mileage - old_total_mileage) > 161)
    {
        // ����������б仯�ҳ�����161m��Լ0.1Ӣ��
        old_total_mileage = mileage_info.total_mileage; // ��¼�ɵ����

        // printf("total mileage: %lum\n", mileage_info.total_mileage);

        {
            // �������ݵĲ������������ñ�־λ
            // Ҫע�ⷢ�͵�����λ��������һλ��С������λΪӢ�������

            getTotalMileage = 1;
        }
    }

    if ((mileage_info.subtotal_mileage - old_subtotal_mileage) > 1610)
    {
        // ���С������б仯�ҳ�����1610m��Լ1Ӣ��
        old_subtotal_mileage = mileage_info.subtotal_mileage; // ��¼�ɵ����

        // printf("subtotal mileage: %lum\n", mileage_info.subtotal_mileage);

        {
            // �������ݵĲ������������ñ�־λ
            // Ҫע�ⷢ�͵�����λ�ġ���λΪӢ�������
            getSubTotalMileage = 1;
        }
    }

#endif // IMPERIAL Ӣ�Ƶ�λ
}
