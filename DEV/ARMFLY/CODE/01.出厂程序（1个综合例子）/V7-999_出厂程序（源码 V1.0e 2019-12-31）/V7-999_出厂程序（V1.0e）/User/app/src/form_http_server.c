/*
*********************************************************************************************************
*
*	ģ������ : web server ��ʾ
*	�ļ����� : http_server.c
*	��    �� : V1.0
*	˵    �� : �ṩWEB���������ܡ���ҪĿ���ǲ���DM9000AEP������·��DM9161 PHY��·��
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-01-01 armfly  ��ʽ����
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "stdio.h"
#include "uip.h"
#include "uip_arp.h"
#include "tapdev.h"
#include "timer.h"
#include "dm9k_uip.h"
#include "bsp.h"
#include "num_pad.h"
#include "param.h"

/* for LwIP */
//#include "netconf.h"
//#include "main_lwip.h"
//#include "httpd_w.h"
#include "lwip/opt.h"
#include "lwip/init.h"
#include "netif/etharp.h"
#include "lwip/netif.h"
#include "lwip/timeouts.h"
#include "lwip/dhcp.h"

#include "ethernetif.h"
#include "app_ethernet.h"

#include "http_cgi_ssi.h"

/* �������ṹ */
typedef struct
{
	FONT_T FontLabel;
	FONT_T FontEdit;
	FONT_T FontBtn;
	FONT_T FontCheck;

	GROUP_T BoxUip;
	GROUP_T BoxLwip;

	LABEL_T LabelIP;
	LABEL_T LabelNetMask;
	LABEL_T LabelGateway;

	LABEL_T LabelLwIP;
	LABEL_T LabelLwNetMask;
	LABEL_T LabelLwGateway;

	EDIT_T EditIP;
	EDIT_T EditNetMask;
	EDIT_T EditGateway;

	EDIT_T EditLwIP;
	EDIT_T EditLwNetMask;
	EDIT_T EditLwGateway;

	BUTTON_T ButtonSetIP;
	BUTTON_T ButtonLwSetIP;

	BUTTON_T ButtonRet;
}FormHttp_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* ���ذ�ť������(��Ļ���½�) */
#define BUTTON_RET_H	32
#define BUTTON_RET_W	60
#define	BUTTON_RET_X	(g_LcdWidth - BUTTON_RET_W - 4)
#define	BUTTON_RET_Y	(g_LcdHeight - BUTTON_RET_H - 4)
#define BUTTON_RET_TEXT	"����"

/* 2���������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	50
#define BOX1_H	150
#define BOX1_W	((g_LcdWidth - 16) / 2)
#define BOX1_TEXT	"DM9000����(uIP)"

#define BOX2_X	(BOX1_X + BOX1_W + 5)
#define BOX2_Y	BOX1_Y
#define BOX2_H	BOX1_H
#define BOX2_W	BOX1_W
#define BOX2_TEXT	"F407 MAC + DM9161 PHY ����"

/* DM9000 ���� */
	#define	BUTTON_IP_X		(BOX1_X + 20)
	#define	BUTTON_IP_Y		(BOX1_Y + 110)
	#define BUTTON_IP_H		32
	#define BUTTON_IP_W		110
	#define BUTTON_IP_TEXT	"����uIP����"

	#define LABEL_IP_X  	(BOX1_X + 6)
	#define LABEL_IP_Y		(BOX1_Y + 20)
	#define LABEL_IP_TEXT	"����IP:"

	#define LABEL_MASK_X  	LABEL_IP_X
	#define LABEL_MASK_Y	(LABEL_IP_Y + 28)
	#define LABEL_MASK_TEXT	"��������:"

	#define LABEL_GATE_X  	LABEL_IP_X
	#define LABEL_GATE_Y	(LABEL_IP_Y + 28 * 2)
	#define LABEL_GATE_TEXT	"Ĭ������:"

	#define	EDIT_IP_X		(LABEL_IP_X + 80)
	#define	EDIT_IP_Y		(LABEL_IP_Y - 2)
	#define EDIT_IP_H		20
	#define EDIT_IP_W		132

	#define	EDIT_MASK_X		(LABEL_MASK_X + 80)
	#define	EDIT_MASK_Y		(LABEL_MASK_Y - 2)
	#define EDIT_MASK_H		EDIT_IP_H
	#define EDIT_MASK_W		EDIT_IP_W

	#define	EDIT_GATE_X		(LABEL_GATE_X + 80)
	#define	EDIT_GATE_Y		(LABEL_GATE_Y - 2)
	#define EDIT_GATE_H		EDIT_IP_H
	#define EDIT_GATE_W		EDIT_IP_W

