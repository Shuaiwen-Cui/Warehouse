/*
*********************************************************************************************************
*
*	ģ������ : ����SIM800 GPRSģ��
*	�ļ����� : gprs_test.c
*	��    �� : V1.0
*	˵    �� : ����GPRSģ�� SIM800
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2015-08-01 armfly  �׷�
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "form_gprs.h"
#include "num_pad.h"

/* �������ṹ */
typedef struct
{
	FONT_T FontBlack;	/* ��̬������ */
	FONT_T FontBlue;	/* �仯���������� */
	FONT_T FontRed;
	FONT_T FontBtn;		/* ��ť������ */
	FONT_T FontBox;		/* ������������ */

	GROUP_T Box1;

	LABEL_T Label1;	LABEL_T Label2;
	LABEL_T Label3; LABEL_T Label4;
	LABEL_T Label5; LABEL_T Label6;
	LABEL_T Label7; LABEL_T Label8;

	LABEL_T Label9;

	EDIT_T Edit1;		/* �绰���� */

	BUTTON_T Btn1;		/* ���� */
	BUTTON_T Btn2;		/* �һ� */
	BUTTON_T Btn3;		/* �������� */
	BUTTON_T Btn4;		/* ����10086 */
	BUTTON_T Btn5;		/* ����10010 */

	BUTTON_T BtnRet;	/* ���� */

	char strHardInfo[32];		/* ģ��Ӳ����Ϣ */
	uint8_t ucNetStatus;	/* ����״̬ */
	uint8_t ucAudioCh;		/* ��ǰ��Ƶͨ�� 0�� 1 */
	uint8_t ucEarVolume;	/* �������� 0 - 5 */
	int16_t ucMicGain;		/* MIC����  -12����С����  12���������  13������*/

}FormGPRS_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* �������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	(g_LcdHeight - BOX1_Y - 10)
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_TEXT	"GPRSģ����Գ���."

/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	60
#define	BTN_RET_X	((BOX1_X + BOX1_W) - BTN_RET_W - 4)
#define	BTN_RET_Y	((BOX1_Y  + BOX1_H) - BTN_RET_H - 4)
#define	BTN_RET_TEXT	"����"

#define LABEL1_X  	(BOX1_X + 6)
#define LABEL1_Y	(BOX1_Y + 20)
#define LABEL1_TEXT	"ģ��汾 : "

	#define LABEL2_X  	(LABEL1_X + 100)
	#define LABEL2_Y	LABEL1_Y
	#define LABEL2_TEXT	"---"

#define LABEL3_X  	(LABEL1_X)
#define LABEL3_Y	(LABEL1_Y + 20)
#define LABEL3_TEXT	"����״̬ : "

	#define LABEL4_X  	(LABEL3_X + 100)
	#define LABEL4_Y	(LABEL3_Y)
	#define LABEL4_TEXT	"---"

#define LABEL5_X  	(LABEL1_X)
#define LABEL5_Y	(LABEL1_Y + 20 * 2)
#define LABEL5_TEXT	"��Ƶͨ�� : "

	#define LABEL6_X  	(LABEL5_X + 100)
	#define LABEL6_Y	LABEL5_Y
	#define LABEL6_TEXT	"1"

#define LABEL7_X  	(LABEL1_X)
#define LABEL7_Y	(LABEL1_Y + 20 * 3)
#define LABEL7_TEXT	"����������MIC���� : "

	#define LABEL8_X  	(LABEL7_X + 160)
	#define LABEL8_Y	LABEL7_Y
	#define LABEL8_TEXT	"---"

#define LABEL9_X  	(LABEL1_X)
#define LABEL9_Y	(LABEL1_Y + 20 * 5)
#define LABEL9_TEXT	"�绰���� : "

/* Edit */
#define	EDIT1_X		(LABEL9_X + 90)
#define	EDIT1_Y	 	(LABEL9_Y - 4)
#define EDIT1_H		26
#define EDIT1_W		132

