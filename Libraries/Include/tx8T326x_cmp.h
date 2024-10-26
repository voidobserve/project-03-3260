/**
  ******************************************************************************
  * @file    cmp.h
  * @author  HUGE-IC Application Team
  * @version V1.0.0
  * @date    01-05-2021
  * @brief   This file contains all the CMP LL firmware functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2021 HUGE-IC</center></h2>
  *
  *
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CMP_H__
#define __CMP_H__

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/******************************************************************************************
|-----------------------------------------------------------------------------------------|
|            | Bit |                    Description                                       |
|            |----------------------------------------------------------------------------|
|            | 7:5 | �Ƚ��������������жϴ�����ʽ���ƼĴ���                             |
|            |     | 0x0:������  0x1:�½���  0x2:˫����  0x3:�ߵ�ƽ  0x4:�͵�ƽ           |
| CMP0_CON0/ |  4  |                                                                      |
| CMP1_CON0  |  3  | �Ƚ������ȡ��ʹ��  0x0:��ʹ��  0x1:ʹ��                             |
|            |  2  | �Ƚ����ıȽϽ��,ֻ��                                                |
|            |  1  | �ж�ʹ���ź�  0x0:��ʹ��  0x1:ʹ��                                   |
|            |  0  | �Ƚ���ʹ���ź�    0x0:��ʹ��  0x1:ʹ��                               |
|-----------------------------------------------------------------------------------------|
******************************************************************************************/
// CMP0_CON0/CMP1CON0
#define _CMP0_CON0
#define _CMP1_CON0
#define CMP_IRQ_TRG_SEL(n)         ((n) << 5)  // �Ƚ��������������жϴ�����ʽ���ƼĴ���
                                               // 0x0:������  0x1:�½���  0x2:˫����  0x3:�ߵ�ƽ  0x4:�͵�ƽ
#define CMP_OUT_INV_EN(n)          ((n) << 3)  // �Ƚ������ȡ��ʹ��  0x0:��ʹ��  0x1:ʹ��
#define CMP_OUT(n)                 ((n) << 2)  // �Ƚ����ıȽϽ��,ֻ��
#define CMP_IRQ_EN(n)              ((n) << 1)  // �ж�ʹ���ź�  0x0:��ʹ��  0x1:ʹ��
#define CMP_EN(n)                  ((n) << 0)  // �Ƚ���ʹ���ź�    0x0:��ʹ��  0x1:ʹ��

/******************************************************************************************
|-----------------------------------------------------------------------------------------|
|            | Bit |                    Description                                       |
|            |----------------------------------------------------------------------------|
|            | 7:6 | �Ƚ������ѡ��λ  0x0:�Ƚ�����������˲����˲���Ľ��               |
|            |     | 0x1:�Ƚ������ͬ����Ľ��  0x2:�Ƚ���������Ͳ������               |
|            |  5  | ���ƱȽ�������Ƿ��������λ                                         |
| CMP0_CON1\ |     | 0x0:�����,������ֹر����ǰ��ֵ  0x1:���                          |
| CMP1_CON1  | 4:2 | �Ƚ�����������ͨ��ѡ��                                               |
|            |     | CMP0:                                                                |
|            |     |  0x0:CMP_PGA 0x1:CMP0_P<0>(P06) 0x2:CMP0_P<1>(P07) 0x3:CMP0_P<2>(P10)|
|            |     |  0x4:CMP0_P<3>(P03)  0x5:CMP0_P<4>(P23)  0x6:CMP0_P<5>(P26)          |
|            |     |  0x7: VTH_OUT_VCC(VTH_PADEN_VDD=0)/VTH_OUT_PAD(VTH_PADEN_VDD=1)      |
|            |     | CMP1:                                                                |
|            |     |  0x0:CMP_PGA 0x1:CMP1_P<0>(P13) 0x2:CMP1_P<1>(P12) 0x3:CMP1_P<2>(P11)|
|            |     |  0x4:CMP1_P<3>(P02)  0x5:CMP1_P<4>(P23)  0x6:CMP1_P<5>(P26) 0x7: CCS |
|            | 1:0 | �Ƚ�����������ͨ��ѡ��                                               |
|            |     | CMP0:                                                                |
|            |     |     0x0:��ʹ��  0x1:CMP0_N<0>/CMP0_N<0>  0x2:CMP0_N<1>  0x3:DAC0_OUT |
|            |     | CMP1:                                                                |
|            |     |     0x0:��ʹ��  0x1:CMP1_N<0>/CMP1_N<0>  0x2:CMP1_N<1>  0x3:DAC1_OUT |
|-----------------------------------------------------------------------------------------|
******************************************************************************************/
// CMP0_CON1\CMP1_CON1
#define _CMP0_CON1
#define _CMP1_CON1
#define CMP_OUT_SEL(n)             ((n) << 6)  // �Ƚ������ѡ��λ  0x0:�Ƚ�����������˲����˲���Ľ��
                                               // 0x1:�Ƚ������ͬ����Ľ��  0x2:�Ƚ���������Ͳ������
