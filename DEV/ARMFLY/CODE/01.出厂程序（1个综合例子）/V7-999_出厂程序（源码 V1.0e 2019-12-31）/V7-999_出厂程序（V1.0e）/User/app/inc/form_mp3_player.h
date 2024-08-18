/*
*********************************************************************************************************
*
*	ģ������ : �ȴ������ĳ������
*	�ļ����� : reserve.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#ifndef __MP3_PLAYER_H
#define __MP3_PLAYER_H

/* ����һ������MP3�������Ľṹ��
����ȫ�ֱ�������
*/
typedef struct
{
	uint8_t ucMuteOn;			/* 0 : ������ 1: ���� */
	uint8_t ucVolume;			/* ��ǰ���� */
	uint32_t uiProgress;		/* ��ǰ����(�Ѷ�ȡ���ֽ���) */
	uint8_t ucPauseEn;			/* ��ͣʹ�� */

	uint8_t ListCount;			/* �����б�ĸ������� */
	uint8_t ListIndex;			/* ��ǰ�������� */
}MP3_T;

/* �����б� */
typedef struct
{
	char FileName[13];		/* 8+3�ṹ�ļ��� */
	uint32_t FileSize;		/* �ļ����� */
	char LenFileName[256];	/* ���ļ���, ����������ʾ */
}PLAY_LIST_T;

void MP3Player(void);

#endif

