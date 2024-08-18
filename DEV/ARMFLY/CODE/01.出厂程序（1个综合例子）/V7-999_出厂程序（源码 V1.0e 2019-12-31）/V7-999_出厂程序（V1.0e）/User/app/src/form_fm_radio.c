/*
*********************************************************************************************************
*
*	ģ������ : ���������ԡ�
*	�ļ����� : fm_radio.c
*	��    �� : V1.1
*	˵    �� : ��Ҫ���ڲ���Si4730����оƬ�Ĺ���
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-01-01  armfly  ��ʽ����
*		V1.1    2014-09-06  armfly  �����������ں�����WM8978��Si47XXоƬ������ͬ�����ڡ�
*								    Volume ������ʾ����. ֧��Si4704�� ����FM ��г������ʾ��
*
*	Copyright (C), 2014-2015, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"			/* printf����������������ڣ����Ա����������ļ� */
#include "form_fm_radio.h"
#include "param.h"

/* ���ذ�ť������(��Ļ���½�) */
#define BUTTON_RET_H	32
#define BUTTON_RET_W	60
#define	BUTTON_RET_X	(g_LcdWidth - BUTTON_RET_W - 4)
#define	BUTTON_RET_Y	(g_LcdHeight - BUTTON_RET_H - 4)

#define BUTTON_RET2_H	32
#define BUTTON_RET2_W	120
#define	BUTTON_RET2_X	(BUTTON_RET_X - BUTTON_RET2_W - 5)
#define	BUTTON_RET2_Y	BUTTON_RET_Y
#define	BUTTON_RET2_TEXT	"��������,����"

#define BUTTON_FM_H	32
#define BUTTON_FM_W	60
#define	BUTTON_FM_X	5
#define	BUTTON_FM_Y	120

#define BUTTON_AM_H	32
#define BUTTON_AM_W	60
#define	BUTTON_AM_X	(BUTTON_FM_X + 70)
#define	BUTTON_AM_Y	BUTTON_FM_Y

#define BUTTON_VOLP_H	32
#define BUTTON_VOLP_W	48
#define BUTTON_VOLP_X	32
#define BUTTON_VOLP_Y	48

#define BUTTON_CHP_H	32
#define BUTTON_CHP_W	48
#define BUTTON_CHP_X	32
#define BUTTON_CHP_Y	48

#define	BUTTON_FREQ_	(BUTTON_FM_X + 70)
#define	BUTTON__Y	BUTTON_FM_Y

/* �����Ǽ��� */
#define CHECK_SPK_X		BUTTON_FM_X
#define CHECK_SPK_Y		(BUTTON_FM_Y + BUTTON_FM_H + 5)
#define	CHECK_SPK_H		CHECK_BOX_H
#define	CHECK_SPK_W		(CHECK_BOX_W + 5 * 16)		/* ����������Ч���� */
#define CHECK_SPK_TEXT	"��������"

#define CHECK_RSSI_X	BUTTON_FM_X
#define CHECK_RSSI_Y	(CHECK_SPK_Y + 30)
#define	CHECK_RSSI_H	CHECK_BOX_H
#define	CHECK_RSSI_W	(CHECK_BOX_W + 14 * 16)		/* ����������Ч���� */
#define CHECK_RSSI_TEXT	"��ʾ�ź���������������������"

#define CHECK_LIST_X	BUTTON_FM_X
#define CHECK_LIST_Y	(CHECK_RSSI_Y + 30)
#define	CHECK_LIST_H	CHECK_BOX_H
#define	CHECK_LIST_W	(CHECK_BOX_W + 5 * 16)		/* ����������Ч���� */
#define CHECK_LIST_TEXT	"ѡ��ȫ����̨�б�"

/*
AM873��FM88.4���人����㲥��̨�������ۺ�Ƶ�ʣ�



	�人����FM��̨Ƶ�ʱ�
	88.4

	FM89.6 �人�㲥��̨��̨ͨ
		90.7
		91.2
	FM91.6 ����㲥��̨����̨
	
	FM92.7 ����㲥��̨��ͨ����̨
	FM93.6 �人�㲥��̨�в�̨
	FM95.6 ����㲥��̨�й�֮��
	96.0==
	FM96.6 �����㲥��̨����Ƶ��
	
	FM97.8 ����㲥��̨����֮��
	
	FM99.8 �����㲥��̨����Ƶ��
	FM100.6 �������ù㲥��̨
	FM101.8 �人�㲥��̨����̨
	FM102.6 �����㲥��̨��Ů��ͯƵ��
	FM103.8 �����㲥��̨����Ƶ��
	FM104.6 �����㲥��̨�в�Ƶ��
	FM105.8 ����㲥��̨����̨
	FM107.8 �����㲥��̨��ͨƵ��
*/

const uint16_t g_InitListFM[] = {
	8640,
	8840,
	8960,
	9060,
	9120,
	9270,
	9360,
	9440,
	9560,
	9660,
	9780,
	9980,
	10060,
	10180,
	10260,
	10380,
	10460,
	10580,
	10780	
};