#define CMP_OUT_EN(n)              ((n) << 5)  // ���ƱȽ�������Ƿ��������λ
                                               // 0x0:�����,������ֹر����ǰ��ֵ  0x1:���
#define CMP_P_SEL(n)               ((n) << 2)  // �Ƚ�����������ͨ��ѡ��.
                                               // CMP0:
                                               //  0x0:CMP_PGA 0x1:CMP0_P<0>(P06) 0x2:CMP0_P<1>(P07) 0x3:CMP0_P<2>(P10)
                                               //  0x4:CMP0_P<3>(P03)  0x5:CMP0_P<4>(P23)  0x6:CMP0_P<5>(P26)
                                               //  0x7: VTH_OUT_VCC(VTH_PADEN_VDD=0)/VTH_OUT_PAD(VTH_PADEN_VDD=1)
                                               // CMP1:
                                               //  0x0:CMP_PGA 0x1:CMP1_P<0>(P13) 0x2:CMP1_P<1>(P12) 0x3:CMP1_P<2>(P11)
                                               //  0x4:CMP1_P<3>(P02)  0x5:CMP1_P<4>(P23)  0x6:CMP1_P<5>(P26) 0x7: CCS
#define CMP_N_SEL(n)               ((n) << 0)  // �Ƚ�����������ͨ��ѡ��
                                               // 0x0:��ʹ��  0x1:CMP0_N<0>  0x2:CMP0_N<1>  0x3:DAC0_OUT

/******************************************************************************************
|-----------------------------------------------------------------------------------------|
|            | Bit |                    Description                                       |
| CMP0_CON2\ |----------------------------------------------------------------------------|
| CMP1_CON2  | 7:0 | �˲�ʱ�Ӹ�������,���ֵ256,����Ϊ1΢��
|-----------------------------------------------------------------------------------------|
******************************************************************************************/
// CMP0_CON2\CMP1_CON2
#define _CMP0_CON2
#define _CMP1_CON2
#define CMP_FILT_NUM(n)            ((n) << 0)  // �˲�ʱ�Ӹ�������,���ֵ256,����Ϊ1΢��

