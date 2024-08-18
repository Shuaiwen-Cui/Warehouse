/*
*********************************************************************************************************
*
*	ģ������ : AD9833���η��������Խ���
*	�ļ����� : form_ad9833.c
*	��    �� : V1.0
*	˵    �� : ��ʾAD9833ģ��Ĺ���
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2015-07-19 armfly  �װ�
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "form_ad9833.h"

/* �������ṹ */
typedef struct
{
	FONT_T FontBlack;	/* ��ɫ */
	FONT_T FontBlue;	/* ��ɫ */
	FONT_T FontBtn;		/* ��ť������ */
	FONT_T FontBox;		/* ������������ */

	GROUP_T Box1;

	BUTTON_T BtnRet;

	BUTTON_T Btn1;
	BUTTON_T Btn2;
	BUTTON_T Btn3;
	BUTTON_T Btn4;
	BUTTON_T Btn5;
	BUTTON_T Btn6;
	BUTTON_T Btn7;
	BUTTON_T Btn8;
	BUTTON_T Btn9;
	BUTTON_T Btn10;
	BUTTON_T Btn11;
	BUTTON_T Btn12;
	BUTTON_T Btn13;
	BUTTON_T Btn14;
	BUTTON_T Btn15;
	BUTTON_T Btn16;	

	LABEL_T Label1;
	LABEL_T Label2;
	LABEL_T Label3;
	LABEL_T Label4;
	LABEL_T Label5;
	LABEL_T Label6;

	uint32_t Freq;		/* ����Ƶ�ʡ���λ 0.1Hz */
	AD9833_WAVE_E  WaveType;	/* �������� */
	uint8_t ScanBand;	/* ɨƵ���� */
	uint32_t FreqBegin;	/* ��ʼƵ�� */
	uint32_t FreqEnd;	/* ��ֹƵ�� */
}Form9833_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* �������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	(g_LcdHeight - 53)
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_T	"AD9833���η�����"

#define BTN1_H	32
#define BTN1_W	105

/* ��1�а�ť���� */
#define	BTN1_X	(BOX1_X + 10)
#define	BTN1_Y	(BOX1_Y + 20)
#define	BTN1_T	"���Ҳ�"

	#define BTN2_H	BTN1_H
	#define BTN2_W	BTN1_W
	#define	BTN2_X	(BTN1_X +  BTN1_W + 10)
	#define	BTN2_Y	BTN1_Y
	#define	BTN2_T	"���ǲ�"

		#define BTN3_H	BTN1_H
		#define BTN3_W	BTN1_W
		#define	BTN3_X	(BTN1_X + 2 * (BTN1_W + 10))
		#define	BTN3_Y	BTN1_Y
		#define	BTN3_T	"����"
		
			#define BTN16_H	BTN1_H
			#define BTN16_W	BTN1_W
			#define	BTN16_X	(BTN1_X + 3 * (BTN1_W + 10))
			#define	BTN16_Y	BTN1_Y
			#define	BTN16_T	"ֹͣ"	

/* ��2�а�ť���� */
#define BTN4_H	BTN1_H
#define BTN4_W	BTN1_W
#define	BTN4_X	BTN1_X
#define	BTN4_Y	(BTN1_Y + BTN1_H + 10)
#define	BTN4_T	"Ƶ��+0.1Hz"

	#define BTN5_H	BTN1_H
	#define BTN5_W	BTN1_W
	#define	BTN5_X	(BTN1_X +  1 * (BTN1_W + 10))
	#define	BTN5_Y	BTN4_Y
	#define	BTN5_T	"Ƶ��-0.1Hz"

		#define BTN6_H	BTN1_H
		#define BTN6_W	BTN1_W
		#define	BTN6_X	(BTN1_X +  2 * (BTN1_W + 10))
		#define	BTN6_Y	BTN4_Y
		#define	BTN6_T	"Ƶ��+1Hz"

			#define BTN7_H	BTN1_H
			#define BTN7_W	BTN1_W
			#define	BTN7_X	(BTN1_X +  3 * (BTN1_W + 10))
			#define	BTN7_Y	BTN4_Y
			#define	BTN7_T	"Ƶ��-1Hz"