const uint16_t g_InitListAM[] = {
	531,
	540,
	549,
	558,
	603,
	639,
	855,
	873,
	882,
	900,
	909,
	918,
	927,
};

RADIO_T g_tRadio;

static void radio_DispStatus(void);
static void radio_SignalQuality(void);
static void radio_FM_FreqList(uint8_t _ucAll);
static void radio_AM_FreqList(uint8_t _ucAll);
static void radio_LoadParam(void);
static void radio_SaveParam(void);
static void radio_AdjustVolume(uint8_t _ucVolume);

/*
*********************************************************************************************************
*	�� �� ��: RadioMain
*	����˵��: ������������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void RadioMain(void)
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

	radio_LoadParam();	/* ��ȡ��̨���������� */
	
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
	LCD_DispStr(x, y, "Si4730/Si4704 ������", &tFont);			/* ��(8,3)���괦��ʾһ������ */
	y += usLineCap;

	/* ����I2C�豸 */
	{
		if (i2c_CheckDevice(I2C_ADDR_SI4730_W) == 0)
		{
			sprintf(buf, "Si47xx Ok (0x%02X)", I2C_ADDR_SI4730_W);
			printf("%s\r\n", buf);
			LCD_DispStr(x, y, buf, &tFont);

			wm8978_OutMute(1);		/* �Ⱦ������, �����ϵ������ */
			
			SI4730_PowerUp_FM_Revice();
			bsp_DelayMS(10);
			
			y += usLineCap;
			/* ��ӡоƬ�İ汾 */
			{
				uint8_t read_buf[8];

				if (SI4730_GetRevision(read_buf))
				{
					sprintf(buf, "%d-%c%c-%02X%02X-%c%c-%c", 	/* Si4730 */
						read_buf[0], read_buf[1], read_buf[2],	/* �̼��汾 */
						read_buf[3], read_buf[4],		/* ����ID */
						read_buf[5], read_buf[6],		/* ����汾 */
						read_buf[7]			/* оƬӲ���汾 */
						);					/* оƬ�ͺű�ʶ */

					if (strcmp(buf, "30-60-0000-70-D") == 0)
					{
						g_tRadio.ChipType = SI4730;
						
						LCD_DispStr(x, y, buf, &tFont);
						LCD_DispStr(x + 130, y, "Si4730", &tFont);
					}
					else if (strcmp(buf, "4-60-0000-70-D") == 0 ||
						strcmp(buf, "4-20-0000-20-B") == 0)
					{
						g_tRadio.ChipType = SI4704;
						LCD_DispStr(x, y, buf, &tFont);
						LCD_DispStr(x + 130, y, "Si4704", &tFont);
						
						g_tRadio.ucMode = FM_RX;
						
						//SI4704_SetFMIntput(1);	/* �л�����ΪPCB���� */
						// 0 ��ʾFM��������(��������)  1 ��ʾLPI��������(PCB����)
						SI4704_SetFMIntput(0);	/* �л�����Ϊ�������� */
					}
					else
					{
						tFont.FrontColor = CL_RED;
						LCD_DispStr(x, y, buf, &tFont);
						tFont.FrontColor = CL_WHITE;
					}
				}
			}			
		}
		else
		{
			sprintf(buf, "Si47xx Err (0x%02X)", I2C_ADDR_SI4730_W);
			printf("%s\r\n", buf);

			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;
	}

	/* ��ʾ��ť */
	{
		tBtn.Font = &tFontBtn;

		tBtn.Left = BUTTON_RET_X;
		tBtn.Top = BUTTON_RET_Y;
		tBtn.Height = BUTTON_RET_H;
		tBtn.Width = BUTTON_RET_W;
		tBtn.Focus = 0;		/* δѡ�� */
		tBtn.pCaption = "����";
		LCD_DrawButton(&tBtn);

		tBtn.Left = BUTTON_RET2_X;
		tBtn.Top = BUTTON_RET2_Y;
		tBtn.Height = BUTTON_RET2_H;
		tBtn.Width = BUTTON_RET2_W;
		tBtn.Focus = 0;		/* δѡ�� */
		tBtn.pCaption = BUTTON_RET2_TEXT;	/* �������� ���� */
		LCD_DrawButton(&tBtn);		

		if (g_tRadio.ChipType == SI4730)
		{
			tBtn.Left = BUTTON_FM_X;
			tBtn.Top = BUTTON_FM_Y;
			tBtn.Height = BUTTON_FM_H;
			tBtn.Width = BUTTON_FM_W;
			tBtn.Focus = 0;		/* ʧȥ���� */
			tBtn.pCaption = "FM";
			LCD_DrawButton(&tBtn);

			tBtn.Left = BUTTON_AM_X;
			tBtn.Top = BUTTON_AM_Y;
			tBtn.Height = BUTTON_AM_H;
			tBtn.Width = BUTTON_AM_W;
			tBtn.Focus = 0;		/* ʧȥ���� */
			tBtn.pCaption = "AM";
			LCD_DrawButton(&tBtn);
		}

		/* ��ʾ���� */
		tCheck.Font = &tFontChk;

		tCheck.Left = CHECK_SPK_X;
		tCheck.Top = CHECK_SPK_Y;
		tCheck.Height = CHECK_SPK_H;
		tCheck.Width = CHECK_SPK_W;
		if (g_tRadio.ucSpkOutEn == 1)
		{
			tCheck.Checked = 1;
		}
		else
		{
			tCheck.Checked = 0;
		}
		tCheck.pCaption = CHECK_SPK_TEXT;
		LCD_DrawCheckBox(&tCheck);

		tCheck.Left = CHECK_RSSI_X;
		tCheck.Top = CHECK_RSSI_Y;
		tCheck.Height = CHECK_RSSI_H;
		tCheck.Width = CHECK_RSSI_W;
		tCheck.Checked = 0;
		tCheck.pCaption = CHECK_RSSI_TEXT;
		
		LCD_DrawCheckBox(&tCheck);

		tCheck.Left = CHECK_LIST_X;
		tCheck.Top = CHECK_LIST_Y;
		tCheck.Height = CHECK_LIST_H;
		tCheck.Width = CHECK_LIST_W;
		if (g_tRadio.ucListType == 1)
		{
			tCheck.Checked = 1;
		}
		else
		{
			tCheck.Checked = 0;
		}
		tCheck.pCaption = CHECK_LIST_TEXT;
		LCD_DrawCheckBox(&tCheck);
	}

	{
		if (g_tRadio.ucSpkOutEn == 0)
		{
			/* ����WM8978оƬ������ΪAUX�ӿ�(������)�����Ϊ���� */
			wm8978_CfgAudioPath(AUX_ON, EAR_LEFT_ON | EAR_RIGHT_ON);
		}
		else
		{
			/* ����WM8978оƬ������ΪAUX�ӿ�(������)�����Ϊ���� �� ������ */
			wm8978_CfgAudioPath(AUX_ON, EAR_LEFT_ON | EAR_RIGHT_ON | SPK_ON);
		}

		//g_tRadio.ucSpkOutEn = 0

		SI4730_PowerDown();
		bsp_DelayMS(10);

		//SI4730_PowerUp_AM_Revice();
		SI4730_PowerUp_FM_Revice();
		bsp_DelayMS(10);

		/* �������� */
		radio_AdjustVolume(g_tRadio.ucVolume);
		
		tFont.FrontColor = CL_YELLOW;
		LCD_DispStr(x, y, "�����ҡ�˻�̨�͵�������,K2 K3��΢��Ƶ��", &tFont);
		if (g_tRadio.ChipType == SI4704)
		{
			LCD_DispStr(x, y + 75, "�뽫�����嵽��ɫ����������FM����", &tFont);			/* ��(8,3)���괦��ʾһ������ */		
		}
		tFont.FrontColor = CL_WHITE;		

		/* Ԥ���人�����ĵ�̨�б� */
		{
			if (g_tRadio.ucListType == 0)	/* 0 ��ʾ �人�����б� 1 ��ʾȫ���б� */
			{
				radio_FM_FreqList(0);	/* Ԥ���人������FM��̨�б� */
				radio_AM_FreqList(0);	/* Ԥ���人������AM��̨�б� */
				//g_tRadio.ucIndexFM  = 1;
			}
			else
			{
				radio_FM_FreqList(1);	/* Ԥ��ȫ��������FM��̨�б� */
				radio_AM_FreqList(1);	/* Ԥ��ȫ��������AM��̨�б� */
				//g_tRadio.ucIndexAM  = 1;
			}
			//g_tRadio.ucMode = FM_RX;	/* ȱʡ��FM���� */
			//g_tRadio.ucMode = AM_RX;	/* ȱʡ��FM���� */
		}

		bsp_DelayMS(300);	/* �����ӳ�һ��ʱ��󣬲��ܱ�֤ÿ���ϵ���ܹ�ѡ��һ��̨ */
		if (g_tRadio.ucMode == FM_RX)
		{
			g_tRadio.usFreq = g_tRadio.usFMList[g_tRadio.ucIndexFM];
			SI4730_SetFMFreq(g_tRadio.usFreq);
		}
		else
		{
			g_tRadio.usFreq = g_tRadio.usAMList[g_tRadio.ucIndexAM];
			SI4730_SetAMFreq(g_tRadio.usFreq);
		}

		bsp_DelayMS(100);	/* �ӳ�100ms������ǿ�ҵĸ����� */

		wm8978_OutMute(0);
	}

	fRefresh = 1;	/* 1��ʾ��Ҫˢ��LCD */
	bsp_StartAutoTimer(0, 1000);
	while (fQuit == 0)
	{
		bsp_Idle();

		if (fRefresh)
		{
			fRefresh = 0;

			radio_DispStatus();
		}

		if (g_tRadio.ucRssiEn)
		{
			if (bsp_CheckTimer(0))
			{
				radio_SignalQuality();	/* ˢ���ź�����״̬�� */
			}
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
						tBtn.pCaption = "����";
						LCD_DrawButton(&tBtn);
					}
					else if (TOUCH_InRect(tpX, tpY, BUTTON_RET2_X, BUTTON_RET2_Y, BUTTON_RET2_H, BUTTON_RET2_W))
					{
						tBtn.Left = BUTTON_RET2_X;
						tBtn.Top = BUTTON_RET2_Y;
						tBtn.Height = BUTTON_RET2_H;
						tBtn.Width = BUTTON_RET2_W;
						tBtn.Focus = 1;	/* ���� */
						tBtn.pCaption =  BUTTON_RET2_TEXT;
						LCD_DrawButton(&tBtn);
					}
					else if (TOUCH_InRect(tpX, tpY, BUTTON_FM_X, BUTTON_FM_Y, BUTTON_FM_H, BUTTON_FM_W))
					{
						if (g_tRadio.ChipType == SI4730)
						{
							tBtn.Left = BUTTON_FM_X;
							tBtn.Top = BUTTON_FM_Y;
							tBtn.Height = BUTTON_FM_H;
							tBtn.Width = BUTTON_FM_W;
							tBtn.Focus = 1;		/* ���� */
							tBtn.pCaption = "FM";
							LCD_DrawButton(&tBtn);
						}
					}
					else if (TOUCH_InRect(tpX, tpY, BUTTON_AM_X, BUTTON_AM_Y, BUTTON_AM_H, BUTTON_AM_W))
					{
						if (g_tRadio.ChipType == SI4730)
						{						
							tBtn.Left = BUTTON_AM_X;
							tBtn.Top = BUTTON_AM_Y;
							tBtn.Height = BUTTON_AM_H;
							tBtn.Width = BUTTON_AM_W;
							tBtn.Focus = 1;		/* ���� */
							tBtn.pCaption = "AM";
							LCD_DrawButton(&tBtn);
						}
					}
					else if (TOUCH_InRect(tpX, tpY, CHECK_SPK_X, CHECK_SPK_Y, CHECK_SPK_H, CHECK_SPK_W))
					{
						if (g_tRadio.ucSpkOutEn)
						{
							g_tRadio.ucSpkOutEn = 0;
							tCheck.Checked = 0;

							/* ����WM8978оƬ������ΪAUX�ӿ�(������)�����Ϊ���� */
							wm8978_CfgAudioPath(AUX_ON, EAR_LEFT_ON | EAR_RIGHT_ON);
						}
						else
						{
							g_tRadio.ucSpkOutEn = 1;
							tCheck.Checked = 1;

							/* ����WM8978оƬ������ΪAUX�ӿ�(������)�����Ϊ���� �� ������ */
							wm8978_CfgAudioPath(AUX_ON, EAR_LEFT_ON | EAR_RIGHT_ON | SPK_ON);
						}

						tCheck.Left = CHECK_SPK_X;
						tCheck.Top = CHECK_SPK_Y;
						tCheck.Height = CHECK_SPK_H;
						tCheck.Width = CHECK_SPK_W;
						tCheck.pCaption = CHECK_SPK_TEXT;
						LCD_DrawCheckBox(&tCheck);
					}
					else if (TOUCH_InRect(tpX, tpY, CHECK_RSSI_X, CHECK_RSSI_Y, CHECK_RSSI_H, CHECK_RSSI_W))
					{
						if (g_tRadio.ucRssiEn)
						{
							g_tRadio.ucRssiEn = 0;
							tCheck.Checked = 0;

							/* ����ź�������ʾ���� */
							LCD_DispStr(5,100, "                                                     ", &tFont);
						}
						else
						{
							g_tRadio.ucRssiEn = 1;
							tCheck.Checked = 1;
						}

						tCheck.Left = CHECK_RSSI_X;
						tCheck.Top = CHECK_RSSI_Y;
						tCheck.Height = CHECK_RSSI_H;
						tCheck.Width = CHECK_RSSI_W;
						tCheck.pCaption = CHECK_RSSI_TEXT;
						LCD_DrawCheckBox(&tCheck);
					}
					else if (TOUCH_InRect(tpX, tpY, CHECK_LIST_X, CHECK_LIST_Y, CHECK_LIST_H, CHECK_LIST_W))
					{
						if (g_tRadio.ucListType)
						{
							g_tRadio.ucListType = 0;	/* �人������̨�б� */
							radio_FM_FreqList(0);
							radio_AM_FreqList(0);

							tCheck.Checked = 0;
						}
						else
						{
							g_tRadio.ucListType = 1;	/* ȫ����̨�б� */
							radio_FM_FreqList(1);
							radio_AM_FreqList(1);

							tCheck.Checked = 1;
						}

						g_tRadio.ucIndexFM = 0;
						g_tRadio.ucIndexAM = 0;

						tCheck.Left = CHECK_LIST_X;
						tCheck.Top = CHECK_LIST_Y;
						tCheck.Height = CHECK_LIST_H;
						tCheck.Width = CHECK_LIST_W;

						tCheck.pCaption = CHECK_LIST_TEXT;
						LCD_DrawCheckBox(&tCheck);

						fRefresh = 1;
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
						tBtn.Focus = 0;	/* ���� */
						tBtn.pCaption = "����";
						LCD_DrawButton(&tBtn);

						fQuit = 1;	/* ���� */
						//return;
					}
					else if (TOUCH_InRect(tpX, tpY, BUTTON_RET2_X, BUTTON_RET2_Y, BUTTON_RET2_H, BUTTON_RET2_W))
					{
						tBtn.Font = &tFontBtn;
						
						tBtn.Left = BUTTON_RET2_X;
						tBtn.Top = BUTTON_RET2_Y;
						tBtn.Height = BUTTON_RET2_H;
						tBtn.Width = BUTTON_RET2_W;
						tBtn.Focus = 0;	/* ���� */
						tBtn.pCaption =  BUTTON_RET2_TEXT;
						LCD_DrawButton(&tBtn);
						
						fQuit = 2;	/* �������� ���� */
					}					
					else if (TOUCH_InRect(tpX, tpY, BUTTON_FM_X, BUTTON_FM_Y, BUTTON_FM_H, BUTTON_FM_W))
					{
						if (g_tRadio.ChipType == SI4730)
						{						
							tBtn.Font = &tFontBtn;

							tBtn.Left = BUTTON_FM_X;
							tBtn.Top = BUTTON_FM_Y;
							tBtn.Height = BUTTON_FM_H;
							tBtn.Width = BUTTON_FM_W;
							tBtn.Focus = 0;		/* ʧȥ���� */
							tBtn.pCaption = "FM";
							LCD_DrawButton(&tBtn);

							/* �л��� FM ״̬ */
							//SI4730_SetOutVolume(0);		/* �Ⱦ����������л������ */
							wm8978_OutMute(1);

							g_tRadio.ucMode = FM_RX;

							SI4730_PowerDown();
							bsp_DelayMS(5);
							SI4730_PowerUp_FM_Revice();
							bsp_DelayMS(10);

							SI4730_SetOutVolume(g_tRadio.ucVolume);	/* ȱʡ���������ֵ63, �޸�Ϊ32 */

							g_tRadio.usFreq = g_tRadio.usFMList[g_tRadio.ucIndexFM];
							SI4730_SetFMFreq(g_tRadio.usFreq);

							bsp_DelayMS(100);	/* �ӳ�100ms������ǿ�ҵĸ����� */

							wm8978_OutMute(0);

							fRefresh = 1;
						}
					}
					else if (TOUCH_InRect(tpX, tpY, BUTTON_AM_X, BUTTON_AM_Y, BUTTON_AM_H, BUTTON_AM_W))
					{
						if (g_tRadio.ChipType == SI4730)
						{							
							tBtn.Left = BUTTON_AM_X;
							tBtn.Top = BUTTON_AM_Y;
							tBtn.Height = BUTTON_AM_H;
							tBtn.Width = BUTTON_AM_W;
							tBtn.Focus = 0;		/* ʧȥ���� */
							tBtn.pCaption = "AM";
							LCD_DrawButton(&tBtn);

							/* �л��� AM ״̬ */
							//SI4730_SetOutVolume(0);		/* �Ⱦ����������л������ */
							wm8978_OutMute(1);

							g_tRadio.ucMode = AM_RX;	/* ȱʡ��AM���� */
							SI4730_PowerDown();
							bsp_DelayMS(5);
							SI4730_PowerUp_AM_Revice();
							bsp_DelayMS(10);

							SI4730_SetOutVolume(g_tRadio.ucVolume);	/* ȱʡ���������ֵ63, �޸�Ϊ32 */

							g_tRadio.usFreq = g_tRadio.usAMList[g_tRadio.ucIndexAM];
							SI4730_SetAMFreq(g_tRadio.usFreq);

							bsp_DelayMS(100);	/* �ӳ�100ms������ǿ�ҵĸ����� */

							wm8978_OutMute(0);

							fRefresh = 1;
						}
					}
					else	/* ��ťʧȥ���� */
					{
						tBtn.Font = &tFontBtn;

						tBtn.Focus = 0;		/* δѡ�� */

						tBtn.Left = BUTTON_RET_X;
						tBtn.Top = BUTTON_RET_Y;
						tBtn.Height = BUTTON_RET_H;
						tBtn.Width = BUTTON_RET_W;
						tBtn.pCaption = "����";
						LCD_DrawButton(&tBtn);

						tBtn.Left = BUTTON_RET2_X;
						tBtn.Top = BUTTON_RET2_Y;
						tBtn.Height = BUTTON_RET2_H;
						tBtn.Width = BUTTON_RET2_W;
						tBtn.pCaption = BUTTON_RET2_TEXT;
						LCD_DrawButton(&tBtn);						
						
						if (g_tRadio.ChipType == SI4730)
						{
							tBtn.Left = BUTTON_FM_X;
							tBtn.Top = BUTTON_FM_Y;
							tBtn.Height = BUTTON_FM_H;
							tBtn.Width = BUTTON_FM_W;
							tBtn.pCaption = "FM";
							LCD_DrawButton(&tBtn);

							tBtn.Left = BUTTON_AM_X;
							tBtn.Top = BUTTON_AM_Y;
							tBtn.Height = BUTTON_AM_H;
							tBtn.Width = BUTTON_AM_W;
							tBtn.pCaption = "AM";
							LCD_DrawButton(&tBtn);
						}
					}
			}
		}

		ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/*
				���ڰ��������¼���ȱʡ��bsp_button.c �������TAMPER��WAKEUP��USER����ҡ��OK���ĵ����¼�
				�������Ӧ�ó�����Ҫ�����������緽������ĵ����¼�������Ҫ���޸�һ��bsp_button.c�ļ�
			*/
			switch (ucKeyCode)
			{
				case KEY_DOWN_K1:			/* K1������ */

					break;

				case KEY_DOWN_K2:			/* K2������ */
					
					if (g_tRadio.ucMode == FM_RX)
					{
						g_tRadio.usFreq -= 10;
						SI4730_SetFMFreq(g_tRadio.usFreq);						
					}
					else
					{
						g_tRadio.usFreq--;
						SI4730_SetAMFreq(g_tRadio.usFreq);
					}
					fRefresh = 1;
					break;

				case KEY_DOWN_K3:			/* K3������ */
					
					if (g_tRadio.ucMode == FM_RX)
					{
						g_tRadio.usFreq += 10;
						SI4730_SetFMFreq(g_tRadio.usFreq);						
					}
					else
					{
						g_tRadio.usFreq++;
						SI4730_SetAMFreq(g_tRadio.usFreq);
					}
					fRefresh = 1;
					break;

				case JOY_DOWN_U:			/* ҡ��UP������ */
					g_tRadio.ucVolume += VOLUME_STEP;
					if (g_tRadio.ucVolume > VOLUME_MAX)
					{
						g_tRadio.ucVolume = VOLUME_MAX;
					}
					radio_AdjustVolume(g_tRadio.ucVolume);				
					fRefresh = 1;
					break;

				case JOY_DOWN_D:			/* ҡ��DOWN������ */
					if (g_tRadio.ucVolume > 0)
					{
						g_tRadio.ucVolume -= VOLUME_STEP;
					}
					else
					{
						g_tRadio.ucVolume = 0;
					}
					radio_AdjustVolume(g_tRadio.ucVolume);	
					fRefresh = 1;
					break;

				case JOY_DOWN_L:			/* ҡ��LEFT������ */
					if (g_tRadio.ucMode == FM_RX)
					{
						if (g_tRadio.ucIndexFM > 0)
						{
							g_tRadio.ucIndexFM--;
							g_tRadio.usFreq = g_tRadio.usFMList[g_tRadio.ucIndexFM];
							SI4730_SetFMFreq(g_tRadio.usFreq );
							fRefresh = 1;
						}
					}
					else
					{
						if (g_tRadio.ucIndexAM > 0)
						{
							g_tRadio.ucIndexAM--;
							g_tRadio.usFreq = g_tRadio.usAMList[g_tRadio.ucIndexAM];
							SI4730_SetAMFreq(g_tRadio.usFreq );
							fRefresh = 1;
						}
					}
					break;

				case JOY_DOWN_R:			/* ҡ��RIGHT������ */
					if (g_tRadio.ucMode == FM_RX)
					{
						if (g_tRadio.ucIndexFM < g_tRadio.ucFMCount - 1)
						{
							g_tRadio.ucIndexFM++;
							g_tRadio.usFreq = g_tRadio.usFMList[g_tRadio.ucIndexFM];
							SI4730_SetFMFreq(g_tRadio.usFreq );
							fRefresh = 1;
						}
					}
					else
					{
						if (g_tRadio.ucIndexAM < g_tRadio.ucAMCount - 1)
						{
							g_tRadio.ucIndexAM++;
							g_tRadio.usFreq = g_tRadio.usAMList[g_tRadio.ucIndexAM];
							SI4730_SetAMFreq(g_tRadio.usFreq );
							fRefresh = 1;
						}
					}
					break;

				case JOY_DOWN_OK:			/* ҡ��OK������ */
					break;

				default:
					/* �����ļ�ֵ������ */
					break;
			}
		}
	}	//while (fQuit == 0)

	if (fQuit == 1)
	{
		SI4730_PowerDown();
	}

	bsp_StopTimer(0);	/* ֹͣ��ʱ��0 */

	radio_SaveParam();	/* �����̨���� */
}