/* DM9161 ���� */
	#define	BUTTON_LWIP_X		(BOX2_X + 20)
	#define	BUTTON_LWIP_Y		(BOX2_Y + 110)
	#define BUTTON_LWIP_H		32
	#define BUTTON_LWIP_W		110
	#define BUTTON_LWIP_TEXT	"����LwIP����"

	#define LABEL_LWIP_X  		(BOX2_X + 6)
	#define LABEL_LWIP_Y		(BOX2_Y + 20)
	#define LABEL_LWIP_TEXT		"����IP:"

	#define LABEL_LWMASK_X  	LABEL_LWIP_X
	#define LABEL_LWMASK_Y		(LABEL_LWIP_Y + 28)
	#define LABEL_LWMASK_TEXT	"��������:"

	#define LABEL_LWGATE_X  	LABEL_LWIP_X
	#define LABEL_LWGATE_Y		(LABEL_LWIP_Y + 28 * 2)
	#define LABEL_LWGATE_TEXT	"Ĭ������:"

	#define	EDIT_LWIP_X			(LABEL_LWIP_X + 80)
	#define	EDIT_LWIP_Y			(LABEL_LWIP_Y - 2)
	#define EDIT_LWIP_H			20
	#define EDIT_LWIP_W			132

	#define	EDIT_LWMASK_X		EDIT_LWIP_X
	#define	EDIT_LWMASK_Y		(LABEL_LWMASK_Y - 2)
	#define EDIT_LWMASK_H		EDIT_LWIP_H
	#define EDIT_LWMASK_W		EDIT_LWIP_W

	#define	EDIT_LWGATE_X		EDIT_LWIP_X
	#define	EDIT_LWGATE_Y		(LABEL_LWGATE_Y - 2)
	#define EDIT_LWGATE_H		EDIT_LWIP_H
	#define EDIT_LWGATE_W		EDIT_LWIP_W


#define BUF ((struct uip_eth_hdr *)&uip_buf[0])

/* ȱʡIP��ַ, ������Flash������ */

struct timer periodic_timer, arp_timer;

void uip_start(void);
void uip_pro(void);
void uip_ChangeNetParam(void);

void lwip_start(void);
void lwip_pro(void);

//static void DispAllControl(void);

static void InitFormHttp(void);
static void DispFormHttp(void);

FormHttp_T *FormHttp;

