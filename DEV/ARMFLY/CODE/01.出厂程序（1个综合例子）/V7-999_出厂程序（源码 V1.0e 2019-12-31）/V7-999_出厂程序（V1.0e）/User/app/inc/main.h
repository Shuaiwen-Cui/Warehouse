/*
*********************************************************************************************************
*
*	ģ������ : mainģ��
*	�ļ����� : main.h
*	��    �� : V1.0
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _MAIN_H_
#define _MAIN_H_

/* ����LCD������� */
#define VER_INFO	"������STM32-V7������  (V1.0e)"

/* ���ڴ��ڴ�ӡ */
/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32-V7�������Գ���"
#define EXAMPLE_DATE	"2019-12-30"
#define DEMO_VER	"1.0e"

/* ������״̬�ֶ���, MS = Main Status */
enum
{
	MS_MAIN_MENU = 0,	/* ������ */
	
	MS_HARDWARE_INFO,	/* Ӳ����Ϣ */
	MS_TEST_TOUCH,		/* �������Ͱ��� */
	MS_RADIO,			/* ������ */	
	MS_WM8978_REC,		/* WM8978¼���� */	
	MS_MEMS,			/* MEMS ���������ԣ�I2C�� ���������ǣ������ƣ���ѹ�� */		
	MS_ETH_WEB,			/* ��̫��DM9000��DM9161 WEB������ */
	
	MS_GPS,				/* GPS���� */
	MS_GPRS,			/* GPRSģ����� */	
	MS_MP3			,	/* MP3������״̬ */	
	MS_MASS_STORAGE,	/* SD����NANDģ��U�� */		
	MS_CAMERA,			/* ����ͷ */
	MS_OLED,			/* OLED��ʾ�� */
	
	MS_AD7606,			/* AD7606���ݲɼ�ģ����� */		
	MS_AD7705,			/* AD7705ģ�� */
	MS_ADS1256,			/* ADS1256ģ�� */
	MS_RS485,			/* RS485ͨ�� */
	MS_DAC8501,			/* DAC8501ģ�� */
	MS_DAC8562,			/* DAC8562ģ�� */
	MS_RA8875,			/* RA8875�ֿ�  */

	MS_CALIBRATION	,	/* У׼����״̬ */	
	MS_CAN,				/* CANͨ�� */	
	MS_DSO,				/* DSO ʾ���� */	
	MS_WIFI,			/* WIFIģ�� */	
	MS_AD9833,			/* AD9833ģ�� */		
		
	MS_RESERVED			/* �����Ĺ���״̬ */
};

extern uint16_t g_MainStatus;

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

