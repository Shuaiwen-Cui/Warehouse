/*
*********************************************************************************************************
*
*	ģ������ : ¼����ʾ����
*	�ļ����� : audio_rec.c
*	��    �� : V1.2
*	˵    �� : ��ʾI2S¼���ͻطŹ��ܡ�ͨ�����ڳ����ն���Ϊ�������档
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2013-02-01 armfly  �׷�
*		V1.1	2014-11-04 armfly  ȥ��LED1ָʾ�ƵĿ��ơ���Ϊ���GPIO������3.5�紥������
*		V1.2	2015-01-08 armfly  �޸�StartPlay(void)�������������������þ����Ƿ��������
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

/*
	���������������õ�I2S��ƵCODECоƬΪWM8978��

	������ʾ��
		[TAMPER]��     = ѡ����Ƶ��ʽ
		[WAKEUP]��     = ��ʼ¼��
		[USER]��       = ��ʼ�ط�
		ҡ����/�¼�    = ��������
		ҡ����/�Ҽ�    = ����MIC����
		ҡ��OK��       = ��ֹ¼��

	¼��ʱ���ݱ����ڰ�����STM32������������2MB SRAM�У�ȱʡʹ��8K�����ʣ�16bit��ʽ��1MB���Ա���64��¼����
	��������ͬ�ĸ�ʽ�����ⲿSRAM�ж�ȡ���ݡ�
		
	WM8978֧�ֵĲ����� : 
		Support for 8, 11.025, 12, 16, 22.05, 24, 32, 44.1 and 48kHz sample rates
		
*/


#include "bsp.h"
#include "form_audio_rec.h"

#define STR_Help1	"ҡ����/�¼� = ���ڶ�������"
#define STR_Help2	"ҡ����/�Ҽ� = ������˷�����"
#define STR_Help3	"K1��       = ѡ����Ƶ��ʽ"

/* ���ذ�ť������(��Ļ���½�) */
#define BUTTON_RET_H	32
#define BUTTON_RET_W	60
#define	BUTTON_RET_X	(g_LcdWidth - BUTTON_RET_W - 4)
#define	BUTTON_RET_Y	(g_LcdHeight - BUTTON_RET_H - 4)
#define	BUTTON_RET_TEXT	"����"

#define BUTTON_REC_H	32
#define BUTTON_REC_W	74
#define	BUTTON_REC_X	5
#define	BUTTON_REC_Y	180
#define	BUTTON_REC_TEXT	"��ʼ¼��"

#define BUTTON_PLAY_H		32
#define BUTTON_PLAY_W		74
#define	BUTTON_PLAY_X		(BUTTON_REC_X + BUTTON_REC_W + 10)
#define	BUTTON_PLAY_Y		BUTTON_REC_Y
#define	BUTTON_PLAY_TEXT	"��ʼ����"

#define BUTTON_STOP_H		32
#define BUTTON_STOP_W		74
#define	BUTTON_STOP_X		(BUTTON_PLAY_X + BUTTON_PLAY_W + 10)
#define	BUTTON_STOP_Y		BUTTON_REC_Y
#define	BUTTON_STOP_TEXT	"ֹͣ"


/* �����Ǽ��� */
#define CHECK_SPK_X		BUTTON_REC_X
#define CHECK_SPK_Y		(BUTTON_REC_Y + BUTTON_REC_H + 5)
#define	CHECK_SPK_H		CHECK_BOX_H
#define	CHECK_SPK_W		(CHECK_BOX_W + 5 * 16)		/* ����������Ч���� */
#define CHECK_SPK_TEXT	"��������"

REC_T g_tRec;

