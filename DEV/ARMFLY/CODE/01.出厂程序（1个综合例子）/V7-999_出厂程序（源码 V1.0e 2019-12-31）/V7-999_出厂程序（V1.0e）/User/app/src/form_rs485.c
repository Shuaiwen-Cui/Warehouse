/*
*********************************************************************************************************
*
*	ģ������ : RS485���Խ���
*	�ļ����� : form_rs485.c
*	��    �� : V1.0
*	˵    �� : ����������LED-485ϵ���������ʾ����
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2014-10-15 armfly  �׷�
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "form_rs485.h"

/* �������ṹ */
typedef struct
{
	FONT_T FontBlack;	/* ��̬������ */
	FONT_T FontBlue;	/* �仯���������� */
	FONT_T FontBtn;		/* ��ť������ */
	FONT_T FontBox;		/* ������������ */

	GROUP_T Box1;
	GROUP_T Box2;
	GROUP_T Box3;

	LABEL_T Label1;	LABEL_T Label2;
	LABEL_T Label3; LABEL_T Label4;
	LABEL_T Label5; LABEL_T Label6;

	BUTTON_T BtnRet;

	BUTTON_T Btn1;
	BUTTON_T Btn2;
	BUTTON_T Btn3;
	BUTTON_T Btn4;
	BUTTON_T Btn5;
	BUTTON_T Btn6;

	BUTTON_T BtnBright[8];	/* �������� */

	LABEL_T LabelOldAddr;
	LABEL_T LabelNewAddr;
	EDIT_T EditOldAddr;
	EDIT_T EditNewAddr;
	BUTTON_T BtnSetAddr;

	LABEL_T LblInfo1; LABEL_T LblInfo2;
}FormRS485_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* �������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	100
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_TEXT	"RS485 LED�����"

	/* ��1�����ڵİ�ť */
	#define BTN1_H	32
	#define BTN1_W	100
	#define	BTN1_X	(BOX1_X + 10)
	#define	BTN1_Y	(BOX1_Y + 20)
	#define	BTN1_TEXT	"ASCIIЭ��"

	#define BTN2_H	BTN1_H
	#define BTN2_W	BTN1_W
	#define	BTN2_X	(BTN1_X +  BTN1_W + 10)
	#define	BTN2_Y	BTN1_Y
	#define	BTN2_TEXT	"ModbusЭ��"

	#define BTN3_H	BTN1_H
	#define BTN3_W	BTN1_W
	#define	BTN3_X	BTN1_X
	#define	BTN3_Y	(BTN1_Y + BTN1_H + 10)
	#define	BTN3_TEXT	"���豸�ͺ�"

	#define BTN4_H	BTN1_H
	#define BTN4_W	BTN1_W
	#define	BTN4_X	(BTN1_X +  BTN1_W + 10)
	#define	BTN4_Y	(BTN1_Y + BTN1_H + 10)
	#define	BTN4_TEXT	"���̼��汾"

	#define BTN5_H	BTN1_H
	#define BTN5_W	BTN1_W
	#define	BTN5_X	(BTN1_X +  2 * (BTN1_W + 10))
	#define	BTN5_Y	(BTN1_Y + BTN1_H + 10)
	#define	BTN5_TEXT	"����Ӧ��"

	#define BTN6_H	BTN1_H
	#define BTN6_W	BTN1_W
	#define	BTN6_X	(BTN1_X +  3 * (BTN1_W + 10))
	#define	BTN6_Y	(BTN1_Y + BTN1_H + 10)
	#define	BTN6_TEXT	"�����Ȳ���"

	#define LABEL1_X  	(BTN5_X + 10)
	#define LABEL1_Y	BTN2_Y
	#define LABEL1_TEXT	"��ַ: "

		#define LABEL2_X  	(LABEL1_X + 48)
		#define LABEL2_Y	LABEL1_Y
		#define LABEL2_TEXT	"0"

	#define LABEL3_X  	(LABEL2_X + 32)
	#define LABEL3_Y	LABEL1_Y
	#define LABEL3_TEXT	"������: "

		#define LABEL4_X  	(LABEL3_X + 64)
		#define LABEL4_Y	(LABEL3_Y)
		#define LABEL4_TEXT	"0"

	#define LABEL5_X  	(LABEL1_X)
	#define LABEL5_Y	(LABEL1_Y + 20)
	#define LABEL5_TEXT	"����:"

		#define LABEL6_X  	(LABEL5_X + 48)
		#define LABEL6_Y	LABEL5_Y
		#define LABEL6_TEXT	" "