/* ��3�а�ť���� */
#define BTN8_H	BTN1_H
#define BTN8_W	BTN1_W
#define	BTN8_X	BTN1_X
#define	BTN8_Y	(BTN1_Y + 2 * (BTN1_H + 10))
#define	BTN8_T	"Ƶ��+1kHz"

	#define BTN9_H	BTN1_H
	#define BTN9_W	BTN1_W
	#define	BTN9_X	(BTN1_X +  1 * (BTN1_W + 10))
	#define	BTN9_Y	BTN8_Y
	#define	BTN9_T	"Ƶ��-1kHz"

		#define BTN10_H	BTN1_H
		#define BTN10_W	BTN1_W
		#define	BTN10_X	(BTN1_X +  2 * (BTN1_W + 10))
		#define	BTN10_Y	BTN8_Y
		#define	BTN10_T	"Ƶ��+100kHz"

			#define BTN11_H	BTN1_H
			#define BTN11_W	BTN1_W
			#define	BTN11_X	(BTN1_X +  3 * (BTN1_W + 10))
			#define	BTN11_Y	BTN8_Y
			#define	BTN11_T	"Ƶ��-100kHz"

/* ��4�а�ť���� */
#define BTN12_H	BTN1_H
#define BTN12_W	BTN1_W
#define	BTN12_X	BTN1_X
#define	BTN12_Y	(BTN1_Y + 3 * (BTN1_H + 10))
#define	BTN12_T	"Ƶ��=10Hz"

	#define BTN13_H	BTN1_H
	#define BTN13_W	BTN1_W
	#define	BTN13_X	(BTN1_X +  1 * (BTN1_W + 10))
	#define	BTN13_Y	BTN12_Y
	#define	BTN13_T	"Ƶ��=10KHz"

		#define BTN14_H	BTN1_H
		#define BTN14_W	BTN1_W
		#define	BTN14_X	(BTN1_X +  2 * (BTN1_W + 10))
		#define	BTN14_Y	BTN12_Y
		#define	BTN14_T	"ɨƵƵ��"

			#define BTN15_H	BTN1_H
			#define BTN15_W	BTN1_W
			#define	BTN15_X	(BTN1_X +  3 * (BTN1_W + 10))
			#define	BTN15_Y	BTN12_Y
			#define	BTN15_T	"��ʼɨƵ"

#define	LBL1_X	BOX1_X + 5
#define	LBL1_Y	190
#define	LBL1_T "�������:"

#define	LBL2_X	LBL1_X + 85
#define	LBL2_Y	LBL1_Y
#define	LBL2_T " "

#define	LBL3_X	LBL1_X
#define	LBL3_Y	LBL1_Y + 20
#define	LBL3_T "����Ƶ��:"

#define	LBL4_X	LBL3_X + 85
#define	LBL4_Y	LBL3_Y
#define	LBL4_T " "

	#define	LBL5_X	LBL1_X
	#define	LBL5_Y	LBL3_Y + 25
	#define	LBL5_T "ɨƵƵ��:"

	#define	LBL6_X	LBL5_X + 85
	#define	LBL6_Y	LBL5_Y
	#define	LBL6_T " "


/* ��ť */
/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	80
#define	BTN_RET_X	(g_LcdWidth - BTN_RET_W - 8)
#define	BTN_RET_Y	(g_LcdHeight - BTN_RET_H - 4)
#define	BTN_RET_T	"����"


#define BAND_NUM	6			
static const uint32_t s_FreqBand[BAND_NUM][3] =
{
	/* ��ʼƵ��  ����Ƶ��   ����Ƶ�� (��λ 0.1Hz) */
	{0, 1000, 10},
	{0, 10000, 10},
	{0, 100000, 100},
	{0, 100000, 1000},
	{0, 1000000, 1000},
	{0, 100000000, 100000}
};

Form9833_T *Form9833;


static void InitForm9833(void);
static void DispForm9833(void);
static void Disp9833Info(void);
static void FreqToStr(uint32_t _freq, char *_dispbuf);

