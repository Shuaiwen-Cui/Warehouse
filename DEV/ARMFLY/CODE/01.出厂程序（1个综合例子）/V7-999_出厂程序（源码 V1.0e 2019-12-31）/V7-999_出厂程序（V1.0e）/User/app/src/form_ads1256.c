/*
*********************************************************************************************************
*
*	ģ������ : AD7606���ݲɼ�ģ����Գ���
*	�ļ����� : ad7606_test.c
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
#include "main.h"

/* �������ṹ */
typedef struct
{
	FONT_T FontBlack;	/* ��̬������ */
	FONT_T FontBlue;	/* �仯���������� */
	FONT_T FontBtn;		/* ��ť������ */
	FONT_T FontBox;		/* ������������ */

	GROUP_T Box1;

	LABEL_T LabelN[8 + 2];
	LABEL_T LabelV[8 + 2];

	LABEL_T LabelS1;

	BUTTON_T BtnRet;
}FormADS1256_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* �������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	(g_LcdHeight - BOX1_Y - 10)
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_TEXT	"ADS1256���ݲɼ�ģ����Գ���"

/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	60
#define	BTN_RET_X	((BOX1_X + BOX1_W) - BTN_RET_W - 4)
#define	BTN_RET_Y	((BOX1_Y  + BOX1_H) - BTN_RET_H - 4)
#define	BTN_RET_TEXT	"����"

#define LabelN1_X  		(BOX1_X + 6)
#define LabelN1_Y		(BOX1_Y + 20)
#define LabelN1_TEXT	"ͨ��1 : "

	#define LabelV1_X  		(LabelN1_X + 64)
	#define LabelV1_Y		LabelN1_Y
	#define LabelV1_TEXT	"-"

#define LabelS1_X  		(BOX1_X + 6)
#define LabelS1_Y		(BOX1_Y + BOX1_H - 20)
#define LabelS1_TEXT	"ҡ�����¼�ѡ������, ���Ҽ�ѡ���˲�����"

static void InitFormADS1256(void);
static void DispADInitFace(void);
static void DispADStatus(void);

FormADS1256_T *FormADS1256;

/*
*********************************************************************************************************
*	�� �� ��: TestADS1256
*	����˵��: ����ADS1256ģ�顣
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TestADS1256(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	int16_t tpX, tpY;
	FormADS1256_T form;
	uint8_t fRefresh;

	FormADS1256 = &form;

	InitFormADS1256();

	DispADInitFace();

	bsp_InitADS1256();	
	
	ADS1256_CfgADC(ADS1256_GAIN_1, ADS1256_30SPS);	/* ����ADC������ ����1:1, ����������� 30Hz */	
	
	/* ��ӡоƬID (ͨ����ID�����ж�Ӳ���ӿ��Ƿ�����) , ����ʱ״̬�Ĵ����ĸ�4bit = 3 */
	#if 0
	{
		uint8_t id;
			
		id = ADS1256_ReadChipID();
		
		if (id != 3)
		{
			printf("Error, ASD1256 Chip ID = 0x%X\r\n", id);
		}
		else
		{
			printf("Ok, ASD1256 Chip ID = 0x%X\r\n", id);
		}
	}
	#endif
	
	/* ����������ѭ���� */
	bsp_StartAutoTimer(0, 500);

	ADS1256_StartScan();	/* ����8ͨ��ADCɨ�� */
	fRefresh = 1;
	while (g_MainStatus == MS_ADS1256)
	{
		bsp_Idle();

		if (bsp_CheckTimer(0) || (fRefresh == 1))
		{
			fRefresh = 0;
			DispADStatus();
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					if (TOUCH_InRect(tpX, tpY, BTN_RET_X, BTN_RET_Y, BTN_RET_H, BTN_RET_W))
					{
						FormADS1256->BtnRet.Focus = 1;
						LCD_DrawButton(&FormADS1256->BtnRet);
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (TOUCH_InRect(tpX, tpY, BTN_RET_X, BTN_RET_Y, BTN_RET_H, BTN_RET_W))
					{
						FormADS1256->BtnRet.Focus = 0;
						LCD_DrawButton(&FormADS1256->BtnRet);
						g_MainStatus = MS_MAIN_MENU;	/* ���� */
					}
					else	/* ��ťʧȥ���� */
					{
						FormADS1256->BtnRet.Focus = 0;
						LCD_DrawButton(&FormADS1256->BtnRet);
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
				case KEY_DOWN_K1:			/* K1������ �л����� */
				case KEY_DOWN_K2:			/* K2������ */
				case KEY_DOWN_K3:			/* K3������ */
					break;

				case JOY_DOWN_U:			/* ҡ��UP������ */
					if (g_tADS1256.Gain < 6)
					{
						g_tADS1256.Gain++;
					}
					ADS1256_CfgADC(g_tADS1256.Gain, g_tADS1256.DataRate);	/* ����ADC������ ���������������� */
					ADS1256_StartScan();
					fRefresh = 1;
					break;

				case JOY_DOWN_D:			/* ҡ��DOWN������ */
					if (g_tADS1256.Gain > 0)
					{
						g_tADS1256.Gain--;
					}
					ADS1256_CfgADC(g_tADS1256.Gain, g_tADS1256.DataRate);	/* ����ADC������ ���������������� */
					ADS1256_StartScan();
					fRefresh = 1;
					break;

				case JOY_DOWN_L:		/* ҡ��LEFT������ */
					if (g_tADS1256.DataRate < ADS1256_2d5SPS)
					{
						g_tADS1256.DataRate++;
					}
					ADS1256_CfgADC(g_tADS1256.Gain, g_tADS1256.DataRate);	/* ����ADC������ ���������������� */
					ADS1256_StartScan();
					fRefresh = 1;					
					break;

				case JOY_DOWN_R:		/* ҡ��RIGHT������ */
					if (g_tADS1256.DataRate > ADS1256_1000SPS)
					{
						g_tADS1256.DataRate--;
					}
					ADS1256_CfgADC(g_tADS1256.Gain, g_tADS1256.DataRate);	/* ����ADC������ ���������������� */
					ADS1256_StartScan();					
					break;

				case JOY_DOWN_OK:		/* ҡ��OK������ */
					break;

				default:
					break;
			}
		}
	}

	ADS1256_StopScan();		/* ֹͣADS1256���ж���Ӧ */
}

