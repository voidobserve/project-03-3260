#include "fun_info.h" 

volatile fun_info_t fun_info = {0}; // ������й���״̬�Ľṹ�����

// ��ʼ�����������Ϣ�Ľṹ�����
void fun_info_init(void)
{
    flash_read(0x00, (u8*)&fun_info, sizeof(fun_info));
}

// ���� �����������Ϣ�Ľṹ�������д��flash��
void fun_info_save(void)
{
    flash_erase_sector(0x00);
    flash_write(0x00, (u8*)&fun_info, sizeof(fun_info));
}
