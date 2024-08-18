/*
*********************************************************************************************************
*
*	ģ������ : GPS��λģ����Գ���
*	�ļ����� : gps_test.c
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
#include "form_gps.h"

/* �������ṹ */
typedef struct
{
	FONT_T FontBlack;	/* ��̬������ */
	FONT_T FontBlue;	/* �仯���������� */
	FONT_T FontRed;		/* ��ɫ���� */
	FONT_T FontBtn;		/* ��ť������ */
	FONT_T FontBox;		/* ������������ */

	GROUP_T Box1;

	LABEL_T Label1;	LABEL_T Label2;	/* γ�� */
	LABEL_T Label3; LABEL_T Label4;	/* ���� */
	LABEL_T Label5; LABEL_T Label6;	/* �ٶ� */
	LABEL_T Label7; LABEL_T Label8;	/* ���� */
	
	LABEL_T Label9; LABEL_T Label10;	/* ״̬ */	

	BUTTON_T BtnRet;
}FormGPS_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* �������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	(g_LcdHeight - BOX1_Y - 10)
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_TEXT	"GPS��λģ����Գ���"

/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	60
#define	BTN_RET_X	((BOX1_X + BOX1_W) - BTN_RET_W - 4)
#define	BTN_RET_Y	((BOX1_Y  + BOX1_H) - BTN_RET_H - 4)
#define	BTN_RET_TEXT	"����"

#define LABEL1_X  	(BOX1_X + 6)
#define LABEL1_Y	(BOX1_Y + 20)
#define LABEL1_TEXT	"γ�� : "

	#define LABEL2_X  	(LABEL1_X + 64)
	#define LABEL2_Y	LABEL1_Y
	#define LABEL2_TEXT	"0000.0000"

#define LABEL3_X  	(LABEL1_X)
#define LABEL3_Y	(LABEL1_Y + 20)
#define LABEL3_TEXT	"���� : "

	#define LABEL4_X  	(LABEL3_X + 64)
	#define LABEL4_Y	(LABEL3_Y)
	#define LABEL4_TEXT	"00000.0000"

#define LABEL5_X  	(LABEL1_X)
#define LABEL5_Y	(LABEL1_Y + 20 * 2)
#define LABEL5_TEXT	"�ٶ� : "

	#define LABEL6_X  	(LABEL5_X + 64)
	#define LABEL6_Y	LABEL5_Y
	#define LABEL6_TEXT	"0.0KM"

#define LABEL7_X  	(LABEL1_X)
#define LABEL7_Y	(LABEL1_Y + 20 * 3)
#define LABEL7_TEXT	"���� : "

	#define LABEL8_X  	(LABEL7_X + 64)
	#define LABEL8_Y	LABEL7_Y
	#define LABEL8_TEXT	"0.0M"

#define LABEL9_X  	(LABEL1_X)
#define LABEL9_Y	(LABEL1_Y + 20 * 5)
#define LABEL9_TEXT	"Ӳ�� : "

	#define LABEL10_X  	(LABEL9_X + 64)
	#define LABEL10_Y	LABEL9_Y
	#define LABEL10_TEXT	""

static void InitFormGPS(void);
static void DispGPSInitFace(void);
static void DispGPSStatus(void);

FormGPS_T *FormGPS;