/******************************************************************************************
|-----------------------------------------------------------------------------------------|
|            | Bit |                    Description                                       |
|            |----------------------------------------------------------------------------|
|            |  7  | ѡ������ź��Ƿ񾭹��˲���                                           |
|            |     | 0x0:�����˲�����ź�  0x1:�˲�ǰ��ͬ���ź�                           |
| CMP0_CON3\ |  6  | �Ƚ������͹���ʹ��λ,ͨ��DAC�������Ĵ���������ֵ                     |
| CMP1_CON3  |  5  | �Ƚ����������ѡ��                                                   |
|            |     | 0x0:�Ƚ����Ƿ������OUTPUT_EN����  0x1:�Ƚ����Ƿ������PWM����       |
|            |  4  | ���ͼ���Դѡ��  0x0:1Mʱ��  0x1:64Kʱ��                              |
|            | 3:0 | ���ͼ�����,�������ó��Ͳ������,����Ϊ16΢���1΢���ѡ              |
|            |     |  0x0:���1�����ڲ���  0x1:���2�����ڲ���  ...  0xF:���16�����ڲ��� |
|-----------------------------------------------------------------------------------------|
******************************************************************************************/
// CMP0_CON3\CMP1_CON3
#define _CMP0_CON3
#define _CMP1_CON3
#define CMP_HY_SEL(n)              ((n) << 7)  // ѡ������ź��Ƿ񾭹��˲���
                                               // 0x0:�����˲�����ź�  0x1:�˲�ǰ��ͬ���ź�
#define CMP_HY_EN(n)               ((n) << 6)  // �Ƚ������͹���ʹ��λ,ͨ��DAC�������Ĵ���������ֵ
#define CMP_OUT_CTRL(n)            ((n) << 5)  // �Ƚ����������ѡ��
                                               // 0x0:�Ƚ����Ƿ������OUTPUT_EN����  0x1:�Ƚ����Ƿ������PWM����
#define CMP_HY_SRC_SEL(n)          ((n) << 4)  // ���ͼ���Դѡ��  0x0:1Mʱ��  0x1:64Kʱ�� 
#define CMP_COUNT_REG(n)           ((n) << 0)  // ���ͼ�����,�������ó��Ͳ������,����Ϊ16΢���1΢���ѡ
                                               // 0x0:���1�����ڲ���  0x1:���2�����ڲ���  ...  0xF:���16�����ڲ���

/******************************************************************************************
|-----------------------------------------------------------------------------------------|
|            | Bit |                    Description                                       |
|            |----------------------------------------------------------------------------|
|            |  7  | �ڲ�DAC���������ʹ���ź�  0x0:�ر�  0x1:��                        |
| CMP0_CON4\ |  6  | N��ʹ��  0x0:��  0x1: �ر�                                         |
| CMP1_CON4  |  5  | ��·ʹ�� �̽ӱȽ���������,���ڱȽ���У׼                             |
|            |     |          0x0:���̽�������  0x1:�̽�������                            |
|            |  4  | P��ʹ��  0x0:��  0x1:�ر�                                          |
|            | 3:0 | �Ƚ���У��ֵ оƬ�ϵ���Զ��������У��ֵ,Ҳ���û�����У��ֵ         |
|-----------------------------------------------------------------------------------------|
******************************************************************************************/
// CMP0_CON4\CMP1_CON4
#define _CMP0_CON4
#define _CMP1_CON4
#define CMP_DAC_TEST_EN(n)         ((n) << 7)  // �ڲ�DAC���������ʹ���ź�  0x0:�ر�  0x1:��
#define CMP_NONLY_EN(n)            ((n) << 6)  // N��ʹ��  0x0:��  0x1:�ر�
#define CMP_OSSHORT_WN(n)          ((n) << 5)  // ��·ʹ�� �̽ӱȽ���������,���ڱȽ���У׼
                                               //          0x0:���̽�������  0x1:�̽�������
#define CMP_PONLY_EN(n)            ((n) << 4)  // P��ʹ��  0x0:��  0x1:�ر�
#define CMP_TRIM(n)                ((n) << 0)  // �Ƚ���У��ֵ оƬ�ϵ���Զ��������У��ֵ,Ҳ���û�����У��ֵ

