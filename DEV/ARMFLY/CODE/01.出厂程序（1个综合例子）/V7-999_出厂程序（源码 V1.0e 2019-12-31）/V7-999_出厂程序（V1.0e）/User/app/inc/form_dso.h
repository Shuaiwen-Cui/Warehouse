/*
*********************************************************************************************************
*
*	ģ������ : ˫ͨ��ʾ��������
*	�ļ����� : form_dso.c
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __FORM_DSO_H
#define __FORM_DSO_H

#define	SAMPLE_COUNT	(1 * 1024) 	/* ������ȣ�������������� */
/*
	ʾ������ص����ݽṹ
*/
typedef struct
{
//	uint16_t Ch1Buf[SAMPLE_COUNT];	 /* ͨ��1���ݻ����� */
//	uint16_t Ch2Buf[SAMPLE_COUNT];	 /* ͨ��2���ݻ����� */

	uint16_t *Ch1Buf;	 /* ͨ��1���ݻ����� */
	uint16_t *Ch2Buf;	 /* ͨ��2���ݻ����� */
	
	uint32_t TimeBaseIdHold;			/* ��ͣʱ��ʱ�� */

	uint32_t TimeBaseId;			/* ʱ������, ���ɵõ� us��λ��ʱ�� */
	uint32_t SampleFreq;			/* ����Ƶ�ʣ���λHz */
	uint32_t TimeBase;				/* ʱ�� ���ɵõ� us��λ��ʱ�� */

	uint8_t  Ch1AttId;				/* CH1 ˥���������� */
	uint8_t  Ch2AttId;				/* CH2 ˥���������� */
	int32_t  Ch1Attenuation; 		/* ����1����˥��ϵ��(ԭʼ����x10���ٳ��������)  */
	int32_t  Ch2Attenuation; 		/* ����2����˥��ϵ��(ԭʼ����x10���ٳ��������)  */
	uint16_t Ch1VScale;				/* ͨ��1��ֱ�ֶ�ֵmV��λ */
	uint16_t Ch2VScale;				/* ͨ��2��ֱ�ֶ�ֵmV��λ */

	uint32_t TriggerLevel;			/* ������ƽ(ADC��������Ƚ�ֵ) */
	uint32_t TriggerUpEdge;			/* 1��ʾ����ģʽ�����أ�0��ʾ������ */

	int16_t Ch1VOffset;				/* ͨ��1 GND��λ��, ����Ϊ���� */
	int16_t Ch2VOffset;				/* ͨ��1 GND��λ��, ����Ϊ���� */

	uint8_t ActiveCH;				/* ��ǰ����ͨ�� 1��ʾCH1, 2��ʾCH2 */
	uint8_t AdjustMode;				/* ��ǰ����ģʽ��0��ʾ���ڷ��ȣ�1��ʾ����ƫ�� */

	/* ʹ��2����������ɲ��εĲ��������� */
	uint16_t xCh1Buf1[310];		/* �������ݣ��������� */
	uint16_t yCh1Buf1[310];		/* �������ݣ��������� */
	uint16_t xCh1Buf2[310];		/* �������ݣ��������� */
	uint16_t yCh1Buf2[310];		/* �������ݣ��������� */

	uint16_t xCh2Buf1[310];		/* �������ݣ��������� */
	uint16_t yCh2Buf1[310];		/* �������ݣ��������� */
	uint16_t xCh2Buf2[310];		/* �������ݣ��������� */
	uint16_t yCh2Buf2[310];		/* �������ݣ��������� */
	uint8_t BufUsed;			/* 0��ʾ��ǰ��Buf1�� 1��ʾ��Buf2 */

	uint8_t HoldEn;				/* ���βɼ���ͣ��־ 1��ʾ��ͣ��0��ʾ���� */
	
	uint8_t CH1_DC;			/* ͨ��1 AC/DC������ */
	uint8_t CH2_DC;			/* ͨ��1 AC/DC������ */
	
	uint8_t CH1_Gain;		/* ͨ��1 ��������� */
	uint8_t CH2_Gain;		/* ͨ��1 ��������� */		
}DSO_T;

/* 
	����֧�ֵĲ���Ƶ��
*/
typedef enum
{
	SR_1K = 0,				/*  */

}SAMPLE_RATE_E;

void InitDSO(void);
void SetSampRate(uint32_t freq);
void DsoMain(void);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/


