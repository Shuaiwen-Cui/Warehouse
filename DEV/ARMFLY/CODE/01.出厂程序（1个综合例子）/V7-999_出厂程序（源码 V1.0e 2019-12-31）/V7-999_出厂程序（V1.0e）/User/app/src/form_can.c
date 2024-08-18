/*
*********************************************************************************************************
*
*	ģ������ : CAN������Խ���
*	�ļ����� : form_can.c
*	��    �� : V1.0
*	˵    �� : ����������֮�����CAN���绥ͨ����
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2015-08-09 armfly  �׷�
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "form_can.h"
#include "can_network.h"

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

	BUTTON_T BtnRet;

	BUTTON_T Btn1;
	BUTTON_T Btn2;
	BUTTON_T Btn3;
	BUTTON_T Btn4;
	BUTTON_T Btn5;
	BUTTON_T Btn6;

	LABEL_T LblInfo1; LABEL_T LblInfo2;
}FormCAN_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* �������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	100
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_TEXT	"CAN�������"

	/* ��1�����ڵİ�ť */
	#define BTN1_H	32
	#define BTN1_W	100
	#define	BTN1_X	(BOX1_X + 10)
	#define	BTN1_Y	(BOX1_Y + 20)
	#define	BTN1_TEXT	"����LED2"

	#define BTN2_H	BTN1_H
	#define BTN2_W	BTN1_W
	#define	BTN2_X	(BTN1_X +  BTN1_W + 10)
	#define	BTN2_Y	BTN1_Y
	#define	BTN2_TEXT	"�ر�LED2"

	#define BTN3_H	BTN1_H
	#define BTN3_W	BTN1_W
	#define	BTN3_X	BTN1_X
	#define	BTN3_Y	(BTN1_Y + BTN1_H + 10)
	#define	BTN3_TEXT	"����2��"

	#define BTN4_H	BTN1_H
	#define BTN4_W	BTN1_W
	#define	BTN4_X	(BTN1_X +  BTN1_W + 10)
	#define	BTN4_Y	(BTN1_Y + BTN1_H + 10)
	#define	BTN4_TEXT	"����3��"

	#define BTN5_H	BTN1_H
	#define BTN5_W	BTN1_W
	#define	BTN5_X	(BTN1_X +  2 * (BTN1_W + 10))
	#define	BTN5_Y	(BTN1_Y + BTN1_H + 10)
	#define	BTN5_TEXT	"����20��"

	#define BTN6_H	BTN1_H
	#define BTN6_W	BTN1_W
	#define	BTN6_X	(BTN1_X +  3 * (BTN1_W + 10))
	#define	BTN6_Y	(BTN1_Y + BTN1_H + 10)
	#define	BTN6_TEXT	"ֹͣ����"

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

#define	LBL_INFO1_X		(BOX1_X)
#define	LBL_INFO1_Y		(BOX1_Y + BOX1_H + 10)
#define	LBL_INFO1_TEXT "�뽫�����������CAN�ӿڻ���"

#define	LBL_INFO2_X		LBL_INFO1_X
#define	LBL_INFO2_Y		(LBL_INFO1_Y + 20)
#define	LBL_INFO2_TEXT "���Ի�����ƶԷ���LED2�ͷ�����"

/* ��ť */
/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	80
#define	BTN_RET_X	(g_LcdWidth - BTN_RET_W - 8)
#define	BTN_RET_Y	(g_LcdHeight - BTN_RET_H - 4)
#define	BTN_RET_TEXT	"����"


static void InitFormCAN(void);
static void DispFormCAN(void);

static void DispLabelBaud(uint32_t _Baud);
static void DispLabelRx(uint8_t *_buf, uint8_t _len);


FormCAN_T *FormCAN;

