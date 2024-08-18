/*
*********************************************************************************************************
*
*	ģ������ : MEMS ���������ԣ�I2C�� ���������ǣ������ƣ���ѹ��, ���նȴ�����
*	�ļ����� : mems_test.c
*	��    �� : V1.0
*	˵    �� : ����MPU-6050, HCM5833L, BMP085, BH1750
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2013-02-01 armfly  �׷�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "form_mems.h"

/* 4���������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	120
#define BOX1_W	190
#define BOX1_TEXT	"MPU6050 ������"

#define BOX2_X	(BOX1_X + BOX1_W + 5)
#define BOX2_Y	BOX1_Y
#define BOX2_H	BOX1_H
#define BOX2_W	250
#define BOX2_TEXT	"������"

#define BOX3_X	BOX1_X
#define BOX3_Y	(BOX1_Y + BOX1_H + 5)
#define BOX3_H	52
#define BOX3_W	220
#define BOX3_TEXT	"BMP085 ��ѹ�ƺ��¶�"

#define BOX4_X	(BOX1_X + BOX3_W + 5)
#define BOX4_Y	(BOX1_Y + BOX1_H + 5)
#define BOX4_H	52
#define BOX4_W	220
#define BOX4_TEXT	"BH1750 ���ն�"

#define BOX5_X	BOX1_X
#define BOX5_Y	(BOX3_Y + BOX3_H + 5)
#define BOX5_H	52
#define BOX5_W	220
#define BOX5_TEXT	"DS18B20 �¶ȴ�����"

#define BOX6_X	(BOX1_X + BOX5_W + 5)
#define BOX6_Y	(BOX3_Y + BOX3_H + 5)
#define BOX6_H	52
#define BOX6_W	220
#define BOX6_TEXT	"DHT11 ��ʪ�ȴ�����"

/* ���ذ�ť������(��Ļ���½�) */
#define BUTTON_RET_ID	0
#define BUTTON_RET_H	32
#define BUTTON_RET_W	60
#define	BUTTON_RET_X	(g_LcdWidth - BUTTON_RET_W - 4)
#define	BUTTON_RET_Y	(g_LcdHeight - BUTTON_RET_H - 4)
#define	BUTTON_RET_TEXT	"����"

static void DispInitFace(void);
static void DispMPU6050(void);
static void DispHMC5833L(void);
static void DispBMP085(void);
static void DispBH1750(float _value);
static void DispDS18B20(void);
static void DispDHT11(void);

static void DispButton(uint8_t _id, uint8_t _focus);

static uint8_t s_use_dht11 = 0;		/* �����Զ�ѡ�� DS18B20�� DHT11 */

/*
*********************************************************************************************************
*	�� �� ��: TestMems
*	����˵��: ���������ǣ����ٶȼƣ������ƣ���ѹ�ƣ����ն�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TestMems(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
//	uint8_t fRefresh;		/* ˢ�������־,1��ʾ��Ҫˢ�� */
	uint8_t fQuit = 0;

	int16_t tpX, tpY;

	DispInitFace();

//	fRefresh = 1;

	/* ��������1��BMP085 */
	bsp_InitBMP085();
	BMP085_ReadTempPress();		/* ��ȡ��������ȫ�ֱ��� */
	
	if (i2c_CheckDevice(HMC5883L_SLAVE_ADDRESS) == 0)
	{
		bsp_InitHMC5883L();
	}
	
	bsp_InitMPU6050();
	
	bsp_InitDS18B20();
	
	/* ����������ѭ���� */
	bsp_StartAutoTimer(0, 250);		/* ������ ���ٶ� ������ ��ʱ�������� */
	bsp_DelayMS(125);
	bsp_StartAutoTimer(1, 1000);	/* ��ѹ�ƺ͹��նȶ�ʱ�������� */
	while (fQuit == 0)
	{
		bsp_Idle();

		if (bsp_CheckTimer(0))
		{
			MPU6050_ReadData();

			DispMPU6050();
			
			HMC5883L_ReadData();
			DispHMC5833L();		

			DispDS18B20();	/* ��ȡ����ʾDS18B20������ */				
		}

		if (bsp_CheckTimer(1))
		{
			BMP085_ReadTempPress();
			DispBMP085();

			DispBH1750(BH1750_GetLux());
		
			DispDHT11();
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					if (TOUCH_InRect(tpX, tpY, BUTTON_RET_X, BUTTON_RET_Y, BUTTON_RET_H, BUTTON_RET_W))
					{
						DispButton(BUTTON_RET_ID, 1);
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (TOUCH_InRect(tpX, tpY, BUTTON_RET_X, BUTTON_RET_Y, BUTTON_RET_H, BUTTON_RET_W))
					{
						DispButton(BUTTON_RET_ID, 0);
						fQuit = 1;	/* ���� */
					}
					else	/* ��ťʧȥ���� */
					{
						DispButton(BUTTON_RET_ID, 0);
					}
					break;
			}
		}

		/* �������¼� */
		ucKeyCode = bsp_GetKey();
		if (ucKeyCode > 0)
		{
			/* �м����� */
			switch (ucKeyCode)
			{
				case KEY_DOWN_K1:		/* K1���л���Ƶ��ʽ�����´ο�ʼ¼���ͷ���ʱ��Ч */
					break;

				case KEY_DOWN_K2:		/* K2�����£�¼�� */
					break;

				case KEY_DOWN_K3:		/* K3�����£����� */
					break;

				case JOY_DOWN_U:		/* ҡ��UP������ */
					break;

				case JOY_DOWN_D:		/* ҡ��DOWN������ */
					break;

				case JOY_DOWN_L:		/* ҡ��LEFT������ */
					break;

				case JOY_DOWN_R:	/* ҡ��RIGHT������ */
					break;

				case JOY_DOWN_OK:		/* ҡ��OK������ */
					break;

				default:
					break;
			}
		}
	}

	bsp_StopTimer(0);
	bsp_StopTimer(1);
}

