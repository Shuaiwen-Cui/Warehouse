/*
*********************************************************************************************************
*
*	ģ������ : ˫ͨ��ʾ��������
*	�ļ����� : form_dso.c
*	��    �� : V1.0
*	˵    �� : ʹ��STM32�ڲ�ADC�������Ρ�CH1 = PC0�� CH2 = PC1
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2015-06-23  armfly  ��ʽ����
*		V1.1    2015-08-07  armfly  ʹ�ö�ջ�����ߴ�����ݻ����������ȫ�ֱ�����
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "form_dso.h"

#define DSO_VER	"V0.6"		  /* ��ǰ�汾 */

/* ������״̬�� */
enum
{
	S_HELP		= 0,	/* ����ҳ�� */
	S_RUN		= 1,	/* �ɼ�״̬ */

	S_EXIT		= 3
};

/* ʱ��ֶȱ� g_DSO->TimeBaseId��Ϊ��������  */
const uint32_t g_TimeTable[] =
{
					10,
	20, 	50, 	100, 
	200,	500,	1000,
	2000,	5000,	10000,
	20000,	50000,	100000,
	200000,	500000,	1000000		
};

/* ˥��������  


���㹫ʽ��
  iTemp = g_DSO->Ch1VOffset + (int16_t)((2024 - g_DSO->Ch1Buf[i + 1]) * 10) / g_DSO->Ch1Attenuation;

	g_DSO->Ch1Attenuation ��ADCֵ������֮��ı��ʵ�10����

	1V ��λʱ:  ADC = 4096 / 5.0 = 819�� ���� = 25
	g_DSO->Ch1Attenuation = 819 / 25 = 32.76

*/
#define ATT_COUNT	6

#ifdef D112_2
	const uint32_t g_AttTable[ATT_COUNT][2] =
	{
		/* {����*0.1, ÿ����ѹ}  1:1 */
		{327,     5000}, 	/* GAIN = 3, �Ŵ�1�� */
		{260,     2000},	/* GAIN = 2  �Ŵ� 2�� */

		{155 * 2, 1000}, /* GAIN = 1 �Ŵ� 5 �� */
		{155,	  500},	/* GAIN = 1 �Ŵ� 5��  */
		{112,     200},	/* Gain = 0 �Ŵ� 10�� */
		{112 / 2, 100},	/* Gain = 0 �Ŵ� 10�� */
	};
#else

	#define Y_RATE      327
		
	const uint32_t g_AttTable[ATT_COUNT][2] =
	{
		/* {����*0.1, ÿ����ѹ}  1:1 */
		//{Y_RATE * 5,	5000},
		//{Y_RATE * 2,	2000},
		{Y_RATE * 5 / 5,	5000}, 	/* GAIN = 3 */
		{Y_RATE * 2 / 5,	2000},

		{Y_RATE,	    1000},   /*��GAIN = 1��*/
		{Y_RATE / 2,	500},
		{Y_RATE / 5,	200},	/*  Gain = 0 */
		{Y_RATE / 10,	100},	/* Gain = 0, �Ŵ� */
	};
#endif

static void DsoHelp(uint8_t *pMainStatus);
static void DsoRun(uint8_t *pMainStatus);

/* ��ť */
/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	80
#define	BTN_RET_X	(g_LcdWidth - BTN_RET_W - 8)
#define	BTN_RET_Y	(g_LcdHeight - BTN_RET_H - 4)
#define	BTN_RET_T	"����"

DSO_T *g_DSO;	/* ȫ�ֱ�������һ���ṹ�� */

/* �������ṹ */
typedef struct
{
	FONT_T FontBtn;		/* ��ť������ */
	
	BUTTON_T BtnRet;
	
	BUTTON_T Btn1;
	BUTTON_T Btn2;
	BUTTON_T Btn3;
	BUTTON_T Btn4;
	BUTTON_T Btn5;
	BUTTON_T Btn6;
	BUTTON_T Btn7;
	BUTTON_T Btn8;	
}FormDSO_T;

FormDSO_T *FormDSO;

static void InitFormDSO(void);