/*
*********************************************************************************************************
*	�� �� ��: FormMainCAN
*	����˵��: CAN����������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void FormMainCAN(void)
{
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fQuit = 0;
	int16_t tpX, tpY;
	FormCAN_T form;
	uint32_t baud;
	
	FormCAN = &form;

	InitFormCAN();
	DispFormCAN();

	baud = 500000;
	DispLabelBaud (baud);
	
	can_Init();				/* ��ʼ��STM32 CANӲ�� */
	
	/* ����������ѭ���� */
	bsp_StartAutoTimer(2, 10);
	while (fQuit == 0)
	{
		bsp_Idle();

//		MODBUS_Poll();

		if (bsp_CheckTimer(2))
		{
			can_LedOn(1, 1);
		}
		
		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					if (LCD_ButtonTouchDown(&FormCAN->BtnRet, tpX, tpY))
					{
						//fQuit = 1;
					}
					else if (LCD_ButtonTouchDown(&FormCAN->Btn1, tpX, tpY))
					{
						can_LedOn(1, 2);						
					}
					else if (LCD_ButtonTouchDown(&FormCAN->Btn2, tpX, tpY))
					{
						can_LedOff(1, 2);
					}
					else if (LCD_ButtonTouchDown(&FormCAN->Btn3, tpX, tpY))
					{
						can_BeepCtrl(1, 2);	/* ���Ʒ���������2�� */
					}
					else if (LCD_ButtonTouchDown(&FormCAN->Btn4, tpX, tpY))
					{
						can_BeepCtrl(1, 3);	/* ���Ʒ���������3�� */
					}
					else if (LCD_ButtonTouchDown(&FormCAN->Btn5, tpX, tpY))
					{
						can_BeepCtrl(1, 20);	/* ���Ʒ���������20�� */
					}
					else if (LCD_ButtonTouchDown(&FormCAN->Btn6, tpX, tpY))
					{
						can_BeepCtrl(1, 0);	/* ���Ʒ�����ֹͣ���� */
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (LCD_ButtonTouchRelease(&FormCAN->BtnRet, tpX, tpY))
					{
						fQuit = 1;	/* ���� */
					}
					else
					{
						LCD_ButtonTouchRelease(&FormCAN->BtnRet, tpX, tpY);
						LCD_ButtonTouchRelease(&FormCAN->Btn1, tpX, tpY);
						LCD_ButtonTouchRelease(&FormCAN->Btn2, tpX, tpY);
						LCD_ButtonTouchRelease(&FormCAN->Btn3, tpX, tpY);
						LCD_ButtonTouchRelease(&FormCAN->Btn4, tpX, tpY);
						LCD_ButtonTouchRelease(&FormCAN->Btn5, tpX, tpY);
						LCD_ButtonTouchRelease(&FormCAN->Btn6, tpX, tpY);
					}
					break;
			}
		}
		

		/* ����Ӧ�ò���Ϣ */
		{
			MSG_T msg;

			if (bsp_GetMsg(&msg))
			{
				switch (msg.MsgCode)
				{
					case MSG_CAN1_RX:		/* ���յ�CAN�豸��Ӧ�� */
						DispLabelRx(g_Can1RxData, g_Can1RxHeader.DataLength);
						can1_Analyze();
						break;	
					
					case MSG_CAN2_RX:		/* ���յ�CAN�豸��Ӧ�� */
						DispLabelRx(g_Can2RxData, g_Can2RxHeader.DataLength);
						can1_Analyze();
						break;					
				}
			}
		}
	}
	
	can_DeInit();	/* �ر�CANʱ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: InitFormCAN
*	����˵��: ��ʼ���ؼ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormCAN(void)
{
	/* ������������ */
	FormCAN->FontBox.FontCode = FC_ST_16;
	FormCAN->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	FormCAN->FontBox.FrontColor = CL_BLACK;
	FormCAN->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	FormCAN->FontBlack.FontCode = FC_ST_16;
	FormCAN->FontBlack.BackColor = CL_MASK;		/* ͸��ɫ */
	FormCAN->FontBlack.FrontColor = CL_BLACK;
	FormCAN->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	FormCAN->FontBlue.FontCode = FC_ST_16;
	FormCAN->FontBlue.BackColor = CL_BTN_FACE;
	FormCAN->FontBlue.FrontColor = CL_BLUE;
	FormCAN->FontBlue.Space = 0;

	/* ��ť���� */
	FormCAN->FontBtn.FontCode = FC_ST_16;
	FormCAN->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormCAN->FontBtn.FrontColor = CL_BLACK;
	FormCAN->FontBtn.Space = 0;

	/* ����� */
	FormCAN->Box1.Left = BOX1_X;
	FormCAN->Box1.Top = BOX1_Y;
	FormCAN->Box1.Height = BOX1_H;
	FormCAN->Box1.Width = BOX1_W;
	FormCAN->Box1.pCaption = BOX1_TEXT;
	FormCAN->Box1.Font = &FormCAN->FontBox;

	/* ��̬��ǩ */
	FormCAN->Label1.Left = LABEL1_X;
	FormCAN->Label1.Top = LABEL1_Y;
	FormCAN->Label1.MaxLen = 0;
	FormCAN->Label1.pCaption = LABEL1_TEXT;
	FormCAN->Label1.Font = &FormCAN->FontBlack;

	FormCAN->Label3.Left = LABEL3_X;
	FormCAN->Label3.Top = LABEL3_Y;
	FormCAN->Label3.MaxLen = 0;
	FormCAN->Label3.pCaption = LABEL3_TEXT;
	FormCAN->Label3.Font = &FormCAN->FontBlack;

	FormCAN->Label5.Left = LABEL5_X;
	FormCAN->Label5.Top = LABEL5_Y;
	FormCAN->Label5.MaxLen = 0;
	FormCAN->Label5.pCaption = LABEL5_TEXT;
	FormCAN->Label5.Font = &FormCAN->FontBlack;

	/* ��̬��ǩ */
	FormCAN->Label2.Left = LABEL2_X;
	FormCAN->Label2.Top = LABEL2_Y;
	FormCAN->Label2.MaxLen = 0;
	FormCAN->Label2.pCaption = LABEL2_TEXT;
	FormCAN->Label2.Font = &FormCAN->FontBlue;

	FormCAN->Label4.Left = LABEL4_X;
	FormCAN->Label4.Top = LABEL4_Y;
	FormCAN->Label4.MaxLen = 0;
	FormCAN->Label4.pCaption = LABEL4_TEXT;
	FormCAN->Label4.Font = &FormCAN->FontBlue;

	FormCAN->Label6.Left = LABEL6_X;
	FormCAN->Label6.Top = LABEL6_Y;
	FormCAN->Label6.MaxLen = 0;
	FormCAN->Label6.pCaption = LABEL6_TEXT;
	FormCAN->Label6.Font = &FormCAN->FontBlue;

	/* ��ť */
	FormCAN->BtnRet.Left = BTN_RET_X;
	FormCAN->BtnRet.Top = BTN_RET_Y;
	FormCAN->BtnRet.Height = BTN_RET_H;
	FormCAN->BtnRet.Width = BTN_RET_W;
	FormCAN->BtnRet.pCaption = BTN_RET_TEXT;
	FormCAN->BtnRet.Font = &FormCAN->FontBtn;
	FormCAN->BtnRet.Focus = 0;

	FormCAN->Btn1.Left = BTN1_X;
	FormCAN->Btn1.Top = BTN1_Y;
	FormCAN->Btn1.Height = BTN1_H;
	FormCAN->Btn1.Width = BTN1_W;
	FormCAN->Btn1.pCaption = BTN1_TEXT;
	FormCAN->Btn1.Font = &FormCAN->FontBtn;
	FormCAN->Btn1.Focus = 0;

	FormCAN->Btn2.Left = BTN2_X;
	FormCAN->Btn2.Top = BTN2_Y;
	FormCAN->Btn2.Height = BTN2_H;
	FormCAN->Btn2.Width = BTN2_W;
	FormCAN->Btn2.pCaption = BTN2_TEXT;
	FormCAN->Btn2.Font = &FormCAN->FontBtn;
	FormCAN->Btn2.Focus = 0;

	FormCAN->Btn3.Left = BTN3_X;
	FormCAN->Btn3.Top = BTN3_Y;
	FormCAN->Btn3.Height = BTN3_H;
	FormCAN->Btn3.Width = BTN3_W;
	FormCAN->Btn3.pCaption = BTN3_TEXT;
	FormCAN->Btn3.Font = &FormCAN->FontBtn;
	FormCAN->Btn3.Focus = 0;

	FormCAN->Btn4.Left = BTN4_X;
	FormCAN->Btn4.Top = BTN4_Y;
	FormCAN->Btn4.Height = BTN4_H;
	FormCAN->Btn4.Width = BTN4_W;
	FormCAN->Btn4.pCaption = BTN4_TEXT;
	FormCAN->Btn4.Font = &FormCAN->FontBtn;
	FormCAN->Btn4.Focus = 0;

	FormCAN->Btn5.Left = BTN5_X;
	FormCAN->Btn5.Top = BTN5_Y;
	FormCAN->Btn5.Height = BTN5_H;
	FormCAN->Btn5.Width = BTN5_W;
	FormCAN->Btn5.pCaption = BTN5_TEXT;
	FormCAN->Btn5.Font = &FormCAN->FontBtn;
	FormCAN->Btn5.Focus = 0;

	FormCAN->Btn6.Left = BTN6_X;
	FormCAN->Btn6.Top = BTN6_Y;
	FormCAN->Btn6.Height = BTN6_H;
	FormCAN->Btn6.Width = BTN6_W;
	FormCAN->Btn6.pCaption = BTN6_TEXT;
	FormCAN->Btn6.Font = &FormCAN->FontBtn;
	FormCAN->Btn6.Focus = 0;

	{
		FormCAN->LblInfo1.Left = LBL_INFO1_X;
		FormCAN->LblInfo1.Top = LBL_INFO1_Y;
		FormCAN->LblInfo1.MaxLen = 0;
		FormCAN->LblInfo1.pCaption = LBL_INFO1_TEXT;
		FormCAN->LblInfo1.Font = &FormCAN->FontBlack;

		FormCAN->LblInfo2.Left = LBL_INFO2_X;
		FormCAN->LblInfo2.Top = LBL_INFO2_Y;
		FormCAN->LblInfo2.MaxLen = 0;
		FormCAN->LblInfo2.pCaption = LBL_INFO2_TEXT;
		FormCAN->LblInfo2.Font = &FormCAN->FontBlack;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DispFormCAN
*	����˵��: ��ʾ���еľ�̬�ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFormCAN(void)
{
	LCD_ClrScr(CL_BTN_FACE);

	/* ����� */
	LCD_DrawGroupBox(&FormCAN->Box1);

	LCD_DrawLabel(&FormCAN->Label1);
	LCD_DrawLabel(&FormCAN->Label3);
	LCD_DrawLabel(&FormCAN->Label5);

	LCD_DrawLabel(&FormCAN->Label2);
	LCD_DrawLabel(&FormCAN->Label4);
	LCD_DrawLabel(&FormCAN->Label6);

	/* ��ť */
	LCD_DrawButton(&FormCAN->Btn1);
	LCD_DrawButton(&FormCAN->Btn2);
	LCD_DrawButton(&FormCAN->Btn3);
	LCD_DrawButton(&FormCAN->Btn4);
	LCD_DrawButton(&FormCAN->Btn5);
	LCD_DrawButton(&FormCAN->Btn6);

	LCD_DrawLabel(&FormCAN->LblInfo1);
	LCD_DrawLabel(&FormCAN->LblInfo2);

	LCD_DrawButton(&FormCAN->BtnRet);
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
	FormCAN->Label4.pCaption = buf;
	LCD_DrawLabel(&FormCAN->Label4);
}

/*
*********************************************************************************************************
*	�� �� ��: DispLabelTx
*	����˵��: ��ʾ���͵�����
*	��    ��: _buf Ҫ���͵�����
*			  _
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispLabelRx(uint8_t *_buf, uint8_t _len)
{
	char disp_buf[32];
	uint8_t len;
	
	len = _len;
	if (len > sizeof(disp_buf) / 3)
	{
		len = sizeof(disp_buf) / 3;
	}
	
	HexToAscll(_buf, disp_buf, len);

	/* ��̬��ǩ */
	FormCAN->Label6.pCaption = disp_buf;
	LCD_DrawLabel(&FormCAN->Label6);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
