/*
*********************************************************************************************************
*
*	ģ������ : ������ģ�顣
*	�ļ����� : main_menu.c
*	��    �� : V1.0
*	˵    �� :
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2012-08-08 armfly  ST�̼���V3.5.0�汾��
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"			/* printf����������������ڣ����Ա����������ļ� */
#include "images.h"			/* ͼ���ļ� */
#include "form_main_menu.h"
#include "main.h"

#define KEY_INFO	"K3:У׼ K1:���� OK:��ʾ����"

#define ICON_LEFT	16
#define ICON_TOP	25

#define TOP_BAR_H		20	/* �������߶� */
#define BOTTOM_BAR_H	20	/* �ײ����߶� */
#define CL_BAR			RGB(0, 73, 108)	/* �������͵ײ���������ɫ */

extern const unsigned char acRecorder[48 * 48 * 4];
extern const unsigned char acTape[48 * 48 * 4];
extern const unsigned char acCamera[48 * 48 * 4];
extern const unsigned char acChiLun[48 * 48 * 4];
extern const unsigned char acchujiao[48 * 48 * 4];

#define ICONS_COUNT     28
static ICON_T s_tMainIcons[ICONS_COUNT] =
{
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)achwinfo, "Ӳ����Ϣ"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acLCD, "�������Ͱ���"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acRadio, "������"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acRecorder, "¼����"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acchujiao, "MEMS������"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acweb, "WEB������"},

	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acGPS, "GPS��λģ��"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acPhone, "GPRSģ��"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acMP3, "MP3ģ��"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acUSBDisk, "ģ��U��"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acCamera, "����ͷ"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "OLED��ʾ��"},

	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acADC, "AD7606ģ��"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acADC, "AD7705ģ��"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acADC, "ADS1256ģ��"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "RS485�����"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "DAC8501"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "DAC8562"},

	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "Ԥ��"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "ʾ����"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "����WiFi"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "CAN����"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "AD9833ģ��"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "Ԥ��4"},

	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "Ԥ��5"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "Ԥ��6"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "Ԥ��7"},
	{ID_ICON, 0, 0, ICON_HEIGHT, ICON_WIDTH, (uint16_t *)acChiLun, "Ԥ��8"},

};

/* ͼ�������л�����״̬ */
static const uint8_t s_RetStatus[ICONS_COUNT] =
{
	MS_HARDWARE_INFO,	/* Ӳ����Ϣ */
	MS_TEST_TOUCH,		/* ���Դ����� */
	MS_RADIO,			/* ������ */
	MS_WM8978_REC,		/* ¼���� */
	MS_MEMS,			/* MEMS ���������ԣ�I2C�� ���������ǣ������ƣ���ѹ�� */
	MS_ETH_WEB,			/* ��̫��DM9000��DM9161 WEB������ */

	MS_GPS,				/* GPS���� */
	MS_GPRS,			/* GPRSģ����� */
	MS_MP3,				/* У׼����״̬ */
	MS_MASS_STORAGE,	/* SD����NANDģ��U�� */
	MS_CAMERA,			/* ����ͷ */
	MS_OLED,			/* OLED��ʾ�� */

	MS_AD7606,			/* ���� AD7606 */
	MS_AD7705,			/* AD7705ģ�� */
	MS_ADS1256,			/* ADS1256ģ�� */
	MS_RS485,			/* RS485ͨ�� */
	MS_DAC8501,			/* DAC8501 ģ�� */
	MS_DAC8562,			/* DAC8562 ģ�� */

	MS_RA8875,			/* RA8875�ֿ� */
	MS_DSO,				/* ʾ���� */
	MS_WIFI,			/* ����WIFI */
	MS_CAN,				/* CAN������� */
	MS_AD9833,			/* AD9833ģ�� */
	MS_RESERVED,

	MS_RESERVED,
	MS_RESERVED,
	MS_RESERVED,
	MS_RESERVED
};

static uint8_t s_IconsPageNo = 0;	/* ͼ��ҳ��, һ����ʾ����ʱ���Զ����е���2�� */
static uint8_t s_IconsPageMax;		/* ͼ��ҳ����� */
static uint8_t s_IconNumPerPage;		/* ÿҳ������������ͼƬ */

static int16_t s_IconsCursor = 0;	/* ѡ�е�ͼ�꣬ -1��ʾ��ѡ�� */
static uint16_t s_down_x;	/* ���ʰ���ʱ��x���� ; ����ʶ�𻬶����� */
static uint16_t s_down_y;	/* ���ʰ���ʱ��y���� ; ����ʶ�𻬶����� */

