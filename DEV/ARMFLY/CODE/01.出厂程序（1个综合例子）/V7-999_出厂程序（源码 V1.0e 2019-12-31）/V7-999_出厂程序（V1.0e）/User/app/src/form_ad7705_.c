/*
*********************************************************************************************************
*
*	ģ������ : �ȴ������ĳ������
*	�ļ����� : reserve.c
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
#include "form_ad7705.h"

/* �������ṹ */
typedef struct
{
	FONT_T FontBlack;	/* ��̬������ */
	FONT_T FontBlue;	/* �仯���������� */
	FONT_T FontBtn;		/* ��ť������ */
	FONT_T FontBox;		/* ������������ */

	GROUP_T Box1;

	LABEL_T Label1;	LABEL_T Label2;
	LABEL_T Label3; LABEL_T Label4;
	LABEL_T Label5; 
	
	BUTTON_T BtnRet;
}FormAD7705_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* �������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	(g_LcdHeight - BOX1_Y - 10)
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_TEXT	"AD7705ģ����Գ���..."

/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	60
#define	BTN_RET_X	((BOX1_X + BOX1_W) - BTN_RET_W - 4)
#define	BTN_RET_Y	((BOX1_Y  + BOX1_H) - BTN_RET_H - 4)
#define	BTN_RET_TEXT	"����"

#define LABEL1_X  	(BOX1_X + 6)
#define LABEL1_Y	(BOX1_Y + 20)
#define LABEL1_TEXT	"ͨ��1 : "

	#define LABEL2_X  	(LABEL1_X + 64)
	#define LABEL2_Y	LABEL1_Y
	#define LABEL2_TEXT	"0000.0000"

#define LABEL3_X  	(LABEL1_X)
#define LABEL3_Y	(LABEL1_Y + 20)
#define LABEL3_TEXT	"ͨ��2 : "

	#define LABEL4_X  	(LABEL3_X + 64)
	#define LABEL4_Y	(LABEL3_Y)
	#define LABEL4_TEXT	"00000.0000"

#define LABEL5_X  	(LABEL1_X)
#define LABEL5_Y	(LABEL1_Y + 20 * 3)
#define LABEL5_TEXT	"----"


static void InitFormAD7705(void);
static void DispFormAD7705(void);

FormAD7705_T *FormAD7705;

