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
#include "form_reserve.h"

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
	LABEL_T Label5; LABEL_T Label6;
	LABEL_T Label7; LABEL_T Label8;

	BUTTON_T BtnRet;
}FormRSV_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* �������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	(g_LcdHeight - BOX1_Y - 10)
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_TEXT	"���򿪷���..."

/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	60
#define	BTN_RET_X	((BOX1_X + BOX1_W) - BTN_RET_W - 4)
#define	BTN_RET_Y	((BOX1_Y  + BOX1_H) - BTN_RET_H - 4)
#define	BTN_RET_TEXT	"����"

#define LABEL1_X  	(BOX1_X + 6)
#define LABEL1_Y	(BOX1_Y + 20)
#define LABEL1_TEXT	"xxxx : "

	#define LABEL2_X  	(LABEL1_X + 64)
	#define LABEL2_Y	LABEL1_Y
	#define LABEL2_TEXT	"0000.0000"

#define LABEL3_X  	(LABEL1_X)
#define LABEL3_Y	(LABEL1_Y + 20)
#define LABEL3_TEXT	"xxxx : "

	#define LABEL4_X  	(LABEL3_X + 64)
	#define LABEL4_Y	(LABEL3_Y)
	#define LABEL4_TEXT	"00000.0000"

#define LABEL5_X  	(LABEL1_X)
#define LABEL5_Y	(LABEL1_Y + 20 * 2)
#define LABEL5_TEXT	"xxxx : "

	#define LABEL6_X  	(LABEL5_X + 64)
	#define LABEL6_Y	LABEL5_Y
	#define LABEL6_TEXT	"0.0"

#define LABEL7_X  	(LABEL1_X)
#define LABEL7_Y	(LABEL1_Y + 20 * 3)
#define LABEL7_TEXT	"xxxx : "

	#define LABEL8_X  	(LABEL7_X + 64)
	#define LABEL8_Y	LABEL7_Y
	#define LABEL8_TEXT	"0.0"

static void InitFormRSV(void);
static void DispFormRSV(void);

FormRSV_T *FormRSV;

