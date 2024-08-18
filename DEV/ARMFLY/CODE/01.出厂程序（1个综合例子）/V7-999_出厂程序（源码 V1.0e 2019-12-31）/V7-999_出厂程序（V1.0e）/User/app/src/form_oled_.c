/*
*********************************************************************************************************
*
*	ģ������ : ����OLED��ʾ��
*	�ļ����� : oled_test.c
*	��    �� : V1.1
*	˵    �� : ����OLED��ʾ��ģ��
*	�޸ļ�¼ :
*		�汾��  ����       ����     ˵��
*		v1.0    2013-02-01 armfly  �׷�
*		v1.1    2015-10-14 armfly  ����24�����32�����ֺ�ASCII��ʾ����
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "form_oled.h"

#define DEMO_PAGE_COUNT		7	/* OLED��ʾҳ��ĸ��� */

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
}FormOLED_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* �������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	(g_LcdHeight - BOX1_Y - 10)
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_TEXT	"OLED��ʾģ����Գ���"

/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	60
#define	BTN_RET_X	((BOX1_X + BOX1_W) - BTN_RET_W - 4)
#define	BTN_RET_Y	((BOX1_Y  + BOX1_H) - BTN_RET_H - 4)
#define	BTN_RET_TEXT	"����"

#define LABEL1_X  	(BOX1_X + 6)
#define LABEL1_Y	(BOX1_Y + 20)
#define LABEL1_TEXT	"ҡ�����Ҽ�: "

	#define LABEL2_X  	(LABEL1_X + 135)
	#define LABEL2_Y	LABEL1_Y
	#define LABEL2_TEXT	"�л�OLED��ʾ����"

#define LABEL3_X  	(LABEL1_X)
#define LABEL3_Y	(LABEL1_Y + 20)
#define LABEL3_TEXT	"ҡ���ϡ��¼�: "

	#define LABEL4_X  	(LABEL3_X + 135)
	#define LABEL4_Y	(LABEL3_Y)
	#define LABEL4_TEXT	"����OLED�Աȶ�"

#define LABEL5_X  	(LABEL1_X)
#define LABEL5_Y	(LABEL1_Y + 20 * 2)
#define LABEL5_TEXT	"ҡ��OK��    : "

	#define LABEL6_X  	(LABEL5_X + 135)
	#define LABEL6_Y	LABEL5_Y
	#define LABEL6_TEXT	"�л���ʾ����"

#define LABEL7_X  	(LABEL1_X)
#define LABEL7_Y	(LABEL1_Y + 20 * 3)
#define LABEL7_TEXT	"��ǰ״̬    :"

	#define LABEL8_X  	(LABEL7_X + 135)
	#define LABEL8_Y	LABEL7_Y
	#define LABEL8_TEXT	"80"

static void InitFormOLED(void);
static void DispFormOLED(void);

FormOLED_T *FormOLED;