#define STYLE_COUNT			4		/* ��ɫ�������� */

/* �������� */
typedef struct
{
	uint16_t *pBmp;			/* ����ͼƬָ�룬�����0, ��ȡ��ɫ�������� BackColor */
	uint16_t BackColor;		/* ������ɫ */
	uint16_t TextColor;		/* ͼ���ı�����ɫ */
}STYLE_T;

/* ���˶δ���δ�ã�
	����ͼƬ��RGB565��ʽ�������NOR Flash
	NOR Flash ����ַ 0x6400 0000

	0x0020 0000   255K�ֽ� 480x272_��̲.bin  + 0x3FC00
	0x0023 FC00   255K�ֽ� 480x272_��Ҷ.bin
	0x0027 F800   255K�ֽ� 480x272_ˮ��.bin

	0x002B F400   755K�ֽ� 800x480_��̲.bin  + 0xBB800
	0x0037 AC00   755K�ֽ� 800x480_��Ҷ.bin
	0x0043 6400   755K�ֽ� 800x480_ˮ��.bin
*/

static const STYLE_T s_UI480[STYLE_COUNT] =
{
	/* ����ͼƬ,		������ɫ,			ͼ��������ɫ */
	{0,					CL_WHITE,			CL_BLACK},	/* ��ɫ���� */
	{0,					RGB(0,78,152),		CL_WHITE},	/* XP���������� */
	{0,					RGB(185,193,204),	CL_BLACK},	/* ��ɫ���� */
	{0,					RGB(38,38,38),		CL_WHITE},	/* ���ɫ���� */

//	{(uint16_t *)(0x64000000 + 0x00200000), 0,	CL_BLACK},	/* 480x272 ɳ̲���� */
//	{(uint16_t *)(0x64000000 + 0x0023FC00), 0,	CL_WHITE},	/* 480x272 ��Ҷ���� */
//	{(uint16_t *)(0x64000000 + 0x0027F800), 0,	CL_BLACK},	/* 480x272 ˮ�鱳�� */
};

static const STYLE_T s_UI800[STYLE_COUNT] =
{
	/* ����ͼƬ,		������ɫ,			ͼ��������ɫ */
	{0,					CL_WHITE,			CL_BLACK},	/* ��ɫ���� */
	{0,					RGB(0,78,152),		CL_WHITE},	/* XP���������� */
	{0,					RGB(185,193,204),	CL_BLACK},	/* ��ɫ���� */
	{0,					RGB(38,38,38),		CL_WHITE},	/* ���ɫ���� */

//	{(uint16_t *)(0x64000000 + 0x002BF400), 0,	CL_BLACK},	/* 800x480 ɳ̲���� */
//	{(uint16_t *)(0x64000000 + 0x0037AC00), 0,	CL_WHITE},	/* 800x480 ��Ҷ���� */
//	{(uint16_t *)(0x64000000 + 0x00436400), 0,	CL_BLACK},	/* 800x480 ˮ�鱳�� */
};

static uint16_t s_ColorIndex = 0;	/* ��ǰѡ�е���ɫ������� */

/* ͼ��������ɫ */
#define MAIN_TEXT_COLOR		((g_LcdWidth == 480)?s_UI480[s_ColorIndex].TextColor:s_UI800[s_ColorIndex].TextColor)

static void DispBackground(void);
static void ArrayIcon(void);
static void DispTopBar(void);
static void DispBottomBar(void);