/*
*********************************************************************************************************
*	�� �� ��: TestAD7705
*	����˵��: ����AD7705ģ�顣
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TestAD7705(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fQuit = 0;
	int16_t tpX, tpY;
	FormAD7705_T form;
	uint16_t adc1, adc2;

	FormAD7705 = &form;
	InitFormAD7705();
	DispFormAD7705();

	bsp_InitTM7705();			/* ��ʼ������TM7705 */

	if (g_TM7705_OK == 0)	
	{
		FormAD7705->Label5.pCaption = "δ��⵽ AD7705";
		LCD_DrawLabel(&FormAD7705->Label5);
	}
	else	
	{		
		FormAD7705->Label5.pCaption = "�Ѽ�⵽ AD7705";
		LCD_DrawLabel(&FormAD7705->Label5);
		
		TM7705_CalibSelf(1);		/* ��У׼��ִ��ʱ��ϳ���Լ180ms */
		adc1 = TM7705_ReadAdc(1);

		TM7705_CalibSelf(2);		/* ��У׼��ִ��ʱ��ϳ���Լ180ms */
		adc2 = TM7705_ReadAdc(2);
	}

	/* ����������ѭ���� */
	while (fQuit == 0)
	{
		bsp_Idle();

		//TM7705_Scan1();		/* ɨ��ADCͨ��1 */
		TM7705_Scan2();		/* ɨ��������ADCͨ��, �޵ȴ��� */

		{
			/* ��ȡɨ���� (�����ʱ��ȡ����) */
			adc1 = TM7705_GetAdc1();
			adc2 = TM7705_GetAdc2();
			
			/* ��ӡ�ɼ����� */
			{
				int volt1, volt2;
				char buf[64];

				/* ����ʵ�ʵ�ѹֵ�����ƹ���ģ�������׼ȷ�������У׼ */
				volt1 = (adc1 * 5000) / 65535;
				volt2 = (adc2 * 5000) / 65535;

				/* ��ʾADC������� */
				sprintf(buf, "%5d (%5dmV)", adc1, volt1);
				FormAD7705->Label2.pCaption = buf;
				LCD_DrawLabel(&FormAD7705->Label2);

				sprintf(buf, "%5d (%5dmV)", adc2, volt2);
				FormAD7705->Label4.pCaption = buf;
				LCD_DrawLabel(&FormAD7705->Label4);
			}
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					if (TOUCH_InRect(tpX, tpY, BTN_RET_X, BTN_RET_Y, BTN_RET_H, BTN_RET_W))
					{
						FormAD7705->BtnRet.Focus = 1;
						LCD_DrawButton(&FormAD7705->BtnRet);
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (TOUCH_InRect(tpX, tpY, BTN_RET_X, BTN_RET_Y, BTN_RET_H, BTN_RET_W))
					{
						FormAD7705->BtnRet.Focus = 0;
						LCD_DrawButton(&FormAD7705->BtnRet);
						fQuit = 1;	/* ���� */
					}
					else	/* ��ťʧȥ���� */
					{
						FormAD7705->BtnRet.Focus = 0;
						LCD_DrawButton(&FormAD7705->BtnRet);
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
				case KEY_DOWN_K1:		/* K1�� */
					break;

				case KEY_DOWN_K2:		/* K2������ */
					break;

				case KEY_DOWN_K3:		/* K3������ */
					break;

				case JOY_DOWN_U:		/* ҡ��UP������ */
					break;

				case JOY_DOWN_D:		/* ҡ��DOWN������ */
					break;

				case JOY_DOWN_L:		/* ҡ��LEFT������ */
					break;

				case JOY_DOWN_R:		/* ҡ��RIGHT������ */
					break;

				case JOY_DOWN_OK:		/* ҡ��OK������ */
					break;

				default:
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: InitFormAD7705
*	����˵��: ��ʼ���ؼ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormAD7705(void)
{
	/* ������������ */
	FormAD7705->FontBox.FontCode = FC_ST_16;
	FormAD7705->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	FormAD7705->FontBox.FrontColor = CL_BLACK;
	FormAD7705->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	FormAD7705->FontBlack.FontCode = FC_ST_16;
	FormAD7705->FontBlack.BackColor = CL_BTN_FACE;		/* ͸��ɫ */
	FormAD7705->FontBlack.FrontColor = CL_BLACK;
	FormAD7705->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	FormAD7705->FontBlue.FontCode = FC_ST_16;
	FormAD7705->FontBlue.BackColor = CL_BTN_FACE;
	FormAD7705->FontBlue.FrontColor = CL_BLUE;
	FormAD7705->FontBlue.Space = 0;

	/* ��ť���� */
	FormAD7705->FontBtn.FontCode = FC_ST_16;
	FormAD7705->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormAD7705->FontBtn.FrontColor = CL_BLACK;
	FormAD7705->FontBtn.Space = 0;

	/* ����� */
	FormAD7705->Box1.Left = BOX1_X;
	FormAD7705->Box1.Top = BOX1_Y;
	FormAD7705->Box1.Height = BOX1_H;
	FormAD7705->Box1.Width = BOX1_W;
	FormAD7705->Box1.pCaption = BOX1_TEXT;
	FormAD7705->Box1.Font = &FormAD7705->FontBox;

	/* ��̬��ǩ */
	FormAD7705->Label1.Left = LABEL1_X;
	FormAD7705->Label1.Top = LABEL1_Y;
	FormAD7705->Label1.MaxLen = 0;
	FormAD7705->Label1.pCaption = LABEL1_TEXT;
	FormAD7705->Label1.Font = &FormAD7705->FontBlack;

	FormAD7705->Label3.Left = LABEL3_X;
	FormAD7705->Label3.Top = LABEL3_Y;
	FormAD7705->Label3.MaxLen = 0;
	FormAD7705->Label3.pCaption = LABEL3_TEXT;
	FormAD7705->Label3.Font = &FormAD7705->FontBlack;

	FormAD7705->Label5.Left = LABEL5_X;
	FormAD7705->Label5.Top = LABEL5_Y;
	FormAD7705->Label5.MaxLen = 0;
	FormAD7705->Label5.pCaption = LABEL5_TEXT;
	FormAD7705->Label5.Font = &FormAD7705->FontBlack;


	/* ��̬��ǩ */
	FormAD7705->Label2.Left = LABEL2_X;
	FormAD7705->Label2.Top = LABEL2_Y;
	FormAD7705->Label2.MaxLen = 0;
	FormAD7705->Label2.pCaption = LABEL2_TEXT;
	FormAD7705->Label2.Font = &FormAD7705->FontBlue;

	FormAD7705->Label4.Left = LABEL4_X;
	FormAD7705->Label4.Top = LABEL4_Y;
	FormAD7705->Label4.MaxLen = 0;
	FormAD7705->Label4.pCaption = LABEL4_TEXT;
	FormAD7705->Label4.Font = &FormAD7705->FontBlue;

	/* ��ť */
	FormAD7705->BtnRet.Left = BTN_RET_X;
	FormAD7705->BtnRet.Top = BTN_RET_Y;
	FormAD7705->BtnRet.Height = BTN_RET_H;
	FormAD7705->BtnRet.Width = BTN_RET_W;
	FormAD7705->BtnRet.pCaption = BTN_RET_TEXT;
	FormAD7705->BtnRet.Font = &FormAD7705->FontBtn;
	FormAD7705->BtnRet.Focus = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: DispFormAD7705
*	����˵��: ��ʾ���еĿؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFormAD7705(void)
{
	LCD_ClrScr(CL_BTN_FACE);

	/* ����� */
	LCD_DrawGroupBox(&FormAD7705->Box1);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormAD7705->Label1);
	LCD_DrawLabel(&FormAD7705->Label3);
	LCD_DrawLabel(&FormAD7705->Label5);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormAD7705->Label2);
	LCD_DrawLabel(&FormAD7705->Label4);

	/* ��ť */
	LCD_DrawButton(&FormAD7705->BtnRet);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