/*
*********************************************************************************************************
*	�� �� ��: WebServer
*	����˵��: web����������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void WebServer(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	int16_t tpX, tpY;
	uint16_t x, y;
	uint16_t usLineCap = 18;
	char buf[128];
	uint8_t fRefresh;
	FONT_T tFont;		/* ��������ṹ����� */
	FormHttp_T tForm;

	FormHttp = &tForm;

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;	/* ������� 16���� */
		tFont.FrontColor = CL_BLACK;	/* ������ɫ */
		tFont.BackColor = FORM_BACK_COLOR;	/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */

	}

	LCD_ClrScr(FORM_BACK_COLOR);  	/* ������������ɫ */

	LCD_DispStr(5, 3, "������������...", &tFont);

	/* UIP ��ʼ�� */
	{
		/* �������������ȫ�ֱ��� g_tParam.uip_ip, g_tParam.uip_net_mask, g_tParam.uip_gateway */
		uip_start();
	}

	/* LwIP ��ʼ�� */
	{
		/* ����������ߣ��˺���ִ��ʱ����� */
		/* �������������ȫ�ֱ��� g_tParam.lwip_ip, g_tParam.lwip_net_mask, g_tParam.lwip_gateway */
		lwip_start();
	}

	LCD_ClrScr(FORM_BACK_COLOR);  	/* ������������ɫ */

	InitFormHttp();

	fRefresh = 1;
	while(1)
	{
		bsp_Idle();

		uip_pro();
		lwip_pro();

		if (fRefresh)	/* ˢ���������� */
		{
			fRefresh = 0;

			//LCD_ClrScr(FORM_BACK_COLOR);  	/* ��Ҫ����ˢ������������˸ */

			x = 5;
			y = 3;
			LCD_DispStr(x, y, "WEB����������", &tFont);
			y += usLineCap;

			/* ��ӡоƬID */
			{
				uint32_t id;

				id = dm9k_ReadID();
				sprintf(buf, "DM9000 ID = 0x%08X", id);

				if (id == 0x0A469000)
				{
					sprintf(&buf[strlen(buf)], ", Ok");
					LCD_DispStr(x, y, buf, &tFont);
				}
				else
				{
					tFont.FrontColor = CL_RED;	/* ��ɫ�����ʾ���� */
					sprintf(&buf[strlen(buf)], ", Err");
					LCD_DispStr(x, y, buf, &tFont);
					tFont.FrontColor = CL_BLACK;/* �ָ�������ɫ */
				}

				/* PHYLinkState ��ethernetif.c���� */
				if (PHYLinkState != DM9162_STATUS_100MBITS_FULLDUPLEX)
				{
					tFont.FrontColor = CL_RED;	/* ��ɫ�����ʾ���� */
					LCD_DispStr(x + 216, y, "| DM9162 PHY ���� Error��", &tFont);
					tFont.FrontColor = CL_BLACK;/* �ָ�������ɫ */
				}
				else
				{
					LCD_DispStrEx(x + 216, y, "| DM9162 PHY ���� OK��", &tFont, 0, 210);
				}

				y += usLineCap;
			}

			DispFormHttp();
		}
		
		if (PHYLinkChanged == 1)	/* DM9162����״̬�仯 */
		{
			PHYLinkChanged = 0;

			fRefresh = 1;
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					if (TOUCH_InRect(tpX, tpY, FormHttp->ButtonRet.Left, FormHttp->ButtonRet.Top,
						FormHttp->ButtonRet.Height, FormHttp->ButtonRet.Width))
					{
						FormHttp->ButtonRet.Focus = 1;
						LCD_DrawButton(&FormHttp->ButtonRet);	/* ����ѡ��״̬�İ�ť */
					}
					else if (TOUCH_InRect(tpX, tpY, FormHttp->ButtonSetIP.Left, FormHttp->ButtonSetIP.Top,
						FormHttp->ButtonSetIP.Height, FormHttp->ButtonSetIP.Width))
					{
						FormHttp->ButtonSetIP.Focus = 1;
						LCD_DrawButton(&FormHttp->ButtonSetIP);	/* ����ѡ��״̬�İ�ť */
					}
					else if (TOUCH_InRect(tpX, tpY, FormHttp->ButtonLwSetIP.Left, FormHttp->ButtonLwSetIP.Top,
						FormHttp->ButtonLwSetIP.Height, FormHttp->ButtonLwSetIP.Width))
					{
						FormHttp->ButtonLwSetIP.Focus = 1;
						LCD_DrawButton(&FormHttp->ButtonLwSetIP);	/* ����ѡ��״̬�İ�ť */
					}

					/* �ж� UIP �༭�� */
					if (TOUCH_InRect(tpX, tpY, FormHttp->EditIP.Left, FormHttp->EditIP.Top,
						FormHttp->EditIP.Height, FormHttp->EditIP.Width))
					{
						{
							uint8_t ip_buf[4];

							if (InputNumber(NUMPAD_IP, "���ñ���IP (For Uip)", 0, (void *)ip_buf))
							{
								memcpy(g_tParam.uip_ip, ip_buf, 4);
								SaveParam();	/* ���������浽Flash */
							}

							ClearWinNumPad(FORM_BACK_COLOR);	/* ������ּ��̴��� */
							fRefresh = 1;
						}
					}
					else if (TOUCH_InRect(tpX, tpY, FormHttp->EditNetMask.Left, FormHttp->EditNetMask.Top,
						FormHttp->EditNetMask.Height, FormHttp->EditNetMask.Width))
					{
						{
							uint8_t ip_buf[4];

							if (InputNumber(NUMPAD_IP, "������������ (For Uip)", 0, (void *)ip_buf))
							{
								memcpy(g_tParam.uip_net_mask, ip_buf, 4);
								SaveParam();	/* ���������浽Flash */
							}

							ClearWinNumPad(FORM_BACK_COLOR);	/* ������ּ��̴��� */
							fRefresh = 1;
						}
					}
					else if (TOUCH_InRect(tpX, tpY, FormHttp->EditGateway.Left, FormHttp->EditGateway.Top,
						FormHttp->EditGateway.Height, FormHttp->EditGateway.Width))
					{
						{
							uint8_t ip_buf[4];

							if (InputNumber(NUMPAD_IP, "����Ĭ������ (For Uip)", 0, (void *)ip_buf))
							{
								memcpy(g_tParam.uip_gateway, ip_buf, 4);
								SaveParam();	/* ���������浽Flash */
							}

							ClearWinNumPad(FORM_BACK_COLOR);	/* ������ּ��̴��� */
							fRefresh = 1;
						}
					}

					/* �ж� LwIP �༭�� */
					if (TOUCH_InRect(tpX, tpY, FormHttp->EditLwIP.Left, FormHttp->EditLwIP.Top,
						FormHttp->EditLwIP.Height, FormHttp->EditLwIP.Width))
					{
						{
							uint8_t ip_buf[4];

							if (InputNumber(NUMPAD_IP, "���ñ���IP (For LwIP)", 0, (void *)ip_buf))
							{
								memcpy(g_tParam.lwip_ip, ip_buf, 4);
								SaveParam();	/* ���������浽Flash */
							}

							ClearWinNumPad(FORM_BACK_COLOR);	/* ������ּ��̴��� */
							fRefresh = 1;
						}
					}
					else if (TOUCH_InRect(tpX, tpY, FormHttp->EditLwNetMask.Left, FormHttp->EditLwNetMask.Top,
						FormHttp->EditLwNetMask.Height, FormHttp->EditLwNetMask.Width))
					{
						{
							uint8_t ip_buf[4];

							if (InputNumber(NUMPAD_IP, "������������ (For LwIP)", 0, (void *)ip_buf))
							{
								memcpy(g_tParam.lwip_net_mask, ip_buf, 4);
								SaveParam();	/* ���������浽Flash */
							}

							ClearWinNumPad(FORM_BACK_COLOR);	/* ������ּ��̴��� */
							fRefresh = 1;
						}
					}
					else if (TOUCH_InRect(tpX, tpY, FormHttp->EditLwGateway.Left, FormHttp->EditLwGateway.Top,
						FormHttp->EditLwGateway.Height, FormHttp->EditLwGateway.Width))
					{
						{
							uint8_t ip_buf[4];

							if (InputNumber(NUMPAD_IP, "����Ĭ������ (For LwIP)", 0, (void *)ip_buf))
							{
								memcpy(g_tParam.lwip_gateway, ip_buf, 4);
								SaveParam();	/* ���������浽Flash */
							}

							ClearWinNumPad(FORM_BACK_COLOR);	/* ������ּ��̴��� */
							fRefresh = 1;
						}
					}
					break;

				case TOUCH_MOVE:		/* �����ƶ��¼� */
					if (TOUCH_InRect(tpX, tpY, BUTTON_RET_X, BUTTON_RET_Y, BUTTON_RET_H, BUTTON_RET_W))
					{
						;
					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (TOUCH_InRect(tpX, tpY, FormHttp->ButtonRet.Left, FormHttp->ButtonRet.Top,
						FormHttp->ButtonRet.Height, FormHttp->ButtonRet.Width))
					{
						return;		/* ���� */
					}
					else if (TOUCH_InRect(tpX, tpY, FormHttp->ButtonSetIP.Left, FormHttp->ButtonSetIP.Top,
						FormHttp->ButtonSetIP.Height, FormHttp->ButtonSetIP.Width))
					{
						uip_ChangeNetParam();	/* ����������� */
					}
					else if (TOUCH_InRect(tpX, tpY, FormHttp->ButtonLwSetIP.Left, FormHttp->ButtonLwSetIP.Top,
						FormHttp->ButtonLwSetIP.Height, FormHttp->ButtonLwSetIP.Width))
					{
//						LwIP_ChangeNetParam();	/* ����lwip������� */
					}

					/* ȥ����ť�Ľ���״̬ */
					FormHttp->ButtonRet.Focus = 0;
					LCD_DrawButton(&FormHttp->ButtonRet);	/* ����ѡ��״̬�İ�ť */

					FormHttp->ButtonSetIP.Focus = 0;
					LCD_DrawButton(&FormHttp->ButtonSetIP);	/* ����ѡ��״̬�İ�ť */

					FormHttp->ButtonLwSetIP.Focus = 0;
					LCD_DrawButton(&FormHttp->ButtonLwSetIP);	/* ����ѡ��״̬�İ�ť */					
					break;
			}
		}

		ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/* �м����� */
			switch (ucKeyCode)
			{
				case  JOY_DOWN_OK:		/* ҡ��OK�� */
					return;		/* ���� */

				default:
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	��������ʾ�ؼ��Ĵ���
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	�� �� ��: InitFormHttp
*	����˵��: ��ʼ���ؼ�����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormHttp(void)
{
	/* ��ť���� */
	FormHttp->FontBtn.FontCode = FC_ST_16;
	FormHttp->FontBtn.BackColor = CL_MASK;		/* ͸��ɫ */
	FormHttp->FontBtn.FrontColor = CL_BLACK;
	FormHttp->FontBtn.Space = 0;

	/* �༭������ */
	FormHttp->FontEdit.FontCode = FC_ST_16;
	FormHttp->FontEdit.BackColor = CL_MASK;		/* ͸��ɫ */
	FormHttp->FontEdit.FrontColor = CL_BLACK;
	FormHttp->FontEdit.Space = 0;

	/* ��ǩ���� */
	FormHttp->FontLabel.FontCode = FC_ST_16;
	FormHttp->FontLabel.BackColor = FORM_BACK_COLOR;	/* ���屳��ɫ */
	FormHttp->FontLabel.FrontColor = CL_BLACK;
	FormHttp->FontLabel.Space = 0;

	/* ����� */
	FormHttp->BoxUip.Left = BOX1_X;
	FormHttp->BoxUip.Top = BOX1_Y;
	FormHttp->BoxUip.Height = BOX1_H;
	FormHttp->BoxUip.Width = BOX1_W;
	FormHttp->BoxUip.pCaption = BOX1_TEXT;
	FormHttp->BoxUip.Font = &FormHttp->FontLabel;

	FormHttp->BoxLwip.Left = BOX2_X;
	FormHttp->BoxLwip.Top = BOX2_Y;
	FormHttp->BoxLwip.Height = BOX2_H;
	FormHttp->BoxLwip.Width = BOX2_W;
	FormHttp->BoxLwip.pCaption = BOX2_TEXT;
	FormHttp->BoxLwip.Font = &FormHttp->FontLabel;

	/* ��ǩ */
	FormHttp->LabelIP.Left = LABEL_IP_X;
	FormHttp->LabelIP.Top = LABEL_IP_Y;
	FormHttp->LabelIP.MaxLen = 0;
	FormHttp->LabelIP.pCaption = LABEL_IP_TEXT;
	FormHttp->LabelIP.Font = &FormHttp->FontLabel;

	FormHttp->LabelNetMask.Left = LABEL_MASK_X;
	FormHttp->LabelNetMask.Top = LABEL_MASK_Y;
	FormHttp->LabelNetMask.MaxLen = 0;
	FormHttp->LabelNetMask.pCaption = LABEL_MASK_TEXT;
	FormHttp->LabelNetMask.Font = &FormHttp->FontLabel;

	FormHttp->LabelGateway.Left = LABEL_GATE_X;
	FormHttp->LabelGateway.Top = LABEL_GATE_Y;
	FormHttp->LabelGateway.MaxLen = 0;
	FormHttp->LabelGateway.pCaption = LABEL_GATE_TEXT;
	FormHttp->LabelGateway.Font = &FormHttp->FontLabel;

	FormHttp->LabelLwIP.Left = LABEL_LWIP_X;
	FormHttp->LabelLwIP.Top = LABEL_LWIP_Y;
	FormHttp->LabelLwIP.MaxLen = 0;
	FormHttp->LabelLwIP.pCaption = LABEL_LWIP_TEXT;
	FormHttp->LabelLwIP.Font = &FormHttp->FontLabel;

	FormHttp->LabelLwNetMask.Left = LABEL_LWMASK_X;
	FormHttp->LabelLwNetMask.Top = LABEL_LWMASK_Y;
	FormHttp->LabelLwNetMask.MaxLen = 0;
	FormHttp->LabelLwNetMask.pCaption = LABEL_LWMASK_TEXT;
	FormHttp->LabelLwNetMask.Font = &FormHttp->FontLabel;

	FormHttp->LabelLwGateway.Left = LABEL_LWGATE_X;
	FormHttp->LabelLwGateway.Top = LABEL_LWGATE_Y;
	FormHttp->LabelLwGateway.MaxLen = 0;
	FormHttp->LabelLwGateway.pCaption = LABEL_LWGATE_TEXT;
	FormHttp->LabelLwGateway.Font = &FormHttp->FontLabel;


	/* �༭�� */
	/* uip */
	FormHttp->EditIP.Left = EDIT_IP_X;
	FormHttp->EditIP.Top = EDIT_IP_Y;
	FormHttp->EditIP.Height = EDIT_IP_H;
	FormHttp->EditIP.Width = EDIT_IP_W;
	FormHttp->EditIP.pCaption = FormHttp->EditIP.Text;
	FormHttp->EditIP.Font = &FormHttp->FontBtn;

	FormHttp->EditNetMask.Left = EDIT_MASK_X;
	FormHttp->EditNetMask.Top = EDIT_MASK_Y;
	FormHttp->EditNetMask.Height = EDIT_MASK_H;
	FormHttp->EditNetMask.Width = EDIT_MASK_W;
	FormHttp->EditNetMask.pCaption = FormHttp->EditNetMask.Text;
	FormHttp->EditNetMask.Font = &FormHttp->FontEdit;

	FormHttp->EditGateway.Left = EDIT_GATE_X;
	FormHttp->EditGateway.Top = EDIT_GATE_Y;
	FormHttp->EditGateway.Height = EDIT_GATE_H;
	FormHttp->EditGateway.Width = EDIT_GATE_W;
	FormHttp->EditGateway.pCaption = FormHttp->EditGateway.Text;
	FormHttp->EditGateway.Font = &FormHttp->FontEdit;

	/* lwip */
	FormHttp->EditLwIP.Left = EDIT_LWIP_X;
	FormHttp->EditLwIP.Top = EDIT_LWIP_Y;
	FormHttp->EditLwIP.Height = EDIT_LWIP_H;
	FormHttp->EditLwIP.Width = EDIT_LWIP_W;
	FormHttp->EditLwIP.pCaption = FormHttp->EditLwIP.Text;
	FormHttp->EditLwIP.Font = &FormHttp->FontBtn;

	FormHttp->EditLwNetMask.Left = EDIT_LWMASK_X;
	FormHttp->EditLwNetMask.Top = EDIT_LWMASK_Y;
	FormHttp->EditLwNetMask.Height = EDIT_LWMASK_H;
	FormHttp->EditLwNetMask.Width = EDIT_LWMASK_W;
	FormHttp->EditLwNetMask.pCaption = FormHttp->EditLwNetMask.Text;
	FormHttp->EditLwNetMask.Font = &FormHttp->FontEdit;

	FormHttp->EditLwGateway.Left = EDIT_LWGATE_X;
	FormHttp->EditLwGateway.Top = EDIT_LWGATE_Y;
	FormHttp->EditLwGateway.Height = EDIT_LWGATE_H;
	FormHttp->EditLwGateway.Width = EDIT_LWGATE_W;
	FormHttp->EditLwGateway.pCaption = FormHttp->EditLwGateway.Text;
	FormHttp->EditLwGateway.Font = &FormHttp->FontEdit;

	/* ��ť */
	FormHttp->ButtonRet.Left = BUTTON_RET_X;
	FormHttp->ButtonRet.Top = BUTTON_RET_Y;
	FormHttp->ButtonRet.Height = BUTTON_RET_H;
	FormHttp->ButtonRet.Width = BUTTON_RET_W;
	FormHttp->ButtonRet.pCaption = BUTTON_RET_TEXT;
	FormHttp->ButtonRet.Font = &FormHttp->FontBtn;
	FormHttp->ButtonRet.Focus = 0;

	FormHttp->ButtonSetIP.Left = BUTTON_IP_X;
	FormHttp->ButtonSetIP.Top = BUTTON_IP_Y;
	FormHttp->ButtonSetIP.Height = BUTTON_IP_H;
	FormHttp->ButtonSetIP.Width = BUTTON_IP_W;
	FormHttp->ButtonSetIP.pCaption = BUTTON_IP_TEXT;
	FormHttp->ButtonSetIP.Font = &FormHttp->FontBtn;
	FormHttp->ButtonSetIP.Focus = 0;

	FormHttp->ButtonLwSetIP.Left = BUTTON_LWIP_X;
	FormHttp->ButtonLwSetIP.Top = BUTTON_LWIP_Y;
	FormHttp->ButtonLwSetIP.Height = BUTTON_LWIP_H;
	FormHttp->ButtonLwSetIP.Width = BUTTON_LWIP_W;
	FormHttp->ButtonLwSetIP.pCaption = BUTTON_LWIP_TEXT;
	FormHttp->ButtonLwSetIP.Font = &FormHttp->FontBtn;
	FormHttp->ButtonLwSetIP.Focus = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: DispFormHttp
*	����˵��: ��ʾ���пؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFormHttp(void)
{
	sprintf(FormHttp->EditIP.Text, "%d.%d.%d.%d", g_tParam.uip_ip[0], g_tParam.uip_ip[1],
		g_tParam.uip_ip[2], g_tParam.uip_ip[3]);

	sprintf(FormHttp->EditNetMask.Text, "%d.%d.%d.%d", g_tParam.uip_net_mask[0], g_tParam.uip_net_mask[1],
		g_tParam.uip_net_mask[2], g_tParam.uip_net_mask[3]);

	sprintf(FormHttp->EditGateway.Text, "%d.%d.%d.%d", g_tParam.uip_gateway[0], g_tParam.uip_gateway[1],
		g_tParam.uip_gateway[2], g_tParam.uip_gateway[3]);

	sprintf(FormHttp->EditLwIP.Text, "%d.%d.%d.%d", g_tParam.lwip_ip[0], g_tParam.lwip_ip[1],
		g_tParam.lwip_ip[2], g_tParam.lwip_ip[3]);

	sprintf(FormHttp->EditLwNetMask.Text, "%d.%d.%d.%d", g_tParam.lwip_net_mask[0], g_tParam.lwip_net_mask[1],
		g_tParam.lwip_net_mask[2], g_tParam.lwip_net_mask[3]);

	sprintf(FormHttp->EditLwGateway.Text, "%d.%d.%d.%d", g_tParam.lwip_gateway[0], g_tParam.lwip_gateway[1],
		g_tParam.lwip_gateway[2], g_tParam.lwip_gateway[3]);

	/* Uip */
	LCD_DrawLabel(&FormHttp->LabelIP);
	LCD_DrawLabel(&FormHttp->LabelNetMask);
	LCD_DrawLabel(&FormHttp->LabelGateway);
	LCD_DrawEdit(&FormHttp->EditIP);
	LCD_DrawEdit(&FormHttp->EditNetMask);
	LCD_DrawEdit(&FormHttp->EditGateway);
	LCD_DrawButton(&FormHttp->ButtonSetIP);

	/* Lwip */
	LCD_DrawLabel(&FormHttp->LabelLwIP);
	LCD_DrawLabel(&FormHttp->LabelLwNetMask);
	LCD_DrawLabel(&FormHttp->LabelLwGateway);
	LCD_DrawEdit(&FormHttp->EditLwIP);
	LCD_DrawEdit(&FormHttp->EditLwNetMask);
	LCD_DrawEdit(&FormHttp->EditLwGateway);
	LCD_DrawButton(&FormHttp->ButtonLwSetIP);

	/* ���ذ�ť */
	LCD_DrawButton(&FormHttp->ButtonRet);

	LCD_DrawGroupBox(&FormHttp->BoxUip);
	LCD_DrawGroupBox(&FormHttp->BoxLwip);
}

/*
*********************************************************************************************************
*	������ uIP ���ֵĴ���
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*	�� �� ��: uip_start
*	����˵��: ����uip�� �������������ȫ�ֱ��� g_tParam.uip_ip, g_tParam.uip_net_mask, 
*			  g_tParam.uip_gateway 
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void uip_start(void)
{
	timer_set(&periodic_timer, CLOCK_SECOND / 2);
	timer_set(&arp_timer, CLOCK_SECOND * 10);

	tapdev_init();	/* ��ʼ������Ӳ�� */
	uip_init();

	//uip_ipaddr(ipaddr, 192,168,1,10);	/* ȱʡIP */
	uip_sethostaddr((uint16_t *)g_tParam.uip_ip);

	//uip_ipaddr(ipaddr, 255,255,255,0);	/* ��ַ���� */
	uip_setnetmask((uint16_t *)g_tParam.uip_net_mask);

	//uip_ipaddr(ipaddr, 192,168,1,1);	/* Ĭ������ */
	uip_setdraddr((uint16_t *)g_tParam.uip_gateway);

	httpd_init_uip();
}

/*
*********************************************************************************************************
*	�� �� ��: uip_ChangeNetParam
*	����˵��: ����������������� �������������ȫ�ֱ��� g_tParam.uip_ip, g_tParam.uip_net_mask, 
*			  g_tParam.uip_gateway 
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void uip_ChangeNetParam(void)
{
	//uip_ipaddr(ipaddr, 192,168,1,10);	/* ȱʡIP */
	uip_sethostaddr((uint16_t *)g_tParam.uip_ip);

	//uip_ipaddr(ipaddr, 255,255,255,0);	/* ��ַ���� */
	uip_setnetmask((uint16_t *)g_tParam.uip_net_mask);

	//uip_ipaddr(ipaddr, 192,168,1,1);	/* Ĭ������ */
	uip_setdraddr((uint16_t *)g_tParam.uip_gateway);
}

/*
*********************************************************************************************************
*	�� �� ��: uip_pro
*	����˵��: uip ��ѯ�����뵽��ѭ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void uip_pro(void)
{
	uint16_t i;

	uip_len = tapdev_read();
	if(uip_len > 0)
	{
		if (BUF->type == htons(UIP_ETHTYPE_IP))
		{
			uip_arp_ipin();
			uip_input();
			/* If the above function invocation resulted in data that
			should be sent out on the network, the global variable
			uip_len is set to a value > 0. */
			if (uip_len > 0)
			{
				uip_arp_out();
				tapdev_send();
			}
		}
		else if (BUF->type == htons(UIP_ETHTYPE_ARP))
		{
			uip_arp_arpin();
			/* If the above function invocation resulted in data that
			should be sent out on the network, the global variable
			uip_len is set to a value > 0. */
			if (uip_len > 0)
			{
				tapdev_send();
			}
		}
	}
	else if(timer_expired(&periodic_timer))
	{
		//printf("  0.5s periodic timer expired\n\r");
		timer_reset(&periodic_timer);
		for(i = 0; i < UIP_CONNS; i++)
		{
			uip_periodic(i);
			/* If the above function invocation resulted in data that
			should be sent out on the network, the global variable
			uip_len is set to a value > 0. */
			if(uip_len > 0)
			{
				uip_arp_out();
				tapdev_send();
			}
		}

		#if UIP_UDP
		for(i = 0; i < UIP_UDP_CONNS; i++)
		{
			uip_udp_periodic(i);
			/* If the above function invocation resulted in data that
			should be sent out on the network, the global variable
			uip_len is set to a value > 0. */
			if(uip_len > 0)
			{
				uip_arp_out();
				tapdev_send();
			}
		}
		#endif /* UIP_UDP */

		/* Call the ARP timer function every 10 seconds. */
		if (timer_expired(&arp_timer))
		{
			//printf("  10s periodic timer expired\n\r");
			timer_reset(&arp_timer);
			uip_arp_timer();
		}
	}
}