/*
*********************************************************************************************************
*	�� �� ��: InitFormDSO
*	����˵��: ��ʼ���ؼ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormDSO(void)
{
	/* ��ť���� */
	FormDSO->FontBtn.FontCode = FC_ST_16;
	FormDSO->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormDSO->FontBtn.FrontColor = CL_BLACK;
	FormDSO->FontBtn.Space = 0;
	
	/* ��ť */
	FormDSO->BtnRet.Left = BTN_RET_X;
	FormDSO->BtnRet.Top = BTN_RET_Y;
	FormDSO->BtnRet.Height = BTN_RET_H;
	FormDSO->BtnRet.Width = BTN_RET_W;
	FormDSO->BtnRet.pCaption = BTN_RET_T;
	FormDSO->BtnRet.Font = &FormDSO->FontBtn;
	FormDSO->BtnRet.Focus = 0;
	
	/*  AC/DC ��Y���� = 224 */
	/* void LCD_InitButton(BUTTON_T *_btn, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, 
		char *_pCaption, FONT_T *_pFont); */
	
	LCD_InitButton(&FormDSO->Btn1, 10      , 244, 24, 30, "AC", &FormDSO->FontBtn);	/* ͨ��1 AC-DC�л� */
	LCD_InitButton(&FormDSO->Btn2, 10 + 35 , 244, 24, 30, "+" , &FormDSO->FontBtn);	/* ͨ��1 ����+ */
	LCD_InitButton(&FormDSO->Btn3, 10 + 65 , 244, 24, 30, "-" , &FormDSO->FontBtn);	/* ͨ��1 ����- */
	LCD_InitButton(&FormDSO->Btn4, 10 + 110, 244, 24, 30, "AC", &FormDSO->FontBtn);	/* ͨ��2 AC-DC�л� */
	LCD_InitButton(&FormDSO->Btn5, 10 + 145, 244, 24, 30, "+" , &FormDSO->FontBtn);	/* ͨ��2 ����+ */
	LCD_InitButton(&FormDSO->Btn6, 10 + 175, 244, 24, 30, "-" , &FormDSO->FontBtn);	/* ͨ��2 ����- */

	LCD_InitButton(&FormDSO->Btn7, 10 + 225, 244, 24, 30, "+" , &FormDSO->FontBtn);	/* ʱ��+ */
	LCD_InitButton(&FormDSO->Btn8, 10 + 265, 244, 24, 30, "-" , &FormDSO->FontBtn);	/* ʱ��- */
	
	/* ���ư�ť */
	LCD_DrawButton(&FormDSO->BtnRet);
	LCD_DrawButton(&FormDSO->Btn1);
	LCD_DrawButton(&FormDSO->Btn2);
	LCD_DrawButton(&FormDSO->Btn3);
	LCD_DrawButton(&FormDSO->Btn4);
	LCD_DrawButton(&FormDSO->Btn5);
	LCD_DrawButton(&FormDSO->Btn6);
	LCD_DrawButton(&FormDSO->Btn7);
	LCD_DrawButton(&FormDSO->Btn8);	
}

