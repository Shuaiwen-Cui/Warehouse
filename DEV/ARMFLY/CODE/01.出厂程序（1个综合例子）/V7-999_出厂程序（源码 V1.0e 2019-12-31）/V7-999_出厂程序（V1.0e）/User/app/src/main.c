/*
*********************************************************************************************************
*
*	ģ������ : ������
*	�ļ����� : main.c
*	��    �� : V1.3
*	˵    �� : �������Գ����Ŀ���ǲ���Ӳ���豸�ĺû�
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-01-01 armfly  ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"		/* printf����������������ڣ����Ա����������ļ� */
#include "images.h"		/* ͼ���ļ� */
#include "main.h"
#include "param.h"
#include "form_ad7606.h"
#include "form_ad7705.h"
#include "form_ad9833.h"
#include "form_ads1256.h"
#include "form_audio_rec.h"
#include "form_camera.h"
#include "form_can.h"
#include "form_dac8501.h"
#include "form_dac8562.h"
#include "form_dso.h"
#include "form_fm_radio.h"
#include "form_gprs.h"
#include "form_gps.h"
#include "form_hard_test.h"
#include "form_http_server.h"
#include "form_main_menu.h"
#include "form_mems.h"
#include "form_mp3_player.h"
#include "form_oled.h"
#include "form_reserve.h"
#include "form_rs485.h"
#include "form_touch_test.h"
#include "form_usbd_msc.h"
#include "form_wifi.h"
#include "ff.h"
#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"

uint16_t g_MainStatus;

static void PrintfLogo(void);

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
	bsp_Init();
	
	PrintfLogo();	/* ��ӡ����Logo������1 */	
	
	LoadParam();	/* ��ȡӦ�ó������, �ú�����param.c */
	LCD_SetBackLight(g_tParam.ucBackLight);		/* ���ñ������ȡ� */
	
	PrintfHardInfo();	/* �ϵ��Լ죬��ӡ������. SDRAM�Լ���� */

	/* ���������״̬��ʵ�ֳ������л� */
	g_MainStatus = MS_MAIN_MENU;	/* ��ʼ״̬ = ͼ����� */
	while (1)
	{
		switch (g_MainStatus)
		{
			 case MS_CALIBRATION:
			 	TOUCH_Calibration(2);		/* 2��У׼����, �ý���ʵ����bsp_touch.c */
			 	g_MainStatus = MS_MAIN_MENU; 	/* У׼��Ϻ󣬷��ص������� */
			 	break;

			 case MS_MAIN_MENU:
			 	MainMenu();		/* ������״̬ */
			 	break;

			 case MS_HARDWARE_INFO:			/* Ӳ����Ϣ */
			 	HardInfo();
			 	g_MainStatus = MS_MAIN_MENU;
			 	break;

			 case MS_TEST_TOUCH:			/* ���Դ����� */
			 	TestTouch();
			 	g_MainStatus = MS_MAIN_MENU;
			 	break;

			 case MS_RADIO:					/* Si4730������ */
			 	RadioMain();
			 	g_MainStatus = MS_MAIN_MENU;
			 	break;

			 case MS_WM8978_REC:			/* WM8978¼���� */
			 	RecorderDemo();
			 	g_MainStatus = MS_MAIN_MENU;
			 	break;

			 case MS_ETH_WEB:				/* �������ԣ�DM9000��DM9161 */
			 	WebServer();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			 case MS_MEMS:					/* MEMS ���������ԣ�I2C�� ���������ǣ������ƣ���ѹ�� */
			 	TestMems();
			 	g_MainStatus = MS_MAIN_MENU;
			 	break;

			case MS_GPS:					/* GPS���� */
			 	TestGPS();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_GPRS:					/* GPRSģ����� */
			 	TestGPRS();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_MP3:
				MP3Player();				/* MP3������ */
			 	g_MainStatus = MS_MAIN_MENU; 	/* У׼��Ϻ󣬷��ص������� */
			 	break;

			case MS_MASS_STORAGE:			/* SD����NANDģ��U�� */
			 	TestUsbdMsc();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_CAMERA:					/* ����ͷ */
				TestCamera();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_OLED:					/*  OLED��ʾ�� */
				TestOLED();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_AD7606:					/* AD7606���ݲɼ�ģ����� */
				TestAD7606();
				break;

			case MS_AD7705:					/* AD7705ģ��  */
				TestAD7705();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_ADS1256:				/* ADS1256ģ��  */
				TestADS1256();
				break;

			case MS_RS485:					/* RS485ͨ�� */
				FormMainRS485();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_DAC8501:				/* DAC8501ģ�� */
				FormMainDAC8501();
			 	g_MainStatus = MS_MAIN_MENU;
				break;			
			
			case MS_DAC8562:				/* DAC8562ģ�� */
				FormMainDAC8562();
			 	g_MainStatus = MS_MAIN_MENU;
				break;				
			
			case MS_DSO:
				DsoMain();					/* ʾ���� */
				g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_WIFI:
				TestWIFI();					/* ����WIFI���� */
				g_MainStatus = MS_MAIN_MENU;
				break;			
			
			case MS_CAN:
				FormMainCAN();				/* CAN���� */
				g_MainStatus = MS_MAIN_MENU;
				break;	

			case MS_AD9833:
				FormMain9833();				/* AD9833ģ�� */
				g_MainStatus = MS_MAIN_MENU;
				break;				
			
			case MS_RESERVED:				/* �����Ľ��� */
				ReserveFunc();
				g_MainStatus = MS_MAIN_MENU;
				break;
			
			default:
				g_MainStatus = MS_MAIN_MENU;
				break;
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	printf("*************************************************************\n\r");
	printf("* ��������   : %s\r\n", EXAMPLE_NAME);	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", DEMO_VER);		/* ��ӡ���̰汾 */
	printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	printf("* \n\r");	/* ��ӡһ�пո� */
	printf("* QQ    : 1295744630 \r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* Copyright www.armfly.com ����������\r\n");
	printf("*************************************************************\n\r");
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