/* ��2���� */
#define BOX2_X	BOX1_X
#define BOX2_Y	(BOX1_Y + BOX1_H + 10)
#define BOX2_H	60
#define BOX2_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX2_TEXT	"��������"

	#define BTNB_H	32
	#define BTNB_W	45
	#define	BTNB_X	(BOX2_X + 10)
	#define	BTNB_Y	(BOX2_Y + 20)

/* ��3���� */
#define BOX3_X	BOX1_X
#define BOX3_Y	(BOX2_Y + BOX2_H + 10)
#define BOX3_H	72
#define BOX3_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX3_TEXT	"�޸�485��ַ"

	#define	LABEL_OLDADDR_X		(BOX3_X + 10)
	#define	LABEL_OLDADDR_Y		(BOX3_Y + 20)
	#define	LABEL_OLDADDR_TEXT "��ǰ��ַ:"

	#define	EDIT_OLDADDR_X		(LABEL_OLDADDR_X + 80)
	#define	EDIT_OLDADDR_Y		LABEL_OLDADDR_Y
	#define	EDIT_OLDADDR_H		20
	#define	EDIT_OLDADDR_W		50

	#define	LABEL_NEWADDR_X		LABEL_OLDADDR_X
	#define	LABEL_NEWADDR_Y		(LABEL_OLDADDR_Y + 30)
	#define	LABEL_NEWADDR_TEXT	"  �µ�ַ:"

	#define	EDIT_NEWADDR_X		EDIT_OLDADDR_X
	#define	EDIT_NEWADDR_Y		LABEL_NEWADDR_Y
	#define	EDIT_NEWDDR_H 		EDIT_OLDADDR_H
	#define	EDIT_NEWADDR_W		EDIT_OLDADDR_W

	#define BTN_SETADDR_H		50
	#define BTN_SETADDR_W		100
	#define	BTN_SETADDR_X		(EDIT_NEWADDR_X + EDIT_NEWADDR_W + 10)
	#define	BTN_SETADDR_Y		EDIT_OLDADDR_Y
	#define	BTN_SETADDR_TEXT	"�޸�485��ַ"

	#define	LBL_INFO1_X		(BOX3_X + 270)
	#define	LBL_INFO1_Y		(BOX3_Y + 11)
	#define	LBL_INFO1_TEXT "ҡ�����¼�: �޸��µ�ַ"

	#define	LBL_INFO2_X		LBL_INFO1_X
	#define	LBL_INFO2_Y		(LBL_INFO1_Y + 20)
	#define	LBL_INFO2_TEXT "ҡ�����Ҽ�: �޸ĵ�ǰ��ַ"

/* ��ť */
/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	80
#define	BTN_RET_X	(g_LcdWidth - BTN_RET_W - 8)
#define	BTN_RET_Y	(g_LcdHeight - BTN_RET_H - 4)
#define	BTN_RET_TEXT	"����"


static void InitFormRS485(void);
static void DispFormRS485(void);

static void DispLabelAddr(uint8_t _addr1, uint8_t _addr2);
static void DispLabelBaud(uint32_t _Baud);
static void DispLabelRx(uint8_t *_buf, uint8_t _len);


