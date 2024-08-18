/*
*********************************************************************************************************
*
*	ģ������ : ���������ԡ�
*	�ļ����� : fm_radio.c
*	��    �� : V1.0
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _FM_RADIO_H_
#define _FM_RADIO_H_

#define FM_RX		0
#define AM_RX		1

#define SI4704		4
#define SI4730		30

/*
	Worldwide FM band support (64�C108 MHz)
	Worldwide AM band support (520�C1710 kHz)

	�й���Χ�� 88-108�׺��źŵ��Ʒ�ʽ�ǵ�Ƶ��Ƶ�ʵ��ƣ�ÿ��Ƶ����Ƶ�ʼ����0.1�׺�
*/

typedef struct
{
	uint8_t ChipType;	/* оƬ�ͺŴ���  */
	
	uint8_t ucMode;		/* AM �� FM */
	uint8_t ucListType;	/* ��̨�б����͡��人������ȫ�� */

	uint8_t ucFMCount;	/* FM ��̨���� */
	uint16_t usFMList[255];	/* FM ��̨�б� */
	uint8_t ucIndexFM;	/* ��ǰ��̨���� */

	uint8_t ucAMCount;	/* FM ��̨���� */
	uint16_t usAMList[128];	/* FM ��̨�б� */
	uint8_t ucIndexAM;	/* ��ǰ��̨���� */

	uint32_t usFreq;	/* ��ǰ��̨Ƶ�� */
	uint8_t ucVolume;	/* ���� */

	uint8_t ucSpkOutEn;	/* ���������ʹ�� */
	uint8_t ucRssiEn;	/* �ź�������ʱˢ�� ʹ�� */
}RADIO_T;

void RadioMain(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