/* ��ť */
#define BTN1_H	32
#define BTN1_W	60
#define	BTN1_X	LABEL1_X
#define	BTN1_Y	(EDIT1_Y + EDIT1_H + 10)
#define	BTN1_TEXT	"����"

#define BTN2_H	32
#define BTN2_W	60
#define	BTN2_X	(BTN1_X +  BTN1_W + 10)
#define	BTN2_Y	BTN1_Y
#define	BTN2_TEXT	"�һ�"

#define BTN3_H	32
#define BTN3_W	90
#define	BTN3_X	(BTN1_X +  2 * (BTN1_W + 10))
#define	BTN3_Y	BTN1_Y
#define	BTN3_TEXT	"��������"

#define BTN4_H	32
#define BTN4_W	124
#define	BTN4_X	BTN1_X
#define	BTN4_Y	(BTN1_Y + BTN1_H + 10)
#define	BTN4_TEXT	"����10086"

#define BTN5_H	32
#define BTN5_W	124
#define	BTN5_X	BTN4_X + BTN4_W + 10
#define	BTN5_Y	(BTN1_Y + BTN1_H + 10)
#define	BTN5_TEXT	"����10010"

static void InitFormGPRS(void);
static void DispFormGPRS(void);

FormGPRS_T *FormGPRS;

void TestGPRS_SIM800(void);
void TestGPRS_MG323(void);