/*
*********************************************************************************************************
*	�� �� ��: MainMenu
*	����˵��: ������
*	��    �Σ���
*	�� �� ֵ: ״̬��
*********************************************************************************************************
*/
uint8_t MainMenu(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fRefresh;		/* ˢ�������־,1��ʾ��Ҫˢ�� */
	FONT_T tIconFont;		/* ����һ������ṹ�����������ͼ���ı� */

	int16_t tpX, tpY;
	uint16_t i;

	DispTopBar();		/* ��ʾ������ */
	DispBottomBar();	/* ��ʾ�ײ��� */
	DispBackground();	/* ��ʾͼ�걳�� */

	/* ����������� */
	{
		tIconFont.FontCode = FC_ST_12;			/* ������� 16���� */
		tIconFont.FrontColor = MAIN_TEXT_COLOR;	/* ������ɫ */
		tIconFont.BackColor = CL_MASK;			/* ���ֱ�����ɫ */
		tIconFont.Space = 1;					/* ���ּ�࣬��λ = ���� */
	}

	//s_IconsPageNo = 0;	/* ͼ��ҳ��ţ���Ҫ���㡣 */

	fRefresh = 1;	/* 1��ʾ��Ҫˢ��LCD */
	while (g_MainStatus == MS_MAIN_MENU)
	{
		bsp_Idle();

		if (fRefresh)
		{
			fRefresh = 0;

			/* ��ʾͼ������ */
			ArrayIcon();	/* ����ͼ�� */
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */
					/* ����ͼ������ */
					{
						s_IconsCursor = -1;	/* -1 ��ʾ��ǰû��ͼ�걻������� */

						for (i = s_IconNumPerPage * s_IconsPageNo;
							i < s_IconNumPerPage * s_IconsPageNo + s_IconNumPerPage && i < ICONS_COUNT;
							i++)
						{
							if (TOUCH_InRect(tpX, tpY, s_tMainIcons[i].Left, s_tMainIcons[i].Top,
								 s_tMainIcons[i].Height, s_tMainIcons[i].Width))
							{
								s_IconsCursor = i;
								LCD_DrawIcon32(&s_tMainIcons[i], &tIconFont, 1);	/* 0 ��ʾ������ʾ�� 1��ʾѡ�� */
							}
						}
						s_down_x = tpX;
						s_down_y = tpY;
					}
					break;

				case TOUCH_MOVE:		/* �����ƶ��¼� */
					if (s_IconsCursor >= 0)
					{
						if (TOUCH_InRect(tpX, tpY, s_tMainIcons[s_IconsCursor].Left, s_tMainIcons[s_IconsCursor].Top,
								 s_tMainIcons[s_IconsCursor].Height, s_tMainIcons[s_IconsCursor].Width))
						{
							;
						}
						else
						{
							LCD_DrawIcon32(&s_tMainIcons[s_IconsCursor], &tIconFont, 0);	/* 0 ��ʾ������ʾ�� 1��ʾѡ�� */
							s_IconsCursor = -1;
						}
					}

					if (s_down_x > 0)
					{
						if (tpX - s_down_x > 50)
						{
							/* ���һ��� */
							if (s_IconsPageNo > 0)
							{
								s_IconsPageNo--;
								DispBackground();
								ArrayIcon();	/* ����ͼ�� */
							}
							s_down_x = 0;
						}
						else if (tpX - s_down_x < -50)
						{
							/* ���󻬶� */
							if (s_IconsPageNo < s_IconsPageMax - 1)
							{
								s_IconsPageNo++;
								DispBackground();
								ArrayIcon();	/* ����ͼ�� */
							}
							s_down_x = 0;
						}

					}
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */
					if (s_IconsCursor >= 0)
					{
						/* ����ʱ������͵���ʱ���궼��ͼ���ڲ�����Ч��� */
						if (TOUCH_InRect(tpX, tpY, s_tMainIcons[s_IconsCursor].Left, s_tMainIcons[s_IconsCursor].Top,
								 s_tMainIcons[s_IconsCursor].Height, s_tMainIcons[s_IconsCursor].Width)
							&& TOUCH_InRect(s_down_x, s_down_y, s_tMainIcons[s_IconsCursor].Left, s_tMainIcons[s_IconsCursor].Top,
								 s_tMainIcons[s_IconsCursor].Height, s_tMainIcons[s_IconsCursor].Width))
						{
							g_MainStatus = s_RetStatus[s_IconsCursor];	/* ���س���״̬ */
						}
						else
						{
							LCD_DrawIcon32(&s_tMainIcons[s_IconsCursor], &tIconFont, 0);	/* 0 ��ʾ������ʾ�� 1��ʾѡ�� */
							s_IconsCursor = -1;
						}
					}
					break;

				default:
					break;
			}
		}

		ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/* �м����� */
			switch (ucKeyCode)
			{
				case  JOY_DOWN_L:	/* ң���Ҽ� 2014-08-22 */
					return MS_RA8875;	/* ����RA8875���� */

				case  JOY_DOWN_U:	/* ң���ϼ� 2014-08-22 */
					return MS_TEST_TOUCH;	/* ���봥������ */

				case  JOY_DOWN_OK:	/* ң��OK�� */
					if (++g_LcdDirection > 3)
					{
						g_LcdDirection = 0;
					}
					/* ��ʾ������� 0 ��������, 1=����180�ȷ�ת, 2=����, 3=����180�ȷ�ת */
					LCD_SetDirection(g_LcdDirection);

					DispTopBar();		/* ��ʾ������ */
					DispBottomBar();	/* ��ʾ�ײ��� */
					DispBackground();	/* ��ʾ���� */
					fRefresh = 1;
					break;

				case  KEY_DOWN_K3:	/* K3�� */
					g_MainStatus = MS_CALIBRATION;	/* ���봥������ */
					break;

				case  KEY_DOWN_K1:	/* K1�� */
					if (++s_ColorIndex >= STYLE_COUNT)
					{
						s_ColorIndex = 0;
					}
					/* ���»��Ʊ��� */
					{
						DispBackground();	/* ��ʾ���� */

						/* ����������� */
						{
							tIconFont.FontCode = FC_ST_12;		/* ������� 16���� */
							tIconFont.FrontColor = MAIN_TEXT_COLOR;	/* ������ɫ */
							tIconFont.BackColor = CL_MASK;		/* ���ֱ�����ɫ */
							tIconFont.Space = 1;				/* ���ּ�࣬��λ = ���� */
						}
					}
					fRefresh = 1;
					break;

				default:
					break;
			}
		}
	}
	return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: DispTopBar