/*
*********************************************************************************************************
*	�� �� ��: DispMPU6050
*	����˵��: ��ʾMPU6050�������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispMPU6050(void)
{
	FONT_T tFont;
	char buf[128];
	uint16_t x, y;
	uint8_t ucLineCap = 17;

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;		/* ������� 16���� */
		tFont.FrontColor = CL_BLUE;		/* ������ɫ */
		tFont.BackColor = CL_BTN_FACE;	/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}

	x = BOX1_X + 5;
	y = BOX1_Y + 18;

	sprintf(buf, "GYRO_X  = %6d", g_tMPU6050.GYRO_X);
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;

	sprintf(buf, "GYRO_Y  = %6d", g_tMPU6050.GYRO_Y);
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;

	sprintf(buf, "GYRO_Z  = %6d", g_tMPU6050.GYRO_Z);
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;

	sprintf(buf, "Accel_X = %6d", g_tMPU6050.Accel_X);
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;

	sprintf(buf, "Accel_Y = %6d", g_tMPU6050.Accel_Y);
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;

	sprintf(buf, "Accel_Z = %6d", g_tMPU6050.Accel_Z);
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;

}

/*
*********************************************************************************************************
*	�� �� ��: DispHMC5833L
*	����˵��: ��ʾHMC5833L���������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispHMC5833L(void)
{
	FONT_T tFont;
	char buf[128];
	uint16_t x, y;
	uint8_t ucLineCap = 17;

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;		/* ������� 16���� */
		tFont.FrontColor = CL_BLUE;		/* ������ɫ */
		tFont.BackColor = CL_BTN_FACE;	/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}

	x = BOX2_X + 5;
	y = BOX2_Y + 18;

	sprintf(buf, "   ��ǰ | ��С, ���,��Χ");
	tFont.FrontColor = CL_GREY;		/* ��ɫ �쳣*/
	LCD_DispStr(x, y, buf, &tFont);	
	y += ucLineCap;
	
	sprintf(buf, "X =%5d|%5d,%5d,%5d", g_tMag.X, g_tMag.X_Min, g_tMag.X_Max, 
		g_tMag.X_Max - g_tMag.X_Min);
	if (g_tMag.X_Max - g_tMag.X_Min < 500)
	{
		tFont.FrontColor = CL_RED;		/* ��ɫ �쳣*/
	}
	else
	{
		tFont.FrontColor = CL_BLACK;	/* ��ɫ  ���� */
	}	
	LCD_DispStr(x, y, buf, &tFont);	
	y += ucLineCap;

	sprintf(buf, "Y =%5d|%5d,%5d,%5d", g_tMag.Y, g_tMag.Y_Min, g_tMag.Y_Max, 
		g_tMag.Y_Max - g_tMag.Y_Min);
	if (g_tMag.Y_Max - g_tMag.Y_Min < 500)
	{
		tFont.FrontColor = CL_RED;		/* ��ɫ �쳣*/
	}
	else
	{
		tFont.FrontColor = CL_BLACK;	/* ��ɫ  ���� */
	}	
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;

	sprintf(buf, "Z =%5d|%5d,%5d,%5d", g_tMag.Z, g_tMag.Z_Min, g_tMag.Z_Max, 
		g_tMag.Z_Max - g_tMag.Z_Min);
	if (g_tMag.Z_Max - g_tMag.Z_Min < 500)
	{
		tFont.FrontColor = CL_RED;		/* ��ɫ �쳣*/
	}
	else
	{
		tFont.FrontColor = CL_BLACK;	/* ��ɫ  ���� */
	}
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;	
	
	/* ��ʾ���üĴ�����ID�Ĵ��� */
	y += 3;	
	tFont.FrontColor = CL_GREY;		/* ������ɫ */
	
	sprintf(buf, "CFG_A = 0x%02X,CFG_B = 0x%02X", g_tMag.CfgRegA,  g_tMag.CfgRegB);	
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;	

	sprintf(buf, "Mode  = 0x%02d,ID = %s", g_tMag.ModeReg,  (char *)g_tMag.IDReg);	
	LCD_DispStr(x, y, buf, &tFont);
}