/*
*********************************************************************************************************
*	�� �� ��: DsoMain
*	����˵��: ʾ��������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void DsoMain(void)
{
	uint8_t MainStatus = S_RUN;		/* ����ִ��״̬ */
	DSO_T tDSO;			/* �ܴ��һ������������ڶ�ջ����Լȫ�ֱ����ռ� */
	
	memset(&tDSO, 0, sizeof(tDSO));
	g_DSO = &tDSO;

	/* DAC1���10KHz�����ֵ2V�����Ҳ� */
	dac1_SetSinWave(1638, 10000);	/* ʹ��STM32�ڲ�DAC1������Ҳ�,  ��1�������Ƿ���(0-2048) ��2����Ƶ�� */

	/* PE6/TIM15_CH2 ���10KHz PWM��ռ�ձ�5000 = 50% */
	bsp_SetTIMOutPWM(GPIOE, GPIO_PIN_6, TIM15, 2, 10000, 5000);
	
	{
		DSO_ConfigCtrlGPIO();	/* ����ʾ����ģ��Ŀ���GPIO: ���ͨ�������� */
	
		g_DSO->CH1_DC = 0;		/* CH1ѡ��AC��� */
		g_DSO->CH2_DC = 0;		/* CH1ѡ��AC��� */
		g_DSO->CH1_Gain = 0;	/* CH1ѡ��С���� ˥��1/5, ��2������1��ʾ��˥��1;1 */
		g_DSO->CH2_Gain = 0;	/* CH2ѡ��С���� ˥��1/5, ��2������1��ʾ��˥��1;1 */		
		
		DSO_SetDC(1, g_DSO->CH1_DC);	
		DSO_SetDC(2, g_DSO->CH2_DC);	
		DSO_SetGain(1, g_DSO->CH1_Gain);	
		DSO_SetGain(2, g_DSO->CH2_Gain);	
	}
	
	/* ��Ϊ����������TIM1_CH1,  ��ʾ������ADC�ɼ���ͻ�������ʱ���ΰ�ť��ʾ�� */
	BEEP_Pause();
	
	/* ����������ѭ���� */
	while (1)
	{
		switch (MainStatus)
		{
			case S_HELP:
				DsoHelp(&MainStatus);		/* ��ʾ���� */
				break;

			case S_RUN:
				DsoRun(&MainStatus);		/* ȫ�ٲɼ���ʵʱ��ʾ */
				break;

			case S_EXIT:
				dac1_StopWave();	/* �ر�DAC1��� */
//				dac2_StopWave();	/* �ر�DAC2��� */
//				bsp_SetTIMOutPWM(GPIOF, GPIO_PIN_9, TIM14, 1, 0, 5000);
			
				BEEP_Resume(); /* �ָ����������� */
				return;
			
			default:
				break;
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DispHelp1
*	����˵��: ��ʾ������ʾ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispHelp1(void)
{
	uint16_t y;
	uint16_t LineCap;
	FONT_T font;

	/* ������������ */
	font.FontCode = FC_ST_16;
	font.BackColor = CL_BLUE;		/* �ͱ���ɫ��ͬ */
	font.FrontColor = CL_WHITE;		/* ��ɫ���� */
	font.Space = 0;

	LCD_ClrScr(CL_BLUE);  		/* ������������ɫ */

	y = 0;
	LineCap = 18; /* �м�� */
	LCD_DispStr(20, y, "������STM32-V5������  www.armfly.com", &font);
	
	font.FrontColor = CL_YELLOW;		/* ��ɫ���� */
	
	y += LineCap;	
	LCD_DispStr(30, y, "QQ:1295744630     Email:armfly@qq.com", &font);
	y += LineCap;

	y += LineCap;

	LCD_DispStr(30, y, "������ʾ:", &font);
	y += LineCap;
	LCD_DispStr(50, y, "K1��     = �л�ͨ�����㡣CH1��CH2", &font);
	y += LineCap; 
	LCD_DispStr(50, y, "K2��     = ��ʾ�������˳�����", &font);
	y += LineCap;
	LCD_DispStr(50, y, "K3��     = ��ͣ��ʵʱ����", &font);
	y += LineCap;
	LCD_DispStr(50, y, "ҡ���ϼ� = �Ŵ��δ�ֱ���Ȼ������ƶ�", &font);
	y += LineCap;
	LCD_DispStr(50, y, "ҡ���¼� = ��С���δ�ֱ���Ȼ������ƶ�", &font);
	y += LineCap;
	LCD_DispStr(50, y, "ҡ����� = ˮƽչ����", &font);
	y += LineCap;
	LCD_DispStr(50, y, "ҡ���Ҽ� = ˮƽ��С����", &font);
	y += LineCap;
	LCD_DispStr(50, y, "ҡ��OK�� = �л�ҡ�˵���ģʽ�����Ȼ�λ��", &font);
}

/*
*********************************************************************************************************
*	�� �� ��: DsoHelp
*	����˵��: ��ʾ������ʾ��״̬��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DsoHelp(uint8_t *pMainStatus)
{
	uint8_t KeyCode;

	uint8_t fRefresh = 1;	/* LCDˢ�±�־ */
	uint8_t SubStatus = 0;

	while (*pMainStatus == S_HELP)
	{
		bsp_Idle();

		if (fRefresh)
		{
			fRefresh = 0;

			if (SubStatus == 0)
			{
				DispHelp1();
			}
		}

		/* ��ȡ����������0��ʾ�м����� */
		KeyCode = bsp_GetKey();
		if (KeyCode > 0)
		{
			/* �м����� */
			switch (KeyCode)
			{
				case KEY_DOWN_K2:				
					/* �˳�,����ȫ������״̬ */
					*pMainStatus = S_RUN;
					break;

				case JOY_DOWN_L:	/* ҡ��LEFT������ */
				case JOY_DOWN_R:	/* ҡ��RIGHT������ */
				case KEY_DOWN_K3:			
				case JOY_DOWN_OK:	/* ҡ��OK�� */
					/* �˳�,����ȫ������״̬ */
					*pMainStatus = S_EXIT;
					break;				

				case JOY_DOWN_U:		/* ҡ��UP������ */
					break;

				case JOY_DOWN_D:		/* ҡ��DOWN������ */
					break;
			
				default:
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DispFrame
*	����˵��: �ܣ���ʾ���δ��ڵı߿�Ϳ̶���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFrame(void)
{
	uint16_t x, y;

	/* ����һ��ʵ�߾��ο� x, y, h, w */
	LCD_DrawRect(9, 19, 202, 302, CL_WHITE);

	/* ���ƴ�ֱ�̶ȵ� */
	for (x = 0; x < 13; x++)
	{
		for (y = 0; y < 41; y++)
		{
			LCD_PutPixel(10 + (x * 25), 20 + (y * 5), CL_WHITE);
		}
	}

	/* ����ˮƽ�̶ȵ� */
	for (y = 0; y < 9; y++)
	{
		for (x = 0; x < 61; x++)
		{
			LCD_PutPixel(10 + (x * 5), 20 + (y * 25), CL_WHITE);
		}
	}

	/* ���ƴ�ֱ���Ŀ̶ȵ� */
	for (y = 0; y < 41; y++)
	{	 
		LCD_PutPixel(9 + (6 * 25), 20 + (y * 5), CL_WHITE);
		LCD_PutPixel(11 + (6 * 25), 20 + (y * 5), CL_WHITE);
	}

	/* ����ˮƽ���Ŀ̶ȵ� */
	for (x = 0; x < 61; x++)
	{	 
		LCD_PutPixel(10 + (x * 5), 19 + (4 * 25), CL_WHITE);
		LCD_PutPixel(10 + (x * 5), 21 + (4 * 25), CL_WHITE);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DispButton
*	����˵��: ��ʾ���δ����ұߵĹ��ܰ�ť������չ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispButton(void)
{

}

/*
*********************************************************************************************************
*	�� �� ��: DispCh1Wave
*	����˵��: ��ʾͨ��1����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispCh1Wave(void)
{
	int16_t i;		/* �з����� */
	//uint16_t pos;
	uint16_t *px;
	uint16_t *py;
	int16_t iTemp;

	if (g_DSO->Ch1Buf == 0)
	{
		return;	/* ��δ�ɼ�����ֱ�ӷ��� */
	}
	
	/* ��ʾͨ��1��ƽ��� */
	{
		static uint16_t y = 70;
	
		LCD_DrawLine(1, y, 7, y, CL_BLUE);	 /* ѡ����ɫ */

		y = g_DSO->Ch1VOffset;

		if (y < 20)
		{
			y = 20;
		}
		else if (y > 220)
		{
			y = 220;
		}
		LCD_DrawLine(1, y, 5, y, CL_YELLOW);
	}

//	if (s_DispFirst == 0)
//	{
//		s_DispFirst = 1;
//		LCD_ClrScr(CL_BLUE);  			/* ������������ɫ */
//	}

	if (g_DSO->BufUsed == 0)
	{
		g_DSO->BufUsed = 1;
	}
	else
	{
		g_DSO->BufUsed = 0;
	}

	if (g_DSO->BufUsed == 0)
	{
		px = g_DSO->xCh1Buf1;
		py = g_DSO->yCh1Buf1;
	}
	else
	{
		px = g_DSO->xCh1Buf2;
		py = g_DSO->yCh1Buf2;
	}

	/* ���㵱ǰ���µ�����λ�ã���ǰ�ݼ�400������ */
	//pos = SAMPLE_COUNT - DMA_GetCurrDataCounter(DMA1_Channel1);
	//pos = 0;

	for (i = 0; i < 300; i++)
	{
		px[i] = 10 + i;
		/* ADC = 2048 ��BNC��������ʱ��ADC��ֵ��ͳ�ƶ����ӻ�õ�  */
		iTemp = g_DSO->Ch1VOffset + (int16_t)((2048 - g_DSO->Ch1Buf[i + 1]) * 10) / g_DSO->Ch1Attenuation;

		if (iTemp > 220)
		{
			iTemp = 220;
		}
		else if (iTemp < 20)
		{
			iTemp = 20;
		}
		py[i] = iTemp;
	}

	/* �����֡���� */
	if (g_DSO->BufUsed == 0)
	{
		LCD_DrawPoints(g_DSO->xCh1Buf2, g_DSO->yCh1Buf2, 300, CL_BLUE);
	}
	else
	{
		LCD_DrawPoints(g_DSO->xCh1Buf1, g_DSO->yCh1Buf1, 300, CL_BLUE);
	}

	/* ��ʾ���µĲ��� */
	LCD_DrawPoints((uint16_t *)px, (uint16_t *)py, 300, CL_YELLOW);
}

/*
*********************************************************************************************************
*	�� �� ��: DispCh2Wave
*	����˵��: ��ʾͨ��2����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispCh2Wave(void)
{
	int16_t i;		/* �з����� */
	//uint16_t pos;
	uint16_t *px;
	uint16_t *py;
	int16_t iTemp;

	if (g_DSO->Ch2Buf == 0)
	{
		return;	/* ��δ�ɼ�����ֱ�ӷ��� */
	}
	
	/* ��ʾͨ��2��ƽ��� */
	{
		static uint16_t y = 170;

		LCD_DrawLine(1, y, 5, y, CL_BLUE);

		y = g_DSO->Ch2VOffset;

		if (y < 20)
		{
			y = 20;
		}
		else if (y > 220)
		{
			y = 220;
		}
		LCD_DrawLine(1, y, 5, y, CL_GREEN);
	}
		
	if (g_DSO->BufUsed == 0)
	{
		px = g_DSO->xCh2Buf1;
		py = g_DSO->yCh2Buf1;
	}
	else
	{
		px = g_DSO->xCh2Buf2;
		py = g_DSO->yCh2Buf2;
	}

	/* ���㵱ǰ���µ�����λ�ã���ǰ�ݼ�400������ */
	//pos = SAMPLE_COUNT - DMA_GetCurrDataCounter(DMA2_Channel5);
	//pos = 0;

	for (i = 0; i < 300; i++)
	{
		px[i] = 10 + i;
		
		/* ADC = 2048 ��BNC��������ʱ��ADC��ֵ��ͳ�ƶ����ӻ�õ�  */
		iTemp = g_DSO->Ch2VOffset + (int16_t)((2048 - g_DSO->Ch2Buf[i + 1]) * 10) / g_DSO->Ch2Attenuation;

		if (iTemp > 220)
		{
			iTemp = 220;
		}
		else if (iTemp < 20)
		{
			iTemp = 20;
		}
		py[i] = iTemp;
	}

	/* �����֡���� */
	if (g_DSO->BufUsed == 0)
	{
		LCD_DrawPoints(g_DSO->xCh2Buf2, g_DSO->yCh2Buf2, 300, CL_BLUE);
	}
	else
	{
		LCD_DrawPoints(g_DSO->xCh2Buf1, g_DSO->yCh2Buf1, 300, CL_BLUE);
	}
	/* ��ʾ���µĲ��� */
	LCD_DrawPoints((uint16_t *)px, (uint16_t *)py, 300, CL_GREEN);
}

/*
*********************************************************************************************************
*	�� �� ��: DispChInfo
*	����˵��: ��ʾͨ����Ϣ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispChInfo(void)
{
	char buf[32];   /* �ַ���ʾ������ */
	FONT_T font;

	/* ������������ */
	font.FontCode = FC_ST_16;
	font.BackColor = CL_BLUE;		/* �ͱ���ɫ��ͬ */
	font.FrontColor = CL_WHITE;		/* ��ɫ���� */
	font.Space = 0;	

	/* ��ʾʾ��������汾 */
	LCD_DispStr(10, 2, DSO_VER, &font);

	/* ��ʾͨ��1��Ϣ */
	if (g_DSO->CH1_DC == 1)
	{
		strcpy(buf, "CH1 DC ");
	}
	else
	{
		strcpy(buf, "CH1 AC ");
	}

	if (g_DSO->Ch1VScale >= 1000)
	{
		sprintf(&buf[7], "%d.00V", g_DSO->Ch1VScale / 1000);
	}
	else
	{
		sprintf(&buf[7], "%dmV", g_DSO->Ch1VScale);
	}

	if (g_DSO->ActiveCH == 1)
	{
		font.BackColor = CL_YELLOW;		/* ��ɫ */
		font.FrontColor = CL_MAGENTA;	/* ��ɫ */	
	}
	else
	{
		font.BackColor = CL_YELLOW;		/* ��ɫ */
		font.FrontColor = CL_BLUE;		/* ��ɫ */	
	}

	LCD_DispStr(10, 224, buf, &font);

	/* ��ʾͨ��2��Ϣ */
	font.FrontColor = CL_RED; /* CH2 ��ɫ */
	if (g_DSO->CH2_DC == 1)
	{
		strcpy(buf, "CH2 DC ");
	}
	else
	{
		strcpy(buf, "CH2 AC ");
	}

	if (g_DSO->Ch2VScale >= 1000)
	{
		sprintf(&buf[7], "%d.00V", g_DSO->Ch2VScale / 1000);
	}
	else
	{
		sprintf(&buf[7], "%dmV", g_DSO->Ch2VScale);
	}
	if (g_DSO->ActiveCH == 2)
	{
		font.BackColor = CL_GREEN;		/* ��ɫ */
		font.FrontColor = CL_MAGENTA;	/* ��ɫ */			
	}
	else
	{
		font.BackColor = CL_GREEN;		/* ��ɫ */
		font.FrontColor = CL_BLUE;		/* ��ɫ */			
	}
	LCD_DispStr(120, 224, buf, &font);

	/* ��ʾʱ�� */
	font.FrontColor = CL_WHITE;		/* ��ɫ */		
	font.BackColor = CL_BLUE;		/* ��ɫ */
		
	if (g_DSO->TimeBase < 1000)
	{
		sprintf(buf, "Time %3dus", g_DSO->TimeBase);
	}
	else if (g_DSO->TimeBase < 1000000)
	{
		sprintf(buf, "Time %3dms", g_DSO->TimeBase / 1000);
	}
	else
	{
		sprintf(buf, "Time %3ds ", g_DSO->TimeBase / 1000000);	
	}
	LCD_DispStr(230, 224,  buf, &font);


	/* ��ʾ����ģʽ */
	font.FrontColor = CL_WHITE;		/* ���� */		
	font.BackColor = CL_BLUE;		/* ���� */
	
	if (g_DSO->AdjustMode == 1)
	{
		LCD_DispStr(245, 2, "����λ��", &font);
	}
	else
	{
		LCD_DispStr(245, 2, "���ڷ���", &font);
	}

	sprintf(buf, "����Ƶ��:%7dHz",	g_DSO->SampleFreq);
	LCD_DispStr(75, 2, buf, &font);
}

/*
*********************************************************************************************************
*	�� �� ��: DispDSO
*	����˵��: DispDSO
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispDSO(void)
{
	DispButton();

	DispFrame();	/* ���ƿ̶ȿ� */
	DispChInfo();	/* ��ʾͨ����Ϣ(���ȣ�ʱ�䵵λ) */

	DispCh1Wave();	/* ��ʾ����1 */	
	DispCh2Wave();	/* ��ʾ����2 */	
}

/*
*********************************************************************************************************
*	�� �� ��: InitDsoParam
*	����˵��: ��ʼ��ȫ�ֲ�������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitDsoParam(void)
{
	g_DSO->Ch1Attenuation = 23;	/* ����ȱʡ˥��ϵ�� */
	g_DSO->Ch2Attenuation = 23;	/* ����ȱʡ˥��ϵ�� */
	g_DSO->Ch1VScale = 1000;	/* ȱʡ��1V */
	g_DSO->Ch2VScale = 1000;	/* ȱʡ��1V */

	g_DSO->Ch1VOffset = 70; 	/* ͨ��1 GND��λ�� */
	g_DSO->Ch2VOffset = 170; /* ͨ��2 GND��λ�� */

	g_DSO->ActiveCH = 1;		/* ȱʡ��CH1 */
	g_DSO->AdjustMode = 1;	/* ȱʡ�ǵ��ڴ�ֱƫ�ƣ� �����л���2���ڷ��� */

	g_DSO->HoldEn = 0;

	g_DSO->TimeBaseId = 2;									 
	g_DSO->TimeBase = 	g_TimeTable[g_DSO->TimeBaseId];
	g_DSO->SampleFreq = 25000000 / g_DSO->TimeBase;

	g_DSO->Ch1AttId = 2;
	g_DSO->Ch1Attenuation = g_AttTable[g_DSO->Ch1AttId][0];
	g_DSO->Ch1VScale =  g_AttTable[g_DSO->Ch1AttId][1];

	g_DSO->Ch2AttId = 2;
	g_DSO->Ch2Attenuation = g_AttTable[g_DSO->Ch2AttId][0];
	g_DSO->Ch2VScale =  g_AttTable[g_DSO->Ch2AttId][1];

}

/*
*********************************************************************************************************
*	�� �� ��: IncSampleFreq
*	����˵��: ���Ӳ���Ƶ�ʣ��� 1-2-5 
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
/*
	ʱ����ֶȣ�ÿ1������ʱ��)
		10us 	      2500000	
		20us 	      1250000
		50us 		   500000
		100us		   250000
		200us		   125000
		500us		    50000
		1ms				 2500
		2ms				 1250
		5ms				  500 
		10ms			  250
		20ms			  125
		50ms			   50
		100ms			   25

//		200ms			   12.5
//		500ms			    5

	g_DSO->TimeScale = 25000000 / g_DSO->SampleRate;
*/
static void IncSampleFreq(void)
{
	if (g_DSO->TimeBaseId < (sizeof(g_TimeTable) / 4) - 1)
	{
		g_DSO->TimeBaseId++;
	}
													 
	g_DSO->TimeBase = 	g_TimeTable[g_DSO->TimeBaseId];
	g_DSO->SampleFreq = 25000000 / g_DSO->TimeBase;

	DSO_SetSampRate(g_DSO->SampleFreq);	/* �ı����Ƶ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: DecSampleFreq
*	����˵��: ���Ͳ���Ƶ�ʣ��� 1-2-5 
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DecSampleFreq(void)
{
	if (g_DSO->TimeBaseId > 0)
	{
		g_DSO->TimeBaseId--;
	}
													 
	g_DSO->TimeBase = g_TimeTable[g_DSO->TimeBaseId];
	g_DSO->SampleFreq = 25000000 / g_DSO->TimeBase;

	DSO_SetSampRate(g_DSO->SampleFreq);	/* �ı����Ƶ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: AdjustAtt
*	����˵��: ���ڵ�ѹ˥����λ���� 1-2-5 
*	��    ��: ch   : ͨ���ţ�1��2
*			  mode : 0 ���ͣ� 1����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void AdjustAtt(uint8_t ch, uint8_t mode)
{

	if (ch == 1)
	{
		if (mode == 0) 	/* ���� */
		{
			if (g_DSO->Ch1AttId > 0)
			{
				g_DSO->Ch1AttId--;
			}
		}
		else		/* ���� */
		{
			if (g_DSO->Ch1AttId < ATT_COUNT - 1)
			{
				g_DSO->Ch1AttId++;
			}
		}

		g_DSO->Ch1Attenuation = g_AttTable[g_DSO->Ch1AttId][0];
		g_DSO->Ch1VScale =  g_AttTable[g_DSO->Ch1AttId][1];
	}
	else if (ch == 2)
	{
		if (mode == 0) 	/* ���� */
		{
			if (g_DSO->Ch2AttId > 0)
			{
				g_DSO->Ch2AttId--;
			}
		}
		else		/* ���� */
		{
			if (g_DSO->Ch2AttId < ATT_COUNT - 1)
			{
				g_DSO->Ch2AttId++;
			}
		}
		g_DSO->Ch2Attenuation = g_AttTable[g_DSO->Ch2AttId][0];
		g_DSO->Ch2VScale =  g_AttTable[g_DSO->Ch2AttId][1];
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DsoRun
*	����˵��: DSO����״̬
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DsoRun(uint8_t *pMainStatus)
{
	uint8_t KeyCode;
	uint8_t fRefresh = 1;	/* LCDˢ�±�־ */
	FormDSO_T form;	
	uint8_t ucTouch;
	int16_t tpX, tpY;	


	InitDsoParam();	/* ��ʼ��ʾ�������� */

	LCD_ClrScr(CL_BLUE);  			/* ������������ɫ */
	
	FormDSO = &form;

	InitFormDSO();	/* ���ư�ť */
	
	bsp_StartTimer(1, 150);		/* ������ʱ��1��100msˢ��1�� */
	while (*pMainStatus == S_RUN)
	{
		bsp_Idle();

		if (fRefresh)
		{
			fRefresh = 0;

			DSO_SetDC(1, g_DSO->CH1_DC);	
			DSO_SetDC(2, g_DSO->CH2_DC);	

			/* �Զ��л�Ӳ������ */	
			switch (g_DSO->Ch1AttId)
			{
				case 0:		/* 5V */				
					DSO_SetGain(1, 3);	
					break;
				
				case 1:		/* 2V */							
					DSO_SetGain(1, 2);	
					break;
				
				case 2:		/* 1V */					
				case 3:		/* 500mV */
					DSO_SetGain(1, 1);
					break;
				
				case 4:		/* 200mV */
				case 5:		/* 100mV */
					DSO_SetGain(1, 0);	
					break;				
			}
			
			switch (g_DSO->Ch2AttId)
			{
				case 0:		/* 5V */				
					DSO_SetGain(2, 3);	
					break;
				
				case 1:		/* 2V */							
					DSO_SetGain(2, 2);	
					break;
				
				case 2:		/* 1V */					
				case 3:		/* 500mV */
					DSO_SetGain(2, 1);
					break;
				
				case 4:		/* 200mV */
				case 5:		/* 100mV */
					DSO_SetGain(2, 0);	
					break;			
			}			
			
			/* �����������ã��ı�Ӳ��˥�� */			
			
			if (g_DSO->HoldEn == 1)
			{
				DispDSO();
			}
		}

		if (bsp_CheckTimer(1))
		{
			bsp_StartTimer(1, 200);		/* ������ʱ��1��200msˢ��1�� */

			/* ����״̬��ÿ��100msˢ��1�β��� */
			if (g_DSO->HoldEn == 0)
			{
				DSO_PauseADC();	/* ��ͣ���� */
				
				DispDSO();	/* ��ʾ���� */
				
				/* ��ʼ���� */			
				DSO_StartADC(&g_DSO->Ch1Buf, &g_DSO->Ch2Buf, g_DSO->SampleFreq);
			}
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					if (LCD_ButtonTouchDown(&FormDSO->BtnRet, tpX, tpY))
					{
						// *pMainStatus = S_EXIT;  <--- ���ɿ�ʱ�˳�����
					}
					else if (LCD_ButtonTouchDown(&FormDSO->Btn1, tpX, tpY))
					{
						/* ͨ��1 AC-DC����л� */
						if (g_DSO->CH1_DC == 0)
						{
							g_DSO->CH1_DC = 1;
						}
						else
						{
							g_DSO->CH1_DC = 0;
						}
						fRefresh = 1;
					}
					else if (LCD_ButtonTouchDown(&FormDSO->Btn2, tpX, tpY))
					{
						AdjustAtt(1, 1);	/* ͨ��1 ���ȵ���+ */
						fRefresh = 1;
					}				
					else if (LCD_ButtonTouchDown(&FormDSO->Btn3, tpX, tpY))
					{
						AdjustAtt(1, 0);	/* ͨ��1 ���ȵ���- */
						fRefresh = 1;
					}
					else if (LCD_ButtonTouchDown(&FormDSO->Btn4, tpX, tpY))
					{
						/* ͨ��2 AC-DC����л� */
						if (g_DSO->CH2_DC == 0)
						{
							g_DSO->CH2_DC = 1;
						}
						else
						{
							g_DSO->CH2_DC = 0;
						}
						fRefresh = 1;
					}
					else if (LCD_ButtonTouchDown(&FormDSO->Btn5, tpX, tpY))
					{
						AdjustAtt(2, 1);	/* ͨ��2 ���ȵ���+ */
						fRefresh = 1;
					}				
					else if (LCD_ButtonTouchDown(&FormDSO->Btn6, tpX, tpY))
					{
						AdjustAtt(2, 0);	/* ͨ��2 ���ȵ���- */
						fRefresh = 1;
					}	
					else if (LCD_ButtonTouchDown(&FormDSO->Btn7, tpX, tpY))
					{
						DecSampleFreq();	/* �ݼ�����Ƶ�� */
						fRefresh = 1;		/* ����ˢ��LCD */						
					}				
					else if (LCD_ButtonTouchDown(&FormDSO->Btn8, tpX, tpY))
					{
						IncSampleFreq();	/* ��������Ƶ�� */
						fRefresh = 1;		/* ����ˢ��LCD */
					}	
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (LCD_ButtonTouchRelease(&FormDSO->BtnRet, tpX, tpY))
					{
						*pMainStatus = S_EXIT;	/*�����ؼ��˳���*/
					}
					else
					{
						LCD_ButtonTouchRelease(&FormDSO->Btn1, tpX, tpY);		
						LCD_ButtonTouchRelease(&FormDSO->Btn2, tpX, tpY);	
						LCD_ButtonTouchRelease(&FormDSO->Btn3, tpX, tpY);	
						LCD_ButtonTouchRelease(&FormDSO->Btn4, tpX, tpY);	
						LCD_ButtonTouchRelease(&FormDSO->Btn5, tpX, tpY);	
						LCD_ButtonTouchRelease(&FormDSO->Btn6, tpX, tpY);
						LCD_ButtonTouchRelease(&FormDSO->Btn7, tpX, tpY);	
						LCD_ButtonTouchRelease(&FormDSO->Btn8, tpX, tpY);
					}
					break;
			}
		}
		
		/* ��ȡ����������0��ʾ�м����� */
		KeyCode = bsp_GetKey();
		if (KeyCode > 0)
		{
			/* �м����� */
			switch (KeyCode)
			{
				case  KEY_DOWN_K1:	/* TAMPER ����ͨ��ѡ��(CH1��CH2) */
					if (g_DSO->ActiveCH == 1)
					{
						g_DSO->ActiveCH = 2;
					}
					else
					{
						g_DSO->ActiveCH = 1;
					}
					fRefresh = 1;		/* ����ˢ��LCD */
					break;

				case  KEY_DOWN_K2:	/* WAKEUP ��, ����ģʽѡ��(���Ȼ��ߴ�ֱƫ��) */
					/* �˳�,����ȫ������״̬ */
					*pMainStatus = S_HELP;
					break;

				case  KEY_DOWN_K3:	/* USER �� */
					if (g_DSO->HoldEn == 0)
					{
						g_DSO->HoldEn = 1;

						/* ������ͣʱ��ʱ��,Ϊ��ˮƽ��չ�� */
						g_DSO->TimeBaseIdHold = g_DSO->TimeBaseId;	

						DSO_StopADC();
					}
					else
					{
						g_DSO->HoldEn = 0;	
					}
					fRefresh = 1;		/* ����ˢ��LCD */
					break;

				case JOY_DOWN_L:	/* ҡ��LEFT������ */
					if (g_DSO->HoldEn == 0)
					{
						DecSampleFreq();	/* �ݼ�����Ƶ�� */
						fRefresh = 1;		/* ����ˢ��LCD */
					}
					else
					{
						; /* ����ˮƽ�ƶ��������� */
					}
					break;

				case JOY_DOWN_R:	/* ҡ��RIGHT������ */
					if (g_DSO->HoldEn == 0)
					{
						IncSampleFreq();  	/* ��������Ƶ�� */					
						fRefresh = 1;		/* ����ˢ��LCD */
					}
					else
					{
						; /* ����ˮƽ�ƶ��������� */
					}
					break;

				case  JOY_DOWN_OK:	/* ҡ��OK�� */
					if (g_DSO->AdjustMode == 0)
					{
						g_DSO->AdjustMode = 1;
					}
					else
					{
						g_DSO->AdjustMode = 0;
					}
					fRefresh = 1;		/* ����ˢ��LCD */
					break;

				case JOY_DOWN_U:	/* ҡ��UP������ */
					if (g_DSO->ActiveCH == 1) 	/* ��ǰ�������CH1 */
					{
						if (g_DSO->AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
						{
							AdjustAtt(1, 1);
						}
						else 	/* ��������ƫ�� */
						{
							g_DSO->Ch1VOffset -= 5;
						}
					}
					else	/* ��ǰ�������CH2 */
					{
						if (g_DSO->AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
						{
							AdjustAtt(2, 1);
						}
						else 	/* ��������ƫ�� */
						{
							g_DSO->Ch2VOffset -= 5;
						}
					}
					fRefresh = 1;		/* ����ˢ��LCD */
					break;

				case JOY_DOWN_D:		/* ҡ��DOWN������ */
					if (g_DSO->ActiveCH == 1) 	/* ��ǰ�������CH1 */
					{
						if (g_DSO->AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
						{
							AdjustAtt(1, 0);
						}
						else 	/* ��������ƫ�� */
						{
							g_DSO->Ch1VOffset += 5;
						}
					}
					else	/* ��ǰ�������CH2 */
					{
						if (g_DSO->AdjustMode == 0)	/* ���ڷ��ȷŴ��� */
						{
							AdjustAtt(2, 0);
						}
						else 	/* ��������ƫ�� */
						{
							g_DSO->Ch2VOffset += 5;
						}
					}
					fRefresh = 1;		/* ����ˢ��LCD */
					break;

				default:
					break;
			}
		}
	}
	
	DSO_StopADC();	/* �رղ��� */
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