/*
*********************************************************************************************************
*	�� �� ��: FormMain9833
*	����˵��: �����������������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void FormMain9833(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fQuit = 0;
	int16_t tpX, tpY;
	Form9833_T form;
	uint8_t fDispInfo = 1;

	Form9833 = &form;

	InitForm9833();
	DispForm9833();
	
	bsp_InitAD9833();		

	Form9833->Freq = 100000;			/* ����Ƶ�ʡ���λ 0.1Hz */
	Form9833->WaveType = NONE_WAVE;	/* �������� */
	Form9833->ScanBand = 0;
	
	fDispInfo = 1;	
	/* ����������ѭ���� */
	while (fQuit == 0)
	{
		bsp_Idle();

		if (fDispInfo)
		{
			fDispInfo = 0;

			AD9833_SetWaveFreq(Form9833->Freq);	/*����Ƶ��ֵ  */			
			AD9833_SelectWave(Form9833->WaveType);
	
			Disp9833Info();		/* ��ʾƵ�� */
		}

		/* ɨƵ */
		if (bsp_CheckTimer(0))
		{
			Form9833->Freq += s_FreqBand[Form9833->ScanBand][2];	/* ����Ƶ�� */
			fDispInfo = 1;
			
			if (Form9833->Freq >= s_FreqBand[Form9833->ScanBand][1])
			{
				bsp_StopTimer(0);	/* ֹͣ��ʱ�� */
			}
		}
		
		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					if (LCD_ButtonTouchDown(&Form9833->BtnRet, tpX, tpY))
					{
						//fQuit = 1;	�ͷ�ʱ���˳�
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn1, tpX, tpY))
					{
						Form9833->WaveType = SINE_WAVE;	/* ���Ҳ� */
						fDispInfo = 1;
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn2, tpX, tpY))
					{
						Form9833->WaveType = TRI_WAVE;	/* ���ǲ� */
						fDispInfo = 1;
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn3, tpX, tpY))
					{
						Form9833->WaveType = SQU_WAVE;	/* ���� */
						fDispInfo = 1;
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn4, tpX, tpY))	/* Ƶ�� +0.1Hz */
					{
						if (Form9833->Freq < AD9833_MAX_FREQ)
						{
							Form9833->Freq++;
						}						
						
						fDispInfo = 1;
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn5, tpX, tpY))	/* Ƶ�� -0.1Hz */
					{
						if (Form9833->Freq > 0)
						{
							Form9833->Freq--;
						}
						fDispInfo = 1;
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn6, tpX, tpY))	/* Ƶ�� +1Hz */
					{
						if (Form9833->Freq < AD9833_MAX_FREQ)
						{
							Form9833->Freq += 10;
						}						
						fDispInfo = 1;
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn7, tpX, tpY))	/* Ƶ�� -1Hz */
					{
						if (Form9833->Freq > 10)
						{
							Form9833->Freq -= 10;
						}					
						fDispInfo = 1;
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn8, tpX, tpY))	/* Ƶ�� +1kHz  */
					{
						if (Form9833->Freq < AD9833_MAX_FREQ)
						{
							Form9833->Freq += 10000;
						}						
						fDispInfo = 1;
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn9, tpX, tpY))	/* Ƶ�� -1kHz */
					{
						if (Form9833->Freq > 10000)
						{
							Form9833->Freq -= 10000;
						}					
						fDispInfo = 1;
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn10, tpX, tpY))	/* Ƶ�� +100kHz */
					{
						if (Form9833->Freq < AD9833_MAX_FREQ)
						{
							Form9833->Freq += 1000000;
						}						
						fDispInfo = 1;
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn11, tpX, tpY))	/* Ƶ�� -100kHz */
					{
						if (Form9833->Freq > 1000000)
						{
							Form9833->Freq -= 1000000;
						}					
						fDispInfo = 1;
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn12, tpX, tpY))	/* Ƶ�� = 10Hz */
					{
						Form9833->Freq = 100;
						fDispInfo = 1;						
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn13, tpX, tpY))	/* Ƶ�� = 10KHz */
					{
						Form9833->Freq = 100000;
						fDispInfo = 1;						
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn14, tpX, tpY))	/* ɨƵƵ�� */
					{
						/* �л�ɨƵƵ�� */
						if (++Form9833->ScanBand >= BAND_NUM)
						{
							Form9833->ScanBand = 0;
						}										
						fDispInfo = 1;						
					}
					else if (LCD_ButtonTouchDown(&Form9833->Btn15, tpX, tpY))	/* ��ʼɨƵ */
					{						
						Form9833->Freq = s_FreqBand[Form9833->ScanBand][0];

						bsp_StartAutoTimer(0, 100);	/* 100ms ����һ��Ƶ�� */
						fDispInfo = 1;
					}							
					else if (LCD_ButtonTouchDown(&Form9833->Btn16, tpX, tpY))
					{
						Form9833->WaveType = NONE_WAVE;	/* ���ֹͣ */
						fDispInfo = 1;
						
						bsp_StopTimer(0);	/* ֹͣ��ʱ�� */
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (LCD_ButtonTouchRelease(&Form9833->BtnRet, tpX, tpY))
					{
						fQuit = 1;	/* ���� */
					}
					else
					{
						LCD_ButtonTouchRelease(&Form9833->BtnRet, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn1, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn2, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn3, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn4, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn5, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn6, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn7, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn8, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn9, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn10, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn11, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn12, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn13, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn14, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn15, tpX, tpY);
						LCD_ButtonTouchRelease(&Form9833->Btn16, tpX, tpY);
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
				case KEY_DOWN_K1:		/* K1�� + 1*/
					break;

				case KEY_DOWN_K2:		/* K2�� - 1 */
					break;

				case KEY_DOWN_K3:		/* K3�� - ��0 */
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
*	�� �� ��: InitForm9833
*	����˵��: ��ʼ���ؼ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitForm9833(void)
{
	/* ������������ */
	Form9833->FontBox.FontCode = FC_ST_16;
	Form9833->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	Form9833->FontBox.FrontColor = CL_BLACK;
	Form9833->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	Form9833->FontBlack.FontCode = FC_ST_16;
	Form9833->FontBlack.BackColor = CL_MASK;		/* ͸��ɫ */
	Form9833->FontBlack.FrontColor = CL_BLACK;
	Form9833->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	Form9833->FontBlue.FontCode = FC_ST_16;
	Form9833->FontBlue.BackColor = CL_BTN_FACE;
	Form9833->FontBlue.FrontColor = CL_BLUE;
	Form9833->FontBlue.Space = 0;

	/* ��ť���� */
	Form9833->FontBtn.FontCode = FC_ST_16;
	Form9833->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	Form9833->FontBtn.FrontColor = CL_BLACK;
	Form9833->FontBtn.Space = 0;

	/* ����� */
	Form9833->Box1.Left = BOX1_X;
	Form9833->Box1.Top = BOX1_Y;
	Form9833->Box1.Height = BOX1_H;
	Form9833->Box1.Width = BOX1_W;
	Form9833->Box1.pCaption = BOX1_T;
	Form9833->Box1.Font = &Form9833->FontBox;

	/* ��ǩ */
	Form9833->Label1.Left = LBL1_X;
	Form9833->Label1.Top = LBL1_Y;
	Form9833->Label1.MaxLen = 0;
	Form9833->Label1.pCaption = LBL1_T;
	Form9833->Label1.Font = &Form9833->FontBlack;

	Form9833->Label2.Left = LBL2_X;
	Form9833->Label2.Top = LBL2_Y;
	Form9833->Label2.MaxLen = 0;
	Form9833->Label2.pCaption = LBL2_T;
	Form9833->Label2.Font = &Form9833->FontBlue;

	Form9833->Label3.Left = LBL3_X;
	Form9833->Label3.Top = LBL3_Y;
	Form9833->Label3.MaxLen = 0;
	Form9833->Label3.pCaption = LBL3_T;
	Form9833->Label3.Font = &Form9833->FontBlack;

	Form9833->Label4.Left = LBL4_X;
	Form9833->Label4.Top = LBL4_Y;
	Form9833->Label4.MaxLen = 0;
	Form9833->Label4.pCaption = LBL4_T;
	Form9833->Label4.Font = &Form9833->FontBlue;

	Form9833->Label5.Left = LBL5_X;
	Form9833->Label5.Top = LBL5_Y;
	Form9833->Label5.MaxLen = 0;
	Form9833->Label5.pCaption = LBL5_T;
	Form9833->Label5.Font = &Form9833->FontBlack;

	Form9833->Label6.Left = LBL6_X;
	Form9833->Label6.Top = LBL6_Y;
	Form9833->Label6.MaxLen = 0;
	Form9833->Label6.pCaption = LBL6_T;
	Form9833->Label6.Font = &Form9833->FontBlue;

	/* ��ť */
	Form9833->BtnRet.Left = BTN_RET_X;
	Form9833->BtnRet.Top = BTN_RET_Y;
	Form9833->BtnRet.Height = BTN_RET_H;
	Form9833->BtnRet.Width = BTN_RET_W;
	Form9833->BtnRet.pCaption = BTN_RET_T;
	Form9833->BtnRet.Font = &Form9833->FontBtn;
	Form9833->BtnRet.Focus = 0;

	Form9833->Btn1.Left = BTN1_X;
	Form9833->Btn1.Top = BTN1_Y;
	Form9833->Btn1.Height = BTN1_H;
	Form9833->Btn1.Width = BTN1_W;
	Form9833->Btn1.pCaption = BTN1_T;
	Form9833->Btn1.Font = &Form9833->FontBtn;
	Form9833->Btn1.Focus = 0;

	Form9833->Btn2.Left = BTN2_X;
	Form9833->Btn2.Top = BTN2_Y;
	Form9833->Btn2.Height = BTN2_H;
	Form9833->Btn2.Width = BTN2_W;
	Form9833->Btn2.pCaption = BTN2_T;
	Form9833->Btn2.Font = &Form9833->FontBtn;
	Form9833->Btn2.Focus = 0;

	Form9833->Btn3.Left = BTN3_X;
	Form9833->Btn3.Top = BTN3_Y;
	Form9833->Btn3.Height = BTN3_H;
	Form9833->Btn3.Width = BTN3_W;
	Form9833->Btn3.pCaption = BTN3_T;
	Form9833->Btn3.Font = &Form9833->FontBtn;
	Form9833->Btn3.Focus = 0;

	Form9833->Btn4.Left = BTN4_X;
	Form9833->Btn4.Top = BTN4_Y;
	Form9833->Btn4.Height = BTN4_H;
	Form9833->Btn4.Width = BTN4_W;
	Form9833->Btn4.pCaption = BTN4_T;
	Form9833->Btn4.Font = &Form9833->FontBtn;
	Form9833->Btn4.Focus = 0;

	Form9833->Btn5.Left = BTN5_X;
	Form9833->Btn5.Top = BTN5_Y;
	Form9833->Btn5.Height = BTN5_H;
	Form9833->Btn5.Width = BTN5_W;
	Form9833->Btn5.pCaption = BTN5_T;
	Form9833->Btn5.Font = &Form9833->FontBtn;
	Form9833->Btn5.Focus = 0;

	Form9833->Btn6.Left = BTN6_X;
	Form9833->Btn6.Top = BTN6_Y;
	Form9833->Btn6.Height = BTN6_H;
	Form9833->Btn6.Width = BTN6_W;
	Form9833->Btn6.pCaption = BTN6_T;
	Form9833->Btn6.Font = &Form9833->FontBtn;
	Form9833->Btn6.Focus = 0;

	Form9833->Btn7.Left = BTN7_X;
	Form9833->Btn7.Top = BTN7_Y;
	Form9833->Btn7.Height = BTN7_H;
	Form9833->Btn7.Width = BTN7_W;
	Form9833->Btn7.pCaption = BTN7_T;
	Form9833->Btn7.Font = &Form9833->FontBtn;
	Form9833->Btn7.Focus = 0;

	Form9833->Btn8.Left = BTN8_X;
	Form9833->Btn8.Top = BTN8_Y;
	Form9833->Btn8.Height = BTN8_H;
	Form9833->Btn8.Width = BTN8_W;
	Form9833->Btn8.pCaption = BTN8_T;
	Form9833->Btn8.Font = &Form9833->FontBtn;
	Form9833->Btn8.Focus = 0;

	Form9833->Btn9.Left = BTN9_X;
	Form9833->Btn9.Top = BTN9_Y;
	Form9833->Btn9.Height = BTN9_H;
	Form9833->Btn9.Width = BTN9_W;
	Form9833->Btn9.pCaption = BTN9_T;
	Form9833->Btn9.Font = &Form9833->FontBtn;
	Form9833->Btn9.Focus = 0;

	Form9833->Btn10.Left = BTN10_X;
	Form9833->Btn10.Top = BTN10_Y;
	Form9833->Btn10.Height = BTN10_H;
	Form9833->Btn10.Width = BTN10_W;
	Form9833->Btn10.pCaption = BTN10_T;
	Form9833->Btn10.Font = &Form9833->FontBtn;
	Form9833->Btn10.Focus = 0;

	Form9833->Btn11.Left = BTN11_X;
	Form9833->Btn11.Top = BTN11_Y;
	Form9833->Btn11.Height = BTN11_H;
	Form9833->Btn11.Width = BTN11_W;
	Form9833->Btn11.pCaption = BTN11_T;
	Form9833->Btn11.Font = &Form9833->FontBtn;
	Form9833->Btn11.Focus = 0;

	Form9833->Btn12.Left = BTN12_X;
	Form9833->Btn12.Top = BTN12_Y;
	Form9833->Btn12.Height = BTN12_H;
	Form9833->Btn12.Width = BTN12_W;
	Form9833->Btn12.pCaption = BTN12_T;
	Form9833->Btn12.Font = &Form9833->FontBtn;
	Form9833->Btn12.Focus = 0;

	Form9833->Btn13.Left = BTN13_X;
	Form9833->Btn13.Top = BTN13_Y;
	Form9833->Btn13.Height = BTN13_H;
	Form9833->Btn13.Width = BTN13_W;
	Form9833->Btn13.pCaption = BTN13_T;
	Form9833->Btn13.Font = &Form9833->FontBtn;
	Form9833->Btn13.Focus = 0;

	Form9833->Btn14.Left = BTN14_X;
	Form9833->Btn14.Top = BTN14_Y;
	Form9833->Btn14.Height = BTN14_H;
	Form9833->Btn14.Width = BTN14_W;
	Form9833->Btn14.pCaption = BTN14_T;
	Form9833->Btn14.Font = &Form9833->FontBtn;
	Form9833->Btn14.Focus = 0;

	Form9833->Btn15.Left = BTN15_X;
	Form9833->Btn15.Top = BTN15_Y;
	Form9833->Btn15.Height = BTN15_H;
	Form9833->Btn15.Width = BTN15_W;
	Form9833->Btn15.pCaption = BTN15_T;
	Form9833->Btn15.Font = &Form9833->FontBtn;
	Form9833->Btn15.Focus = 0;

	Form9833->Btn16.Left = BTN16_X;
	Form9833->Btn16.Top = BTN16_Y;
	Form9833->Btn16.Height = BTN16_H;
	Form9833->Btn16.Width = BTN16_W;
	Form9833->Btn16.pCaption = BTN16_T;
	Form9833->Btn16.Font = &Form9833->FontBtn;
	Form9833->Btn16.Focus = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: DispForm9833
*	����˵��: ��ʾ���еľ�̬�ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispForm9833(void)
{
	LCD_ClrScr(CL_BTN_FACE);

	/* ����� */
	LCD_DrawGroupBox(&Form9833->Box1);

	LCD_DrawLabel(&Form9833->Label1);
	LCD_DrawLabel(&Form9833->Label2);
	LCD_DrawLabel(&Form9833->Label3);
	LCD_DrawLabel(&Form9833->Label4);
	LCD_DrawLabel(&Form9833->Label5);
	LCD_DrawLabel(&Form9833->Label6);	

	/* ��ť */
	LCD_DrawButton(&Form9833->Btn1);
	LCD_DrawButton(&Form9833->Btn2);
	LCD_DrawButton(&Form9833->Btn3);
	LCD_DrawButton(&Form9833->Btn4);
	LCD_DrawButton(&Form9833->Btn5);
	LCD_DrawButton(&Form9833->Btn6);
	LCD_DrawButton(&Form9833->Btn5);
	LCD_DrawButton(&Form9833->Btn6);
	LCD_DrawButton(&Form9833->Btn7);
	LCD_DrawButton(&Form9833->Btn8);
	LCD_DrawButton(&Form9833->Btn9);
	LCD_DrawButton(&Form9833->Btn10);
	LCD_DrawButton(&Form9833->Btn11);
	LCD_DrawButton(&Form9833->Btn12);
	LCD_DrawButton(&Form9833->Btn13);
	LCD_DrawButton(&Form9833->Btn14);
	LCD_DrawButton(&Form9833->Btn15);
	LCD_DrawButton(&Form9833->Btn16);	
	LCD_DrawButton(&Form9833->BtnRet);
}

/*
*********************************************************************************************************
*	�� �� ��: FreqToStr
*	����˵��: ��Ƶ��ֵת��Ϊ��ʾ�ַ�����
*	��    ��: _freq Ƶ��ֵ����λ��0.1Hz
*			  _dispbuf �����ʾ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void FreqToStr(uint32_t _freq, char *_dispbuf)
{
	if (_freq < 10000)
	{
		sprintf(_dispbuf, "%d.%dHz", _freq / 10, _freq % 10);	
	}
	else if (_freq >= 10000 && _freq < 10000000)
	{
		sprintf(_dispbuf, "%d.%04dKHz", _freq / 10000, _freq % 10000);	
	}
	else if (_freq >= 10000000)
	{
		sprintf(_dispbuf, "%d.%07d%MHz", _freq / 10000000, (_freq % 10000000)) ;	
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DispDacVoltage
*	����˵��: ��ʾ��ѹ
*	��    ��: ��
*			  _
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void Disp9833Info(void)
{
	char buf[128];
		
	{
		/* ��ӡ��ǰ�Ĳ������� */
		if (Form9833->WaveType == NONE_WAVE)
		{
			strcpy(buf, "�ر�");	
		}
		else if (Form9833->WaveType == TRI_WAVE)
		{
			strcpy(buf, "���ǲ�");	
		}
		else if (Form9833->WaveType == SINE_WAVE)
		{
			strcpy(buf, "���Ҳ�");	
		}
		else if (Form9833->WaveType == SQU_WAVE)
		{
			strcpy(buf, "����");	
		}	
		else
		{
			strcpy(buf, "xxxxx");	
		}

		Form9833->Label2.pCaption = buf;
		LCD_DrawLabel(&Form9833->Label2);
	}
	
	{
		/* ��ӡ��ǰ��ɨƵƵ�� */
		uint32_t freq1, freq2, step;
		char buf1[32];
		char buf2[32];
		char buf3[32];
		
		freq1 = s_FreqBand[Form9833->ScanBand][0];
		freq2 = s_FreqBand[Form9833->ScanBand][1];
		step = s_FreqBand[Form9833->ScanBand][2];
		
		FreqToStr(freq1, buf1);
		FreqToStr(freq2, buf2);
		FreqToStr(step, buf3);
		
		sprintf(buf, "%s-%s, ���� %s", buf1, buf2, buf3);

		Form9833->Label6.pCaption = buf;
		LCD_DrawLabel(&Form9833->Label6);			
	}
	
	{				
		/* ��ӡ��ǰ��Ƶ��ֵ */
		FreqToStr(Form9833->Freq, buf);	/* ��������ʽ��Ƶ��ֵת��Ϊ����ʾ��ASCII�ַ��� */

		Form9833->Label4.pCaption = buf;
		LCD_DrawLabel(&Form9833->Label4);
	}
}


/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