/*
*********************************************************************************************************
*	�� �� ��: DispBMP085
*	����˵��: ��ʾBMP085���������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispBMP085(void)
{
	FONT_T tFont;
	char buf[128];
	uint16_t x, y;
	uint8_t ucLineCap = 17;

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;		/* ������� 16���� */
		tFont.FrontColor = CL_BLUE;		/* ������ɫ */
		tFont.BackColor = CL_BTN_FACE;	/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}

	x = BOX3_X + 5;
	y = BOX3_Y + 18;

	/* �¶�ֵ�� ��λ 0.1���϶� */
	/* ѹ��ֵ�� ��λ Pa */
	sprintf(buf, "�����¶�  = %3d.%d��", g_tBMP085.Temp / 10, g_tBMP085.Temp % 10);
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;

	sprintf(buf, "����ѹ��  = %3d.%03dKPa", g_tBMP085.Press / 1000, g_tBMP085.Press % 1000);
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;
}

/*
*********************************************************************************************************
*	�� �� ��: DispBH1750
*	����˵��: ��ʾBH1750���������
*	��    ��: _value : �����ʽ�Ĺ��ն�ֵ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispBH1750(float _value)
{
	FONT_T tFont;
	char buf[128];
	uint16_t x, y;
	uint8_t ucLineCap = 17;

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;		/* ������� 16���� */
		tFont.FrontColor = CL_BLUE;		/* ������ɫ */
		tFont.BackColor = CL_BTN_FACE;	/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}

	x = BOX4_X + 5;
	y = BOX4_Y + 18;

	sprintf(buf, "���ն�  = %6.2f lux", _value);
	LCD_DispStr(x, y, buf, &tFont);
	y += ucLineCap;
}

/*
*********************************************************************************************************
*	�� �� ��: DispDS18B20
*	����˵��: ��ʾDS18B20���������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispDS18B20(void)
{
	FONT_T tFont;
	char buf[128];
	uint16_t x, y;

	if (s_use_dht11 == 1)
	{		
		/* ��DS18B20������ ��ʱ����, ����ӡ�� DHT11 */
		return;
	}
	
	/* �� ROM ID */
	{
		uint8_t id[8];
		uint8_t ret;
		uint8_t i;		

		x = BOX5_X + 5;
		y = BOX5_Y + 18;
			
		ret = DS18B20_ReadID(id);
		
		if (ret == 0)
		{
			/* ����������� ��ɫ */
			{
				tFont.FontCode = FC_ST_16;		/* ������� 16���� */
				tFont.FrontColor = CL_RED;		/* ������ɫ */
				tFont.BackColor = CL_BTN_FACE;	/* ���ֱ�����ɫ */
				tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
			}		
		}
		else
		{
			/* ����������� ��ɫ */
			{
				tFont.FontCode = FC_ST_16;		/* ������� 16���� */
				tFont.FrontColor = CL_BLUE;		/* ������ɫ */
				tFont.BackColor = CL_BTN_FACE;	/* ���ֱ�����ɫ */
				tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
			}	
		}
		
		sprintf(buf, "id = ");
		for (i = 0; i < 8; i++)
		{
			sprintf(&buf[strlen(buf)], "%02X", id[i]);
			if (i == 3)
			{
				sprintf(&buf[strlen(buf)], " ");
			}
		}
		LCD_DispStr(x, y, buf, &tFont);		
	}

	{
		int16_t reg;
			
		reg = DS18B20_ReadTempReg();
		sprintf(buf, "reg = 0x%04X -> %-4.04f�� ", (uint16_t)reg, (float)reg / 16);
		
		x = BOX5_X + 5;
		y += 17;
		
		/* ����������� ��ɫ */
		{
			tFont.FontCode = FC_ST_16;		/* ������� 16���� */
			tFont.FrontColor = CL_BLUE;		/* ������ɫ */
			tFont.BackColor = CL_BTN_FACE;	/* ���ֱ�����ɫ */
			tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
		}	
		LCD_DispStr(x, y, buf, &tFont);		

		LCD_DispStr(x, y + 26, "DS18B20��DHT11����ͬʱ���뿪����", &tFont);			
	}		
}