/******************************************************************************************
|-----------------------------------------------------------------------------------------|
|            | Bit |                    Description                                       |
|            |----------------------------------------------------------------------------|
|            |  7  | ��·������ֵ(VCC-0.08/0.2/0.32/0.48)����ʹ���ź�  0x0:��ʹ�� 0x1:ʹ��|
|            |  6  | ��·������ֵ(PAD-0.08/0.2/0.32/0.48)����ʹ���ź�  0x0:��ʹ�� 0x1:ʹ��|
| CMP0_CON5  | 5:4 | �Ƚ���������ֵѡ��                                                   |
|            |     | 0x0:VCC(PAD)-80mv  0x1:VCC(PAD)-200mv                                |
|            |     | 0x2:VCC(PAD)-320mv  0x3:VCC(PAD)-480mv                               |
|            |  3  | �Ƚ����Ĳο���ѹ�ο�����ʹ���ź�  0x0:��ʹ��  0x1:ʹ��               |
|            |  2  | DAC0������ο���ѹѡ���ź�                                           |
|            |     | 0x0:ѡ���ڲ�1.2V��Ϊ�ο���ѹ  0x1:ѡ���ڲ�VCCA��Ϊ�ο���ѹ           |
|            | 1:0 |                                                                      |
|-----------------------------------------------------------------------------------------|
******************************************************************************************/
// CMP0_CON5
#define _CMP0_CON5
#define CMP_VTH_VCC_EN(n)          ((n) << 7)  // ��·������ֵ(VCC-0.08/0.2/0.32/0.48)����ʹ���ź�  0x0:��ʹ�� 0x1:ʹ��
#define CMP_VTH_PAD_EN(n)          ((n) << 6)  // ��·������ֵ(PAD-0.08/0.2/0.32/0.48)����ʹ���ź�  0x0:��ʹ�� 0x1:ʹ��
#define CMP_VTH_VAL_SEL(n)         ((n) << 4)  // �Ƚ���������ֵѡ��
                                               // 0x0:VCC(PAD)-80mv  0x1:VCC(PAD)-200mv
                                               // 0x2:VCC(PAD)-320mv  0x3:VCC(PAD)-480mv
#define CMP_DAC1_VREF_EN(n)        ((n) << 3)  // �Ƚ����Ĳο���ѹ�ο�����ʹ���ź�  0x0:��ʹ��  0x1:ʹ��
#define CMP_DAC0_VREF_SEL(n)       ((n) << 2)  // DAC0������ο���ѹѡ���ź�
                                               // 0x0:ѡ���ڲ�1.2V��Ϊ�ο���ѹ  0x1:ѡ���ڲ�VCCA��Ϊ�ο���ѹ

/******************************************************************************************
|-----------------------------------------------------------------------------------------|
|            | Bit |                    Description                                       |
|            |----------------------------------------------------------------------------|
|            |  7  | �Ƚ����Ĳο���ѹ�ο�����ʹ���ź�  0x0:��ʹ��  0x1:ʹ��               |
|  CMP_CON   |  6  | ����Դ��ʹ���ź�  0x0:��ʹ��  0x1:ʹ��                               |
|            | 5:0 | ����Դ���������ź�(step=2.5%)  0x00:11.8mA  0x0F:20mA  0x1F:28.4mA   |
|-----------------------------------------------------------------------------------------|
******************************************************************************************/
// CMP_CON
#define _CMP_CON
#define CMP_REF_EN(n)              ((n) << 7)  // �Ƚ����Ĳο���ѹ�ο�����ʹ���ź�  0x0:��ʹ��  0x1:ʹ��
#define CMP_CCS_EN(n)              ((n) << 6)  // ����Դ��ʹ���ź�  0x0:��ʹ��  0x1:ʹ��
#define CMP_TRIMIB(n)              ((n) << 0)  // ����Դ���������ź�(step=2.5%)  0x00:11.8mA  0x0F:20mA  0x1F:28.4mA