/*
*********************************************************************************************************
*	�� �� ��: ReserveFunc
*	����˵��: �������ܡ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ReserveFunc(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fQuit = 0;
	int16_t tpX, tpY;
	FormRSV_T form;

	FormRSV= &form;

	InitFormRSV();

	DispFormRSV();

	/* ����������ѭ���� */
	while (fQuit == 0)
	{
		bsp_Idle();

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					if (TOUCH_InRect(tpX, tpY, BTN_RET_X, BTN_RET_Y, BTN_RET_H, BTN_RET_W))
					{
						FormRSV->BtnRet.Focus = 1;
						LCD_DrawButton(&FormRSV->BtnRet);
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (TOUCH_InRect(tpX, tpY, BTN_RET_X, BTN_RET_Y, BTN_RET_H, BTN_RET_W))
					{
						FormRSV->BtnRet.Focus = 0;
						LCD_DrawButton(&FormRSV->BtnRet);
						fQuit = 1;	/* ���� */
					}
					else	/* ��ťʧȥ���� */
					{
						FormRSV->BtnRet.Focus = 0;
						LCD_DrawButton(&FormRSV->BtnRet);
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
*	�� �� ��: InitFormRSV
*	����˵��: ��ʼ���ؼ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormRSV(void)
{
	/* ������������ */
	FormRSV->FontBox.FontCode = FC_ST_16;
	FormRSV->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	FormRSV->FontBox.FrontColor = CL_BLACK;
	FormRSV->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	FormRSV->FontBlack.FontCode = FC_ST_16;
	FormRSV->FontBlack.BackColor = CL_MASK;		/* ͸��ɫ */
	FormRSV->FontBlack.FrontColor = CL_BLACK;
	FormRSV->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	FormRSV->FontBlue.FontCode = FC_ST_16;
	FormRSV->FontBlue.BackColor = CL_BTN_FACE;
	FormRSV->FontBlue.FrontColor = CL_BLUE;
	FormRSV->FontBlue.Space = 0;

	/* ��ť���� */
	FormRSV->FontBtn.FontCode = FC_ST_16;
	FormRSV->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormRSV->FontBtn.FrontColor = CL_BLACK;
	FormRSV->FontBtn.Space = 0;

	/* ����� */
	FormRSV->Box1.Left = BOX1_X;
	FormRSV->Box1.Top = BOX1_Y;
	FormRSV->Box1.Height = BOX1_H;
	FormRSV->Box1.Width = BOX1_W;
	FormRSV->Box1.pCaption = BOX1_TEXT;
	FormRSV->Box1.Font = &FormRSV->FontBox;

	/* ��̬��ǩ */
	FormRSV->Label1.Left = LABEL1_X;
	FormRSV->Label1.Top = LABEL1_Y;
	FormRSV->Label1.MaxLen = 0;
	FormRSV->Label1.pCaption = LABEL1_TEXT;
	FormRSV->Label1.Font = &FormRSV->FontBlack;

	FormRSV->Label3.Left = LABEL3_X;
	FormRSV->Label3.Top = LABEL3_Y;
	FormRSV->Label3.MaxLen = 0;
	FormRSV->Label3.pCaption = LABEL3_TEXT;
	FormRSV->Label3.Font = &FormRSV->FontBlack;

	FormRSV->Label5.Left = LABEL5_X;
	FormRSV->Label5.Top = LABEL5_Y;
	FormRSV->Label5.MaxLen = 0;
	FormRSV->Label5.pCaption = LABEL5_TEXT;
	FormRSV->Label5.Font = &FormRSV->FontBlack;

	FormRSV->Label7.Left = LABEL7_X;
	FormRSV->Label7.Top = LABEL7_Y;
	FormRSV->Label7.MaxLen = 0;
	FormRSV->Label7.pCaption = LABEL7_TEXT;
	FormRSV->Label7.Font = &FormRSV->FontBlack;

	/* ��̬��ǩ */
	FormRSV->Label2.Left = LABEL2_X;
	FormRSV->Label2.Top = LABEL2_Y;
	FormRSV->Label2.MaxLen = 0;
	FormRSV->Label2.pCaption = LABEL2_TEXT;
	FormRSV->Label2.Font = &FormRSV->FontBlue;

	FormRSV->Label4.Left = LABEL4_X;
	FormRSV->Label4.Top = LABEL4_Y;
	FormRSV->Label4.MaxLen = 0;
	FormRSV->Label4.pCaption = LABEL4_TEXT;
	FormRSV->Label4.Font = &FormRSV->FontBlue;

	FormRSV->Label6.Left = LABEL6_X;
	FormRSV->Label6.Top = LABEL6_Y;
	FormRSV->Label6.MaxLen = 0;
	FormRSV->Label6.pCaption = LABEL6_TEXT;
	FormRSV->Label6.Font = &FormRSV->FontBlue;

	FormRSV->Label8.Left = LABEL8_X;
	FormRSV->Label8.Top = LABEL8_Y;
	FormRSV->Label8.MaxLen = 0;
	FormRSV->Label8.pCaption = LABEL8_TEXT;
	FormRSV->Label8.Font = &FormRSV->FontBlue;

	/* ��ť */
	FormRSV->BtnRet.Left = BTN_RET_X;
	FormRSV->BtnRet.Top = BTN_RET_Y;
	FormRSV->BtnRet.Height = BTN_RET_H;
	FormRSV->BtnRet.Width = BTN_RET_W;
	FormRSV->BtnRet.pCaption = BTN_RET_TEXT;
	FormRSV->BtnRet.Font = &FormRSV->FontBtn;
	FormRSV->BtnRet.Focus = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: DispFormRSV
*	����˵��: ��ʾ���еĿؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFormRSV(void)
{
	LCD_ClrScr(CL_BTN_FACE);

	/* ����� */
	LCD_DrawGroupBox(&FormRSV->Box1);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormRSV->Label1);
	LCD_DrawLabel(&FormRSV->Label3);
	LCD_DrawLabel(&FormRSV->Label5);
	LCD_DrawLabel(&FormRSV->Label7);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormRSV->Label2);
	LCD_DrawLabel(&FormRSV->Label4);
	LCD_DrawLabel(&FormRSV->Label6);
	LCD_DrawLabel(&FormRSV->Label8);

	/* ��ť */
	LCD_DrawButton(&FormRSV->BtnRet);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
