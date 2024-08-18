/*
*********************************************************************************************************
*
*	ģ������ : mainģ��
*	�ļ����� : main.h
*	��    �� : V1.1
*
*	Copyright (C), 2014-2015, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _MAIN_H_
#define _MAIN_H


typedef enum
{
	NUMPAD_IP = 0,			/* IP��ַ */
	NUMPAD_TEL = 1,			/* �绰���� */
	NUMPAD_INT = 2,			/* ����������Χ�жϣ� */
	NUMPAD_STR = 3			/* ������ĸ���� С���� */	
}NUMPAD_MODE_E;

/* �β�����0�� IP��ַ */
typedef struct
{
	uint8_t ip_buf[4];
}NUMPAD_IP_T;

/* �β�����1��֧��С���� */
typedef struct
{
	int32_t Min;	/* ��Сֵ */
	int32_t Max;	/* ���ֵ */
	uint8_t DotNum;	/* С����λ�� */
}NUMPAD_INT_T;

void DispInvlidInput(void);
uint8_t InputNumber(NUMPAD_MODE_E _Mode, char *_Caption, void *_pInParam, void *_pOutParam);
void ClearWinNumPad(uint16_t _usColor);
uint8_t InputInt(char *_Caption, int32_t _min, int32_t _max, int32_t *_value);

#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/