/*
*********************************************************************************************************
*	�� �� ��: TestOLED
*	����˵��: ����OLED��ʾģ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TestOLED(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fQuit = 0;
	int16_t tpX, tpY;
	FormOLED_T form;
	uint8_t fRefreshTFT;
	FONT_T tFont12, tFont16, tFont24, tFont32;
	uint8_t fRefreshOled;
	uint8_t ucItem;
	uint8_t ucContrast = 0x80;	/* �Աȶ� */
	uint8_t ucDir = 0;	/* ��ʾ����, 0 ��ʾ��������1��ʾ��180�� */

	FormOLED= &form;
	InitFormOLED();
	DispFormOLED();

	OLED_InitHard();	 /* ��ʼ��OLEDӲ�� */
	OLED_ClrScr(0x00);	 /* ������0x00��ʾ�ڵף� 0xFF ��ʾ�׵� */

	/* ����������� */
	{
		tFont16.FontCode = FC_ST_16;	/* ������� 16���� */
		tFont16.FrontColor = 1;		/* ������ɫ 0 �� 1 */
		tFont16.BackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
		tFont16.Space = 0;			/* ���ּ�࣬��λ = ���� */

		tFont12.FontCode = FC_ST_12;	/* ������� 12���� */
		tFont12.FrontColor = 1;		/* ������ɫ 0 �� 1 */
		tFont12.BackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
		tFont12.Space = 1;			/* ���ּ�࣬��λ = ���� */

		tFont24.FontCode = FC_ST_24;	/* ������� 24���� */
		tFont24.FrontColor = 1;		/* ������ɫ 0 �� 1 */
		tFont24.BackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
		tFont24.Space = 1;			/* ���ּ�࣬��λ = ���� */
		
		tFont32.FontCode = FC_ST_32;	/* ������� 32���� */
		tFont32.FrontColor = 1;		/* ������ɫ 0 �� 1 */
		tFont32.BackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
		tFont32.Space = 1;			/* ���ּ�࣬��λ = ���� */		
	}
	ucItem = 0;
	fRefreshOled = 1;
	fRefreshTFT = 1;
	/* ����������ѭ���� */
	while (fQuit == 0)
	{
		bsp_Idle();

		if (fRefreshTFT)
		{
			char buf[32];

			fRefreshTFT = 0;

			sprintf(buf, "��%dҳ  �Աȶ� = %3d", ucItem + 1, ucContrast);

			FormOLED->Label8.pCaption = buf;
			LCD_DrawLabel(&FormOLED->Label8);
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					if (TOUCH_InRect(tpX, tpY, BTN_RET_X, BTN_RET_Y, BTN_RET_H, BTN_RET_W))
					{
						FormOLED->BtnRet.Focus = 1;
						LCD_DrawButton(&FormOLED->BtnRet);
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (TOUCH_InRect(tpX, tpY, BTN_RET_X, BTN_RET_Y, BTN_RET_H, BTN_RET_W))
					{
						FormOLED->BtnRet.Focus = 0;
						LCD_DrawButton(&FormOLED->BtnRet);
						fQuit = 1;	/* ���� */
					}
					else	/* ��ťʧȥ���� */
					{
						FormOLED->BtnRet.Focus = 0;
						LCD_DrawButton(&FormOLED->BtnRet);
					}
					break;
			}
		}

		if (fRefreshOled == 1)
		{
			fRefreshOled = 0;

			switch (ucItem)
			{
				case 0:
					OLED_ClrScr(0);			 /* �������ڵ� */
					OLED_DrawRect(0, 0, 64, 128, 1);
					OLED_DispStr(8, 3, "������OLED����", &tFont16);		/* ��(8,3)���괦��ʾһ������ */
					OLED_DispStr(10, 22, "�����ҡ��!", &tFont16);
					OLED_DispStr(5, 22 + 20, "www.ARMfly.com", &tFont16);
					break;

				case 1:
					//OLED_StartDraw();	  ���øĺ�����ֻˢ�»�������������ʾ
					OLED_ClrScr(0);
					OLED_DispStr(0, 0,  "�������ǻƺ�¥��", &tFont16);
					OLED_DispStr(0, 16, "�̻����������ݡ�", &tFont16);
					OLED_DispStr(0, 32, "�·�ԶӰ�̿վ���", &tFont16);
					OLED_DispStr(0, 48, "Ψ�������������", &tFont16);
					//OLED_EndDraw();	  ���øĺ�����������������������ʾ
					break;

				case 2:
					OLED_ClrScr(0);
					OLED_DispStr(5, 0,  "�����Ϻ�Ȼ֮���꡷", &tFont12);
					OLED_DispStr(0, 13, "�������ǻƺ�¥��", &tFont12);
					OLED_DispStr(0, 26, "�̻����������ݡ�", &tFont12);
					OLED_DispStr(0, 39, "�·�ԶӰ�̿վ���", &tFont12);
					OLED_DispStr(0, 52, "Ψ�������������", &tFont12);

					OLED_DispStr(110, 14, "��", &tFont16);
					OLED_DispStr(110, 30, "��", &tFont16);
					OLED_DispStr(110, 46, "��", &tFont16);
					OLED_DrawRect(109,13, 50, 17,1);
					break;

				case 3:	
					OLED_ClrScr(0);
					OLED_DispStr(5, 0,  "������123", &tFont24);
					OLED_DispStr(0, 26, "������8", &tFont32);
					break;

				case 4:
					OLED_ClrScr(0);
					OLED_DrawRect(0,0, 10,10,1);	/* ��(0,0)���괦����һ����10��10�ľ��� */
					OLED_DrawRect(10,10, 20,30,1);	/* ��(10,10)���괦����һ����20��30�ľ��� */
					OLED_DrawCircle(64,32,30,1);	/* ��(64,32)���ư뾶30��Բ */
					OLED_DrawLine(127,0,0,63,1);	/* ��(127,0) �� (0,63) ֮�����һ��ֱ�� */
					break;

				case 5:
					OLED_ClrScr(0x00);				/* �������ڵ� */
					break;

				case 6:
					OLED_ClrScr(0xFF);				/* �������׵� */
					{
						//char buf[32];

						//sprintf(buf, "%d", ucContrast);
						//OLED_DispStr(10,10,buf,&tFont16);
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

				case JOY_DOWN_U:		/* ҡ���ϼ����� */
					if (ucContrast < 255)
					{
						ucContrast++;
					}
					OLED_SetContrast(ucContrast);
					fRefreshOled = 1;
					fRefreshTFT = 1;
					break;

				case JOY_DOWN_D:		/* ҡ���¼����� */
					if (ucContrast > 0)
					{
						ucContrast--;
					}
					OLED_SetContrast(ucContrast);
					fRefreshOled = 1;
					fRefreshTFT = 1;
					break;

				case JOY_DOWN_L:		/* ҡ��LEFT������ */
					if (ucItem > 0)
					{
						ucItem--;
					}
					else
					{
						ucItem = DEMO_PAGE_COUNT - 1;
					}
					fRefreshOled = 1;
					fRefreshTFT = 1;
					break;

				case JOY_DOWN_R:	/* ҡ��RIGHT������ */
					if (ucItem < DEMO_PAGE_COUNT - 1)
					{
						ucItem++;
					}
					else
					{
						ucItem = 0;
					}
					fRefreshOled = 1;
					fRefreshTFT = 1;
					break;

				case JOY_DOWN_OK:		/* ҡ��OK�� */
					if (ucDir == 0)
					{
						ucDir = 1;
						OLED_SetDir(1);	/* ������ʾ���� */
					}
					else
					{
						ucDir = 0;
						OLED_SetDir(0);	/* ������ʾ���� */
					}
					fRefreshOled = 1;
					break;

				default:
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: InitFormOLED
*	����˵��: ��ʼ���ؼ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormOLED(void)
{
	/* ������������ */
	FormOLED->FontBox.FontCode = FC_ST_16;
	FormOLED->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	FormOLED->FontBox.FrontColor = CL_BLACK;
	FormOLED->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	FormOLED->FontBlack.FontCode = FC_ST_16;
	FormOLED->FontBlack.BackColor = CL_MASK;		/* ͸��ɫ */
	FormOLED->FontBlack.FrontColor = CL_BLACK;
	FormOLED->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	FormOLED->FontBlue.FontCode = FC_ST_16;
	FormOLED->FontBlue.BackColor = CL_BTN_FACE;
	FormOLED->FontBlue.FrontColor = CL_BLUE;
	FormOLED->FontBlue.Space = 0;

	/* ��ť���� */
	FormOLED->FontBtn.FontCode = FC_ST_16;
	FormOLED->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormOLED->FontBtn.FrontColor = CL_BLACK;
	FormOLED->FontBtn.Space = 0;

	/* ����� */
	FormOLED->Box1.Left = BOX1_X;
	FormOLED->Box1.Top = BOX1_Y;
	FormOLED->Box1.Height = BOX1_H;
	FormOLED->Box1.Width = BOX1_W;
	FormOLED->Box1.pCaption = BOX1_TEXT;
	FormOLED->Box1.Font = &FormOLED->FontBox;

	/* ��̬��ǩ */
	FormOLED->Label1.Left = LABEL1_X;
	FormOLED->Label1.Top = LABEL1_Y;
	FormOLED->Label1.MaxLen = 0;
	FormOLED->Label1.pCaption = LABEL1_TEXT;
	FormOLED->Label1.Font = &FormOLED->FontBlack;

	FormOLED->Label3.Left = LABEL3_X;
	FormOLED->Label3.Top = LABEL3_Y;
	FormOLED->Label3.MaxLen = 0;
	FormOLED->Label3.pCaption = LABEL3_TEXT;
	FormOLED->Label3.Font = &FormOLED->FontBlack;

	FormOLED->Label5.Left = LABEL5_X;
	FormOLED->Label5.Top = LABEL5_Y;
	FormOLED->Label5.MaxLen = 0;
	FormOLED->Label5.pCaption = LABEL5_TEXT;
	FormOLED->Label5.Font = &FormOLED->FontBlack;

	FormOLED->Label7.Left = LABEL7_X;
	FormOLED->Label7.Top = LABEL7_Y;
	FormOLED->Label7.MaxLen = 0;
	FormOLED->Label7.pCaption = LABEL7_TEXT;
	FormOLED->Label7.Font = &FormOLED->FontBlack;

	/* ��̬��ǩ */
	FormOLED->Label2.Left = LABEL2_X;
	FormOLED->Label2.Top = LABEL2_Y;
	FormOLED->Label2.MaxLen = 0;
	FormOLED->Label2.pCaption = LABEL2_TEXT;
	FormOLED->Label2.Font = &FormOLED->FontBlue;

	FormOLED->Label4.Left = LABEL4_X;
	FormOLED->Label4.Top = LABEL4_Y;
	FormOLED->Label4.MaxLen = 0;
	FormOLED->Label4.pCaption = LABEL4_TEXT;
	FormOLED->Label4.Font = &FormOLED->FontBlue;

	FormOLED->Label6.Left = LABEL6_X;
	FormOLED->Label6.Top = LABEL6_Y;
	FormOLED->Label6.MaxLen = 0;
	FormOLED->Label6.pCaption = LABEL6_TEXT;
	FormOLED->Label6.Font = &FormOLED->FontBlue;

	FormOLED->Label8.Left = LABEL8_X;
	FormOLED->Label8.Top = LABEL8_Y;
	FormOLED->Label8.MaxLen = 0;
	FormOLED->Label8.pCaption = LABEL8_TEXT;
	FormOLED->Label8.Font = &FormOLED->FontBlue;

	/* ��ť */
	FormOLED->BtnRet.Left = BTN_RET_X;
	FormOLED->BtnRet.Top = BTN_RET_Y;
	FormOLED->BtnRet.Height = BTN_RET_H;
	FormOLED->BtnRet.Width = BTN_RET_W;
	FormOLED->BtnRet.pCaption = BTN_RET_TEXT;
	FormOLED->BtnRet.Font = &FormOLED->FontBtn;
	FormOLED->BtnRet.Focus = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: DispRSVInitFace
*	����˵��: ��ʾ���еľ�̬�ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFormOLED(void)
{
	LCD_ClrScr(CL_BTN_FACE);

	/* ����� */
	LCD_DrawGroupBox(&FormOLED->Box1);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormOLED->Label1);
	LCD_DrawLabel(&FormOLED->Label3);
	LCD_DrawLabel(&FormOLED->Label5);
	LCD_DrawLabel(&FormOLED->Label7);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormOLED->Label2);
	LCD_DrawLabel(&FormOLED->Label4);
	LCD_DrawLabel(&FormOLED->Label6);
	LCD_DrawLabel(&FormOLED->Label8);

	/* ��ť */
	LCD_DrawButton(&FormOLED->BtnRet);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