/*---------------------------------------------------------------------------*/
void uip_log(char *m)
{
	printf("uIP log message: %s\n", m);
}

void resolv_found(char *name, u16_t *ipaddr)
{
	//  u16_t *ipaddr2;

	if (ipaddr == NULL)
	{
		printf("Host '%s' not found.\n", name);
	}
	else
	{
		printf("Found name '%s' = %d.%d.%d.%d\n", name,
		htons(ipaddr[0]) >> 8,
		htons(ipaddr[0]) & 0xff,
		htons(ipaddr[1]) >> 8,
		htons(ipaddr[1]) & 0xff);
		/*    webclient_get("www.sics.se", 80, "/~adam/uip");*/
	}
}

#ifdef __DHCPC_H__
	void dhcpc_configured(const struct dhcpc_state *s)
	{
		uip_sethostaddr(s->ipaddr);
		uip_setnetmask(s->netmask);
		uip_setdraddr(s->default_router);
		resolv_conf(s->dnsaddr);
	}
#endif /* __DHCPC_H__ */

void smtp_done(unsigned char code)
{
	printf("SMTP done with code %d\n", code);
}
void webclient_closed(void)
{
	printf("Webclient: connection closed\n");
}
void webclient_aborted(void)
{
	printf("Webclient: connection aborted\n");
}