/*
*********************************************************************************************************
*	�� �� ��: DispADStatus
*	����˵��: ��ʾADC״̬
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispADStatus(void)
{
	char buf[128];
	uint8_t i;
	const char *strGain[7] = { "X1 (��5V)", "X2 (��2.5V)", "X4 (��1.25V)", "X8 (��0.625V)", "X16 (��312.5mV)", 
								"X32 (��156.25mV)", "X64 (��78.125mV)"};				

	const char *strRate[ADS1256_DRATE_MAX] = 
		{ 
			"1, 30000SPS",
			"2, 15000SPS",
			"4, 7500SPS",
			"8, 3750SPS",
			"15, 2000SPS",
			"30, 1000SPS",
			"60, 500SPS",
			"300, 100SPS",
			"500, 60SPS",
			"600, 50SPS",
			"1000, 30SPS",
			"1200, 25SPS",
			"2000, 15SPS",
			"3000, 10SPS",
			"6000, 5SPS",
			"12000, 2d5SPS"
		};
								
	/* ��ʾ ADC��� */
	for (i = 0; i < 8; i++)
	{
		int32_t iTemp;
		
		/* ����ʵ�ʵ�ѹֵ�����ƹ���ģ�������׼ȷ�������У׼ */		
		iTemp = ((int64_t)g_tADS1256.AdcNow[i] * 2500000) / 4194303;
		
		if (iTemp < 0)
		{
			iTemp = -iTemp;
			sprintf(buf, "%6d,(-%d.%03d %03d V) ", g_tADS1256.AdcNow[i] , iTemp /1000000, (iTemp%1000000)/1000, iTemp%1000);
		}
		else
		{
			sprintf(buf, "%6d,( %d.%03d %03d V) ", g_tADS1256.AdcNow[i] , iTemp/1000000, (iTemp%1000000)/1000, iTemp%1000);
		}		
		
		FormADS1256->LabelV[i].pCaption = buf;
		LCD_DrawLabel(&FormADS1256->LabelV[i]);
	}

	/* ��ʾ ���� */
	FormADS1256->LabelV[8].pCaption = (char *)strGain[g_tADS1256.Gain];
	LCD_DrawLabel(&FormADS1256->LabelV[8]);
	
	/* ��ʾ �������� */
	FormADS1256->LabelV[9].pCaption = (char *)strRate[g_tADS1256.DataRate];
	LCD_DrawLabel(&FormADS1256->LabelV[9]);
}

