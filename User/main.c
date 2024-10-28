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
 * ��Ȩ˵����������
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
    // ���Ź�Ĭ�ϴ�, ��λʱ��2s
    system_init();

    WDT_KEY = WDT_KEY_VAL(0xDD); //  �رտ��Ź�

    // �ر�HCK��HDA�ĵ��Թ���
    WDT_KEY = 0x55;  // ���д����
    IO_MAP &= ~0x01; // �������Ĵ�����ֵ��ʵ�ֹر�HCK��HDA���ŵĵ��Թ��ܣ����ӳ�䣩
    WDT_KEY = 0xBB;

    /* �û������ʼ���ӿ� */
    user_init();

    /* ������ʼ�� */
    // tk_param_init();

    /* ϵͳ��ѭ�� */
    while (1)
    {
        /* ����ɨ�躯�� */
        // __tk_scan(); // ʹ���˿�����Ľӿڣ���Դ�⣩

        /* �û�ѭ��ɨ�躯���ӿ� */
        // user_handle();

        
        // ɨ��״̬�Ƿ�仯������仯����±�־λ������״̬����Ϣ���ṹ����
        // pin_level_scan();
        // speed_scan();        // ɨ�赱ǰʱ��
        // engine_speed_scan(); // ɨ�赱ǰ������ת��
        // adc_scan(); // adcɨ�裬��ת���ɰٷֱȵ���ʽ����
        // mileage_scan(); // ���ɨ�裨������ɨ��+С�����ɨ�裩

        uart0_scan_handle(); // ��鴮�ڽ��ջ������������Ƿ����Э��,�������ȷ��ָ���浽��һ����������
        instruction_scan();  // ɨ���Ƿ��кϷ���ָ��
        // __uart_buff_check();
        instruction_handle(); // ɨ���Ƿ��ж�Ӧ�Ļ�ȡ/״̬���²���

        
        


        // flash_write_test();
    }
}

/**
 * @}
 */

/*************************** (C) COPYRIGHT 2022 HUGE-IC ***** END OF FILE *****/
