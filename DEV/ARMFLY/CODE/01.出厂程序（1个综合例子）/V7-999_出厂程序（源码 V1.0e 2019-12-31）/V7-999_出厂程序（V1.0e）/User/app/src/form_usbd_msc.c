/*
*********************************************************************************************************
*
*	ģ������ : SD����NAND Flashģ��U�̳���
*	�ļ����� : usbd_msc_test.c
*	��    �� : V1.1
*	˵    �� : ʹ��USB Device�ӿڣ���PC�������2��U���豸:SD�� �� NAND Flash
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2013-02-01 armfly  �׷�
*		V1.1	2015-10-13 armfly  K3������NAND�������±��, ֮ǰ�ȹر�USB������PC��STM32ͬʱ����NAND��
*
*	Copyright (C), 2015-2020, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "usbd_usr.h"	/* usb�ײ����� */

#include "form_usbd_msc.h"

/* �������ṹ */
typedef struct
{
	FONT_T FontBlack;	/* ��̬������ */
	FONT_T FontBlue;	/* �仯���������� ��ɫ */
	FONT_T FontRed;	/* �仯���������� ��ɫ */
	FONT_T FontBtn;		/* ��ť������ */
	FONT_T FontBox;		/* ������������ */

	GROUP_T Box1;

	LABEL_T Label1;	LABEL_T Label2;	/* SD��״̬ */
	LABEL_T Label3; LABEL_T Label4;	/* NAND״̬ */
	LABEL_T Label5; LABEL_T Label6;	/* USBD״̬ */

	BUTTON_T Btn1;
	BUTTON_T Btn2;
	BUTTON_T Btn3;
	BUTTON_T Btn4;

	BUTTON_T BtnRet;

}FormUSB_T;

/* ���屳��ɫ */
#define FORM_BACK_COLOR		CL_BTN_FACE

/* 4���������ʹ�С */
#define BOX1_X	5
#define BOX1_Y	8
#define BOX1_H	(g_LcdHeight - BOX1_Y - 10)
#define BOX1_W	(g_LcdWidth -  2 * BOX1_X)
#define BOX1_TEXT	"SD����NAND Flashģ��U��"

/* ���ذ�ť������(��Ļ���½�) */
#define BTN_RET_H	32
#define BTN_RET_W	60
#define	BTN_RET_X	((BOX1_X + BOX1_W) - BTN_RET_W - 4)
#define	BTN_RET_Y	((BOX1_Y  + BOX1_H) - BTN_RET_H - 4)
#define	BTN_RET_TEXT	"����"

#define BTN1_H	32
#define BTN1_W	100
#define	BTN1_X	(BOX1_X + 5)
#define	BTN1_Y	(BOX1_Y + 100)
#define	BTN1_TEXT	"��ģ��U��"

#define BTN2_H	32
#define BTN2_W	100
#define	BTN2_X	(BTN1_X + BTN1_W + 10)
#define	BTN2_Y	BTN1_Y
#define	BTN2_TEXT	"�ر�ģ��U��"

#define BTN3_H	32
#define BTN3_W	100
#define	BTN3_X	BTN1_X
#define	BTN3_Y	(BTN1_Y + BTN1_H + 10)
#define	BTN3_TEXT	"�͸�NAND"

#define BTN4_H	32
#define BTN4_W	100
#define	BTN4_X	(BTN1_X + BTN1_W + 10)
#define	BTN4_Y	(BTN1_Y + BTN1_H + 10)
#define	BTN4_TEXT	"ɨ�軵��"

/* ��ǩ */
#define LABEL1_X  	(BOX1_X + 6)
#define LABEL1_Y	(BOX1_Y + 20)
#define LABEL1_TEXT	"SD��״̬ : "

	#define LABEL2_X  	(LABEL1_X + 100)
	#define LABEL2_Y	LABEL1_Y
	#define LABEL2_TEXT	"--"

#define LABEL3_X  	(LABEL1_X)
#define LABEL3_Y	(LABEL1_Y + 20)
#define LABEL3_TEXT	"NAND״̬ : "

	#define LABEL4_X  	(LABEL3_X + 100)
	#define LABEL4_Y	(LABEL3_Y)
	#define LABEL4_TEXT	"--"