*	����˵��: ��ʾ̧ͷ������������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispTopBar(void)
{
	FONT_T tFont;			/* ����һ������ṹ���������������������� */

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;		/* ������� 16���� */
		tFont.FrontColor = CL_WHITE;	/* ������ɫ */
		tFont.BackColor = CL_BAR;		/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}
	LCD_Fill_Rect(0, 0, TOP_BAR_H, g_LcdWidth, CL_BAR);
	LCD_DispStr(5, 2, VER_INFO, &tFont);							/* ��ʾ����汾��Ϣ */
}

/*
*********************************************************************************************************
*	�� �� ��: DispBottomBar
*	����˵��: ��ʾ�ײ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispBottomBar(void)
{
	FONT_T tFont;			/* ����һ������ṹ���������������������� */

	/* ����������� */
	{
		tFont.FontCode = FC_ST_16;		/* ������� 16���� */
		tFont.FrontColor = CL_WHITE;	/* ������ɫ */
		tFont.BackColor = CL_BAR;		/* ���ֱ�����ɫ */
		tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
	}
	LCD_Fill_Rect(0, g_LcdHeight - BOTTOM_BAR_H, BOTTOM_BAR_H, g_LcdWidth, CL_BAR);
	LCD_DispStr(5, g_LcdHeight - 18, KEY_INFO, &tFont);			/* ��ʾ����������ʾ */
}

/*
*********************************************************************************************************
*	�� �� ��: DispBackground
*	����˵��: ��ʾ���汳��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispBackground(void)
{
#if 1
	if ((g_LcdWidth == 480) && (g_LcdHeight == 272))	/* 480 x 272 */
	{
		if (s_UI480[s_ColorIndex].pBmp == 0)
		{
			//LCD_ClrScr(s_UI480[s_ColorIndex].BackColor);  	/* ����, ��䵥ɫ */
			LCD_Fill_Rect(0, TOP_BAR_H, g_LcdHeight - TOP_BAR_H - BOTTOM_BAR_H, g_LcdWidth, s_UI480[s_ColorIndex].BackColor);
		}
		else
		{
			LCD_DrawBMP(0, TOP_BAR_H, g_LcdHeight - TOP_BAR_H - BOTTOM_BAR_H, g_LcdWidth, s_UI480[s_ColorIndex].pBmp);
		}
	}
	else if ((g_LcdWidth == 480) && (g_LcdHeight == 320))		/* 480 x 320 */
	{
		if (s_UI480[s_ColorIndex].pBmp == 0)
		{
			//LCD_ClrScr(s_UI800[s_ColorIndex].BackColor);  	/* ����, ��䵥ɫ */
			LCD_Fill_Rect(0, TOP_BAR_H, g_LcdHeight - TOP_BAR_H - BOTTOM_BAR_H, g_LcdWidth, s_UI800[s_ColorIndex].BackColor);
		}
		else
		{
			LCD_DrawBMP(0, TOP_BAR_H, g_LcdHeight - TOP_BAR_H - BOTTOM_BAR_H, g_LcdWidth, s_UI800[s_ColorIndex].pBmp);
		}
	}
	else	/* 800 x 480 */
	{
		if (s_UI800[s_ColorIndex].pBmp == 0)
		{
			//LCD_ClrScr(s_UI800[s_ColorIndex].BackColor);  	/* ����, ��䵥ɫ */
			LCD_Fill_Rect(0, TOP_BAR_H, g_LcdHeight - TOP_BAR_H - BOTTOM_BAR_H, g_LcdWidth, s_UI800[s_ColorIndex].BackColor);
		}
		else
		{
			LCD_DrawBMP(0, TOP_BAR_H, g_LcdHeight - TOP_BAR_H - BOTTOM_BAR_H, g_LcdWidth, s_UI800[s_ColorIndex].pBmp);
		}
	}