void webclient_timedout(void)
{
	printf("Webclient: connection timed out\n");
}

void webclient_connected(void)
{
	printf("Webclient: connected, waiting for data...\n");
}

void webclient_datahandler(char *data, u16_t len)
{
	printf("Webclient: got %d bytes of data.\n", len);
}

/*
*********************************************************************************************************
*	������ LwIP ���ֵĴ���
*********************************************************************************************************
*/

struct netif gnetif;

/*
*********************************************************************************************************
*	�� �� ��: uip_ChangeNetParam
*	����˵��: ����������������� �������������ȫ�ֱ��� g_tParam.uip_ip, g_tParam.uip_net_mask, 
*			  g_tParam.uip_gateway 
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void lwip_ChangeNetParam(void)
{
//	//uip_ipaddr(ipaddr, 192,168,1,10);	/* ȱʡIP */
//	uip_sethostaddr((uint16_t *)g_tParam.uip_ip);

//	//uip_ipaddr(ipaddr, 255,255,255,0);	/* ��ַ���� */
//	uip_setnetmask((uint16_t *)g_tParam.uip_net_mask);

//	//uip_ipaddr(ipaddr, 192,168,1,1);	/* Ĭ������ */
//	uip_setdraddr((uint16_t *)g_tParam.uip_gateway);
}