/* ��Ƶ��ʽ�л��б�(�����Զ���) */
#define FMT_COUNT	9		/* ��Ƶ��ʽ����Ԫ�ظ��� */
uint32_t g_FmtList[FMT_COUNT][3] =
{	
	{I2S_STANDARD_PHILIPS, SAI_DATASIZE_16, I2S_AUDIOFREQ_8K},
	{I2S_STANDARD_PHILIPS, SAI_DATASIZE_16, I2S_AUDIOFREQ_11K},	
	{I2S_STANDARD_PHILIPS, SAI_DATASIZE_16, I2S_AUDIOFREQ_16K},
	{I2S_STANDARD_PHILIPS, SAI_DATASIZE_16, I2S_AUDIOFREQ_22K},		
	
	{I2S_STANDARD_PHILIPS, SAI_DATASIZE_16, I2S_AUDIOFREQ_32K},
	{I2S_STANDARD_PHILIPS, SAI_DATASIZE_16, I2S_AUDIOFREQ_44K},	
	{I2S_STANDARD_PHILIPS, SAI_DATASIZE_16, I2S_AUDIOFREQ_48K},
	{I2S_STANDARD_PHILIPS, SAI_DATASIZE_16, I2S_AUDIOFREQ_96K},
	{I2S_STANDARD_PHILIPS, SAI_DATASIZE_16, I2S_AUDIOFREQ_192K},
};

/* ����¼������������ */
#define REC_MEM_ADDR	SDRAM_APP_BUF
#define REC_MEM_SIZE	(2 * 1024 * 1024)

/* �������ļ��ڵ��õĺ������� */
static void DispStatus(void);

static void StartPlay(void);
static void StartRecord(void);
static void StopRec(void);