/******************************************************************************************
|-----------------------------------------------------------------------------------------|
|            | Bit |                    Description                                       |
| CMP0_DAC0\ |----------------------------------------------------------------------------|
| CMP1_DAC0  | 7:0 | �Ƚ�����DAC0�Ĵ���                                                   |
|            |     | ������������ֳ���,���Ĵ�����Ϊ�Ƚ���0����DAC������,����������ֳ�   |
|            |     | ��,DAC������ֵ��CMP0_DAC0��CMP0_DAC1֮��ѡ��,���Ƚ����Ϊ1ʱѡ��     |
|            |     | CMP0_DAC0,���Ƚ������Ϊ0ʱѡ��CMP0_DAC1                             |
|            |     | 1) DAC0Ϊ0x00~0xF0ʱ: DAC��ѹֵ=(�ο�Դ/0xF0)*DAC0                   |
|            |     | 2) DAC0Ϊ0xF0~0xFFʱ: DAC��ѹֵ=�ο�Դ                               |
|            |     | CMP0_CON5[2]ѡ��DAC�Ĳο�Դ,��ѡ�ڲ�1.2V���ڲ�VCCA                   |
|-----------------------------------------------------------------------------------------|
******************************************************************************************/
// CMP0_DAC0\CMP1_DAC0
#define _CMP0_DAC0
#define _CMP1_DAC0
#define CMP_DAC0_VAL(n)            ((n) << 0)  // �Ƚ�����DAC0�Ĵ���

/******************************************************************************************
|-----------------------------------------------------------------------------------------|
|            | Bit |                    Description                                       |
| CMP0_DAC1\ |----------------------------------------------------------------------------|
| CMP1_DAC1  | 7:0 | �Ƚ�����DAC1�Ĵ���,ʹ�����ֳ���ʱ��Ч                                |
|            |     | 1) DAC0Ϊ0x00~0xF0ʱ: DAC��ѹֵ=(�ο�Դ/0xF0)*DAC0                   |
|            |     | 2) DAC0Ϊ0xF0~0xFFʱ: DAC��ѹֵ=�ο�Դ                               |
|            |     | CMP0_CON5[2]ѡ��DAC�Ĳο�Դ,��ѡ�ڲ�1.2V���ڲ�VCCA                   |
|-----------------------------------------------------------------------------------------|
******************************************************************************************/
// CMP0_DAC1\CMP1_DAC1
#define _CMP0_DAC1
#define _CMP1_DAC1
#define CMP_DAC1_VAL(n)            ((n) << 0)  // �Ƚ�����DAC1�Ĵ���,ʹ�����ֳ���ʱ��Ч
                                               // 1) DAC0Ϊ0x00~0xF0ʱ: DAC��ѹֵ=(�ο�Դ/0xF0)*DAC0
                                               // 2) DAC0Ϊ0xF0~0xFFʱ: DAC��ѹֵ=�ο�Դ
                                               // CMP0_CON5[2]ѡ��DAC�Ĳο�Դ,��ѡ�ڲ�1.2V���ڲ�VCCA