/*
*********************************************************************************************************
*	�� �� ��: radio_DispStatus
*	����˵��: ��ʾ��ǰ״̬
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void radio_DispStatus(void)
{
	char buf[128];
	FONT_T tFont;			/* ����һ������ṹ���������������������� */
	uint16_t usLineCap = 18;
	uint16_t x, y;

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;	/* ������� 16���� */
		tFont.FrontColor = CL_WHITE;	/* ������ɫ */
		tFont.BackColor = CL_BLUE;	/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}

	if (g_tRadio.ucMode == FM_RX)
	{
		sprintf(buf, "FM (%3d/%d) Ƶ��=%5d.%dMHz, Volume = %2d    ", g_tRadio.ucIndexFM + 1,
			g_tRadio.ucFMCount, g_tRadio.usFreq / 100,
			(g_tRadio.usFreq % 100) / 10, g_tRadio.ucVolume);
	}
	else
	{
		sprintf(buf, "AM (%3d/%d) Ƶ��=%5dKHz, Volume = %2d    ",g_tRadio.ucIndexAM + 1,
			g_tRadio.ucAMCount,	g_tRadio.usFreq, g_tRadio.ucVolume);
	}

	x = 5;
	y = 80;
	LCD_DispStr(x, y, buf, &tFont);
	y += usLineCap;
}