#else
	if ((g_LcdWidth == 480) && (g_LcdHeight == 272))	/* 480 x 272 */
	{
		if (s_UI480[s_ColorIndex].pBmp == 0)
		{
			LCD_ClrScr(s_UI480[s_ColorIndex].BackColor);  	/* ����, ��䵥ɫ */
		}
		else
		{
			LCD_DrawBMP(0, 0, g_LcdHeight, g_LcdWidth, s_UI480[s_ColorIndex].pBmp);
		}
	}
	else if ((g_LcdWidth == 480) && (g_LcdHeight == 320))		/* 480 x 320 */
	{
		if (s_UI480[s_ColorIndex].pBmp == 0)
		{
			LCD_ClrScr(s_UI800[s_ColorIndex].BackColor);  	/* ����, ��䵥ɫ */
		}
		else
		{
			LCD_DrawBMP(0, 0, g_LcdHeight, g_LcdWidth, s_UI800[s_ColorIndex].pBmp);
		}
	}
	else	/* 800 x 480 */
	{
		if (s_UI800[s_ColorIndex].pBmp == 0)
		{
			LCD_ClrScr(s_UI800[s_ColorIndex].BackColor);  	/* ����, ��䵥ɫ */
		}
		else
		{
			LCD_DrawBMP(0, 0, g_LcdHeight, g_LcdWidth, s_UI800[s_ColorIndex].pBmp);
		}
	}
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: ArrayIcon
*	����˵��: ����ͼ�� s_tMainIcons �����꣬ ȫ�ֱ��� s_IconsPageNo ��ʾҳ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void ArrayIcon(void)
{
	uint16_t x;
	uint16_t y;
	uint8_t h_count;	/* ˮƽ����ͼ�����и��� */
	uint8_t v_count;	/* ��ֱ����ͼ�����и��� */
	uint16_t icon_index;	/* ͼ������ */
	uint16_t icons_left;	/* ͼ������ X ���� */
	uint16_t icons_top;		/* ͼ������ Y ���� */
	uint8_t m, n;
	FONT_T tIconFont;

	tIconFont.FontCode = FC_ST_12;		/* ������� 16���� */
	tIconFont.FrontColor = MAIN_TEXT_COLOR;	/* ������ɫ */
	tIconFont.BackColor = CL_MASK;		/* ���ֱ�����ɫ */
	tIconFont.Space = 1;				/* ���ּ�࣬��λ = ���� */

	h_count = g_LcdWidth / ICON_STEP_X;
	v_count = (g_LcdHeight - TOP_BAR_H - BOTTOM_BAR_H) / ICON_STEP_Y;

	s_IconNumPerPage = h_count * v_count;	/* ÿ��ҳ�������ͼ����� */

	/* ������Ҫ����ͼƬҳ�� */
	s_IconsPageMax = (ICONS_COUNT -  1) / (h_count * v_count) + 1;

	icons_left = (g_LcdWidth - (h_count - 1) * ICON_STEP_X - ICON_WIDTH) / 2;
	icons_top = ICON_TOP;

	icon_index = s_IconsPageNo * h_count * v_count;

	x = icons_left;
	y = icons_top;
	for (m = 0; m < v_count; m++)
	{
		x = icons_left;
		y = icons_top + m * ICON_STEP_Y;
		for (n = 0; n < h_count; n++)
		{
			s_tMainIcons[icon_index].Left = x;
			s_tMainIcons[icon_index].Top = y;

			LCD_DrawIcon32(&s_tMainIcons[icon_index], &tIconFont, 0);	/* 0 ��ʾ������ʾ�� 1��ʾѡ�� */

			icon_index++;
			if (icon_index >= ICONS_COUNT)
			{
				break;
			}
			x += ICON_STEP_X;
		}
		if (icon_index >= ICONS_COUNT)
		{
			break;
		}
	}


}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