/******************************************************************************************
|-----------------------------------------------------------------------------------------|
|            | Bit |                    Description                                       |
|            |----------------------------------------------------------------------------|
|            |  7  | �Ƚ���1�����ź�wakeupʹ��  0x0:��ʹ��  0x1:ʹ��                      |
|            |  6  | �Ƚ���0�����ź�wakeupʹ��  0x0:��ʹ��  0x1:ʹ��                      |
|  CMP_STA   |  5  | �Ƚ���1ģ��WAKEUPȡ������ʹ��  0x0:��ʹ��  0x1:ʹ��                  |
|            |  4  | �Ƚ���0ģ��WAKEUPȡ������ʹ��  0x0:��ʹ��  0x1:ʹ��                  |
|            |  3  | �Ƚ���1ģ���ź�WAKEUPʹ��  0x0:��ʹ��  0x1:ʹ��                      |
|            |  2  | �Ƚ���0ģ���ź�WAKEUPʹ��  0x0:��ʹ��  0x1:ʹ��                      |
|            |  1  | �Ƚ���1�жϱ�־λ,д1����                                            |
|            |  0  | �Ƚ���0�жϱ�־λ,д1����                                            |
|-----------------------------------------------------------------------------------------|
******************************************************************************************/
// CMP_STA
#define _CMP_STA
#define CMP1_WKUP_EN(n)           ((n) << 7)  // �Ƚ���1�����ź�wakeupʹ��  0x0:��ʹ��  0x1:ʹ��
#define CMP0_WKUP_EN(n)           ((n) << 6)  // �Ƚ���0�����ź�wakeupʹ��  0x0:��ʹ��  0x1:ʹ��
#define CMP1_ANA_INV_EN(n)        ((n) << 5)  // �Ƚ���1ģ��WAKEUPȡ������ʹ��  0x0:��ʹ��  0x1:ʹ��
#define CMP0_ANA_INV_EN(n)        ((n) << 4)  // �Ƚ���0ģ��WAKEUPȡ������ʹ��  0x0:��ʹ��  0x1:ʹ��
#define CMP1_ANA_WKUP(n)          ((n) << 3)  // �Ƚ���1ģ���ź�WAKEUPʹ��  0x0:��ʹ��  0x1:ʹ��
#define CMP0_ANA_WKUP(n)          ((n) << 2)  // �Ƚ���0ģ���ź�WAKEUPʹ��  0x0:��ʹ��  0x1:ʹ��
#define CMP1_IRQ_FLAG(n)          ((n) << 1)  // �Ƚ���1�жϱ�־λ,д1����
#define CMP0_IRQ_FLAG(n)          ((n) << 0)  // �Ƚ���0�жϱ�־λ,д1����

/******************************************************************************************
|-----------------------------------------------------------------------------------------|
|            | Bit |                    Description                                       |
|            |----------------------------------------------------------------------------|
|            | 7:6 | �Ƚ���0��������������ѡ���ź�                                        |
|            |     | 0x0:disable�������  0x1:���������  0x2:���������  0x3:�����������|
| CMP_HYCON  | 5:4 | �Ƚ���0�ĳ��͵�ѹѡ��  0x0:0mv  0x1:10mv  0x2:20mv  0x3:60mv         |
|            |     | Note:CMP1_HYSPNS_VDD=0x0��ʱ��, CMP0_HYSVTH_VDD����ѡ�����0x0       |
|            | 3:2 | �Ƚ���1��������������ѡ���ź�                                        |
|            |     | 0x0:disable�������  0x1:���������  0x2:���������  0x3:�����������|
|            | 1:0 | �Ƚ���1�ĳ��͵�ѹѡ��                                                |
|            |     | 0x0:0mv  0x1:10mv  0x2:20mv  0x3:60mv                                |
|-----------------------------------------------------------------------------------------|
******************************************************************************************/
// CMP_HYCON
#define _CMP_HYCON
#define CMP0_HYSPNS_VDD(n)          ((n) << 6)  // �Ƚ���0��������������ѡ���ź�
                                                // 0x0:disable�������  0x1:���������  0x2:���������  0x3:�����������
#define CMP0_HYSVTH_VDD(n)          ((n) << 4)  // �Ƚ���0�ĳ��͵�ѹѡ��  0x0:0mv  0x1:10mv  0x2:20mv  0x3:60mv
                                                // Note:CMP1_HYSPNS_VDD=0x0��ʱ��, CMP0_HYSVTH_VDD����ѡ�����0x0
#define CMP1_HYSPNS_VDD(n)          ((n) << 2)  // �Ƚ���1��������������ѡ���ź�
                                                // 0x0:disable�������  0x1:���������  0x2:���������  0x3:�����������
#define CMP1_HYSVTH_VDD(n)          ((n) << 0)  // �Ƚ���1�ĳ��͵�ѹѡ��
                                                // 0x0:0mv  0x1:10mv  0x2:20mv  0x3:60mv

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

#ifdef __cplusplus
}
#endif

#endif // __CMP_H__

/**
  * @}
  */

/**
  * @}
  */

/*************************** (C) COPYRIGHT 2021 HUGE-IC ***** END OF FILE *****/