/**
  * @brief  Setup the network interface
  * @param  None
  * @retval None
  */
/*Static IP ADDRESS: IP_ADDR0.IP_ADDR1.IP_ADDR2.IP_ADDR3 */
#define IP_ADDR0   ((uint8_t) 192U)
#define IP_ADDR1   ((uint8_t) 168U)
#define IP_ADDR2   ((uint8_t) 1U)
#define IP_ADDR3   ((uint8_t) 86U)
   
/*NETMASK*/
#define NETMASK_ADDR0   ((uint8_t) 255U)
#define NETMASK_ADDR1   ((uint8_t) 255U)
#define NETMASK_ADDR2   ((uint8_t) 255U)
#define NETMASK_ADDR3   ((uint8_t) 0U)

/*Gateway Address*/
#define GW_ADDR0   ((uint8_t) 192U)
#define GW_ADDR1   ((uint8_t) 168U)
#define GW_ADDR2   ((uint8_t) 1U)
#define GW_ADDR3   ((uint8_t) 1U) 

static void Netif_Config(void)
{
  ip_addr_t ipaddr;
  ip_addr_t netmask;
  ip_addr_t gw;
	
#if LWIP_DHCP 
  ip_addr_set_zero_ip4(&ipaddr);
  ip_addr_set_zero_ip4(&netmask);
  ip_addr_set_zero_ip4(&gw);
#else	

  /* IP address default setting */
  IP4_ADDR(&ipaddr, IP_ADDR0, IP_ADDR1, IP_ADDR2, IP_ADDR3);
  IP4_ADDR(&netmask, NETMASK_ADDR0, NETMASK_ADDR1 , NETMASK_ADDR2, NETMASK_ADDR3);
  IP4_ADDR(&gw, GW_ADDR0, GW_ADDR1, GW_ADDR2, GW_ADDR3); 
	
#endif
  
  /* add the network interface */    
  netif_add(&gnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &ethernet_input);
  
  /*  Registers the default network interface */
  netif_set_default(&gnetif);
  
  ethernet_link_status_updated(&gnetif);
  
#if LWIP_NETIF_LINK_CALLBACK  
  netif_set_link_callback(&gnetif, ethernet_link_status_updated);
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: lwip_start
*	����˵��: ����lwip_start,  �������������ȫ�ֱ��� g_tParam.lwip_ip, g_tParam.lwip_net_mask, 
*			  g_tParam.lwip_gateway 
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void lwip_start(void)
{
	/* Initialize the LwIP stack */
	lwip_init();

	/* Configure the Network interface */
	Netif_Config();

	/* Http webserver Init */
	http_server_init();
}

/*
*********************************************************************************************************
*	�� �� ��: lwip_pro
*	����˵��: lwip ��ѯ�����뵽��ѭ����
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void lwip_pro(void)
{
    /* Read a received packet from the Ethernet buffers and send it 
       to the lwIP for handling */
    ethernetif_input(&gnetif);

    /* Handle timeouts */
    sys_check_timeouts();

#if LWIP_NETIF_LINK_CALLBACK     
    Ethernet_Link_Periodic_Handle(&gnetif);
#endif
    
#if LWIP_DHCP   
    DHCP_Periodic_Handle(&gnetif);
#endif 
}



/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