/*
*********************************************************************************************************
*	�� �� ��: InitFormADS1256
*	����˵��: ��ʼ��GPS��ʼ����ؼ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormADS1256(void)
{
	/* ������������ */
	FormADS1256->FontBox.FontCode = FC_ST_16;
	FormADS1256->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	FormADS1256->FontBox.FrontColor = CL_BLACK;
	FormADS1256->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	FormADS1256->FontBlack.FontCode = FC_ST_16;
	FormADS1256->FontBlack.BackColor = CL_MASK;		/* ͸��ɫ */
	FormADS1256->FontBlack.FrontColor = CL_BLACK;
	FormADS1256->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	FormADS1256->FontBlue.FontCode = FC_ST_16;
	FormADS1256->FontBlue.BackColor = CL_BTN_FACE;
	FormADS1256->FontBlue.FrontColor = CL_BLUE;
	FormADS1256->FontBlue.Space = 0;

	/* ��ť���� */
	FormADS1256->FontBtn.FontCode = FC_ST_16;
	FormADS1256->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormADS1256->FontBtn.FrontColor = CL_BLACK;
	FormADS1256->FontBtn.Space = 0;

	/* ����� */
	FormADS1256->Box1.Left = BOX1_X;
	FormADS1256->Box1.Top = BOX1_Y;
	FormADS1256->Box1.Height = BOX1_H;
	FormADS1256->Box1.Width = BOX1_W;
	FormADS1256->Box1.pCaption = BOX1_TEXT;
	FormADS1256->Box1.Font = &FormADS1256->FontBox;

	/* ��ǩ */
	{
		uint8_t i;
		const char *strName[10] = {"ͨ��1","ͨ��2","ͨ��3","ͨ��4","ͨ��5","ͨ��6","ͨ��7","ͨ��8", 
				"����(����)", "�˲�(����)"};

		for (i = 0; i < 10; i++)
		{
			FormADS1256->LabelN[i].Left = LabelN1_X;
			FormADS1256->LabelN[i].Top = LabelN1_Y + i * 20;
			FormADS1256->LabelN[i].MaxLen = 0;
			FormADS1256->LabelN[i].pCaption = (char *)strName[i];
			FormADS1256->LabelN[i].Font = &FormADS1256->FontBlack;		/* ��ɫ */

			FormADS1256->LabelV[i].Left = LabelN1_X + 60;
			FormADS1256->LabelV[i].Top = LabelN1_Y + i * 20;		/* ��ɫ */
			FormADS1256->LabelV[i].MaxLen = 0;
			FormADS1256->LabelV[i].pCaption = "-";
			FormADS1256->LabelV[i].Font = &FormADS1256->FontBlue;
		}
		
		FormADS1256->LabelV[8].Left = LabelN1_X + 120;
		FormADS1256->LabelV[9].Left = LabelN1_X + 120;		

		FormADS1256->LabelS1.Left = LabelS1_X;
		FormADS1256->LabelS1.Top = LabelS1_Y;
		FormADS1256->LabelS1.MaxLen = 0;
		FormADS1256->LabelS1.pCaption = LabelS1_TEXT;
		FormADS1256->LabelS1.Font = &FormADS1256->FontBlack;		/* ��ɫ */
	}

	/* ��ť */
	FormADS1256->BtnRet.Left = BTN_RET_X;
	FormADS1256->BtnRet.Top = BTN_RET_Y;
	FormADS1256->BtnRet.Height = BTN_RET_H;
	FormADS1256->BtnRet.Width = BTN_RET_W;
	FormADS1256->BtnRet.pCaption = BTN_RET_TEXT;
	FormADS1256->BtnRet.Font = &FormADS1256->FontBtn;
	FormADS1256->BtnRet.Focus = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: DispADInitFace
*	����˵��: ��ʾ��ʼ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispADInitFace(void)
{
	LCD_ClrScr(CL_BTN_FACE);

	/* ����� */
	LCD_DrawGroupBox(&FormADS1256->Box1);

	/* ��ǩ */
	{
		uint8_t i;

		for (i = 0; i < 10; i++)
		{
			LCD_DrawLabel(&FormADS1256->LabelN[i]);
		}

		LCD_DrawLabel(&FormADS1256->LabelS1);
	}

	/* ��ť */
	LCD_DrawButton(&FormADS1256->BtnRet);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
