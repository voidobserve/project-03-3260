#include "fun_info.h" 

volatile fun_info_t fun_info = {0}; // 存放所有功能状态的结构体变量

// 初始化存放所有信息的结构体变量
void fun_info_init(void)
{
    flash_read(0x00, (u8*)&fun_info, sizeof(fun_info));
}

// 保存 存放了所有信息的结构体变量，写入flash中
void fun_info_save(void)
{
    flash_erase_sector(0x00);
    flash_write(0x00, (u8*)&fun_info, sizeof(fun_info));
}