/*
*********************************************************************************************************
*	�� �� ��: TestGPRS
*	����˵��: ���Ի�ΪGPRSģ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TestGPRS(void)
{
	TestGPRS_SIM800();
}

/*
*********************************************************************************************************
*	�� �� ��: TestGPRS_SIM800
*	����˵��: ����SIM800 GPRSģ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TestGPRS_SIM800(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fQuit = 0;
	int16_t tpX, tpY;
	uint8_t ucValue;
	uint8_t fRefresh = 1;
	FormGPRS_T form;

	FormGPRS = &form;

	LCD_ClrScr(CL_BTN_FACE);

	InitFormGPRS();

	DispFormGPRS();

	bsp_InitSIM800();

	printf("���ڸ�GPRSģ���ϵ�...\r\n");

	/* ��GSMģ���ϵ� */
	if (SIM800_PowerOn())
	{
		printf("\r\n�ϵ����\r\n");
	}
	else
	{
		printf("\r\nģ����Ӧ��\r\n");
	}

	{
		SIM800_INFO_T tInfo;

		if (SIM800_GetHardInfo(&tInfo))
		{
			FormGPRS->Label2.Font = &FormGPRS->FontBlue;
			sprintf(FormGPRS->strHardInfo, "%s %s %s", tInfo.Manfacture, tInfo.Model, tInfo.Revision);
		}
		else
		{
			FormGPRS->Label2.Font = &FormGPRS->FontRed;
			sprintf(FormGPRS->strHardInfo, "δ��⵽ģ��");
		}
		FormGPRS->Label2.pCaption = FormGPRS->strHardInfo;
		LCD_DrawLabel(&FormGPRS->Label2);
	}

	//SIM800_SetAutoReport();		/* �����¼��Զ��ϱ� */

	SIM800_SetMicGain(0, FormGPRS->ucMicGain);	/* ����MIC���� */
	SIM800_SetEarVolume(FormGPRS->ucEarVolume);	/* ���ö������� */

	bsp_DelayMS(100);
	SIM800_SendAT("AT+CMICBIAS=0");	/* ��MICƫ�õ�· */
	
	bsp_StartAutoTimer(0, 500);
	/* ����������ѭ���� */
	while (fQuit == 0)
	{
		bsp_Idle();

		/* ��MG323�յ������ݷ��͵�����1 */
		if (comGetChar(COM_SIM800, &ucValue))
		{
			comSendChar(COM1, ucValue);
			continue;
		}
		/* ������1�����ݷ��͵�MG323ģ�� */
		if (comGetChar(COM1, &ucValue))
		{
			comSendChar(COM_SIM800, ucValue);
			continue;
		}

		/* ÿ��0.5���ѯһ������ע��״̬ */
		if (bsp_CheckTimer(0))
		{
			FormGPRS->ucNetStatus = SIM800_GetNetStatus();

			if ((FormGPRS->ucNetStatus == CREG_LOCAL_OK) || (FormGPRS->ucNetStatus == CREG_REMOTE_OK))
			{
				bsp_StopTimer(0);	/* ֹͣ�Զ�ˢ�� */

				/* ����WM8978оƬ������ΪLINE�ӿڣ����Ϊ���� �� ������ */
				wm8978_CfgAudioPath(LINE_ON, EAR_LEFT_ON | EAR_RIGHT_ON | SPK_ON);
			}			
			else if (FormGPRS->ucNetStatus == CREG_NO_REG)
			{
				bsp_StopTimer(0);	/* ֹͣ�Զ�ˢ�� */
			}
			fRefresh = 1;
		}

		if (fRefresh)
		{
			fRefresh = 0;

			DispFormGPRS();	/* ˢ�����пؼ� */
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */			
					LCD_ButtonTouchDown(&FormGPRS->BtnRet, tpX, tpY);
					LCD_ButtonTouchDown(&FormGPRS->Btn1, tpX, tpY);
					LCD_ButtonTouchDown(&FormGPRS->Btn2, tpX, tpY);
					LCD_ButtonTouchDown(&FormGPRS->Btn3, tpX, tpY);
					LCD_ButtonTouchDown(&FormGPRS->Btn4, tpX, tpY);
					LCD_ButtonTouchDown(&FormGPRS->Btn5, tpX, tpY);				

					/* �༭�� */
					if (TOUCH_InRect(tpX, tpY, EDIT1_X, EDIT1_Y, EDIT1_H, EDIT1_W))
					{
						{
							if (InputNumber(NUMPAD_TEL, "���õ绰����", 0, (void *)FormGPRS->Edit1.Text))
							{
								;
							}

							ClearWinNumPad(FORM_BACK_COLOR);	/* ������ּ��̴��� */
							fRefresh = 1;
						}
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (LCD_ButtonTouchRelease(&FormGPRS->BtnRet, tpX, tpY))
					{
						fQuit = 1;	/* ���� */
					}
					else if (LCD_ButtonTouchRelease(&FormGPRS->Btn1, tpX, tpY))	/* ���� */
					{
						/* ����绰 */
						SIM800_DialTel(FormGPRS->Edit1.Text);
					}
					else if (LCD_ButtonTouchRelease(&FormGPRS->Btn2, tpX, tpY))
					{
						/* �һ� */
						SIM800_Hangup();
					}
					else if (LCD_ButtonTouchRelease(&FormGPRS->Btn3, tpX, tpY))	/* �������� */
					{
						SIM800_SendAT("ATA");
					}
					else if (LCD_ButtonTouchRelease(&FormGPRS->Btn4, tpX, tpY))	/* ����10086 */
					{
						SIM800_DialTel("10086");
					}
					else if (LCD_ButtonTouchRelease(&FormGPRS->Btn5, tpX, tpY))	/* ����10010 */
					{
						SIM800_DialTel("10010");
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
					SIM800_SendAT("ATD10086;");	/* ����10086 */
					break;

				case KEY_DOWN_K2:		/* K2������ */
					SIM800_SendAT("ATH");	/* �Ҷϵ绰 */
					break;

				case KEY_DOWN_K3:		/* K3������ */
					fRefresh = 1;
					break;

				case JOY_DOWN_U:		/* ҡ��UP������  ���ڶ������� */
					if (FormGPRS->ucEarVolume < EAR_VOL_MAX)
					{
						FormGPRS->ucEarVolume++;
						SIM800_SetEarVolume(FormGPRS->ucEarVolume);
					}
					fRefresh = 1;
					break;

				case JOY_DOWN_D:		/* ҡ��DOWN������  ���ڶ������� */
					if (FormGPRS->ucEarVolume > EAR_VOL_MIN)
					{
						FormGPRS->ucEarVolume--;
						SIM800_SetEarVolume(FormGPRS->ucEarVolume);
					}
					fRefresh = 1;
					break;

				case JOY_DOWN_L:		/* ҡ��LEFT������  ����MIC���� */
					if (FormGPRS->ucMicGain > MIC_GAIN_MIN)
					{
						FormGPRS->ucMicGain--;
						SIM800_SetMicGain(0, FormGPRS->ucMicGain);
					}
					fRefresh = 1;
					break;

				case JOY_DOWN_R:		/* ҡ��RIGHT������  ����MIC���� */
					if (FormGPRS->ucMicGain < MIC_GAIN_MAX)
					{
						FormGPRS->ucMicGain++;
						SIM800_SetMicGain(0, FormGPRS->ucMicGain);
					}
					fRefresh = 1;
					break;

				case JOY_DOWN_OK:		/* ҡ��OK������ */
					break;

				default:
					break;
			}
		}
	}

	/* �ر�WM8978оƬ */
	wm8978_PowerDown();

	SIM800_PowerOff();	/* �µ� */
	printf("GPRSģ�����µ�.\r\n");
}

