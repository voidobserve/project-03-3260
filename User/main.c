/**
 ******************************************************************************
 * @file    main.c
 * @author  HUGE-IC Application Team
 * @version V1.0.0
 * @date    05-11-2022
 * @brief   Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT 2021 HUGE-IC</center></h2>
 *
 * 版权说明后续补上
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "include.h"

/** @addtogroup Template_Project
 * @{
 */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Main program.
 * @param  None
 * @retval None
 */
extern void __uart_buff_check(void);
extern bit test_bit;
void main(void)
{
    // 看门狗默认打开, 复位时间2s
    system_init();

    WDT_KEY = WDT_KEY_VAL(0xDD); //  关闭看门狗

    // 关闭HCK和HDA的调试功能
    WDT_KEY = 0x55;  // 解除写保护
    IO_MAP &= ~0x01; // 清除这个寄存器的值，实现关闭HCK和HDA引脚的调试功能（解除映射）
    WDT_KEY = 0xBB;

    /* 用户代码初始化接口 */
    user_init();

    /* 按键初始化 */
    // tk_param_init();

    /* 系统主循环 */
    while (1)
    {
        /* 按键扫描函数 */
        // __tk_scan(); // 使用了库里面的接口（闭源库）

        /* 用户循环扫描函数接口 */
        // user_handle();

        
        // 扫描状态是否变化，如果变化则更新标志位，更新状态的信息到结构体中
        // pin_level_scan();
        // speed_scan();        // 扫描当前时速
        // engine_speed_scan(); // 扫描当前发动机转速
        // adc_scan(); // adc扫描，并转换成百分比的形式发送
        // mileage_scan(); // 里程扫描（大计里程扫描+小计里程扫描）

        uart0_scan_handle(); // 检查串口接收缓冲区的数据是否符合协议,如果有正确的指令，会存到另一个缓冲区中
        instruction_scan();  // 扫描是否有合法的指令
        // __uart_buff_check();
        instruction_handle(); // 扫描是否有对应的获取/状态更新操作

        
        


        // flash_write_test();
    }
}

/**
 * @}
 */

/*************************** (C) COPYRIGHT 2022 HUGE-IC ***** END OF FILE *****/