#define LABEL5_X  	(LABEL1_X)
#define LABEL5_Y	(LABEL1_Y + 20 * 2)
#define LABEL5_TEXT	"USBD״̬ : "

	#define LABEL6_X  	(LABEL5_X + 100)
	#define LABEL6_Y	(LABEL5_Y)
	#define LABEL6_TEXT	"--"

static void InitFormUSB(void);
static void DispUSBInitFace(void);

FormUSB_T *FormUSB;

/*
*********************************************************************************************************
*	�� �� ��: TestUsbdMsc
*	����˵��: ����U�̳������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void TestUsbdMsc(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fQuit = 0;
	int16_t tpX, tpY;
	uint8_t ucNandOk;
	uint8_t ucCardOk;
	uint8_t ucUsbOk;
	FormUSB_T form;
	uint8_t fRefresh;
	NAND_BLOCK_INFO_T nand;	
	char buf[64];

	FormUSB = &form;

	InitFormUSB();
	DispUSBInitFace();


	{	
	#if 1		
		NAND_DispBadBlockInfo();	/* �򴮿�1��ӡNAND Flash������Ϣ (�˺�����ͷ��ʼFSMC) */

		/* ����FSMC����NAND Flash�� ��λNAND Flash���ؽ�LUT�� */
		if (NAND_Init() == NAND_OK)
		{
			printf("NAND_Init() Ok\r\n");
			ucNandOk = 1;
		}
		else
		{
			/* ��������ʽ�Ĳ�Ʒ�в�����Ϊ��Ԥ�ķ�ʽ�����ͼ���ʽ�� */
			/* �Զ����nand flash�Ƿ�����˵ͼ���ʽ�������û����ִ�и�ʽ����2�룩 */
			printf("NAND_Init() Error! \r\n");
			printf("Start Format(Low Level) NAND Flash......\r\n");
			if (NAND_Format() == NAND_OK)
			{
				printf("NAND Flash Format Ok\r\n");
			}
			else
			{
				printf("NAND Flash Format Error\r\n");
			}

			ucNandOk = 1;
		}

		if (NAND_GetBlockInfo(&nand) == 1)
		{
			ucNandOk = 1;
			
			#if 0
			NAND_DispParamPage();	/* ��ʾNAND оƬ�汾��Ϣ */
			#endif
		}
		else
		{
			ucNandOk = 0;
		}	
	#endif
		if (BSP_SD_Init() == MSD_OK)
		{
			ucCardOk = 1;
		}
		else
		{
			ucCardOk = 0;
		}
	}


	ucUsbOk = 1;
	usbd_OpenMassStorage();		/* ��ʼ��USB Device������ΪMass Storage */

	fRefresh = 1;

	/* ����������ѭ���� */
	while (fQuit == 0)
	{
		bsp_Idle();

		if (fRefresh)
		{
			fRefresh = 0;

			if (ucCardOk)
			{
				FormUSB->Label2.Font = &FormUSB->FontBlue;
				FormUSB->Label2.pCaption = "Ok ";
			}
			else
			{
				FormUSB->Label2.Font = &FormUSB->FontRed;
				FormUSB->Label2.pCaption = "Err";
			}
			LCD_DrawLabel(&FormUSB->Label2);

			if (ucNandOk)
			{
				FormUSB->Label4.pCaption = buf;
				sprintf(buf, "OK, %s, Bad=%d, Used=%d, Free=%d", nand.ChipName, nand.Bad, nand.Used, nand.Free);
				
				if (nand.Bad > 30)
				{
					/* ��������������30��������ʾ��ɫ */
					FormUSB->Label4.Font = &FormUSB->FontRed;
				}
				else
				{
					/* ����������������Χ�ڣ���ʾ��ɫ */
					FormUSB->Label4.Font = &FormUSB->FontBlue;
				}
			}
			else
			{
				FormUSB->Label4.Font = &FormUSB->FontRed;
				FormUSB->Label4.pCaption = "Err";
			}
			LCD_DrawLabel(&FormUSB->Label4);

			if (ucUsbOk)
			{
				FormUSB->Label6.Font = &FormUSB->FontBlue;
				FormUSB->Label6.pCaption = "�Ѵ� ";
			}
			else
			{
				FormUSB->Label6.Font = &FormUSB->FontRed;
				FormUSB->Label6.pCaption = "�ѹر�";
			}
			LCD_DrawLabel(&FormUSB->Label6);
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */					
					LCD_ButtonTouchDown(&FormUSB->BtnRet, tpX, tpY);
					LCD_ButtonTouchDown(&FormUSB->Btn1, tpX, tpY);
					LCD_ButtonTouchDown(&FormUSB->Btn2, tpX, tpY);
					LCD_ButtonTouchDown(&FormUSB->Btn3, tpX, tpY);
					LCD_ButtonTouchDown(&FormUSB->Btn4, tpX, tpY);					
					break;

				case TOUCH_MOVE:		/* �����ƶ��¼� */
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (LCD_ButtonTouchRelease(&FormUSB->BtnRet, tpX, tpY))
					{
						FormUSB->BtnRet.Focus = 0;
						LCD_DrawButton(&FormUSB->BtnRet);
						fQuit = 1;	/* ���� */
					}
					else if (LCD_ButtonTouchRelease(&FormUSB->Btn1, tpX, tpY))
					{
						FormUSB->Btn1.Focus = 0;
						LCD_DrawButton(&FormUSB->Btn1);

						if (BSP_SD_Init() == MSD_OK)
						{
							ucCardOk = 1;
						}
						else
						{
							ucCardOk = 0;
						}
						usbd_OpenMassStorage();	/* ��U�� */
						ucUsbOk = 1;
						fRefresh = 1;
					}
					else if (LCD_ButtonTouchRelease(&FormUSB->Btn2, tpX, tpY))
					{
						usbd_CloseMassStorage();	/* �ر�U�� */
						ucUsbOk = 0;
						fRefresh = 1;
					}
					else if (LCD_ButtonTouchRelease(&FormUSB->Btn3, tpX, tpY))
					{						
						if (NAND_Format() == NAND_OK)
						{
							FormUSB->Label4.pCaption = "�ͼ���ʽ���ɹ�";
						}
						else
						{
							FormUSB->Label4.pCaption = "�ͼ���ʽ��ʧ��";
						}
						LCD_DrawLabel(&FormUSB->Label4);
						
					}			
					else if (LCD_ButtonTouchRelease(&FormUSB->Btn4, tpX, tpY))
					{
						if (ucUsbOk == 0)	/* �ر�USB����ʱ���ܷ���NAND������USB�жϳ�������ɨ����� */
						{
							/* ����ɨ����Ի��飬ִ��ʱ��ܳ��� ����NAND �����޸� */						
							uint32_t i;
							
							FormUSB->Label4.pCaption = buf;
							FormUSB->Label4.Font = &FormUSB->FontRed;
							for (i = 0; i < NAND_BLOCK_COUNT; i++)
							{
								sprintf(buf, "ɨ�軵��... %d (%d%%) --- K1����ֹ", i, (i + 1) * 100 / NAND_BLOCK_COUNT);
								LCD_DrawLabel(&FormUSB->Label4);
								
								if (NAND_ScanBlock(i) == NAND_OK)
								{
									;
								}
								else
								{
									NAND_MarkBadBlock(i);	/* ��ǻ��� */
								}
								
								/* �������������£����˳� */
								if (bsp_GetKey() == KEY_DOWN_K1)
								{
									sprintf(buf, "ɨ����ֹ");
									LCD_DrawLabel(&FormUSB->Label4);
									break;
								}
							}		
						}
		
						else
						{
							FormUSB->Label4.Font = &FormUSB->FontRed;
							FormUSB->Label4.pCaption = buf;
							sprintf(buf, "���ȹر�ģ��U�̹���");
							LCD_DrawLabel(&FormUSB->Label4);
						}
					}				
					else	/* ��ťʧȥ���� */
					{
						LCD_ButtonTouchRelease(&FormUSB->BtnRet, tpX, tpY);
						LCD_ButtonTouchRelease(&FormUSB->Btn1, tpX, tpY);
						LCD_ButtonTouchRelease(&FormUSB->Btn2, tpX, tpY);
						LCD_ButtonTouchRelease(&FormUSB->Btn3, tpX, tpY);
						LCD_ButtonTouchRelease(&FormUSB->Btn4, tpX, tpY);
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
					//printf("��1 - �Ƴ�U�̡�\r\n");
					//usbd_CloseMassStorage();
					break;

				case KEY_DOWN_K2:		/* K2������ */
					/* ʹ��U�̣����ģ��U�̲��� */
					//printf("��2 - ʹ��U�̡�\r\n");
					//usbd_OpenMassStorage();
					break;

				case KEY_DOWN_K3:		/* K3������ */
					;
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

	usbd_CloseMassStorage();	/* �Ƴ�U�� */
}

/*
*********************************************************************************************************
*	�� �� ��: InitFormUSB
*	����˵��: ��ʼ��GPS��ʼ����ؼ�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormUSB(void)
{
	/* ������������ */
	FormUSB->FontBox.FontCode = FC_ST_16;
	FormUSB->FontBox.BackColor = CL_BTN_FACE;	/* �ͱ���ɫ��ͬ */
	FormUSB->FontBox.FrontColor = CL_BLACK;
	FormUSB->FontBox.Space = 0;

	/* ����1 ���ھ�ֹ��ǩ */
	FormUSB->FontBlack.FontCode = FC_ST_16;
	FormUSB->FontBlack.BackColor = CL_MASK;		/* ͸��ɫ */
	FormUSB->FontBlack.FrontColor = CL_BLACK;
	FormUSB->FontBlack.Space = 0;

	/* ����2 ���ڱ仯������ */
	FormUSB->FontBlue.FontCode = FC_ST_16;
	FormUSB->FontBlue.BackColor = CL_BTN_FACE;
	FormUSB->FontBlue.FrontColor = CL_BLUE;
	FormUSB->FontBlue.Space = 0;

	/* ����3 ���ڱ仯������ */
	FormUSB->FontRed.FontCode = FC_ST_16;
	FormUSB->FontRed.BackColor = CL_BTN_FACE;
	FormUSB->FontRed.FrontColor = CL_RED;
	FormUSB->FontRed.Space = 0;

	/* ��ť���� */
	FormUSB->FontBtn.FontCode = FC_ST_16;
	FormUSB->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormUSB->FontBtn.FrontColor = CL_BLACK;
	FormUSB->FontBtn.Space = 0;

	/* ����� */
	FormUSB->Box1.Left = BOX1_X;
	FormUSB->Box1.Top = BOX1_Y;
	FormUSB->Box1.Height = BOX1_H;
	FormUSB->Box1.Width = BOX1_W;
	FormUSB->Box1.pCaption = BOX1_TEXT;
	FormUSB->Box1.Font = &FormUSB->FontBox;

	/* ��̬��ǩ */
	FormUSB->Label1.Left = LABEL1_X;
	FormUSB->Label1.Top = LABEL1_Y;
	FormUSB->Label1.MaxLen = 0;
	FormUSB->Label1.pCaption = LABEL1_TEXT;
	FormUSB->Label1.Font = &FormUSB->FontBlack;

	FormUSB->Label3.Left = LABEL3_X;
	FormUSB->Label3.Top = LABEL3_Y;
	FormUSB->Label3.MaxLen = 0;
	FormUSB->Label3.pCaption = LABEL3_TEXT;
	FormUSB->Label3.Font = &FormUSB->FontBlack;

	FormUSB->Label5.Left = LABEL5_X;
	FormUSB->Label5.Top = LABEL5_Y;
	FormUSB->Label5.MaxLen = 0;
	FormUSB->Label5.pCaption = LABEL5_TEXT;
	FormUSB->Label5.Font = &FormUSB->FontBlack;

	/* ��̬��ǩ */
	FormUSB->Label2.Left = LABEL2_X;
	FormUSB->Label2.Top = LABEL2_Y;
	FormUSB->Label2.MaxLen = 0;
	FormUSB->Label2.pCaption = LABEL2_TEXT;
	FormUSB->Label2.Font = &FormUSB->FontBlue;

	FormUSB->Label4.Left = LABEL4_X;
	FormUSB->Label4.Top = LABEL4_Y;
	FormUSB->Label4.MaxLen = 0;
	FormUSB->Label4.pCaption = LABEL4_TEXT;
	FormUSB->Label4.Font = &FormUSB->FontBlue;

	FormUSB->Label6.Left = LABEL6_X;
	FormUSB->Label6.Top = LABEL6_Y;
	FormUSB->Label6.MaxLen = 0;
	FormUSB->Label6.pCaption = LABEL6_TEXT;
	FormUSB->Label6.Font = &FormUSB->FontBlue;

	/* ��ť */
	FormUSB->BtnRet.Left = BTN_RET_X;
	FormUSB->BtnRet.Top = BTN_RET_Y;
	FormUSB->BtnRet.Height = BTN_RET_H;
	FormUSB->BtnRet.Width = BTN_RET_W;
	FormUSB->BtnRet.pCaption = BTN_RET_TEXT;
	FormUSB->BtnRet.Font = &FormUSB->FontBtn;
	FormUSB->BtnRet.Focus = 0;

	FormUSB->Btn1.Left = BTN1_X;
	FormUSB->Btn1.Top = BTN1_Y;
	FormUSB->Btn1.Height = BTN1_H;
	FormUSB->Btn1.Width = BTN1_W;
	FormUSB->Btn1.pCaption = BTN1_TEXT;
	FormUSB->Btn1.Font = &FormUSB->FontBtn;
	FormUSB->Btn1.Focus = 0;

	FormUSB->Btn2.Left = BTN2_X;
	FormUSB->Btn2.Top = BTN2_Y;
	FormUSB->Btn2.Height = BTN2_H;
	FormUSB->Btn2.Width = BTN2_W;
	FormUSB->Btn2.pCaption = BTN2_TEXT;
	FormUSB->Btn2.Font = &FormUSB->FontBtn;
	FormUSB->Btn2.Focus = 0;

	FormUSB->Btn3.Left = BTN3_X;
	FormUSB->Btn3.Top = BTN3_Y;
	FormUSB->Btn3.Height = BTN3_H;
	FormUSB->Btn3.Width = BTN3_W;
	FormUSB->Btn3.pCaption = BTN3_TEXT;
	FormUSB->Btn3.Font = &FormUSB->FontBtn;
	FormUSB->Btn3.Focus = 0;


	FormUSB->Btn4.Left = BTN4_X;
	FormUSB->Btn4.Top = BTN4_Y;
	FormUSB->Btn4.Height = BTN4_H;
	FormUSB->Btn4.Width = BTN4_W;
	FormUSB->Btn4.pCaption = BTN4_TEXT;
	FormUSB->Btn4.Font = &FormUSB->FontBtn;
	FormUSB->Btn4.Focus = 0;
}

/*
*********************************************************************************************************
*	�� �� ��: DispUSBInitFace
*	����˵��: ��ʾ���еĿؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispUSBInitFace(void)
{
	LCD_ClrScr(CL_BTN_FACE);

	/* ����� */
	LCD_DrawGroupBox(&FormUSB->Box1);

	/* ��ǩ */
	LCD_DrawLabel(&FormUSB->Label1);
	LCD_DrawLabel(&FormUSB->Label2);
	LCD_DrawLabel(&FormUSB->Label3);
	LCD_DrawLabel(&FormUSB->Label4);
	LCD_DrawLabel(&FormUSB->Label5);
	LCD_DrawLabel(&FormUSB->Label6);

	/* ��ť */
	LCD_DrawButton(&FormUSB->BtnRet);
	LCD_DrawButton(&FormUSB->Btn1);
	LCD_DrawButton(&FormUSB->Btn2);
	LCD_DrawButton(&FormUSB->Btn3);
	LCD_DrawButton(&FormUSB->Btn4);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