FormRS485_T *FormRS485;
/*
*********************************************************************************************************
*	�� �� ��: FormMainRS485
*	����˵��: RS485����������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void FormMainRS485(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fQuit = 0;
	int16_t tpX, tpY;
	FormRS485_T form;
	uint8_t OldAddr;
	uint8_t NewAddr;
	uint32_t baud;
	uint32_t i;
	uint32_t count = 0;
	uint8_t fUpdateCount = 1;
	uint8_t fUpdateAddr = 1;
	uint8_t fLed888 = 0;

	FormRS485 = &form;

	InitFormRS485();

	DispFormRS485();

	OldAddr = 1;
	NewAddr = 1;
	baud = 9600;

	DispLabelBaud (baud);

	/* ����������ѭ���� */
	while (fQuit == 0)
	{
		bsp_Idle();

		MODH_Poll();

		if (fUpdateAddr == 1)
		{
			fUpdateAddr = 0;

			DispLabelAddr(OldAddr, NewAddr);
		}

		if (fUpdateCount == 1)
		{
			fUpdateCount = 0;

			LED485_DispNumberA(OldAddr, count);	/* ����LED�������ʾ����ֵ */
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					if (LCD_ButtonTouchDown(&FormRS485->BtnRet, tpX, tpY))
					{
						//fQuit = 1;
					}
					else if (LCD_ButtonTouchDown(&FormRS485->Btn1, tpX, tpY))
					{
						LED485_SetProtAscii(OldAddr);	/* ����ΪASCIIЭ�� */
					}
					else if (LCD_ButtonTouchDown(&FormRS485->Btn2, tpX, tpY))
					{
						LED485_SetProtRTU(OldAddr);	/* ����ΪModbus RTU Э�� */
					}
					else if (LCD_ButtonTouchDown(&FormRS485->Btn3, tpX, tpY))
					{
						LED485_ReadModel(OldAddr);	/* ���豸�ͺ� */
					}
					else if (LCD_ButtonTouchDown(&FormRS485->Btn4, tpX, tpY))
					{
						LED485_ReadVersion(OldAddr);	/* ���̼��汾 */
					}
					else if (LCD_ButtonTouchDown(&FormRS485->Btn5, tpX, tpY))
					{
						LED485_TestOk(OldAddr);	/* ����OKӦ�� */
					}
					else if (LCD_ButtonTouchDown(&FormRS485->Btn6, tpX, tpY))
					{
						LED485_ReadBright(OldAddr);	/* �����Ȳ��� */
					}
					else if (LCD_ButtonTouchDown(&FormRS485->BtnSetAddr, tpX, tpY))
					{
						/* �޸ĵ�ַ */
						LED485_ModifyAddrA(OldAddr, NewAddr);
					}
					else
					{
						for (i = 0; i < 8; i++)
						{
							if (LCD_ButtonTouchDown(&FormRS485->BtnBright[i], tpX, tpY))
							{
								LED485_SetBrightA(OldAddr, i);	/* �������Ȳ���(ASCIIЭ��) */
							}
						}
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (LCD_ButtonTouchRelease(&FormRS485->BtnRet, tpX, tpY))
					{
						fQuit = 1;	/* ���� */
					}
					else
					{
						LCD_ButtonTouchRelease(&FormRS485->BtnRet, tpX, tpY);
						LCD_ButtonTouchRelease(&FormRS485->Btn1, tpX, tpY);
						LCD_ButtonTouchRelease(&FormRS485->Btn2, tpX, tpY);
						LCD_ButtonTouchRelease(&FormRS485->Btn3, tpX, tpY);
						LCD_ButtonTouchRelease(&FormRS485->Btn4, tpX, tpY);
						LCD_ButtonTouchRelease(&FormRS485->Btn5, tpX, tpY);
						LCD_ButtonTouchRelease(&FormRS485->Btn6, tpX, tpY);
						LCD_ButtonTouchRelease(&FormRS485->BtnSetAddr, tpX, tpY);
						for (i = 0; i < 8; i++)
						{
							LCD_ButtonTouchRelease(&FormRS485->BtnBright[i], tpX, tpY);
						}
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
				case MSG_485_RX_NOT_RTU:		/* ���յ�485�豸��Ӧ�� */
					DispLabelRx(g_tModH.AppRxBuf, g_tModH.AppRxCount);
					break;

				case KEY_DOWN_K1:		/* K1�� + 1*/
					count++;
					fUpdateCount = 1;
					break;

				case KEY_DOWN_K2:		/* K2�� - 1 */
					if (count > 0)
					{
						count--;
					}
					fUpdateCount = 1;
					break;

				case KEY_DOWN_K3:		/* K3�� - ��0 */
					count = 0;
					fUpdateCount = 1;
					break;

				case JOY_DOWN_U:		/* ҡ��UP������ */
					NewAddr++;
					fUpdateAddr = 1;
					break;

				case JOY_DOWN_D:		/* ҡ��DOWN������ */
					NewAddr--;
					fUpdateAddr = 1;
					break;

				case JOY_DOWN_L:		/* ҡ��LEFT������ */
					OldAddr++;
					fUpdateAddr = 1;
					break;

				case JOY_DOWN_R:		/* ҡ��RIGHT������ */
					OldAddr--;
					fUpdateAddr = 1;
					break;

				case JOY_DOWN_OK:		/* ҡ��OK������ */
					/* �Զ����� */
					if (fLed888 == 0)
					{
						fLed888 = 1;
						LED485_DispStrA(OldAddr, "8.8.8.");
					}
					else if (fLed888 == 1)
					{
						fLed888 = 2;
						LED485_DispStrA(OldAddr, "8.8.8.8");
					}
					else
					{
						fLed888 = 0;
						LED485_DispStrA(OldAddr, "   ");						
					}
					break;

				default:
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: InitFormRS485
*	����˵��: ��ʼ���ؼ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormRS485(void)
{
	/* ������������ */
	FormRS485->FontBox.FontCode = FC_ST_16;
	FormRS485->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	FormRS485->FontBox.FrontColor = CL_BLACK;
	FormRS485->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	FormRS485->FontBlack.FontCode = FC_ST_16;
	FormRS485->FontBlack.BackColor = CL_MASK;		/* ͸��ɫ */
	FormRS485->FontBlack.FrontColor = CL_BLACK;
	FormRS485->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	FormRS485->FontBlue.FontCode = FC_ST_16;
	FormRS485->FontBlue.BackColor = CL_BTN_FACE;
	FormRS485->FontBlue.FrontColor = CL_BLUE;
	FormRS485->FontBlue.Space = 0;

	/* ��ť���� */
	FormRS485->FontBtn.FontCode = FC_ST_16;
	FormRS485->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormRS485->FontBtn.FrontColor = CL_BLACK;
	FormRS485->FontBtn.Space = 0;

	/* ����� */
	FormRS485->Box1.Left = BOX1_X;
	FormRS485->Box1.Top = BOX1_Y;
	FormRS485->Box1.Height = BOX1_H;
	FormRS485->Box1.Width = BOX1_W;
	FormRS485->Box1.pCaption = BOX1_TEXT;
	FormRS485->Box1.Font = &FormRS485->FontBox;

	FormRS485->Box2.Left = BOX2_X;
	FormRS485->Box2.Top = BOX2_Y;
	FormRS485->Box2.Height = BOX2_H;
	FormRS485->Box2.Width = BOX2_W;
	FormRS485->Box2.pCaption = BOX2_TEXT;
	FormRS485->Box2.Font = &FormRS485->FontBox;

	/* ��̬��ǩ */
	FormRS485->Label1.Left = LABEL1_X;
	FormRS485->Label1.Top = LABEL1_Y;
	FormRS485->Label1.MaxLen = 0;
	FormRS485->Label1.pCaption = LABEL1_TEXT;
	FormRS485->Label1.Font = &FormRS485->FontBlack;

	FormRS485->Label3.Left = LABEL3_X;
	FormRS485->Label3.Top = LABEL3_Y;
	FormRS485->Label3.MaxLen = 0;
	FormRS485->Label3.pCaption = LABEL3_TEXT;
	FormRS485->Label3.Font = &FormRS485->FontBlack;

	FormRS485->Label5.Left = LABEL5_X;
	FormRS485->Label5.Top = LABEL5_Y;
	FormRS485->Label5.MaxLen = 0;
	FormRS485->Label5.pCaption = LABEL5_TEXT;
	FormRS485->Label5.Font = &FormRS485->FontBlack;

	/* ��̬��ǩ */
	FormRS485->Label2.Left = LABEL2_X;
	FormRS485->Label2.Top = LABEL2_Y;
	FormRS485->Label2.MaxLen = 0;
	FormRS485->Label2.pCaption = LABEL2_TEXT;
	FormRS485->Label2.Font = &FormRS485->FontBlue;

	FormRS485->Label4.Left = LABEL4_X;
	FormRS485->Label4.Top = LABEL4_Y;
	FormRS485->Label4.MaxLen = 0;
	FormRS485->Label4.pCaption = LABEL4_TEXT;
	FormRS485->Label4.Font = &FormRS485->FontBlue;

	FormRS485->Label6.Left = LABEL6_X;
	FormRS485->Label6.Top = LABEL6_Y;
	FormRS485->Label6.MaxLen = 0;
	FormRS485->Label6.pCaption = LABEL6_TEXT;
	FormRS485->Label6.Font = &FormRS485->FontBlue;

	/* ��ť */
	FormRS485->BtnRet.Left = BTN_RET_X;
	FormRS485->BtnRet.Top = BTN_RET_Y;
	FormRS485->BtnRet.Height = BTN_RET_H;
	FormRS485->BtnRet.Width = BTN_RET_W;
	FormRS485->BtnRet.pCaption = BTN_RET_TEXT;
	FormRS485->BtnRet.Font = &FormRS485->FontBtn;
	FormRS485->BtnRet.Focus = 0;

	FormRS485->Btn1.Left = BTN1_X;
	FormRS485->Btn1.Top = BTN1_Y;
	FormRS485->Btn1.Height = BTN1_H;
	FormRS485->Btn1.Width = BTN1_W;
	FormRS485->Btn1.pCaption = BTN1_TEXT;
	FormRS485->Btn1.Font = &FormRS485->FontBtn;
	FormRS485->Btn1.Focus = 0;

	FormRS485->Btn2.Left = BTN2_X;
	FormRS485->Btn2.Top = BTN2_Y;
	FormRS485->Btn2.Height = BTN2_H;
	FormRS485->Btn2.Width = BTN2_W;
	FormRS485->Btn2.pCaption = BTN2_TEXT;
	FormRS485->Btn2.Font = &FormRS485->FontBtn;
	FormRS485->Btn2.Focus = 0;

	FormRS485->Btn3.Left = BTN3_X;
	FormRS485->Btn3.Top = BTN3_Y;
	FormRS485->Btn3.Height = BTN3_H;
	FormRS485->Btn3.Width = BTN3_W;
	FormRS485->Btn3.pCaption = BTN3_TEXT;
	FormRS485->Btn3.Font = &FormRS485->FontBtn;
	FormRS485->Btn3.Focus = 0;

	FormRS485->Btn4.Left = BTN4_X;
	FormRS485->Btn4.Top = BTN4_Y;
	FormRS485->Btn4.Height = BTN4_H;
	FormRS485->Btn4.Width = BTN4_W;
	FormRS485->Btn4.pCaption = BTN4_TEXT;
	FormRS485->Btn4.Font = &FormRS485->FontBtn;
	FormRS485->Btn4.Focus = 0;

	FormRS485->Btn5.Left = BTN5_X;
	FormRS485->Btn5.Top = BTN5_Y;
	FormRS485->Btn5.Height = BTN5_H;
	FormRS485->Btn5.Width = BTN5_W;
	FormRS485->Btn5.pCaption = BTN5_TEXT;
	FormRS485->Btn5.Font = &FormRS485->FontBtn;
	FormRS485->Btn5.Focus = 0;

	FormRS485->Btn6.Left = BTN6_X;
	FormRS485->Btn6.Top = BTN6_Y;
	FormRS485->Btn6.Height = BTN6_H;
	FormRS485->Btn6.Width = BTN6_W;
	FormRS485->Btn6.pCaption = BTN6_TEXT;
	FormRS485->Btn6.Font = &FormRS485->FontBtn;
	FormRS485->Btn6.Focus = 0;

	{
		uint8_t i;
		char * BrightStr[8] = {
			"0","1","2","3",
			"4","5","6","7",
		};

		for (i = 0; i < 8; i++)
		{
			FormRS485->BtnBright[i].Left = BTNB_X +(BTNB_W + 10) * i;
			FormRS485->BtnBright[i].Top = BTNB_Y;
			FormRS485->BtnBright[i].Height = BTNB_H;
			FormRS485->BtnBright[i].Width = BTNB_W;
			FormRS485->BtnBright[i].pCaption = BrightStr[i];
			FormRS485->BtnBright[i].Font = &FormRS485->FontBtn;
			FormRS485->BtnBright[i].Focus = 0;
		}
	}

	{
		FormRS485->Box3.Left = BOX3_X;
		FormRS485->Box3.Top = BOX3_Y;
		FormRS485->Box3.Height = BOX3_H;
		FormRS485->Box3.Width = BOX3_W;
		FormRS485->Box3.pCaption = BOX3_TEXT;
		FormRS485->Box3.Font = &FormRS485->FontBox;

		FormRS485->LabelOldAddr.Left = LABEL_OLDADDR_X;
		FormRS485->LabelOldAddr.Top = LABEL_OLDADDR_Y;
		FormRS485->LabelOldAddr.MaxLen = 0;
		FormRS485->LabelOldAddr.pCaption = LABEL_OLDADDR_TEXT;
		FormRS485->LabelOldAddr.Font = &FormRS485->FontBlack;

		FormRS485->LabelNewAddr.Left = LABEL_NEWADDR_X;
		FormRS485->LabelNewAddr.Top = LABEL_NEWADDR_Y;
		FormRS485->LabelNewAddr.MaxLen = 0;
		FormRS485->LabelNewAddr.pCaption = LABEL_NEWADDR_TEXT;
		FormRS485->LabelNewAddr.Font = &FormRS485->FontBlack;

		FormRS485->EditOldAddr.Left = EDIT_OLDADDR_X;
		FormRS485->EditOldAddr.Top = EDIT_OLDADDR_Y;
		FormRS485->EditOldAddr.Height = EDIT_OLDADDR_H;
		FormRS485->EditOldAddr.Width = EDIT_OLDADDR_W;
		FormRS485->EditOldAddr.pCaption = FormRS485->EditOldAddr.Text;
		FormRS485->EditOldAddr.Font = &FormRS485->FontBtn;

		FormRS485->EditNewAddr.Left = EDIT_NEWADDR_X;
		FormRS485->EditNewAddr.Top = EDIT_NEWADDR_Y;
		FormRS485->EditNewAddr.Height = EDIT_NEWDDR_H;
		FormRS485->EditNewAddr.Width = EDIT_NEWADDR_W;
		FormRS485->EditNewAddr.pCaption = FormRS485->EditNewAddr.Text;
		FormRS485->EditNewAddr.Font = &FormRS485->FontBtn;

		FormRS485->BtnSetAddr.Left = BTN_SETADDR_X;
		FormRS485->BtnSetAddr.Top = BTN_SETADDR_Y;
		FormRS485->BtnSetAddr.Height = BTN_SETADDR_H;
		FormRS485->BtnSetAddr.Width = BTN_SETADDR_W;
		FormRS485->BtnSetAddr.pCaption = BTN_SETADDR_TEXT;
		FormRS485->BtnSetAddr.Font = &FormRS485->FontBtn;
		FormRS485->BtnSetAddr.Focus = 0;

		FormRS485->EditOldAddr.Text[0] = 0;
		FormRS485->EditNewAddr.Text[0] = 0;

		FormRS485->LblInfo1.Left = LBL_INFO1_X;
		FormRS485->LblInfo1.Top = LBL_INFO1_Y;
		FormRS485->LblInfo1.MaxLen = 0;
		FormRS485->LblInfo1.pCaption = LBL_INFO1_TEXT;
		FormRS485->LblInfo1.Font = &FormRS485->FontBlack;

		FormRS485->LblInfo2.Left = LBL_INFO2_X;
		FormRS485->LblInfo2.Top = LBL_INFO2_Y;
		FormRS485->LblInfo2.MaxLen = 0;
		FormRS485->LblInfo2.pCaption = LBL_INFO2_TEXT;
		FormRS485->LblInfo2.Font = &FormRS485->FontBlack;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DispFormRS485
*	����˵��: ��ʾ���еľ�̬�ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFormRS485(void)
{
	LCD_ClrScr(CL_BTN_FACE);

	/* ����� */
	LCD_DrawGroupBox(&FormRS485->Box1);
	LCD_DrawGroupBox(&FormRS485->Box2);

	LCD_DrawLabel(&FormRS485->Label1);
	LCD_DrawLabel(&FormRS485->Label3);
	LCD_DrawLabel(&FormRS485->Label5);

	LCD_DrawLabel(&FormRS485->Label2);
	LCD_DrawLabel(&FormRS485->Label4);
	LCD_DrawLabel(&FormRS485->Label6);

	/* ��ť */
	LCD_DrawButton(&FormRS485->Btn1);
	LCD_DrawButton(&FormRS485->Btn2);
	LCD_DrawButton(&FormRS485->Btn3);
	LCD_DrawButton(&FormRS485->Btn4);
	LCD_DrawButton(&FormRS485->Btn5);
	LCD_DrawButton(&FormRS485->Btn6);

	{
		uint8_t i;

		for (i = 0; i < 8; i++)
		{
			LCD_DrawButton(&FormRS485->BtnBright[i]);
		}
	}

	LCD_DrawGroupBox(&FormRS485->Box3);
	LCD_DrawLabel(&FormRS485->LabelOldAddr);
	LCD_DrawLabel(&FormRS485->LabelNewAddr);
	LCD_DrawEdit(&FormRS485->EditOldAddr);
	LCD_DrawEdit(&FormRS485->EditNewAddr);
	LCD_DrawButton(&FormRS485->BtnSetAddr);
	LCD_DrawLabel(&FormRS485->LblInfo1);
	LCD_DrawLabel(&FormRS485->LblInfo2);

	LCD_DrawButton(&FormRS485->BtnRet);
}

/*
*********************************************************************************************************
*	�� �� ��: DispLabelAddr
*	����˵��: ��ʾ485��ַ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispLabelAddr(uint8_t _addr1, uint8_t _addr2)
{
	char buf[10];

	sprintf(buf, "%d", _addr1);

	/* ��̬��ǩ */
	FormRS485->Label2.pCaption = buf;
	LCD_DrawLabel(&FormRS485->Label2);

	sprintf(FormRS485->EditOldAddr.Text, "%d", _addr1);
	LCD_DrawEdit(&FormRS485->EditOldAddr);

	sprintf(FormRS485->EditNewAddr.Text, "%d", _addr2);
	LCD_DrawEdit(&FormRS485->EditNewAddr);
}

/*
*********************************************************************************************************
*	�� �� ��: DispLabelTx
*	����˵��: ��ʾ���͵�����
*	��    ��: _Baud ������
*			  _
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispLabelBaud(uint32_t _Baud)
{
	char buf[10];

	sprintf(buf, "%d", _Baud);

	/* ��̬��ǩ */
	FormRS485->Label4.pCaption = buf;
	LCD_DrawLabel(&FormRS485->Label4);
}

/*
*********************************************************************************************************
*	�� �� ��: DispLabelRx
*	����˵��: ��ʾ���յ�������
*	��    ��: _buf Ҫ���͵�����
*			  _
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispLabelRx(uint8_t *_buf, uint8_t _len)
{
	char buf[16];

	_buf[_len] = 0;
	sprintf(buf, "%d | %s", _len, _buf);

	/* ��̬��ǩ */
	FormRS485->Label6.pCaption = buf;
	LCD_DrawLabel(&FormRS485->Label6);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
