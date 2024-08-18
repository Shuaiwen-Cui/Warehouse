/*
*********************************************************************************************************
*
*	ģ������ : ����WIFIģ��
*	�ļ����� : wifi_test.c
*	��    �� : V1.0
*	˵    �� : ���Դ���WiFiģ��. ʹ�ô��ڳ����ն˹��߿��Բ��������ӡ�Ϊ�˲���ATָ��
*				SecureCRT ����Ҫ����Ϊ: �˵�ѡ�� -> �Ựѡ�� -> ������ն� -> ���� -> ģʽ
*					�Ҳര�ڣ���ǰģʽ�й�ѡ"����ģʽ"
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2015-07-16 armfly  �׷�
*
*	Copyright (C), 2015-2016, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "form_wifi.h"
#include "num_pad.h"

#define AP_MAX_NUM	30

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

	EDIT_T Edit1;		/* WIFI���� */

	BUTTON_T Btn1;		/* �о�AP */
	BUTTON_T Btn2;		/* ����AP */
	BUTTON_T Btn3;		/*   */
	BUTTON_T Btn4;		/*   */

	BUTTON_T BtnRet;	/* ���� */
	
	
	WIFI_AP_T APList[AP_MAX_NUM];	/* AP �б� */	

}FormWIFI_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* �������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	(g_LcdHeight - BOX1_Y - 10)
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_TEXT	"ESP8266 WiFiģ����Գ���."

/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	60
#define	BTN_RET_X	((BOX1_X + BOX1_W) - BTN_RET_W - 4)
#define	BTN_RET_Y	((BOX1_Y  + BOX1_H) - BTN_RET_H - 4)
#define	BTN_RET_TEXT	"����"

#define LABEL1_X  	(BOX1_X + 6)
#define LABEL1_Y	(g_LcdHeight - 28)
#define LABEL1_TEXT	"--- "

/* ��ť */
#define BTN1_H		32
#define BTN1_W		120
#define	BTN1_X		(g_LcdWidth - BTN1_W - 10)
#define	BTN1_Y		20
#define	BTN1_TEXT	"�о�AP"

/* Edit */
#define	EDIT1_X		BTN1_X
#define	EDIT1_Y	 	(BTN1_Y + (BTN1_H + 2))
#define EDIT1_H		BTN1_H
#define EDIT1_W		BTN1_W

#define LABEL2_X  	EDIT1_X - 45
#define LABEL2_Y	EDIT1_Y + 4
#define LABEL2_TEXT	"����:"

#define BTN2_H		BTN1_H
#define BTN2_W		BTN1_W
#define	BTN2_X		BTN1_X
#define	BTN2_Y		(BTN1_Y + (BTN1_H + 2) * 2)
#define	BTN2_TEXT	"����AP"

#define BTN3_H		BTN1_H
#define BTN3_W		BTN1_W
#define	BTN3_X		BTN1_X
#define	BTN3_Y		(BTN1_Y + (BTN1_H + 2) * 3)
#define	BTN3_TEXT	"�鿴����IP"

#define BTN4_H		BTN1_H
#define BTN4_W		BTN1_W
#define	BTN4_X		BTN1_X
#define	BTN4_Y		(BTN1_Y + (BTN1_H + 2) * 4)
#define	BTN4_TEXT	"����TCP����"

	
static void InitFormWIFI(void);
static void DispFormWIFI(void);
static void DispInfoWiFi(char *_str);

FormWIFI_T *FormWIFI;

int16_t g_APCount = 0;	
uint8_t g_TCPServerOk = 0;

static void ScanAP(void);
static void DispAP(void);
void AnlyzeHostCmd(void);