/*
*********************************************************************************************************
*	�� �� ��: InitFormGPRS
*	����˵��: ��ʼ���ؼ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormGPRS(void)
{
	/* ������������ */
	FormGPRS->FontBox.FontCode = FC_ST_16;
	FormGPRS->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	FormGPRS->FontBox.FrontColor = CL_BLACK;
	FormGPRS->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	FormGPRS->FontBlack.FontCode = FC_ST_16;
	FormGPRS->FontBlack.BackColor = CL_MASK;		/* ͸��ɫ */
	FormGPRS->FontBlack.FrontColor = CL_BLACK;
	FormGPRS->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	FormGPRS->FontBlue.FontCode = FC_ST_16;
	FormGPRS->FontBlue.BackColor = CL_BTN_FACE;
	FormGPRS->FontBlue.FrontColor = CL_BLUE;
	FormGPRS->FontBlue.Space = 0;

	FormGPRS->FontRed.FontCode = FC_ST_16;
	FormGPRS->FontRed.BackColor = CL_BTN_FACE;
	FormGPRS->FontRed.FrontColor = CL_RED;
	FormGPRS->FontRed.Space = 0;

	/* ��ť���� */
	FormGPRS->FontBtn.FontCode = FC_ST_16;
	FormGPRS->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormGPRS->FontBtn.FrontColor = CL_BLACK;
	FormGPRS->FontBtn.Space = 0;

	/* ����� */
	FormGPRS->Box1.Left = BOX1_X;
	FormGPRS->Box1.Top = BOX1_Y;
	FormGPRS->Box1.Height = BOX1_H;
	FormGPRS->Box1.Width = BOX1_W;
	FormGPRS->Box1.pCaption = BOX1_TEXT;
	FormGPRS->Box1.Font = &FormGPRS->FontBox;

	/* ��̬��ǩ */
	FormGPRS->Label1.Left = LABEL1_X;
	FormGPRS->Label1.Top = LABEL1_Y;
	FormGPRS->Label1.MaxLen = 0;
	FormGPRS->Label1.pCaption = LABEL1_TEXT;
	FormGPRS->Label1.Font = &FormGPRS->FontBlack;

	FormGPRS->Label3.Left = LABEL3_X;
	FormGPRS->Label3.Top = LABEL3_Y;
	FormGPRS->Label3.MaxLen = 0;
	FormGPRS->Label3.pCaption = LABEL3_TEXT;
	FormGPRS->Label3.Font = &FormGPRS->FontBlack;

	FormGPRS->Label5.Left = LABEL5_X;
	FormGPRS->Label5.Top = LABEL5_Y;
	FormGPRS->Label5.MaxLen = 0;
	FormGPRS->Label5.pCaption = LABEL5_TEXT;
	FormGPRS->Label5.Font = &FormGPRS->FontBlack;

	FormGPRS->Label7.Left = LABEL7_X;
	FormGPRS->Label7.Top = LABEL7_Y;
	FormGPRS->Label7.MaxLen = 0;
	FormGPRS->Label7.pCaption = LABEL7_TEXT;
	FormGPRS->Label7.Font = &FormGPRS->FontBlack;

	FormGPRS->Label9.Left = LABEL9_X;
	FormGPRS->Label9.Top = LABEL9_Y;
	FormGPRS->Label9.MaxLen = 0;
	FormGPRS->Label9.pCaption = LABEL9_TEXT;
	FormGPRS->Label9.Font = &FormGPRS->FontBlack;

	/* ��̬��ǩ */
	FormGPRS->Label2.Left = LABEL2_X;
	FormGPRS->Label2.Top = LABEL2_Y;
	FormGPRS->Label2.MaxLen = 0;
	FormGPRS->Label2.pCaption = LABEL2_TEXT;
	FormGPRS->Label2.Font = &FormGPRS->FontBlue;

	FormGPRS->Label4.Left = LABEL4_X;
	FormGPRS->Label4.Top = LABEL4_Y;
	FormGPRS->Label4.MaxLen = 0;
	FormGPRS->Label4.pCaption = LABEL4_TEXT;
	FormGPRS->Label4.Font = &FormGPRS->FontBlue;

	FormGPRS->Label6.Left = LABEL6_X;
	FormGPRS->Label6.Top = LABEL6_Y;
	FormGPRS->Label6.MaxLen = 0;
	FormGPRS->Label6.pCaption = LABEL6_TEXT;
	FormGPRS->Label6.Font = &FormGPRS->FontBlue;

	FormGPRS->Label8.Left = LABEL8_X;
	FormGPRS->Label8.Top = LABEL8_Y;
	FormGPRS->Label8.MaxLen = 0;
	FormGPRS->Label8.pCaption = LABEL8_TEXT;
	FormGPRS->Label8.Font = &FormGPRS->FontBlue;

	/* ��ť */
	FormGPRS->BtnRet.Left = BTN_RET_X;
	FormGPRS->BtnRet.Top = BTN_RET_Y;
	FormGPRS->BtnRet.Height = BTN_RET_H;
	FormGPRS->BtnRet.Width = BTN_RET_W;
	FormGPRS->BtnRet.pCaption = BTN_RET_TEXT;
	FormGPRS->BtnRet.Font = &FormGPRS->FontBtn;
	FormGPRS->BtnRet.Focus = 0;

	FormGPRS->Btn1.Left = BTN1_X;
	FormGPRS->Btn1.Top = BTN1_Y;
	FormGPRS->Btn1.Height = BTN1_H;
	FormGPRS->Btn1.Width = BTN1_W;
	FormGPRS->Btn1.pCaption = BTN1_TEXT;
	FormGPRS->Btn1.Font = &FormGPRS->FontBtn;
	FormGPRS->Btn1.Focus = 0;

	FormGPRS->Btn2.Left = BTN2_X;
	FormGPRS->Btn2.Top = BTN2_Y;
	FormGPRS->Btn2.Height = BTN2_H;
	FormGPRS->Btn2.Width = BTN2_W;
	FormGPRS->Btn2.pCaption = BTN2_TEXT;
	FormGPRS->Btn2.Font = &FormGPRS->FontBtn;
	FormGPRS->Btn2.Focus = 0;

	FormGPRS->Btn3.Left = BTN3_X;
	FormGPRS->Btn3.Top = BTN3_Y;
	FormGPRS->Btn3.Height = BTN3_H;
	FormGPRS->Btn3.Width = BTN3_W;
	FormGPRS->Btn3.pCaption = BTN3_TEXT;
	FormGPRS->Btn3.Font = &FormGPRS->FontBtn;
	FormGPRS->Btn3.Focus = 0;

	FormGPRS->Btn4.Left = BTN4_X;
	FormGPRS->Btn4.Top = BTN4_Y;
	FormGPRS->Btn4.Height = BTN4_H;
	FormGPRS->Btn4.Width = BTN4_W;
	FormGPRS->Btn4.pCaption = BTN4_TEXT;
	FormGPRS->Btn4.Font = &FormGPRS->FontBtn;
	FormGPRS->Btn4.Focus = 0;

	FormGPRS->Btn5.Left = BTN5_X;
	FormGPRS->Btn5.Top = BTN5_Y;
	FormGPRS->Btn5.Height = BTN5_H;
	FormGPRS->Btn5.Width = BTN5_W;
	FormGPRS->Btn5.pCaption = BTN5_TEXT;
	FormGPRS->Btn5.Font = &FormGPRS->FontBtn;
	FormGPRS->Btn5.Focus = 0;

	/* �༭�� */
	FormGPRS->Edit1.Left = EDIT1_X;
	FormGPRS->Edit1.Top = EDIT1_Y;
	FormGPRS->Edit1.Height = EDIT1_H;
	FormGPRS->Edit1.Width = EDIT1_W;
	sprintf(FormGPRS->Edit1.Text, "10086");
	FormGPRS->Edit1.pCaption = FormGPRS->Edit1.Text;
	FormGPRS->Edit1.Font = &FormGPRS->FontBtn;

	FormGPRS->ucMicGain = MIC_GAIN_DEFAULT;		/* ȱʡMIC���� */
	FormGPRS->ucEarVolume = EAR_VOL_DEFAULT;	/* ȱʡ�������� */
}