/*
*********************************************************************************************************
*	�� �� ��: radio_SignalQuality
*	����˵��: ��ʾ��ǰ�ź����� RSSI  SNR
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void radio_SignalQuality(void)
{
	char buf[128];
	FONT_T tFont;			/* ����һ������ṹ���������������������� */
	uint8_t read_buf[7];
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

	if (g_tRadio.ucMode == FM_RX)
	{
		uint8_t rssi, snr, cap;

		SI4730_GetFMSignalQuality(read_buf);
		/*
			CMD      0x23    FM_RSQ_STATUS
			ARG1     0x01    Clear RSQINT
			STATUS   ?0x80   Reply Status. Clear-to-send high.
			RESP1    ?0x00   No blend, SNR high, low, RSSI high or low interrupts.
			RESP2    ?0x01   Soft mute is not engaged, no AFC rail, valid frequency.
			RESP3    ?0xD9   Pilot presence, 89% blend
			RESP4    ?0x2D   RSSI = 45 dB��V
			RESP5    ?0x33   SNR = 51 dB
			RESP6    ?0x00
			RESP7    ?0x00   Freq offset = 0 kHz
		*/		
		rssi = read_buf[3];
		snr = read_buf[4];
		
		SI4730_GetFMTuneStatus(read_buf);
		/*
			CMD      0x22     FM_TUNE_STATUS
			ARG1     0x01     Clear STC interrupt.
			STATUS   ?0x80    Reply Status. Clear-to-send high.

			RESP1    ?0x01    Valid Frequency.
			RESP2    ?0x27    Frequency = 0x27F6 = 102.3 MHz
			RESP3    ?0xF6
			RESP4    ?0x2D    RSSI = 45 dB��V
			RESP5    ?0x33    SNR = 51 dB
			RESP6    ?0x00    MULT[7:0]
			RESP7    ?0x00    Antenna tuning capacitor = 0 (range = 0�C191)  READANTCAP[7:0] (Si4704/05/06/2x only)
		*/	
		cap = read_buf[6];
		
		sprintf(buf, "RSSI = %ddBuV  SNR = %ddB CAP = %d",  rssi, snr, cap);

		LCD_DispStrEx(x, y, buf, &tFont, 300, ALIGN_LEFT);			
	}
	else
	{
		uint32_t cap;

		/* ��ȡAM��г״̬ */
		SI4730_GetAMTuneStatus(read_buf);		
		/*
			CMD       0x42           AM_TUNE_STATUS
			ARG1      0x01           Clear STC interrupt.
			STATUS    ?0x80          Reply Status. Clear-to-send high.

			RESP1     ?0x01          Channel is valid
			RESP2     ?0x03
			RESP3     ?0xE8          Frequency = 0x03E8 = 1000 kHz
			RESP4     ?0x2A          RSSI = 0x2A = 42d = 42 dB��V
			RESP5     ?0x1A          SNR = 0x1A = 26d = 26 dB
			RESP6     ?0x0D          Value the antenna tuning capacitor is set to.
			RESP7     ?0x95          0x0D95 = 3477 dec.

			���ݼ��� The tuning capacitance is 95 fF x READANTCAP + 7 pF			
		*/	
		cap = (read_buf[5] << 8) | read_buf[6];
		cap = (cap * 95)  + 7000;	/* ʵ�� 342pF -- 10pF */
		
		/* ��ȡAM�ź����� */
		SI4730_GetAMSignalQuality(read_buf);
		
		sprintf(buf, "RSSI = %ddBuV  SNR = %ddB  Tuning Cap. = %d.%03dpF    ", 
				read_buf[3], read_buf[4], cap / 1000, cap % 1000);
		LCD_DispStr(x, y, buf, &tFont);					
	}

	
}

