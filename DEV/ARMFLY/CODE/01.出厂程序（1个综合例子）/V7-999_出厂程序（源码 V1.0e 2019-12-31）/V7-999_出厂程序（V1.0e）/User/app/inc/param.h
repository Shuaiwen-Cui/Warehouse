/*
*********************************************************************************************************
*
*	ģ������ : Ӧ�ó������ģ��
*	�ļ����� : param.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __PARAM_H
#define __PARAM_H

/* ����2�к�ֻ��ѡ����һ */
#define PARAM_SAVE_TO_EEPROM			/* �����洢���ⲿ��EEPROM (AT24C128) */
//#define PARAM_SAVE_TO_FLASH		/* �����洢��CPU�ڲ�Flash */

#ifdef PARAM_SAVE_TO_EEPROM
	#define PARAM_ADDR		0			/* ��������ַ */
#endif

#ifdef PARAM_SAVE_TO_FLASH
	#define PARAM_ADDR		ADDR_FLASH_SECTOR_3			/* 0x0800C000 �м��16KB����������Ų��� */
	//#define PARAM_ADDR	 ADDR_FLASH_SECTOR_11		/* 0x080E0000 Flash���128K����������Ų��� */
#endif

#define PARAM_VER			0x00000104					/* �����汾 */

/* ȫ�ֲ��� */
typedef struct
{
	uint32_t ParamVer;			/* �������汾���ƣ������ڳ�������ʱ�������Ƿ�Բ��������������� */

	/* LCD�������� */
	uint8_t ucBackLight;

	/* uip ip ��ַ���� */
	uint8_t uip_ip[4];			/* ����IP��ַ */
	uint8_t uip_net_mask[4];	/* �������� */
	uint8_t uip_gateway[4];	/* Ĭ������ */

	/* lwip ip ��ַ���� */
	uint8_t lwip_ip[4];			/* ����IP��ַ */
	uint8_t lwip_net_mask[4];	/* �������� */
	uint8_t lwip_gateway[4];	/* Ĭ������ */

	/* ���������� */
	uint8_t ucRadioMode;		/* AM �� FM */
	uint8_t ucRadioListType;		/* ��̨�б����͡��人������ȫ�� */
	uint8_t ucIndexFM;			/* ��ǰFM��̨���� */
	uint8_t ucIndexAM;			/* ��ǰ��̨���� */
	uint8_t ucRadioVolume;		/* ���� */
	uint8_t ucSpkOutEn;			/* ���������ʹ�� */
	
	uint8_t Addr485;
	uint32_t Baud485;
}
PARAM_T;

extern PARAM_T g_tParam;

void LoadParam(void);
void SaveParam(void);

#endif
