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
}FormAD_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* �������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	(g_LcdHeight - BOX1_Y - 10)
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_TEXT	"AD7606���ݲɼ�ģ����Գ���"

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
#define LabelS1_TEXT	"K1���л�����, ҡ�����¼�ѡ�����������"

static void InitFormAD(void);
static void DispADInitFace(void);
static void DispADStatus(void);

FormAD_T *FormAD;

/*
*********************************************************************************************************
*	�� �� ��: TestAD7606
*	����˵��: ����AD7606ģ�顣
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TestAD7606(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	int16_t tpX, tpY;
	FormAD_T form;
	uint8_t fRefresh;

	FormAD = &form;

	InitFormAD();

	DispADInitFace();

	/* ����������ѭ���� */
	bsp_StartAutoTimer(0, 500);		/* ÿ0.5�붨ʱ��ʾ״̬ */

	bsp_InitAD7606();				/* ����AD7606���GPIO */
	AD7606_StartRecord(1000);		/* �����Զ��ɼ�ģʽ������Ƶ��1KHz�����ݴ����ȫ��FIFO */

	fRefresh = 1;
	while (g_MainStatus == MS_AD7606)
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
						FormAD->BtnRet.Focus = 1;
						LCD_DrawButton(&FormAD->BtnRet);
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (TOUCH_InRect(tpX, tpY, BTN_RET_X, BTN_RET_Y, BTN_RET_H, BTN_RET_W))
					{
						FormAD->BtnRet.Focus = 0;
						LCD_DrawButton(&FormAD->BtnRet);
						g_MainStatus = MS_MAIN_MENU;
					}
					else	/* ��ťʧȥ���� */
					{
						FormAD->BtnRet.Focus = 0;
						LCD_DrawButton(&FormAD->BtnRet);
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
					if (g_tAD7606.ucRange == 0)
					{
						AD7606_SetInputRange(1);
					}
					else
					{
						AD7606_SetInputRange(0);
					}
					fRefresh = 1;
					break;

				case KEY_DOWN_K2:			/* K2������ */
					break;

				case KEY_DOWN_K3:			/* K3������ */
					break;

				case JOY_DOWN_U:			/* ҡ��UP������ */
					if (g_tAD7606.ucOS < 6)
					{
						g_tAD7606.ucOS++;
					}
					AD7606_SetOS(g_tAD7606.ucOS);
					fRefresh = 1;
					break;

				case JOY_DOWN_D:			/* ҡ��DOWN������ */
					if (g_tAD7606.ucOS > 0)
					{
						g_tAD7606.ucOS--;
					}
					AD7606_SetOS(g_tAD7606.ucOS);
					fRefresh = 1;
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

	bsp_StopTimer(0);		/* ֹͣ�����ʱ�� */
	AD7606_StopRecord();	/* ֹͣ�Զ��ɼ� */
}

/*
*********************************************************************************************************
*	�� �� ��: DispADStatus
*	����˵��: ��ʾAD7606״̬
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispADStatus(void)
{
	char buf[128];
	uint8_t i;
	int32_t volt;
	uint8_t m;

	if (g_tAD7606.ucRange == 1)
	{
		FormAD->LabelV[8].pCaption = "��10V";
		m = 2;
	}
	else
	{
		FormAD->LabelV[8].pCaption = "��5V ";
		m = 1;
	}
	LCD_DrawLabel(&FormAD->LabelV[8]);
	
	for (i = 0; i < 8; i++)
	{
		/*  ʵ�� 21508 = 3.300V; �����в��� */
		volt = m * g_tAD7606.sNowAdc[i] * 3300 / 21508;
		sprintf(buf, "%6d  %6dmV  ", g_tAD7606.sNowAdc[i], volt);
		FormAD->LabelV[i].pCaption = buf;
		LCD_DrawLabel(&FormAD->LabelV[i]);
	}



	sprintf(buf,  "%d", g_tAD7606.ucOS);
	FormAD->LabelV[9].pCaption = buf;
	LCD_DrawLabel(&FormAD->LabelV[9]);
}