/*
*********************************************************************************************************
*	�� �� ��: TestGPS
*	����˵��: ����GPSģ�顣
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TestGPS(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fQuit = 0;
	int16_t tpX, tpY;
	FormGPS_T form;

	FormGPS = &form;

	bsp_InitGPS();
	
	InitFormGPS();

	DispGPSInitFace();

	
	bsp_StartAutoTimer(0, 1000);	/* ÿ�붨ʱ��ʾGPS״̬ */
	
	bsp_StartTimer(1, 3000);	/* 3�볬ʱû���յ�GPS���ݣ�����Ϊ��������ʧ�� */
	
	/* ����������ѭ���� */	
	while (fQuit == 0)
	{
		bsp_Idle();

		gps_pro();

		if (bsp_CheckTimer(0))
		{
			DispGPSStatus();
		}

		/* ��δ��������ж�CPU�Ƿ��ܹ��յ�GPSģ�鷵�ص����� */
		{
			if (bsp_CheckTimer(1))
			{
				FormGPS->Label10.Font = &FormGPS->FontRed;
				FormGPS->Label10.pCaption = "δ��⵽GPSģ��";
				LCD_DrawLabel(&FormGPS->Label10);						
			}
			if (g_tGPS.UartOk == 1)	/* ����ͨ�������ı�־, ����Ժ��յ���У��ϸ�����������Ϊ1 */		
			{			
				bsp_StartTimer(1, 3000);	/* 3�볬ʱû���յ�GPS���ݣ�����Ϊ��������ʧ�� */	

				FormGPS->Label10.Font = &FormGPS->FontBlue;
				FormGPS->Label10.pCaption = "��⵽GPSģ��. ���������շ�OK";
				LCD_DrawLabel(&FormGPS->Label10);						
				
				g_tGPS.UartOk = 0;
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
						FormGPS->BtnRet.Focus = 1;
						LCD_DrawButton(&FormGPS->BtnRet);
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (TOUCH_InRect(tpX, tpY, BTN_RET_X, BTN_RET_Y, BTN_RET_H, BTN_RET_W))
					{
						FormGPS->BtnRet.Focus = 0;
						LCD_DrawButton(&FormGPS->BtnRet);
						fQuit = 1;	/* ���� */
					}
					else	/* ��ťʧȥ���� */
					{
						FormGPS->BtnRet.Focus = 0;
						LCD_DrawButton(&FormGPS->BtnRet);
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

	bsp_StopTimer(0);
}

/*
*********************************************************************************************************
*	�� �� ��: DispGPSStatus
*	����˵��: ��ʾGPS״̬
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispGPSStatus(void)
{
	char buf[128];

	/* γ�� */
	if (g_tGPS.NS == 'S')
	{
		sprintf(buf, "��γ %02d��%02d.%04d'=%02d.%06d��", g_tGPS.WeiDu_Du,
			g_tGPS.WeiDu_Fen / 10000, g_tGPS.WeiDu_Fen % 10000,
			g_tGPS.WeiDu_Du, gps_FenToDu(g_tGPS.WeiDu_Fen));

		sprintf(&buf[strlen(buf)], "=%02d��%02d'%02d\"", g_tGPS.WeiDu_Du,
			g_tGPS.WeiDu_Fen / 10000, gps_FenToMiao(g_tGPS.WeiDu_Fen));
	}
	else
	{
		sprintf(buf, "��γ %02d��%02d.%04d'=%02d.%06d��", g_tGPS.WeiDu_Du,
			g_tGPS.WeiDu_Fen / 10000, g_tGPS.WeiDu_Fen % 10000,
			g_tGPS.WeiDu_Du, gps_FenToDu(g_tGPS.WeiDu_Fen));

		sprintf(&buf[strlen(buf)], "=%02d��%02d'%02d\"", g_tGPS.WeiDu_Du,
			g_tGPS.WeiDu_Fen / 10000, gps_FenToMiao(g_tGPS.WeiDu_Fen));
	}
	FormGPS->Label2.pCaption = buf;
	LCD_DrawLabel(&FormGPS->Label2);

	/* ���� */
	if (g_tGPS.EW == 'E')
	{
		sprintf(buf, "���� %03d��%02d.%04d'=%03d.%06d��", g_tGPS.JingDu_Du,
			g_tGPS.JingDu_Fen / 10000, g_tGPS.JingDu_Fen % 10000,
			g_tGPS.JingDu_Du, gps_FenToDu(g_tGPS.JingDu_Fen));

		sprintf(&buf[strlen(buf)], "=%03d��%02d'%02d\"", g_tGPS.JingDu_Du,
			g_tGPS.WeiDu_Fen / 10000, gps_FenToMiao(g_tGPS.JingDu_Fen));
	}
	else
	{
		sprintf(buf, "���� %03d��%02d.%04d'=%03d.%06d��", g_tGPS.JingDu_Du,
			g_tGPS.JingDu_Fen / 10000, g_tGPS.JingDu_Fen % 10000,
			g_tGPS.JingDu_Du, gps_FenToDu(g_tGPS.JingDu_Fen));

		sprintf(&buf[strlen(buf)], "=%03d��%02d'%02d\"", g_tGPS.JingDu_Du,
			g_tGPS.JingDu_Fen / 10000, gps_FenToMiao(g_tGPS.JingDu_Fen));
	}
	FormGPS->Label4.pCaption = buf;
	LCD_DrawLabel(&FormGPS->Label4);

	/* �ٶ� */
	sprintf(buf, "%5d.%d KM/h", g_tGPS.SpeedKM / 10, g_tGPS.SpeedKM % 10);
	FormGPS->Label6.pCaption = buf;
	LCD_DrawLabel(&FormGPS->Label6);

	/* ���� */
	sprintf(buf, "%5d.%d M", g_tGPS.Altitude / 10, g_tGPS.Altitude % 10);
	FormGPS->Label8.pCaption = buf;
	LCD_DrawLabel(&FormGPS->Label8);
}

/*
*********************************************************************************************************
*	�� �� ��: InitFormGPS
*	����˵��: ��ʼ��GPS��ʼ����ؼ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormGPS(void)
{
	/* ������������ */
	FormGPS->FontBox.FontCode = FC_ST_16;
	FormGPS->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	FormGPS->FontBox.FrontColor = CL_BLACK;
	FormGPS->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	FormGPS->FontBlack.FontCode = FC_ST_16;
	FormGPS->FontBlack.BackColor = CL_MASK;		/* ͸��ɫ */
	FormGPS->FontBlack.FrontColor = CL_BLACK;
	FormGPS->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	FormGPS->FontBlue.FontCode = FC_ST_16;
	FormGPS->FontBlue.BackColor = CL_BTN_FACE;
	FormGPS->FontBlue.FrontColor = CL_BLUE;
	FormGPS->FontBlue.Space = 0;

	FormGPS->FontRed.FontCode = FC_ST_16;
	FormGPS->FontRed.BackColor = CL_BTN_FACE;
	FormGPS->FontRed.FrontColor = CL_RED;
	FormGPS->FontRed.Space = 0;	

	/* ��ť���� */
	FormGPS->FontBtn.FontCode = FC_ST_16;
	FormGPS->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormGPS->FontBtn.FrontColor = CL_BLACK;
	FormGPS->FontBtn.Space = 0;

	/* ����� */
	FormGPS->Box1.Left = BOX1_X;
	FormGPS->Box1.Top = BOX1_Y;
	FormGPS->Box1.Height = BOX1_H;
	FormGPS->Box1.Width = BOX1_W;
	FormGPS->Box1.pCaption = BOX1_TEXT;
	FormGPS->Box1.Font = &FormGPS->FontBox;

	/* ��̬��ǩ */
	FormGPS->Label1.Left = LABEL1_X;
	FormGPS->Label1.Top = LABEL1_Y;
	FormGPS->Label1.MaxLen = 0;
	FormGPS->Label1.pCaption = LABEL1_TEXT;
	FormGPS->Label1.Font = &FormGPS->FontBlack;

	FormGPS->Label3.Left = LABEL3_X;
	FormGPS->Label3.Top = LABEL3_Y;
	FormGPS->Label3.MaxLen = 0;
	FormGPS->Label3.pCaption = LABEL3_TEXT;
	FormGPS->Label3.Font = &FormGPS->FontBlack;

	FormGPS->Label5.Left = LABEL5_X;
	FormGPS->Label5.Top = LABEL5_Y;
	FormGPS->Label5.MaxLen = 0;
	FormGPS->Label5.pCaption = LABEL5_TEXT;
	FormGPS->Label5.Font = &FormGPS->FontBlack;

	FormGPS->Label7.Left = LABEL7_X;
	FormGPS->Label7.Top = LABEL7_Y;
	FormGPS->Label7.MaxLen = 0;
	FormGPS->Label7.pCaption = LABEL7_TEXT;
	FormGPS->Label7.Font = &FormGPS->FontBlack;

	FormGPS->Label9.Left = LABEL9_X;
	FormGPS->Label9.Top = LABEL9_Y;
	FormGPS->Label9.MaxLen = 0;
	FormGPS->Label9.pCaption = LABEL9_TEXT;
	FormGPS->Label9.Font = &FormGPS->FontBlack;

	/* ��̬��ǩ */
	FormGPS->Label2.Left = LABEL2_X;
	FormGPS->Label2.Top = LABEL2_Y;
	FormGPS->Label2.MaxLen = 0;
	FormGPS->Label2.pCaption = LABEL2_TEXT;
	FormGPS->Label2.Font = &FormGPS->FontBlue;

	FormGPS->Label4.Left = LABEL4_X;
	FormGPS->Label4.Top = LABEL4_Y;
	FormGPS->Label4.MaxLen = 0;
	FormGPS->Label4.pCaption = LABEL4_TEXT;
	FormGPS->Label4.Font = &FormGPS->FontBlue;

	FormGPS->Label6.Left = LABEL6_X;
	FormGPS->Label6.Top = LABEL6_Y;
	FormGPS->Label6.MaxLen = 0;
	FormGPS->Label6.pCaption = LABEL6_TEXT;
	FormGPS->Label6.Font = &FormGPS->FontBlue;

	FormGPS->Label8.Left = LABEL8_X;
	FormGPS->Label8.Top = LABEL8_Y;
	FormGPS->Label8.MaxLen = 0;
	FormGPS->Label8.pCaption = LABEL8_TEXT;
	FormGPS->Label8.Font = &FormGPS->FontBlue;

	FormGPS->Label10.Left = LABEL10_X;
	FormGPS->Label10.Top = LABEL10_Y;
	FormGPS->Label10.MaxLen = 0;
	FormGPS->Label10.pCaption = LABEL10_TEXT;
	FormGPS->Label10.Font = &FormGPS->FontBlue;
	
	/* ��ť */
	FormGPS->BtnRet.Left = BTN_RET_X;
	FormGPS->BtnRet.Top = BTN_RET_Y;
	FormGPS->BtnRet.Height = BTN_RET_H;
	FormGPS->BtnRet.Width = BTN_RET_W;
	FormGPS->BtnRet.pCaption = BTN_RET_TEXT;
	FormGPS->BtnRet.Font = &FormGPS->FontBtn;
	FormGPS->BtnRet.Focus = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: DispGPSInitFace
*	����˵��: ��ʾ���еľ�̬�ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispGPSInitFace(void)
{
	LCD_ClrScr(CL_BTN_FACE);

	/* ����� */
	LCD_DrawGroupBox(&FormGPS->Box1);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormGPS->Label1);
	LCD_DrawLabel(&FormGPS->Label3);
	LCD_DrawLabel(&FormGPS->Label5);
	LCD_DrawLabel(&FormGPS->Label7);
	LCD_DrawLabel(&FormGPS->Label9);	

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormGPS->Label2);
	LCD_DrawLabel(&FormGPS->Label4);
	LCD_DrawLabel(&FormGPS->Label6);
	LCD_DrawLabel(&FormGPS->Label8);
	LCD_DrawLabel(&FormGPS->Label10);

	/* ��ť */
	LCD_DrawButton(&FormGPS->BtnRet);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
