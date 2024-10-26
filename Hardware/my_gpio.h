// GPIO���õ�ͷ�ļ�
#ifndef __MY_GPIO_H
#define __MY_GPIO_H

#include "include.h" // ����оƬ�ٷ��ṩ��ͷ�ļ�

// ����������LED�������ӵ�����
#define LED6 P01
#define LED7 P26

// p11��p12�ڿ���������Ϊ����ʹ��
void p11_output_config(void);
void p12_output_config(void); // P12����Ϊ���ģʽ

// void p01_output_config(void); // P01���ӵ��˿������LED6������
// void p26_output_config(void); // P26���ӵ��˿������LED7������P26����Ϊ���ģʽ
void p03_output_config(void);

void p06_input_config(void); // p06���ó���������ģʽ
void p07_input_config(void); // p07���ó���������ģʽ

void p10_input_config(void); // p10���ó���������ģʽ
void p13_input_config(void); // p13���ó���������ģʽ
void p14_input_config(void); // p14���ó���������ģʽ
// void p17_input_config(void); // p17���ó���������ģʽ

void p21_input_config(void); // p21���ó���������ģʽ
void p22_input_config(void); // p22���ó���������ģʽ
void p23_input_config(void); // p23���ó���������ģʽ

void p25_input_config(void);// p25���ó���������ģʽ

void p27_input_config(void); // p27���ó���������ģʽ

void p30_input_config(void); // p30���ó���������ģʽ
// void p31_input_config(void); // p31���ó���������ģʽ

#endif