/*
*********************************************************************************************************
*	�� �� ��: InitFormAD
*	����˵��: ��ʼ��GPS��ʼ����ؼ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormAD(void)
{
	/* ������������ */
	FormAD->FontBox.FontCode = FC_ST_16;
	FormAD->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	FormAD->FontBox.FrontColor = CL_BLACK;
	FormAD->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	FormAD->FontBlack.FontCode = FC_ST_16;
	FormAD->FontBlack.BackColor = CL_MASK;		/* ͸��ɫ */
	FormAD->FontBlack.FrontColor = CL_BLACK;
	FormAD->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	FormAD->FontBlue.FontCode = FC_ST_16;
	FormAD->FontBlue.BackColor = CL_BTN_FACE;
	FormAD->FontBlue.FrontColor = CL_BLUE;
	FormAD->FontBlue.Space = 0;

	/* ��ť���� */
	FormAD->FontBtn.FontCode = FC_ST_16;
	FormAD->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormAD->FontBtn.FrontColor = CL_BLACK;
	FormAD->FontBtn.Space = 0;

	/* ����� */
	FormAD->Box1.Left = BOX1_X;
	FormAD->Box1.Top = BOX1_Y;
	FormAD->Box1.Height = BOX1_H;
	FormAD->Box1.Width = BOX1_W;
	FormAD->Box1.pCaption = BOX1_TEXT;
	FormAD->Box1.Font = &FormAD->FontBox;

	/* ��ǩ */
	{
		uint8_t i;
		const char *strName[10] = {"ͨ��1","ͨ��2","ͨ��3","ͨ��4","ͨ��5","ͨ��6","ͨ��7","ͨ��8", "����", "������"};

		for (i = 0; i < 10; i++)
		{
			FormAD->LabelN[i].Left = LabelN1_X;
			FormAD->LabelN[i].Top = LabelN1_Y + i * 20;
			FormAD->LabelN[i].MaxLen = 0;
			FormAD->LabelN[i].pCaption = (char *)strName[i];
			FormAD->LabelN[i].Font = &FormAD->FontBlack;		/* ��ɫ */

			FormAD->LabelV[i].Left = LabelN1_X + 60;
			FormAD->LabelV[i].Top = LabelN1_Y + i * 20;		/* ��ɫ */
			FormAD->LabelV[i].MaxLen = 0;
			FormAD->LabelV[i].pCaption = "-";
			FormAD->LabelV[i].Font = &FormAD->FontBlue;
		}

		FormAD->LabelS1.Left = LabelS1_X;
		FormAD->LabelS1.Top = LabelS1_Y;
		FormAD->LabelS1.MaxLen = 0;
		FormAD->LabelS1.pCaption = LabelS1_TEXT;
		FormAD->LabelS1.Font = &FormAD->FontBlack;		/* ��ɫ */
	}

	/* ��ť */
	FormAD->BtnRet.Left = BTN_RET_X;
	FormAD->BtnRet.Top = BTN_RET_Y;
	FormAD->BtnRet.Height = BTN_RET_H;
	FormAD->BtnRet.Width = BTN_RET_W;
	FormAD->BtnRet.pCaption = BTN_RET_TEXT;
	FormAD->BtnRet.Font = &FormAD->FontBtn;
	FormAD->BtnRet.Focus = 0;
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
	LCD_DrawGroupBox(&FormAD->Box1);

	/* ��ǩ */
	{
		uint8_t i;

		for (i = 0; i < 10; i++)
		{
			LCD_DrawLabel(&FormAD->LabelN[i]);
		}

		LCD_DrawLabel(&FormAD->LabelS1);
	}

	/* ��ť */
	LCD_DrawButton(&FormAD->BtnRet);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