/*
*********************************************************************************************************
*	�� �� ��: radio_FM_FreqList
*	����˵��: Ԥ���̨�б�
*	��    �Σ�_ucAll = 0 ��ʾ�人����FM̨�� 1 ��ʾ���е�̨������0.1M
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void radio_FM_FreqList(uint8_t _ucAll)
{
	/* Ԥ���人�����ĵ�̨�б� */
	uint32_t i;

	if (_ucAll == 0)
	{
		g_tRadio.ucFMCount = sizeof(g_InitListFM) / 2;
		for (i = 0 ;i < g_tRadio.ucFMCount; i ++)
		{
			g_tRadio.usFMList[i] = g_InitListFM[i];
		}
	}
	else
	{
		/*
			�й���Χ�� 88-108�׺��źŵ��Ʒ�ʽ�ǵ�Ƶ��Ƶ�ʵ��ƣ�ÿ��Ƶ����Ƶ�ʼ����0.1�׺�
		*/
		g_tRadio.ucFMCount = 0;
		for (i = 8800 ;i <= 10800; i += 10)
		{
			g_tRadio.usFMList[g_tRadio.ucFMCount++] = i;
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: radio_AM_FreqList
*	����˵��: Ԥ��AM��̨�б�
*	��    �Σ�_ucAll = 0 ��ʾ�人����AM̨�� 1 ��ʾ���е�̨������9KHz
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void radio_AM_FreqList(uint8_t _ucAll)
{
	/* Ԥ���人�����ĵ�̨�б� */
	uint8_t i;

	if (_ucAll == 0)
	{
		g_tRadio.ucAMCount = sizeof(g_InitListAM) / 2;
		for (i = 0 ;i < g_tRadio.ucAMCount; i ++)
		{
			g_tRadio.usAMList[i] = g_InitListAM[i];
		}
	}
	else
	{
		/*
			���� 9KHz
		*/
		g_tRadio.ucAMCount = 120;
		for (i = 0 ;i < g_tRadio.ucAMCount; i ++)
		{
			g_tRadio.usAMList[i] = 531 + i*9;
		}
		g_tRadio.ucIndexAM  = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: radio_LoadParam
*	����˵��: ��ȡ��̨����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void radio_LoadParam(void)
{
	//LoadPara(); ����Ҫ��ȡ��main() ���Ѷ�ȡ

	g_tRadio.ucMode       = g_tParam.ucRadioMode;			/* AM �� FM */
	g_tRadio.ucListType   = g_tParam.ucRadioListType;		/* ��̨�б����͡��人������ȫ�� */
	g_tRadio.ucIndexFM    = g_tParam.ucIndexFM;				/* ��ǰFM��̨���� */
	g_tRadio.ucIndexAM    = g_tParam.ucIndexAM;				/* ��ǰ��̨���� */
	g_tRadio.ucVolume     = g_tParam.ucRadioVolume;			/* ���� */
	g_tRadio.ucSpkOutEn   = g_tParam.ucSpkOutEn;			/* ���������ʹ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: radio_LoadParam
*	����˵��: ���浱ǰ�ĵ�̨������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void radio_SaveParam(void)
{
	g_tParam.ucRadioMode     = g_tRadio.ucMode;				/* AM �� FM */
	g_tParam.ucRadioListType = g_tRadio.ucListType;			/* ��̨�б����͡��人������ȫ�� */
	g_tParam.ucIndexFM       = g_tRadio.ucIndexFM;			/* ��ǰFM��̨���� */
	g_tParam.ucIndexAM       = g_tRadio.ucIndexAM;			/* ��ǰ��̨���� */
	g_tParam.ucRadioVolume   = g_tRadio.ucVolume;			/* ���� */
	g_tParam.ucSpkOutEn      = g_tRadio.ucSpkOutEn;			/* ���������ʹ�� */

	SaveParam();
}

/*
*********************************************************************************************************
*	�� �� ��: radio_AdjustVolume
*	����˵��: ��������
*	��    ��: _ucVolume : 0-63
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void radio_AdjustVolume(uint8_t _ucVolume)
{
	uint8_t volume;
	
	wm8978_SetEarVolume(g_tRadio.ucVolume);
	wm8978_SetSpkVolume(g_tRadio.ucVolume);
	
	if (g_tRadio.ucVolume == 0)
	{
		SI4730_SetOutVolume(0);
	}
	else
	{
		volume = 21 + (g_tRadio.ucVolume * 2) / 3;
		SI4730_SetOutVolume(volume);		
	}
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