/*
*********************************************************************************************************
*	�� �� ��: RecorderDemo
*	����˵��: ¼����������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RecorderDemo(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fRefresh;		/* ˢ�������־,1��ʾ��Ҫˢ�� */
	uint8_t fQuit = 0;
	FONT_T tFont, tFontBtn, tFontChk;			/* ����һ������ṹ���������������������� */

	char buf[128];
	uint16_t x, y;
	uint16_t usLineCap = 18;

	int16_t tpX, tpY;
	BUTTON_T tBtn;
	CHECK_T tCheck;

	LCD_ClrScr(CL_BLUE);  	/* ������������ɫ */
	
	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;	/* ������� 16���� */
		tFont.FrontColor = CL_WHITE;	/* ������ɫ */
		tFont.BackColor = CL_BLUE;	/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */

		/* ��ť���� */
		tFontBtn.FontCode = FC_ST_16;
		tFontBtn.BackColor = CL_MASK;	/* ͸��ɫ */
		tFontBtn.FrontColor = CL_BLACK;
		tFontBtn.Space = 0;

		/* CheckBox ���� */
		tFontChk.FontCode = FC_ST_16;
		tFontChk.BackColor = CL_MASK;	/* ͸��ɫ */
		tFontChk.FrontColor = CL_YELLOW;
		tFontChk.Space = 0;
	}

	x = 5;
	y = 3;
	LCD_DispStr(x, y, "WM8978¼����", &tFont);			/* ��(8,3)���괦��ʾһ������ */
	y += usLineCap;

	/* ����I2C�豸 */
	{
		if (i2c_CheckDevice(WM8978_SLAVE_ADDRESS) == 0)
		{
			sprintf(buf, "WM8978 Ok (0x%02X)", WM8978_SLAVE_ADDRESS);
			LCD_DispStr(x, y, buf, &tFont);
		}
		else
		{
			sprintf(buf, "WM8978 Err (0x%02X)", WM8978_SLAVE_ADDRESS);

			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;

		tFont.FrontColor = CL_YELLOW;
		LCD_DispStr(x, y, STR_Help1, &tFont);
		y += usLineCap;

		LCD_DispStr(x, y, STR_Help2, &tFont);
		y += usLineCap;

		LCD_DispStr(x, y, STR_Help3, &tFont);
		y += usLineCap;

		tFont.FrontColor = CL_WHITE;
	}

	/* �����ⲿSRAM�ӿ��Ѿ��� bsp.c �е� bsp_Init() ����ִ�й� */

	/* ���WM8978оƬ���˺������Զ�����CPU��GPIO */
	if (!wm8978_Init())
	{
		bsp_DelayMS(500);
		return;
	}

	/* ��ʾ��ť */
	{
		tBtn.Font = &tFontBtn;

		tBtn.Left = BUTTON_RET_X;
		tBtn.Top = BUTTON_RET_Y;
		tBtn.Height = BUTTON_RET_H;
		tBtn.Width = BUTTON_RET_W;
		tBtn.Focus = 0;		/* δѡ�� */
		tBtn.pCaption = BUTTON_RET_TEXT;
		LCD_DrawButton(&tBtn);

		tBtn.Left = BUTTON_REC_X;
		tBtn.Top = BUTTON_REC_Y;
		tBtn.Height = BUTTON_REC_H;
		tBtn.Width = BUTTON_REC_W;
		tBtn.Focus = 0;		/* ʧȥ���� */
		tBtn.pCaption = BUTTON_REC_TEXT;
		LCD_DrawButton(&tBtn);

		tBtn.Left = BUTTON_PLAY_X;
		tBtn.Top = BUTTON_PLAY_Y;
		tBtn.Height = BUTTON_PLAY_H;
		tBtn.Width = BUTTON_PLAY_W;
		tBtn.Focus = 0;		/* ʧȥ���� */
		tBtn.pCaption = BUTTON_PLAY_TEXT;
		LCD_DrawButton(&tBtn);

		tBtn.Left = BUTTON_STOP_X;
		tBtn.Top = BUTTON_STOP_Y;
		tBtn.Height = BUTTON_STOP_H;
		tBtn.Width = BUTTON_STOP_W;
		tBtn.Focus = 0;		/* ʧȥ���� */
		tBtn.pCaption = BUTTON_STOP_TEXT;
		LCD_DrawButton(&tBtn);

		/* ��ʾ���� */
		tCheck.Font = &tFontChk;

		tCheck.Left = CHECK_SPK_X;
		tCheck.Top = CHECK_SPK_Y;
		tCheck.Height = CHECK_SPK_H;
		tCheck.Width = CHECK_SPK_W;
		if (g_tRec.ucSpkOutEn == 1)
		{
			tCheck.Checked = 1;
		}
		else
		{
			tCheck.Checked = 0;
		}
		tCheck.pCaption = CHECK_SPK_TEXT;
		LCD_DrawCheckBox(&tCheck);
	}


	/* ��ʼ��ȫ�ֱ��� */
	g_tRec.ucVolume = 52;		/* ȱʡ���� */
	g_tRec.ucMicGain = 34;			/* ȱʡPGA���� */
	

	
	fRefresh = 1;

	g_tRec.ucFmtIdx = 1;		/* ȱʡ��Ƶ��ʽ(16Bit, 16KHz) */
	g_tRec.pAudio = (int16_t *)SDRAM_APP_BUF;	/* ͨ�� init16_t * ��ָ������ⲿSRAM */

	/* ����¼��������. SRAM����2M�ֽ� */
//	{
//		int i;

//		for (i = 0 ; i < 1 * 1024 * 1024; i++)
//		{
//			g_tRec.pAudio[i] = 0;
//		}
//	}

	/* �������Ҳ����� */
	AUDIO_MakeSine16bit((int16_t *)REC_MEM_ADDR, 1000, 16000, 16000*5);

	g_tRec.ucStatus = STA_IDLE;		/* ���Ƚ������״̬ */

	/* ����������ѭ���� */
	while (fQuit == 0)
	{
		bsp_Idle();

		AUDIO_Poll();
		
		/* ���д�����ʾ����ˢ�� */
		if (fRefresh == 1)
		{
			fRefresh = 0;
			DispStatus();		/* ��ʾ��ǰ״̬��Ƶ�ʣ������� */
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					if (TOUCH_InRect(tpX, tpY, BUTTON_RET_X, BUTTON_RET_Y, BUTTON_RET_H, BUTTON_RET_W))
					{
						tBtn.Left = BUTTON_RET_X;
						tBtn.Top = BUTTON_RET_Y;
						tBtn.Height = BUTTON_RET_H;
						tBtn.Width = BUTTON_RET_W;
						tBtn.Focus = 1;	/* ���� */
						tBtn.pCaption = BUTTON_RET_TEXT;
						LCD_DrawButton(&tBtn);
					}
					else if (TOUCH_InRect(tpX, tpY, BUTTON_REC_X, BUTTON_REC_Y, BUTTON_REC_H, BUTTON_REC_W))
					{
						tBtn.Left = BUTTON_REC_X;
						tBtn.Top = BUTTON_REC_Y;
						tBtn.Height = BUTTON_REC_H;
						tBtn.Width = BUTTON_REC_W;
						tBtn.Focus = 1;		/* ���� */
						tBtn.pCaption = BUTTON_REC_TEXT;
						LCD_DrawButton(&tBtn);
					}
					else if (TOUCH_InRect(tpX, tpY, BUTTON_PLAY_X, BUTTON_PLAY_Y, BUTTON_PLAY_H, BUTTON_PLAY_W))
					{
						tBtn.Left = BUTTON_PLAY_X;
						tBtn.Top = BUTTON_PLAY_Y;
						tBtn.Height = BUTTON_PLAY_H;
						tBtn.Width = BUTTON_PLAY_W;
						tBtn.Focus = 1;		/* ���� */
						tBtn.pCaption = BUTTON_PLAY_TEXT;
						LCD_DrawButton(&tBtn);
					}
					else if (TOUCH_InRect(tpX, tpY, BUTTON_STOP_X, BUTTON_STOP_Y, BUTTON_STOP_H, BUTTON_STOP_W))
					{
						tBtn.Left = BUTTON_STOP_X;
						tBtn.Top = BUTTON_STOP_Y;
						tBtn.Height = BUTTON_STOP_H;
						tBtn.Width = BUTTON_STOP_W;
						tBtn.Focus = 1;		/* ���� */
						tBtn.pCaption = BUTTON_STOP_TEXT;
						LCD_DrawButton(&tBtn);
					}
					else if (TOUCH_InRect(tpX, tpY, CHECK_SPK_X, CHECK_SPK_Y, CHECK_SPK_H, CHECK_SPK_W))
					{
						if (g_tRec.ucSpkOutEn)
						{
							g_tRec.ucSpkOutEn = 0;
							tCheck.Checked = 0;

							/* ����WM8978оƬ������ΪAUX�ӿ�(������)�����Ϊ���� */
							/* �� StartPlay() �����ڲ����� */
						}
						else
						{
							g_tRec.ucSpkOutEn = 1;
							tCheck.Checked = 1;

							/* ����WM8978оƬ������ΪAUX�ӿ�(������)�����Ϊ���� �� ������ */
							/* �� StartPlay() �����ڲ����� */
						}

						tCheck.Left = CHECK_SPK_X;
						tCheck.Top = CHECK_SPK_Y;
						tCheck.Height = CHECK_SPK_H;
						tCheck.Width = CHECK_SPK_W;
						tCheck.pCaption = CHECK_SPK_TEXT;
						LCD_DrawCheckBox(&tCheck);
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (TOUCH_InRect(tpX, tpY, BUTTON_RET_X, BUTTON_RET_Y, BUTTON_RET_H, BUTTON_RET_W))
					{
						tBtn.Font = &tFontBtn;

						tBtn.Left = BUTTON_RET_X;
						tBtn.Top = BUTTON_RET_Y;
						tBtn.Height = BUTTON_RET_H;
						tBtn.Width = BUTTON_RET_W;
						tBtn.Focus = 1;	/* ���� */
						tBtn.pCaption = BUTTON_RET_TEXT;
						LCD_DrawButton(&tBtn);

						fQuit = 1;	/* ���� */
					}
					else if (TOUCH_InRect(tpX, tpY, BUTTON_REC_X, BUTTON_REC_Y, BUTTON_REC_H, BUTTON_REC_W))
					{
						tBtn.Font = &tFontBtn;

						tBtn.Left = BUTTON_REC_X;
						tBtn.Top = BUTTON_REC_Y;
						tBtn.Height = BUTTON_REC_H;
						tBtn.Width = BUTTON_REC_W;
						tBtn.Focus = 0;		/* ʧȥ���� */
						tBtn.pCaption = BUTTON_REC_TEXT;
						LCD_DrawButton(&tBtn);

						StartRecord();	/* ��ʼ¼�� */
						fRefresh = 1;
					}
					else if (TOUCH_InRect(tpX, tpY, BUTTON_PLAY_X, BUTTON_PLAY_Y, BUTTON_PLAY_H, BUTTON_PLAY_W))
					{
						tBtn.Font = &tFontBtn;

						tBtn.Left = BUTTON_PLAY_X;
						tBtn.Top = BUTTON_PLAY_Y;
						tBtn.Height = BUTTON_PLAY_H;
						tBtn.Width = BUTTON_PLAY_W;
						tBtn.Focus = 0;		/* ʧȥ���� */
						tBtn.pCaption = BUTTON_PLAY_TEXT;
						LCD_DrawButton(&tBtn);

						StartPlay();	/* ��ʼ���� */
						fRefresh = 1;
					}
					else if (TOUCH_InRect(tpX, tpY, BUTTON_STOP_X, BUTTON_STOP_Y, BUTTON_STOP_H, BUTTON_STOP_W))
					{
						tBtn.Left = BUTTON_STOP_X;
						tBtn.Top = BUTTON_STOP_Y;
						tBtn.Height = BUTTON_STOP_H;
						tBtn.Width = BUTTON_STOP_W;
						tBtn.Focus = 0;		/* ʧȥ���� */
						tBtn.pCaption = BUTTON_STOP_TEXT;
						LCD_DrawButton(&tBtn);

						StopRec();		/* ֹͣ¼���ͷ��� */
						fRefresh = 1;
					}
					else	/* ��ťʧȥ���� */
					{
						tBtn.Font = &tFontBtn;

						tBtn.Focus = 0;		/* δѡ�� */

						tBtn.Left = BUTTON_RET_X;
						tBtn.Top = BUTTON_RET_Y;
						tBtn.Height = BUTTON_RET_H;
						tBtn.Width = BUTTON_RET_W;
						tBtn.pCaption = BUTTON_RET_TEXT;
						LCD_DrawButton(&tBtn);

						tBtn.Left = BUTTON_REC_X;
						tBtn.Top = BUTTON_REC_Y;
						tBtn.Height = BUTTON_REC_H;
						tBtn.Width = BUTTON_REC_W;
						tBtn.pCaption = BUTTON_REC_TEXT;
						LCD_DrawButton(&tBtn);

						tBtn.Left = BUTTON_PLAY_X;
						tBtn.Top = BUTTON_PLAY_Y;
						tBtn.Height = BUTTON_PLAY_H;
						tBtn.Width = BUTTON_PLAY_W;
						tBtn.pCaption = BUTTON_PLAY_TEXT;
						LCD_DrawButton(&tBtn);

						tBtn.Left = BUTTON_STOP_X;
						tBtn.Top = BUTTON_STOP_Y;
						tBtn.Height = BUTTON_STOP_H;
						tBtn.Width = BUTTON_STOP_W;
						tBtn.pCaption = BUTTON_STOP_TEXT;
						LCD_DrawButton(&tBtn);
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
				case KEY_DOWN_K1:		/* K1���л���Ƶ��ʽ�����´ο�ʼ¼���ͷ���ʱ��Ч */
					if (++g_tRec.ucFmtIdx >= FMT_COUNT)
					{
						g_tRec.ucFmtIdx = 0;
					}
					fRefresh = 1;
					break;

				case KEY_DOWN_K2:		/* K2�����£�¼�� */
					StartRecord();
					fRefresh = 1;
					break;

				case KEY_DOWN_K3:		/* K3�����£����� */
					StartPlay();
					fRefresh = 1;
					break;

				case JOY_DOWN_U:		/* ҡ��UP������ */
					if (g_tRec.ucVolume <= VOLUME_MAX - VOLUME_STEP)
					{
						g_tRec.ucVolume += VOLUME_STEP;
						wm8978_SetEarVolume(g_tRec.ucVolume);
						wm8978_SetSpkVolume(g_tRec.ucVolume);
						fRefresh = 1;
					}
					break;

				case JOY_DOWN_D:		/* ҡ��DOWN������ */
					if (g_tRec.ucVolume >= VOLUME_STEP)
					{
						g_tRec.ucVolume -= VOLUME_STEP;
						wm8978_SetEarVolume(g_tRec.ucVolume);
						wm8978_SetSpkVolume(g_tRec.ucVolume);
						fRefresh = 1;
					}
					break;

				case JOY_DOWN_L:		/* ҡ��LEFT������ */
					if (g_tRec.ucMicGain >= GAIN_STEP)
					{
						g_tRec.ucMicGain -= GAIN_STEP;
						wm8978_SetMicGain(g_tRec.ucMicGain);
						fRefresh = 1;
					}
					break;

				case JOY_DOWN_R:	/* ҡ��RIGHT������ */
					if (g_tRec.ucMicGain <= GAIN_MAX - GAIN_STEP)
					{
						g_tRec.ucMicGain += GAIN_STEP;
						wm8978_SetMicGain(g_tRec.ucMicGain);
						fRefresh = 1;
					}
					break;

				case JOY_DOWN_OK:		/* ҡ��OK������ */
					StopRec();		/* ֹͣ¼���ͷ��� */
					fRefresh = 1;
					break;

				default:
					break;
			}
		}
		
		/* ������Ϣ */
		{
			MSG_T msg;
			
			if (bsp_GetMsg(&msg))
			{
				switch (msg.MsgCode)
				{
					case MSG_WM8978_DMA_END:
						StopRec();		/* ֹͣ¼���ͷ��� */
						fRefresh = 1;						
						break;
					
					default:
						break;
				}
			}
		}
	}

	StopRec();		/* ֹͣ¼���ͷ��� */
}

/*
*********************************************************************************************************
*	�� �� ��: StartPlay
*	����˵��: ����WM8978��STM32��I2S��ʼ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void StartPlay(void)
{
	/* ����Ѿ���¼���ͷ���״̬������Ҫ��ֹͣ�ٿ��� */
	AUDIO_Stop();		/* ֹͣI2S¼���ͷ��� */
	wm8978_Init();	/* ��λWM8978����λ״̬ */
	
	bsp_DelayMS(20);	/* �ӳ�һ��ʱ�䣬�ȴ�I2S�жϽ��� */
	

	g_tRec.ucStatus = STA_PLAYING;		/* ����״̬ */

	g_tRec.uiCursor = 0;		/* ����ָ�븴λΪ0����ͷ��ʼ���� */

	/* ����WM8978оƬ������ΪDAC�����Ϊ���� */
	if (g_tRec.ucSpkOutEn == 1)
	{
		wm8978_CfgAudioPath(DAC_ON, EAR_LEFT_ON | EAR_RIGHT_ON | SPK_ON);	/* ��������  */
	}
	else
	{
		wm8978_CfgAudioPath(DAC_ON, EAR_LEFT_ON | EAR_RIGHT_ON);	/* �ر������� */
	}

	/* ����������������ͬ���� */
	wm8978_SetEarVolume(g_tRec.ucVolume);
	wm8978_SetSpkVolume(g_tRec.ucVolume);

	/* ����WM8978��Ƶ�ӿ�Ϊ�����ֱ�׼I2S�ӿڣ�16bit */
	wm8978_CfgAudioIF(I2S_STANDARD_PHILIPS, 16);

	/* ����STM32��I2S��Ƶ�ӿ�(��������ֱ�׼I2S�ӿڣ�16bit�� 8K������), ��ʼ���� */	
	AUDIO_Init(1, I2S_STANDARD_PHILIPS, g_FmtList[g_tRec.ucFmtIdx][1], g_FmtList[g_tRec.ucFmtIdx][2]);

	{
		int16_t *pWave = (int16_t *)REC_MEM_ADDR;
		
		AUDIO_Play(pWave, AUDIO_GetRecordSampleCount());
	}
}