/*
*********************************************************************************************************
*	�� �� ��: TestWIFI
*	����˵��: ���Դ���WiFiģ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TestWIFI(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fQuit = 0;
	int16_t tpX, tpY;
	uint8_t ucValue;
	uint8_t fRefresh = 0;
	FormWIFI_T form;

	FormWIFI = &form;

	LCD_ClrScr(CL_BTN_FACE);

	InitFormWIFI();
	DispFormWIFI();
	
	bsp_InitESP8266();
	
	//WiFiDispHelp();
		
	DispInfoWiFi("��1�����ڸ�ESP8266ģ���ϵ�...(������: 74880bsp)");
	printf("\r\n��1�����ڸ�ESP8266ģ���ϵ�...(������: 74880bsp)\r\n");
	
	ESP8266_PowerOn();

	DispInfoWiFi("��2���ϵ���ɡ�������: 115200bsp");
	printf("\r\n��2���ϵ���ɡ�������: 115200bsp\r\n");
	
	// 
	DispInfoWiFi("��3������ATָ��");
	ESP8266_SendAT("AT");
	if (ESP8266_WaitResponse("OK", 50) == 1)
	{
		DispInfoWiFi("��3��ģ��Ӧ��AT�ɹ�");
		bsp_DelayMS(1000);
	}
	else
	{
		DispInfoWiFi("��3��ģ����Ӧ��, �밴K3���޸�ģ��Ĳ�����Ϊ115200");
		bsp_DelayMS(1000);
	}
	
	
	g_TCPServerOk = 0;
	
	/* ����������ѭ���� */
	while (fQuit == 0)
	{
		bsp_Idle();

		if (g_TCPServerOk == 1)
		{
			AnlyzeHostCmd();
		}
		else
		{			
			/* ��WIFI�յ������ݷ��͵�����1 */
			if (comGetChar(COM_ESP8266, &ucValue))
			{
				comSendChar(COM1, ucValue);
				continue;
			}
			/* ������1�����ݷ��͵�MG323ģ�� */
			if (comGetChar(COM1, &ucValue))
			{
				comSendChar(COM_ESP8266, ucValue);
				continue;
			}
		}

		if (fRefresh)
		{
			fRefresh = 0;

			LCD_ClrScr(CL_BTN_FACE);
			DispFormWIFI();	/* ˢ�����пؼ� */
			DispAP();
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */			
					LCD_ButtonTouchDown(&FormWIFI->BtnRet, tpX, tpY);
					LCD_ButtonTouchDown(&FormWIFI->Btn1, tpX, tpY);
					LCD_ButtonTouchDown(&FormWIFI->Btn2, tpX, tpY);
					LCD_ButtonTouchDown(&FormWIFI->Btn3, tpX, tpY);
					LCD_ButtonTouchDown(&FormWIFI->Btn4, tpX, tpY);			

					/* �༭�� */
					if (TOUCH_InRect(tpX, tpY, EDIT1_X, EDIT1_Y, EDIT1_H, EDIT1_W))
					{
						{
							uint8_t len = 30;
							
							if (InputNumber(NUMPAD_STR, "����WiFi����", &len, (void *)FormWIFI->Edit1.Text))
							{
								;
							}							
							fRefresh = 1;
						}
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (LCD_ButtonTouchRelease(&FormWIFI->BtnRet, tpX, tpY))
					{
						fQuit = 1;	/* ���� */
					}
					else if (LCD_ButtonTouchRelease(&FormWIFI->Btn1, tpX, tpY))
					{
						ScanAP();	/* ɨ��AP */
						DispAP();
					}
					else if (LCD_ButtonTouchRelease(&FormWIFI->Btn2, tpX, tpY))
					{
						int32_t sn = 0;
						char buf[64];
						
						if (InputInt("ѡ��AP���", 0, 20, &sn))
						{		
							LCD_ClrScr(CL_BTN_FACE);
							DispFormWIFI();	/* ˢ�����пؼ� */
							DispAP();
							
							if (sn > 1)
							{
								sn--;
							}			

							sprintf(buf, "���ڼ���AP... ��ȴ� %s", FormWIFI->APList[sn].ssid);
							DispInfoWiFi(buf);							
							/* ����AP ��ʱʱ�� 10000ms��10�� */
							if (ESP8266_JoinAP(FormWIFI->APList[sn].ssid, FormWIFI->Edit1.Text, 15000))
							{
								DispInfoWiFi("����AP�ɹ�");
							}
							else
							{
								DispInfoWiFi("����APʧ��");
							}
						}	
						else
						{
							LCD_ClrScr(CL_BTN_FACE);
							DispFormWIFI();	/* ˢ�����пؼ� */
							DispAP();
						}
					}
					else if (LCD_ButtonTouchRelease(&FormWIFI->Btn3, tpX, tpY))	/* �鿴����IP */
					{
						char ip[20];
						char mac[32];
						char buf[128];
						
						if (ESP8266_GetLocalIP(ip, mac) == 1)
						{
							sprintf(buf, "%s, %s", ip, mac);
							DispInfoWiFi(buf);
						}
						else
						{
							DispInfoWiFi("��ѯIPʧ��");
						}
					}
					else if (LCD_ButtonTouchRelease(&FormWIFI->Btn4, tpX, tpY))	/* ����10086 */
					{
						if (g_TCPServerOk == 0)
						{
							if (ESP8266_CreateTCPServer(1000) == 1)
							{
								DispInfoWiFi("���ڼ���1000�˿�...");
								g_TCPServerOk = 1;
							}
							else
							{
								DispInfoWiFi("����TCP����ʧ��!");
							}							
						}
						else
						{
							ESP8266_CloseTcpUdp(0);
							g_TCPServerOk = 0;
							DispInfoWiFi("�رյ�ǰTCP����!");
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
				case KEY_DOWN_K1:			/* K1������ */
					//ESP8266_SendAT("AT+CWLAP");	/* �о�AP */
					break;

				case KEY_DOWN_K2:			/* K2������ */
					//ESP8266_SendAT("AT+CWJAP=\"Tenda_5BD8A8\",\"123456887af\"");	/* ����ĳ��WIFI ���� */
					//ESP8266_JoinAP("Tenda_5BD8A8", "123456887af");
					break;

				case KEY_DOWN_K3:			/* K3��-9600�������л���115200 */
					comSetBaud(COM_ESP8266, 9600);			/* ģ��ȱʡ��9600bps */
					ESP8266_SendAT("AT+CIOBAUD=115200");	/* �� 9600bps ����ָ���л�Ϊ 115200 */
					ESP8266_WaitResponse("OK", 2000);		/* ��� OK ��ģ�鰴 9600 Ӧ��� */
					comSetBaud(COM_ESP8266, 115200);		/* �л�STM32�Ĳ�����Ϊ 115200 */

					/* �л�Ϊ Stationģʽ */
					bsp_DelayMS(100);
					ESP8266_SendAT("AT+CWMODE=1");
					ESP8266_WaitResponse("OK", 2000);
					bsp_DelayMS(1500);
					ESP8266_SendAT("AT+RST");
					break;

				case JOY_DOWN_U:		/* ҡ���ϼ��� AT+CIFSR��ȡ����IP��ַ */
					ESP8266_SendAT("AT+CIFSR");
					break;

				case JOY_DOWN_D:		/* ҡ��DOWN�� AT+CIPSTATUS���IP����״̬ */
					ESP8266_SendAT("AT+CIPSTATUS");
					break;

				case JOY_DOWN_L:		/* ҡ��LEFT������   AT+CIPSTART ����TCP����. ����www,armfly.com http����˿� */
					ESP8266_SendAT("AT+CIPSTART=\"TCP\",\"WWW.ARMFLY.COM\",80");
					break;

				case JOY_DOWN_R:		/* ҡ��RIGHT������  AT+CIPCLOSE�رյ�ǰ��TCP��UDP����  */
					ESP8266_SendAT("AT+CIPCLOSE");
					break;

				case JOY_DOWN_OK:		/* ҡ��OK������  */
					//printf("\r\n����̼�����ģʽ\r\n");
					break;

				default:
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AnlyzeHostCmd
*	����˵��: ����TCP�ͻ��˷���������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
		
void AnlyzeHostCmd(void)
{
	uint8_t cmd_buf[2048];	
	//uint8_t *cmd_buf;
	uint8_t cmd_len;
	static uint8_t s_test = 0;

	//cmd_buf = (uint8_t *)EXT_SRAM_ADDR;	/* ָ���ⲿSRAM */
	cmd_len = ESP8266_RxNew(cmd_buf);
	if (cmd_len == 0)
	{
		return;
	}
	
	if (s_test == 1)
	{
		if ((cmd_len == 1) && (memcmp(cmd_buf, "A", 1)) == 0)
		{
			ESP8266_SendTcpUdp(cmd_buf, 1300);
		}
	}
	else if (s_test == 2)
	{
		ESP8266_SendTcpUdp("OK", 2);
	}
	
	if ((cmd_len == 6) && (memcmp(cmd_buf, "txtest", 6)) == 0)
	{
		s_test = 1;
		ESP8266_SendTcpUdp(cmd_buf, 1300);
	}
	else if ((cmd_len == 6) && (memcmp(cmd_buf, "rxtest", 6)) == 0)
	{
		ESP8266_SendTcpUdp("OK", 2);
		s_test = 2;
	}
	else if ((cmd_len == 4) && (memcmp(cmd_buf, "stop", 4)) == 0)
	{
		s_test = 0;
	}	
	else 
	{
		if (cmd_len == 7)
		{
			if (memcmp(cmd_buf, "ledon 1 ", 7) == 0)
			{
				bsp_LedOn(1);
				ESP8266_SendTcpUdp("led1 on", 7);
			}
			else if (memcmp(cmd_buf, "ledon 2 ", 7) == 0)
			{
				bsp_LedOn(2);
				ESP8266_SendTcpUdp("led2 on", 7);
			}
			else if (memcmp(cmd_buf, "ledon 3 ", 7) == 0)
			{
				bsp_LedOn(3);
				ESP8266_SendTcpUdp("led3 on", 7);
			}
			else if (memcmp(cmd_buf, "ledon 4 ", 7) == 0)
			{
				bsp_LedOn(4);
				ESP8266_SendTcpUdp("led4 on", 7);
			}
		}
		else if (cmd_len == 8)
		{
			if (memcmp(cmd_buf, "ledoff 1", 8) == 0)
			{
				bsp_LedOff(1);
				ESP8266_SendTcpUdp("led1 off", 8);
			}
			else if (memcmp(cmd_buf, "ledoff 2", 8) == 0)
			{
				bsp_LedOff(2);
				ESP8266_SendTcpUdp("led2 off", 8);
			}
			else if (memcmp(cmd_buf, "ledoff 3", 8) == 0)
			{
				bsp_LedOff(3);
				ESP8266_SendTcpUdp("led3 off", 8);
			}
			else if (memcmp(cmd_buf, "ledoff 4", 8) == 0)
			{
				bsp_LedOff(4);
				ESP8266_SendTcpUdp("led4 off", 8);
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: ScanAP
*	����˵��: ɨ��AP������ʾ����.
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ScanAP(void)
{
	/* ɨ�� AP�б�, ����AP���� */
	g_APCount = ESP8266_ScanAP(FormWIFI->APList, AP_MAX_NUM);
}

/*
*********************************************************************************************************
*	�� �� ��: DispAP
*	����˵��: ��ʾɨ�赽��AP
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispAP(void)
{
	char buf[48];
	FONT_T tFont;
	uint16_t x, y;	

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;	/* ������� 16���� */
		tFont.FrontColor = CL_GREY;	/* ������ɫ */
		tFont.BackColor = CL_BTN_FACE;		/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}
	
	x = 10;
	y = 25;	
	
	
	LCD_Fill_Rect(x, y, 234, 280, CL_BTN_FACE);
	
	sprintf(buf, "��ɨ�赽%d��WiFi AP", g_APCount);
	DispInfoWiFi(buf);
	
	{
		uint8_t i;
		uint8_t m;
		//const char *ecn_name[5] = 
		//{
		//	"OPEN", "WEP", "WPA_PSK", "WPA2_PSK", "WPA_WPA2_PSK"
		//};
		m = g_APCount;
		if (m > 13)
		{
			m = 13;
		}
		for (i = 0; i < m; i++)
		{			
			//sprintf(buf, "  %02d=%s, rssi=%d, [%s]\r\n", i+1, g_APList[i].ssid, g_APList[i].rssi,
			//	ecn_name[g_APList[i].ecn]);
			
			sprintf(buf, "%02d=%s, %ddBm", i+1, FormWIFI->APList[i].ssid, FormWIFI->APList[i].rssi);
			
			LCD_DispStr(x , y, buf, &tFont);	
			y += 17;
		}
	}	
}

/*
*********************************************************************************************************
*	�� �� ��: InitFormWIFI
*	����˵��: ��ʼ���ؼ�����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormWIFI(void)
{
	/* ������������ */
	FormWIFI->FontBox.FontCode = FC_ST_16;
	FormWIFI->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	FormWIFI->FontBox.FrontColor = CL_BLACK;
	FormWIFI->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	FormWIFI->FontBlack.FontCode = FC_ST_16;
	FormWIFI->FontBlack.BackColor = CL_BTN_FACE;		/* ͸��ɫ */
	FormWIFI->FontBlack.FrontColor = CL_BLACK;
	FormWIFI->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	FormWIFI->FontBlue.FontCode = FC_ST_16;
	FormWIFI->FontBlue.BackColor = CL_BTN_FACE;
	FormWIFI->FontBlue.FrontColor = CL_BLUE;
	FormWIFI->FontBlue.Space = 0;

	FormWIFI->FontRed.FontCode = FC_ST_16;
	FormWIFI->FontRed.BackColor = CL_BTN_FACE;
	FormWIFI->FontRed.FrontColor = CL_RED;
	FormWIFI->FontRed.Space = 0;

	/* ��ť���� */
	FormWIFI->FontBtn.FontCode = FC_ST_16;
	FormWIFI->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormWIFI->FontBtn.FrontColor = CL_BLACK;
	FormWIFI->FontBtn.Space = 0;

	/* ����� */
	FormWIFI->Box1.Left = BOX1_X;
	FormWIFI->Box1.Top = BOX1_Y;
	FormWIFI->Box1.Height = BOX1_H;
	FormWIFI->Box1.Width = BOX1_W;
	FormWIFI->Box1.pCaption = BOX1_TEXT;
	FormWIFI->Box1.Font = &FormWIFI->FontBox;

	/* ��̬��ǩ */
	FormWIFI->Label1.Left = LABEL1_X;
	FormWIFI->Label1.Top = LABEL1_Y;
	FormWIFI->Label1.MaxLen = 0;
	FormWIFI->Label1.pCaption = LABEL1_TEXT;
	FormWIFI->Label1.Font = &FormWIFI->FontBlack;

	FormWIFI->Label2.Left = LABEL2_X;
	FormWIFI->Label2.Top = LABEL2_Y;
	FormWIFI->Label2.MaxLen = 0;
	FormWIFI->Label2.pCaption = LABEL2_TEXT;
	FormWIFI->Label2.Font = &FormWIFI->FontBlack;

	/* ��ť */
	FormWIFI->BtnRet.Left = BTN_RET_X;
	FormWIFI->BtnRet.Top = BTN_RET_Y;
	FormWIFI->BtnRet.Height = BTN_RET_H;
	FormWIFI->BtnRet.Width = BTN_RET_W;
	FormWIFI->BtnRet.pCaption = BTN_RET_TEXT;
	FormWIFI->BtnRet.Font = &FormWIFI->FontBtn;
	FormWIFI->BtnRet.Focus = 0;

	FormWIFI->Btn1.Left = BTN1_X;
	FormWIFI->Btn1.Top = BTN1_Y;
	FormWIFI->Btn1.Height = BTN1_H;
	FormWIFI->Btn1.Width = BTN1_W;
	FormWIFI->Btn1.pCaption = BTN1_TEXT;
	FormWIFI->Btn1.Font = &FormWIFI->FontBtn;
	FormWIFI->Btn1.Focus = 0;

	FormWIFI->Btn2.Left = BTN2_X;
	FormWIFI->Btn2.Top = BTN2_Y;
	FormWIFI->Btn2.Height = BTN2_H;
	FormWIFI->Btn2.Width = BTN2_W;
	FormWIFI->Btn2.pCaption = BTN2_TEXT;
	FormWIFI->Btn2.Font = &FormWIFI->FontBtn;
	FormWIFI->Btn2.Focus = 0;

	FormWIFI->Btn3.Left = BTN3_X;
	FormWIFI->Btn3.Top = BTN3_Y;
	FormWIFI->Btn3.Height = BTN3_H;
	FormWIFI->Btn3.Width = BTN3_W;
	FormWIFI->Btn3.pCaption = BTN3_TEXT;
	FormWIFI->Btn3.Font = &FormWIFI->FontBtn;
	FormWIFI->Btn3.Focus = 0;

	FormWIFI->Btn4.Left = BTN4_X;
	FormWIFI->Btn4.Top = BTN4_Y;
	FormWIFI->Btn4.Height = BTN4_H;
	FormWIFI->Btn4.Width = BTN4_W;
	FormWIFI->Btn4.pCaption = BTN4_TEXT;
	FormWIFI->Btn4.Font = &FormWIFI->FontBtn;
	FormWIFI->Btn4.Focus = 0;

	/* �༭�� */
	FormWIFI->Edit1.Left = EDIT1_X;
	FormWIFI->Edit1.Top = EDIT1_Y;
	FormWIFI->Edit1.Height = EDIT1_H;
	FormWIFI->Edit1.Width = EDIT1_W;
	sprintf(FormWIFI->Edit1.Text, "123456887af");
	FormWIFI->Edit1.pCaption = FormWIFI->Edit1.Text;
	FormWIFI->Edit1.Font = &FormWIFI->FontBtn;
}

/*
*********************************************************************************************************
*	�� �� ��: DispInfoWiFi
*	����˵��: ��ʾ��Ϣ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispInfoWiFi(char *_str)
{
	FormWIFI->Label1.pCaption = _str;
	LCD_DrawLabel(&FormWIFI->Label1);
}

/*
*********************************************************************************************************
*	�� �� ��: DispFormWIFI
*	����˵��: ��ʾ���еĿؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFormWIFI(void)
{
	//LCD_ClrScr(CL_BTN_FACE);

	/* ����� */
	LCD_DrawGroupBox(&FormWIFI->Box1);

	/* ��ǩ */
	LCD_DrawLabel(&FormWIFI->Label1);
	LCD_DrawLabel(&FormWIFI->Label2);

	/* ��ť */
	LCD_DrawButton(&FormWIFI->BtnRet);
	LCD_DrawButton(&FormWIFI->Btn1);
	LCD_DrawButton(&FormWIFI->Btn2);
	LCD_DrawButton(&FormWIFI->Btn3);
	LCD_DrawButton(&FormWIFI->Btn4);	

	/* �༭�� */
	LCD_DrawEdit(&FormWIFI->Edit1);

	/* ��̬��ǩ */
	LCD_DrawLabel(&FormWIFI->Label2);
}


/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