/*
*********************************************************************************************************
*	�� �� ��: DispFormGPRS
*	����˵��: ��ʾ���еĿؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFormGPRS(void)
{
	//LCD_ClrScr(CL_BTN_FACE);

	/* ����� */
	LCD_DrawGroupBox(&FormGPRS->Box1);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormGPRS->Label1);
	LCD_DrawLabel(&FormGPRS->Label3);
	LCD_DrawLabel(&FormGPRS->Label5);
	LCD_DrawLabel(&FormGPRS->Label7);
	LCD_DrawLabel(&FormGPRS->Label9);

	/* ��ť */
	LCD_DrawButton(&FormGPRS->BtnRet);
	LCD_DrawButton(&FormGPRS->Btn1);
	LCD_DrawButton(&FormGPRS->Btn2);
	LCD_DrawButton(&FormGPRS->Btn3);
	LCD_DrawButton(&FormGPRS->Btn4);
	LCD_DrawButton(&FormGPRS->Btn5);	

	/* �༭�� */
	LCD_DrawEdit(&FormGPRS->Edit1);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormGPRS->Label2);

	/* ����ע��״̬ */
	if (FormGPRS->ucNetStatus == CREG_NO_REG)
	{
		FormGPRS->Label4.pCaption = "û��ע��, ��ֹͣ��Ѱ";
	}
	else if (FormGPRS->ucNetStatus == CREG_LOCAL_OK)
	{
		FormGPRS->Label4.pCaption = "��ע�ᵽ��������      ";
	}
	else if (FormGPRS->ucNetStatus == CREG_SEARCH)
	{
		FormGPRS->Label4.pCaption = "������Ѱ������Ӫ��  ";
	}
	else if (FormGPRS->ucNetStatus == CREG_REJECT)
	{
		FormGPRS->Label4.pCaption = "ע�ᱻ�ܾ�          ";
	}
	else if (FormGPRS->ucNetStatus == CREG_REMOTE_OK)
	{
		FormGPRS->Label4.pCaption = "��ע�ᵽ��������     ";
	}
	else
	{
		FormGPRS->Label4.pCaption = "xxx                 ";
	}
	LCD_DrawLabel(&FormGPRS->Label4);

	if (FormGPRS->ucAudioCh == 0)
	{
		FormGPRS->Label6.pCaption = "��1ͨ�� INTMIC, INTEAR";
	}
	else
	{
		FormGPRS->Label6.pCaption = "��2ͨ�� EXTMIC, EXTEAR";
	}
	LCD_DrawLabel(&FormGPRS->Label6);

	/* �������� */
	{
		char buf[64];

		sprintf(buf, "Ear = %d, Mic = %d", FormGPRS->ucEarVolume, FormGPRS->ucMicGain);
		FormGPRS->Label8.pCaption = buf;
		LCD_DrawLabel(&FormGPRS->Label8);
	}
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
