/*
*********************************************************************************************************
*
*	模块名称 : 主程序
*	文件名称 : main.c
*	版    本 : V1.3
*	说    明 : 出厂测试程序的目的是测试硬件设备的好坏
*
*	修改记录 :
*		版本号  日期        作者     说明
*		V1.0    2013-01-01 armfly  正式发布
*
*	Copyright (C), 2013-2014, 安富莱电子 www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"		/* printf函数定向输出到串口，所以必须包含这个文件 */
#include "images.h"		/* 图标文件 */
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
*	函 数 名: main
*	功能说明: c程序入口
*	形    参：无
*	返 回 值: 错误代码(无需处理)
*********************************************************************************************************
*/
int main(void)
{
	bsp_Init();
	
	PrintfLogo();	/* 打印例程Logo到串口1 */	
	
	LoadParam();	/* 读取应用程序参数, 该函数在param.c */
	LCD_SetBackLight(g_tParam.ucBackLight);		/* 设置背光亮度。 */
	
	PrintfHardInfo();	/* 上电自检，打印到串口. SDRAM自检很慢 */

	/* 主程序采用状态机实现程序功能切换 */
	g_MainStatus = MS_MAIN_MENU;	/* 初始状态 = 图标界面 */
	while (1)
	{
		switch (g_MainStatus)
		{
			 case MS_CALIBRATION:
			 	TOUCH_Calibration(2);		/* 2点校准触摸, 该界面实现在bsp_touch.c */
			 	g_MainStatus = MS_MAIN_MENU; 	/* 校准完毕后，返回到主界面 */
			 	break;

			 case MS_MAIN_MENU:
			 	MainMenu();		/* 主界面状态 */
			 	break;

			 case MS_HARDWARE_INFO:			/* 硬件信息 */
			 	HardInfo();
			 	g_MainStatus = MS_MAIN_MENU;
			 	break;

			 case MS_TEST_TOUCH:			/* 测试触摸屏 */
			 	TestTouch();
			 	g_MainStatus = MS_MAIN_MENU;
			 	break;

			 case MS_RADIO:					/* Si4730收音机 */
			 	RadioMain();
			 	g_MainStatus = MS_MAIN_MENU;
			 	break;

			 case MS_WM8978_REC:			/* WM8978录音机 */
			 	RecorderDemo();
			 	g_MainStatus = MS_MAIN_MENU;
			 	break;

			 case MS_ETH_WEB:				/* 网卡测试，DM9000和DM9161 */
			 	WebServer();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			 case MS_MEMS:					/* MEMS 传感器测试（I2C） 三轴陀螺仪，磁力计，气压计 */
			 	TestMems();
			 	g_MainStatus = MS_MAIN_MENU;
			 	break;

			case MS_GPS:					/* GPS测试 */
			 	TestGPS();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_GPRS:					/* GPRS模块测试 */
			 	TestGPRS();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_MP3:
				MP3Player();				/* MP3播放器 */
			 	g_MainStatus = MS_MAIN_MENU; 	/* 校准完毕后，返回到主界面 */
			 	break;

			case MS_MASS_STORAGE:			/* SD卡，NAND模拟U盘 */
			 	TestUsbdMsc();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_CAMERA:					/* 摄像头 */
				TestCamera();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_OLED:					/*  OLED显示器 */
				TestOLED();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_AD7606:					/* AD7606数据采集模块测试 */
				TestAD7606();
				break;

			case MS_AD7705:					/* AD7705模块  */
				TestAD7705();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_ADS1256:				/* ADS1256模块  */
				TestADS1256();
				break;

			case MS_RS485:					/* RS485通信 */
				FormMainRS485();
			 	g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_DAC8501:				/* DAC8501模块 */
				FormMainDAC8501();
			 	g_MainStatus = MS_MAIN_MENU;
				break;			
			
			case MS_DAC8562:				/* DAC8562模块 */
				FormMainDAC8562();
			 	g_MainStatus = MS_MAIN_MENU;
				break;				
			
			case MS_DSO:
				DsoMain();					/* 示波器 */
				g_MainStatus = MS_MAIN_MENU;
				break;

			case MS_WIFI:
				TestWIFI();					/* 串口WIFI测试 */
				g_MainStatus = MS_MAIN_MENU;
				break;			
			
			case MS_CAN:
				FormMainCAN();				/* CAN网络 */
				g_MainStatus = MS_MAIN_MENU;
				break;	

			case MS_AD9833:
				FormMain9833();				/* AD9833模块 */
				g_MainStatus = MS_MAIN_MENU;
				break;				
			
			case MS_RESERVED:				/* 保留的界面 */
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
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	printf("*************************************************************\n\r");
	printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
	printf("* 例程版本   : %s\r\n", DEMO_VER);		/* 打印例程版本 */
	printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	printf("* \n\r");	/* 打印一行空格 */
	printf("* QQ    : 1295744630 \r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* Copyright www.armfly.com 安富莱电子\r\n");
	printf("*************************************************************\n\r");
}

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