/*
*********************************************************************************************************
*	�� �� ��: StartRecord
*	����˵��: ����WM8978��STM32��I2S��ʼ¼����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void StartRecord(void)
{	
	/* ����Ѿ���¼���ͷ���״̬������Ҫ��ֹͣ�ٿ��� */	
	AUDIO_Stop();		/* ֹͣI2S¼���ͷ��� */
	wm8978_Init();	/* ��λWM8978����λ״̬ */
	
	bsp_DelayMS(20);	/* �ӳ�һ��ʱ�䣬�ȴ�I2S�жϽ��� */

	g_tRec.ucStatus = STA_RECORDING;		/* ¼��״̬ */

	g_tRec.uiCursor = 0;	/* ����ָ�븴λΪ0����ͷ��ʼ¼�� */

	/* ����WM8978оƬ������ΪMic�����Ϊ���� */
	//wm8978_CfgAudioPath(MIC_LEFT_ON | ADC_ON, EAR_LEFT_ON | EAR_RIGHT_ON);
	//wm8978_CfgAudioPath(MIC_RIGHT_ON | ADC_ON, EAR_LEFT_ON | EAR_RIGHT_ON);
	wm8978_CfgAudioPath(MIC_LEFT_ON | MIC_RIGHT_ON | ADC_ON, EAR_LEFT_ON | EAR_RIGHT_ON);

	/* ���ڷ���������������ͬ���� */
	wm8978_SetEarVolume(g_tRec.ucVolume);

	/* ����MICͨ������ */
	wm8978_SetMicGain(g_tRec.ucMicGain);

	/* ����WM8978��Ƶ�ӿ�Ϊ�����ֱ�׼I2S�ӿڣ�16bit */
	wm8978_CfgAudioIF(I2S_STANDARD_PHILIPS, 16);

	/* ����STM32��I2S��Ƶ�ӿ�(��������ֱ�׼I2S�ӿڣ�16bit�� 8K������), ��ʼ���� */	
	AUDIO_Init(3, I2S_STANDARD_PHILIPS, g_FmtList[g_tRec.ucFmtIdx][1], g_FmtList[g_tRec.ucFmtIdx][2]);

	{
		int16_t *pWave1 = (int16_t *)REC_MEM_ADDR;
		int16_t *pWave2 = (int16_t *)(REC_MEM_ADDR + 1*1024*1024);
		
		AUDIO_Play(pWave2, 1*1024*1024);
		AUDIO_Record(pWave1, 1*1024*1024);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: StopRec
*	����˵��: ֹͣ¼���ͷ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void StopRec(void)
{
	g_tRec.ucStatus = STA_IDLE;		/* ����״̬ */
	AUDIO_Stop();		/* ֹͣI2S¼���ͷ��� */
	wm8978_Init();	/* ��λWM8978����λ״̬ */
}

/*
*********************************************************************************************************
*	�� �� ��: DispStatus
*	����˵��: ��ʾ��ǰ״̬
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispStatus(void)
{
	FONT_T tFont;
	char buf[128];
	uint16_t x, y;

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;	/* ������� 16���� */
		tFont.FrontColor = CL_WHITE;	/* ������ɫ */
		tFont.BackColor = CL_BLUE;	/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}

	x = 5;
	y = 100;
	/* ��ʾ��ǰ��Ƶ��ʽ */
	sprintf(buf, "��Ƶ��ʽ: Philips,16Bit,%d.%dkHz     ",
		g_FmtList[g_tRec.ucFmtIdx][2]/1000,
		(g_FmtList[g_tRec.ucFmtIdx][2]%1000)/100
		);

	sprintf(&buf[strlen(buf)], "¼��ʱ��: %d.%03d��    ",
		(REC_MEM_SIZE / 2) / g_FmtList[g_tRec.ucFmtIdx][2],
		(((REC_MEM_SIZE / 2) * 1000) / g_FmtList[g_tRec.ucFmtIdx][2]) % 1000
		);

	LCD_DispStr(x, y, buf, &tFont);
	y += 18;

	sprintf(buf, "��˷����� = %d ", g_tRec.ucMicGain);
	sprintf(&buf[strlen(buf)], "�������� = %d         ", g_tRec.ucVolume);
	LCD_DispStr(x, y, buf, &tFont);
	y += 18;

	if (g_tRec.ucStatus == STA_IDLE)
	{
		sprintf(buf, "״̬ = ����    ");
	}
	else if (g_tRec.ucStatus == STA_RECORDING)
	{
		sprintf(buf,"״̬ = ����¼��");
	}
	else if (g_tRec.ucStatus == STA_PLAYING)
	{
		sprintf(buf,"״̬ = ���ڻط�");
	}
	LCD_DispStr(x, y, buf, &tFont);
}



/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