/*
*********************************************************************************************************
*	�� �� ��: DispDHT11
*	����˵��: ��ʾDHT11���������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispDHT11(void)
{
	FONT_T tFont;
	char buf[128];
	uint16_t x, y;
	
	DHT11_T tDHT;
	uint8_t ret;

	x = BOX6_X + 5;
	y = BOX6_Y + 18;

	ret = DHT11_ReadData(&tDHT);
	if (ret == 1)
	{
		/* ����������� ��ɫ */
		{
			tFont.FontCode = FC_ST_16;		/* ������� 16���� */
			tFont.FrontColor = CL_BLUE;		/* ������ɫ */
			tFont.BackColor = CL_BTN_FACE;	/* ���ֱ�����ɫ */
			tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
		}			
		sprintf(buf, "ʪ�� %d%%,�¶� %d��       ", tDHT.Hum, tDHT.Temp);
		LCD_DispStr(x, y, buf, &tFont);					
		
		s_use_dht11 = 1;	/* ��DS18B20������ ��ʱ���� */
	}			
	else
	{
		/* ����������� ��ɫ */
		{
			tFont.FontCode = FC_ST_16;		/* ������� 16���� */
			tFont.FrontColor = CL_RED;		/* ������ɫ */
			tFont.BackColor = CL_BTN_FACE;	/* ���ֱ�����ɫ */
			tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
		}		
		sprintf(buf, "δ����DHT11       ");		
		LCD_DispStr(x, y, buf, &tFont);			

		s_use_dht11 = 0;	/* ��DS18B20������ ȡ������ */
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DispInitFace
*	����˵��: ��ʾ��ʼ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispInitFace(void)
{
	FONT_T tFontBox;		/* ����һ������ṹ���������������������� */
	GROUP_T tBox;

	LCD_ClrScr(CL_BTN_FACE);  	/* ������������ɫ */

	/* ����������� */
	{
		/* ��������� */
		tFontBox.FontCode = FC_ST_16;
		tFontBox.BackColor = CL_BTN_FACE;
		tFontBox.FrontColor = CL_BLACK;
		tFontBox.Space = 0;
	}

	/* ��ʾ����� */
	{
		tBox.Font = &tFontBox;

		tBox.Left = BOX1_X;
		tBox.Top = BOX1_Y;
		tBox.Height = BOX1_H;
		tBox.Width = BOX1_W;
		tBox.pCaption = BOX1_TEXT;
		LCD_DrawGroupBox(&tBox);

		tBox.Left = BOX2_X;
		tBox.Top = BOX2_Y;
		tBox.Height = BOX2_H;
		tBox.Width = BOX2_W;
		tBox.pCaption = BOX2_TEXT;
		LCD_DrawGroupBox(&tBox);

		tBox.Left = BOX3_X;
		tBox.Top = BOX3_Y;
		tBox.Height = BOX3_H;
		tBox.Width = BOX3_W;
		tBox.pCaption = BOX3_TEXT;
		LCD_DrawGroupBox(&tBox);

		tBox.Left = BOX4_X;
		tBox.Top = BOX4_Y;
		tBox.Height = BOX4_H;
		tBox.Width = BOX4_W;
		tBox.pCaption = BOX4_TEXT;
		LCD_DrawGroupBox(&tBox);

		tBox.Left = BOX5_X;
		tBox.Top = BOX5_Y;
		tBox.Height = BOX5_H;
		tBox.Width = BOX5_W;
		tBox.pCaption = BOX5_TEXT;
		LCD_DrawGroupBox(&tBox);

		tBox.Left = BOX6_X;
		tBox.Top = BOX6_Y;
		tBox.Height = BOX6_H;
		tBox.Width = BOX6_W;
		tBox.pCaption = BOX6_TEXT;
		LCD_DrawGroupBox(&tBox);
	}

	DispButton(BUTTON_RET_ID, 0);
}

/*
*********************************************************************************************************
*	�� �� ��: DispButton
*	����˵��: ��ʾָ��λ�õİ�ť
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispButton(uint8_t _id, uint8_t _focus)
{
	FONT_T tFontBtn;		/* ����һ������ṹ���������������������� */
	BUTTON_T tBtn;

	/* ����������� */
	{
		/* ��ť���� */
		tFontBtn.FontCode = FC_ST_16;
		tFontBtn.BackColor = CL_MASK;	/* ͸��ɫ */
		tFontBtn.FrontColor = CL_BLACK;
		tFontBtn.Space = 0;
	}

	tBtn.Font = &tFontBtn;

	switch (_id)
	{
		case BUTTON_RET_ID:
			tBtn.Left = BUTTON_RET_X;
			tBtn.Top = BUTTON_RET_Y;
			tBtn.Height = BUTTON_RET_H;
			tBtn.Width = BUTTON_RET_W;
			tBtn.Focus = _focus;
			tBtn.pCaption = BUTTON_RET_TEXT;
			break;

		default:
			return;
	}

	LCD_DrawButton(&tBtn);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
