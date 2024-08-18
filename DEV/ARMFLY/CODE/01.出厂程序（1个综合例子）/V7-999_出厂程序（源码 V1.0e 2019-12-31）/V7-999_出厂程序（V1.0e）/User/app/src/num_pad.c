/*
*********************************************************************************************************
*
*	ģ������ : ����С���̽���
*	�ļ����� : main.c
*	��    �� : V1.0
*	˵    �� : ����С���̽��������򡣵�������С�����������롣
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-01-01  armfly  ��ʽ����
*		V1.1    2014-04-25  armfly  (1) �������������֧�� �����ַ������루Ӧ�ͻ�Ҫ��
*		V1.2	2015-07-22  armfly  
*					(1) �������������ĺ�����InputInt( ), �� InputNumber() �������·�װ
*					(2) �����ַ����빦��, NUMPAD_STR
*									
*
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"
#include "num_pad.h"

/* ����������󳤶� */
#define INPUT_MAX_LEN	32
	
/* �������ṹ */
typedef struct
{
	FONT_T FontBlack;	/* ��ɫ */
	FONT_T FontBlue;	/* ��ɫ */
	FONT_T FontBtn;		/* ���ư�ťF1-F4������ */
	FONT_T FontBtnNum;	/* ���ְ�ť������ */
	FONT_T FontWin;		/* WIN������ */	
	FONT_T FontEdit;	/* EDIT������ */

	EDIT_T Edit;
	WIN_T  Win;
	char *pTitle;	/* ���ڱ��� */
	
	BUTTON_T Btn1;
	BUTTON_T Btn2;
	BUTTON_T Btn3;
	BUTTON_T Btn4;
	BUTTON_T Btn5;
	BUTTON_T Btn6;
	BUTTON_T Btn7;
	BUTTON_T Btn8;
	BUTTON_T Btn9;
	BUTTON_T Btn0;
	BUTTON_T BtnA;
	BUTTON_T BtnB;
	BUTTON_T BtnF1;
	BUTTON_T BtnF2;
	BUTTON_T BtnF3;
	BUTTON_T BtnF4;	
	
	/* �༭����*/
	char *pEditText;	
}FormPad_T;

/* ������ʾģʽģʽ */
enum
{
	PAD_DIGIT	= 0,	/* ������ģʽ */
	PAD_CHAR,		/* �����ַ� */	
	PAD_ABC,		/* T9�����ֺ���ĸ */
	PAD_ABC0,		/* T9���������棬ѡ��0 ֮��*/
	PAD_ABC1,		/* T9���������棬ѡ��1 ֮��*/	
	PAD_ABC2,
	PAD_ABC3,
	PAD_ABC4,
	PAD_ABC5,
	PAD_ABC6,
	PAD_ABC7,
	PAD_ABC8,
	PAD_ABC9,	
	PAD_ABCA,
	PAD_ABCB
};

/* ����С���� ���� */
enum
{
#if  1		/* fot 480*272 */	
	/* ����С���̴���λ�� */
	WIN_NUM_X		= 100,
	WIN_NUM_Y		= 8,

	/* ���ְ�ť(0-9,.*)�߶ȺͿ��,ȫ��һ�� */
	BTN_H = 40,
	BTN_W = 70,

	/* ���ư�ť���˸�,CLR,RET,OK���߶ȺͿ��,ȫ��һ�� */
	BUTTON_CTRL_H = 40,
	BUTTON_CTRL_W = 80,

	/* ����С���̴��ڴ�С */
	WIN_NUM_H		= 249,
	WIN_NUM_W		= (BTN_W * 3) + BUTTON_CTRL_W + 8 + 6,

	/* �������ֱ༭�� */
	EDIT_NUM_X	= WIN_NUM_X + 4,
	EDIT_NUM_Y	= WIN_NUM_Y + 34,
	EDIT_NUM_H	= 40,
	EDIT_NUM_W	= WIN_NUM_W - 8,

#else	/* for 800 * 480 */
	/* ���ְ�ť(0-9,.*)�߶ȺͿ��,ȫ��һ�� */
	//BTN_H = 40,
	BTN_H = 60,
	BTN_W = 70,

	/* ���ư�ť���˸�,CLR,RET,OK���߶ȺͿ��,ȫ��һ�� */
	//BUTTON_CTRL_H = 40,
	BTN_F_H = 60,
	BTN_F_W = 80,

	/* ����С���̴��ڴ�С */
	WIN_NUM_H		= (BTN_H * 4 + 90),
	WIN_NUM_W		= (BTN_W * 3) + BUTTON_CTRL_W + 8 + 6,

	/* ����С���̴���λ�� (��800 * 480�ķֱ��ʾ���) */
	WIN_NUM_X		= ((800 - WIN_NUM_W) / 2),
	WIN_NUM_Y		= ((480 - WIN_NUM_H) / 2),
	
	/* �������ֱ༭�� */
	EDIT_NUM_X	= WIN_NUM_X + 4,
	EDIT_NUM_Y	= WIN_NUM_Y + 34,
	EDIT_NUM_H	= 40,
	EDIT_NUM_W	= WIN_NUM_W - 8,

#endif
	BUTTON_X_SPACE = BTN_W + 2,
	BUTTON_Y_SPACE = BTN_H + 2,
	
	/* ����1 ��ť */
	BTN1_X	= WIN_NUM_X + 4,
	BTN1_Y	= EDIT_NUM_Y + 43,
		/* ����2 ��ť */
		BTN2_X	= BTN1_X + BUTTON_X_SPACE,
		BTN2_Y	= BTN1_Y,
			/* ����3 ��ť */
			BTN3_X	= BTN1_X + 2 * BUTTON_X_SPACE,
			BTN3_Y	= BTN1_Y,
				/* F1 ��ť */
				BTNF1_X = BTN1_X + 3 * BUTTON_X_SPACE,
				BTNF1_Y = BTN1_Y,

	/* ����4 ��ť */
	BTN4_X	= BTN1_X,
	BTN4_Y	= BTN1_Y + BUTTON_Y_SPACE,
		/* ��5 ��ť */
		BTN5_X	= BTN1_X + BUTTON_X_SPACE,
		BTN5_Y	= BTN1_Y + BUTTON_Y_SPACE,
			/* ����6 ��ť */
			BTN6_X	= BTN1_X + 2 * BUTTON_X_SPACE,
			BTN6_Y	= BTN1_Y + BUTTON_Y_SPACE,
				/* F2 ��ť */
				BTNF2_X	= BTN1_X + 3 * BUTTON_X_SPACE,
				BTNF2_Y	= BTN1_Y + BUTTON_Y_SPACE,

	/* ����7 ��ť */
	BTN7_X	= BTN1_X,
	BTN7_Y	= BTN1_Y + 2 * BUTTON_Y_SPACE,
		/* ����8 ��ť */
		BTN8_X	= BTN1_X + BUTTON_X_SPACE,
		BTN8_Y	= BTN1_Y + 2 * BUTTON_Y_SPACE,
			/* ����9 ��ť */
			BTN9_X	= BTN1_X + 2 * BUTTON_X_SPACE,
			BTN9_Y	= BTN1_Y + 2 * BUTTON_Y_SPACE,
				/* F3 ��ť */
				BTNF3_X	= BTN1_X + 3 * BUTTON_X_SPACE,
				BTNF3_Y	= BTN1_Y + 2 * BUTTON_Y_SPACE,

	/* ����A ��ť */
	BTNA_X	= BTN1_X,
	BTNA_Y	= BTN1_Y + 3 * BUTTON_Y_SPACE,
		/* ����0 ��ť */
		BTN0_X	= BTN1_X + BUTTON_X_SPACE,
		BTN0_Y	= BTN1_Y + 3 * BUTTON_Y_SPACE,
			/* B ��ť */
			BTNB_X	= BTN1_X + 2 * BUTTON_X_SPACE,
			BTNB_Y	= BTN1_Y + 3 * BUTTON_Y_SPACE,
				/* F4��ť */
				BTNF4_X	= BTN1_X + 3 * BUTTON_X_SPACE,
				BTNF4_Y	= BTN1_Y + 3 * BUTTON_Y_SPACE,
};

/*��������Ч���ڡ�*/
enum
{
	WIN_INVALID_X = WIN_NUM_X + 15,
	WIN_INVALID_Y = WIN_NUM_Y + 90,
	WIN_INVALID_H = 100,
	WIN_INVALID_W = WIN_NUM_W - 40,

	LABLE_INVALID_X = WIN_INVALID_X + 5,
	LABLE_INVALID_Y = WIN_INVALID_Y + 38,
};

#define F1_NAME		"�˸�"
#define F2_NAME 	"���"
#define F3_NAME		"����"
#define F4_NAME		"ȷ��"

/* �ַ�����ģʽ��,2���˵������ַ� */
const char *g_key_tab[12 * 12] =
{
#if 1	
	"0", " ", "!", "@", "#", "$", "%", "=", "*", "(",  ")", "|",
	"1", "-", "[", "]", "\\", "/", ":", ";", "\042", " ",  " ", " ",
	"2", "a", "b", "c", "A", "B", "C", " ", " ", " ",  " ", " ",
	"3", "d", "e", "f", "D", "E", "F", " ", " ", " ",  " ", " ",
	"4", "g", "h", "i", "G", "H", "I", " ", " ", " ",  " ", " ",
	"5", "j", "k", "l", "J", "K", "L", " ", " ", " ",  " ", " ",
	"6", "m", "n", "o", "M", "N", "O", " ", " ", " ",  " ", " ",
	"7", "p", "q", "r", "s", "P", "Q", "R", "S", " ",  " ", " ",
	"8", "t", "u", "v", "T", "U", "V", " ", " ", " ",  " ", " ",
	"9", "w", "x", "y", "z", "W", "X", "Y", "Z", " ",  " ", " ",
	" ", " ", " ", " ", " ", " ", " ", " ", " ", " ",  " ", " ",
	" ", " ", " ", " ", " ", " ", " ", " ", " ", " ",  " ", " ",      
#else
	" ", "!", "@", "#", "$", "%", "=", "*", "(", "0", ")", "|", 
	"-", "[", "]", "\\", "/", ":", ";", "\042", " ", "1", " ",  
	"a", "b", "c", "A", "B", "C", " ", " ", " ", "2", " ", " ", 
	"d", "e", "f", "D", "E", "F", " ", " ", " ", "3", " ", " ", 
	"g", "h", "i", "G", "H", "I", " ", " ", " ", "4", " ", " ", 
	"j", "k", "l", "J", "K", "L", " ", " ", " ", "5", " ", " ", 
	"m", "n", "o", "M", "N", "O", " ", " ", " ", "6", " ", " ", 
	"p", "q", "r", "s", "P", "Q", "R", "S", " ", "7", " ", " ", 
	"t", "u", "v", "T", "U", "V", " ", " ", " ", "8", " ", " ", 
	"w", "x", "y", "z", "W", "X", "Y", "Z", " ", "9", " ", " ", 
	" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", 
	" ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ",    
#endif	
};
	
static void InitFormPad(void);
static void DispFormPad(void);
static void DispAllBtn(void);
static void SetBtnText(uint8_t _mode);
static uint8_t GetCharPad(uint8_t _key1, uint8_t _key2);

FormPad_T *FormPad;


/*
*********************************************************************************************************
*	�� �� ��: InputInt
*	����˵��: ��������С���̣������������ڲ����ж�ֵ��.
*	��    �Σ�
*			 _ucMode : ����ģʽ�� 0 ��ʾIP��ַ�� 1��ʾ������ͨ����
*			_Caption : ����С���̽���Ĵ��ڱ���
*			_pInParam : �������
*					_ucMode = NUMPAD_STR  ʱ����ʾ�����ַ����ĳ��ȣ�uint8_t ��
*					_ucMode = NUMPAD_INT  ��С������������ģʽ��(NUMPAD_INT_T *) 
*							typedef struct
*							{
*								int32_t Min;	// ��Сֵ
*								int32_t Max;	// ���ֵ
*								int32_t Return;	// ���������
*								uint8_t DotNum;	// С����λ��
*							}NUMPAD_INT_T;
*					_ucMode = NUMPAD_IP	���˲���������0
*					_ucMode = NUMPAD_TEL ���˲���������0
*			_pOutParam  : �������
*	�� �� ֵ: 1��ʾ������Ч  0 ��ʾ������Ч
*********************************************************************************************************
*/
uint8_t InputInt(char *_Caption, int32_t _min, int32_t _max, int32_t *_value)
{
	char buf[13];
	NUMPAD_INT_T Param;

	Param.Min = _min;		/* ��Сֵ */	
	Param.Max = _max;		/* ���ֵ */
	Param.DotNum = 0;	/* С����λ�� */
	
	if (InputNumber(NUMPAD_INT, _Caption, &Param, (void *)buf))
	{
		*_value = str_to_int(buf);
		return 1;
	}
	else
	{
		return 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: InputNumber
*	����˵��: ����С����
*	��    �Σ�
*			 _ucMode : ����ģʽ�� 0 ��ʾIP��ַ�� 1��ʾ������ͨ����
*			_Caption : ����С���̽���Ĵ��ڱ���
*			_pInParam : �������
*					_ucMode = NUMPAD_STR  ʱ����ʾ�����ַ����ĳ��ȣ�uint8_t ��
*					_ucMode = NUMPAD_INT  ��С������������ģʽ��(NUMPAD_INT_T *) 
*							typedef struct
*							{
*								int32_t Min;	// ��Сֵ
*								int32_t Max;	// ���ֵ
*								int32_t Return;	// ���������
*								uint8_t DotNum;	// С����λ��
*							}NUMPAD_INT_T;
*					_ucMode = NUMPAD_IP	���˲���������0
*					_ucMode = NUMPAD_TEL ���˲���������0
*			_pOutParam  : �������
*	�� �� ֵ: 1��ʾ������Ч  0 ��ʾ������Ч
*********************************************************************************************************
*/
uint8_t InputNumber(NUMPAD_MODE_E _Mode, char *_Caption, void *_pInParam, void *_pOutParam)
{
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fRefresh;		/* ˢ�������־,1��ʾ��Ҫˢ�� */
	int16_t tpX, tpY;
	uint8_t ucCursor;	/* ��� */
	uint8_t ucKeyValue = 0xFF;
	FormPad_T form;
	char cEditBuf[INPUT_MAX_LEN+2];	/* �༭������ֵ */
	uint8_t pad_layer = 0;
	uint8_t pad_key = 0;

	FormPad = &form;
	
	FormPad->pTitle = _Caption;
	FormPad->pEditText = cEditBuf;
	
	InitFormPad();
	
	/* IP��ַ */ /* �绰���� */ /* ����������Χ�жϣ� */ /* ������ĸ���� С���� */
	if (_Mode == NUMPAD_IP || _Mode == NUMPAD_TEL || _Mode == NUMPAD_INT)
	{
		SetBtnText(PAD_DIGIT);	/* �����ּ��� */
	}
	else
	{
		SetBtnText(PAD_ABC);	/* T9�����ֺ���ĸ */
	}	
	
	DispFormPad();
	
	ucCursor = 0;
	cEditBuf[0] = 0;
	fRefresh = 1;	/* 1��ʾ��Ҫˢ��LCD */
	while (1)
	{
		bsp_Idle();

		if (fRefresh)
		{
			fRefresh = 0;

			LCD_DrawEdit(&FormPad->Edit);
		}

		ucTouch = TOUCH_GetKey(&tpX, &tpY);	/* ��ȡ�����¼� */
		if (ucTouch != TOUCH_NONE)
		{
			switch (ucTouch)
			{
				case TOUCH_DOWN:		/* ���ʰ����¼� */				
					if (LCD_ButtonTouchDown(&FormPad->BtnF1, tpX, tpY))	/* �˸� */
					{
						if (ucCursor > 0)
						{
							ucCursor--;
						}		
						cEditBuf[ucCursor] = 0;	
						fRefresh = 1;
					
					}
					else if (LCD_ButtonTouchDown(&FormPad->BtnF2, tpX, tpY))
					{
						ucCursor = 0;		/* "���"; */
						cEditBuf[ucCursor] = 0;	
						fRefresh = 1;
					}
					else if (LCD_ButtonTouchDown(&FormPad->BtnF3, tpX, tpY))
					{
						if (pad_layer == 1)
						{								
							fRefresh = 1;
							FormPad->FontBtnNum.FrontColor = CL_BLACK;		/* ���ְ�ť�ú�ɫ���� */
							SetBtnText(PAD_ABC);
							DispAllBtn();	/* ֻˢ�°�ť */							
							pad_layer = 0;
						}
						else
						{						
							return 0;			/* "����"; */
						}
					}				
					else if (LCD_ButtonTouchDown(&FormPad->BtnF4, tpX, tpY))
					{
						;	/* "ȷ�� ���Ȱ�ť�ͷ�ʱ��ȷ���޸� */
					}				
					else
					{
						ucKeyValue = 0xFF;
						if (LCD_ButtonTouchDown(&FormPad->Btn0, tpX, tpY))
						{	
							ucKeyValue = '0';							
						}				
						else if (LCD_ButtonTouchDown(&FormPad->Btn1, tpX, tpY))
						{
							ucKeyValue = '1';
						}				
						else if (LCD_ButtonTouchDown(&FormPad->Btn2, tpX, tpY))
						{
							ucKeyValue = '2';
						}
						else if (LCD_ButtonTouchDown(&FormPad->Btn3, tpX, tpY))
						{
							ucKeyValue = '3';
						}
						else if (LCD_ButtonTouchDown(&FormPad->Btn4, tpX, tpY))
						{
							ucKeyValue = '4';
						}
						else if (LCD_ButtonTouchDown(&FormPad->Btn5, tpX, tpY))
						{
							ucKeyValue = '5';
						}	
						else if (LCD_ButtonTouchDown(&FormPad->Btn6, tpX, tpY))
						{
							ucKeyValue = '6';
						}	
						else if (LCD_ButtonTouchDown(&FormPad->Btn7, tpX, tpY))
						{
							ucKeyValue = '7';
						}
						else if (LCD_ButtonTouchDown(&FormPad->Btn8, tpX, tpY))
						{
							ucKeyValue = '8';
						}	
						else if (LCD_ButtonTouchDown(&FormPad->Btn9, tpX, tpY))
						{
							ucKeyValue = '9';
						}	
						else if (LCD_ButtonTouchDown(&FormPad->BtnA, tpX, tpY))
						{
							ucKeyValue = 'A';
						}
						/* �Ǻż��޸�Ϊ���ţ������������ʾ ----- */
						else if (LCD_ButtonTouchDown(&FormPad->BtnB, tpX, tpY))
						{
							ucKeyValue = 'B';
						}
							
						if (_Mode == NUMPAD_STR)
						{
							if (ucKeyValue != 0xFF)
							{
								if (pad_layer == 0)
								{
									pad_layer = 1;			/* ����2������ */
									pad_key = ucKeyValue;		
			
									if (ucKeyValue == 'A')
									{
										SetBtnText(PAD_ABCA);
									}
									else if (ucKeyValue == 'B')
									{
										SetBtnText(PAD_ABCB);
									}
									else  if (ucKeyValue >= '0' && ucKeyValue <= '9')
									{
										SetBtnText(pad_key - '0' + PAD_ABC0);	/* �����ּ��� */
									}		
									else
									{
										continue;
									}
									FormPad->FontBtnNum.FrontColor = CL_BLUE;		/* ���ְ�ť����ɫ���� */
									DispAllBtn();	/* ֻˢ�°�ť */
									continue;
								}
								else		/* �ڶ����˵� */
								{
									ucKeyValue = GetCharPad(pad_key, ucKeyValue);	/* �õ�������ַ� */									
									pad_layer = 2;
																	
									LCD_DrawEdit(&FormPad->Edit);
								}
							}								
						}
						else
						{
							if (ucKeyValue == 'A')
							{
								ucKeyValue = '.';
							}
							else if (ucKeyValue == 'B')
							{
								ucKeyValue = '-';
							}
						}
	
						if (ucKeyValue != 0xFF)
						{
							if (ucCursor < INPUT_MAX_LEN)
							{
								cEditBuf[ucCursor++] = ucKeyValue;
							}
							else
							{
								cEditBuf[ucCursor - 1] = ucKeyValue;	/* ��겻�ƶ����������1������ */
							}			
							cEditBuf[ucCursor] = 0;	
							fRefresh = 1;		
						}
					}
					break;

				case TOUCH_MOVE:		/* �����ƶ��¼� */
					break;

				case TOUCH_RELEASE:		/* �����ͷ��¼� */	
					
					if (_Mode == NUMPAD_STR)	/* �����ַ��ķ�֧ */				
					{
						if (pad_layer == 2)
						{
							pad_layer = 0;			/* ����2������ */
					
							FormPad->FontBtnNum.FrontColor = CL_BLACK;		/* ���ְ�ť�ú�ɫ���� */
							SetBtnText(PAD_ABC);
							DispAllBtn();	/* ˢ�°�ť */
						}				
					}
				
					LCD_ButtonTouchRelease(&FormPad->Btn0, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->Btn1, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->Btn2, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->Btn3, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->Btn4, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->Btn5, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->Btn6, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->Btn7, tpX, tpY);			
					LCD_ButtonTouchRelease(&FormPad->Btn8, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->Btn9, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->BtnA, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->BtnB, tpX, tpY);	
					LCD_ButtonTouchRelease(&FormPad->BtnF1, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->BtnF2, tpX, tpY);
					LCD_ButtonTouchRelease(&FormPad->BtnF3, tpX, tpY);		
				
					if (LCD_ButtonTouchRelease(&FormPad->BtnF4, tpX, tpY))	/* F4�� */
					{
						/* ����� ok ��ť */
						if (_Mode == NUMPAD_IP)			/* IP��ַ����ģʽ. ��IP��ַ��4���ֶν���Ϊ4������ */
						{
							int32_t iIP;
							uint8_t ucDotNum;	/* С��������������ж����ݺϷ� */
							int16_t i;
							NUMPAD_IP_T *pIP = (NUMPAD_IP_T *)_pOutParam;	/* IP��ַ����ģʽ */

							ucDotNum = 0;
							for (i = 0; i < ucCursor; i++)
							{
								if (cEditBuf[i] == '.')
								{
									ucDotNum++;
								}
							}
							if (ucDotNum != 3)	/* �������3��С���� */
							{
								DispInvlidInput();	/* ��ʾ������Ч */

								fRefresh = 1;
							}
							else
							{
								/* ��3��С���㣬��ʼ����IP��ַ */
								ucDotNum = 0;
								iIP = 0;
								for (i = 0; i < ucCursor; i++)
								{
									if ((cEditBuf[i] == '.') || (i == ucCursor - 1))
									{
										if (i == ucCursor - 1)
										{
											iIP = iIP * 10 + cEditBuf[i] - '0';
										}
										if (iIP <= 255)
										{
											pIP->ip_buf[ucDotNum] = iIP;
											ucDotNum++;

											if (ucDotNum == 4)
											{
												return 1;	/* ������ȷ��� */
											}
											iIP = 0;
										}
										else
										{
											DispInvlidInput();	/* ��ʾ������Ч */
											fRefresh = 1;
											break;
										}
									}
									else
									{
										iIP = iIP * 10 + cEditBuf[i] - '0';
									}
								}
							}
						}
						else if (_Mode == NUMPAD_TEL)			/* �绰��������ģʽ�� ֱ�Ӹ��ƽ����С��������ǰ��ֹ�� */
						{
							if (ucCursor < 3)
							{
								DispInvlidInput();	/* ��ʾ������Ч */
								fRefresh = 1;
								break;
							}

							strcpy((char *)_pOutParam, cEditBuf);
							return 1;	/* ������ȷ��� */
						}
						else if (_Mode == NUMPAD_INT)	/* NUMPAD_INT - ��С������������ģʽ ���÷�֧��δ���ԡ�*/
						{
							int32_t iRet;
							uint8_t ucDotNum;	/* С��������������ж����ݺϷ� */
							int16_t i;
							NUMPAD_INT_T *pInt = (NUMPAD_INT_T *)_pInParam;	/* ��С������������ģʽ */

							ucDotNum = 0;
							for (i = 0; i < ucCursor; i++)
							{
								if (cEditBuf[i] == '.')
								{
									ucDotNum++;
								}
							}
							if (ucDotNum > 1)
							{
								/* С�����������1 */
								iRet = -1;
							}
							else if (ucDotNum == 1)	/* 1��С���� */
							{
								int32_t iDotValue;
								int32_t iMult;

								/* �ȴ���С����ǰ������� */
								iRet = 0;
								for (i = 0; i <  ucCursor; i++)
								{
									if (cEditBuf[i] == '.')
									{
										break;
									}
									iRet *= 10;
									iRet += cEditBuf[i] - '0';
								}
								iRet *= 1000;

								/* �ٴ���С������������ */
								iDotValue = 0;
								iMult = 100;
								i++;
								for ( ; i <  ucCursor; i++)
								{
									iDotValue += iMult * (cEditBuf[i] - '0');
									iMult /= 10;
								}

								iRet += iDotValue;
							}
							else
							{
								/* ȫ������ */
								iRet = 0;
								for (i = 0; i <  ucCursor; i++)
								{
									iRet *= 10;
									iRet += cEditBuf[i] - '0';
								}
							}

							if ((ucCursor == 0) || (iRet < pInt->Min) || (iRet > pInt->Max ))
							{
								DispInvlidInput();	/* ��ʾ������Ч */

								DispFormPad();	/* ��ˢ���� */
								fRefresh = 1;
							}
							else
							{
								strcpy((char *)_pOutParam, cEditBuf);
								return 1;	/* ������Ч���� */
							}
						}
						else if (_Mode == NUMPAD_STR)	/* �����ַ��ķ�֧ */
						{
							uint8_t len = *(uint8_t *)_pInParam;								
							
							/* ���ַ������Ƚ������� */
							if (ucCursor > len)
							{
								DispInvlidInput();	/* ��ʾ������Ч */
								DispFormPad();	/* ��ˢ���� */
								fRefresh = 1;
								break;
							}
							strcpy((char *)_pOutParam, cEditBuf);							
							return 1;	/* ������ȷ��� */							
						}
					}
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ����DispInvlidInput
*	����˵������ʾ������ЧС���ڣ��ӳ�2��
*	��    �Σ���
*	�� �� ֵ����
*********************************************************************************************************
*/
void DispInvlidInput(void)
{
	WIN_T tWin;
	LABEL_T tLable;
	FONT_T tFont;

	tFont.FontCode = FC_ST_16;			/* ������� 16���� */
	tFont.FrontColor = CL_RED;			/* ������ɫ */
	tFont.BackColor = CL_MASK;			/* ���ֱ�����ɫ */
	tFont.Space = 0;					/* ���ּ�࣬��λ = ���� */

	/* ���������� */
	tWin.Font = &tFont;

	tWin.Left = WIN_INVALID_X;
	tWin.Top = WIN_INVALID_Y;
	tWin.Height = WIN_INVALID_H;
	tWin.Width = WIN_INVALID_W;
	tWin.pCaption = "Warning";
	LCD_DrawWin(&tWin);

	tLable.Font = &tFont;
	tLable.Left = LABLE_INVALID_X;
	tLable.Top = LABLE_INVALID_Y;
	tLable.MaxLen = 0;
	tLable.pCaption = "������Ч!";
	LCD_DrawLabel(&tLable);

	bsp_DelayMS(2000);
}

/*
*********************************************************************************************************
*	�� �� ��: ClearWinNumPad
*	����˵����������ּ��̴��ڣ������������ػ�)
*	��    �Σ�_usColor : ��ɫ
*	�� �� ֵ����
*********************************************************************************************************
*/
void ClearWinNumPad(uint16_t _usColor)
{
	LCD_Fill_Rect(WIN_NUM_X, WIN_NUM_Y, WIN_NUM_H, WIN_NUM_W, _usColor);
}

/*
*********************************************************************************************************
*	�� �� ��: InitFormPad
*	����˵��: ��ʼ���ؼ�����, ���ꡢ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void InitFormPad(void)
{
	/* ����1  */
	FormPad->FontBlack.FontCode = FC_ST_16;
	FormPad->FontBlack.BackColor = CL_MASK;		/* ͸��ɫ */
	FormPad->FontBlack.FrontColor = CL_BLACK;
	FormPad->FontBlack.Space = 0;

	/* ���� ��ɫ */
	FormPad->FontBlue.FontCode = FC_ST_16;
	FormPad->FontBlue.BackColor = CL_BTN_FACE;
	FormPad->FontBlue.FrontColor = CL_BLUE;
	FormPad->FontBlue.Space = 0;

	/* ��ť���� */
	FormPad->FontBtn.FontCode = FC_ST_16;
	FormPad->FontBtn.BackColor = CL_MASK;		/* ͸������ */
	FormPad->FontBtn.FrontColor = CL_BLACK;
	FormPad->FontBtn.Space = 0;

	FormPad->FontBtnNum.FontCode = FC_ST_16;
	FormPad->FontBtnNum.BackColor = CL_MASK;		/* ͸������ */
	FormPad->FontBtnNum.FrontColor = CL_BLACK;
	FormPad->FontBtnNum.Space = 0;	

	/* WIN���� */
	FormPad->FontWin.FontCode = FC_ST_16;
	FormPad->FontWin.BackColor = CL_MASK;	/* ͸��ɫ */
	FormPad->FontWin.FrontColor = CL_WHITE;
	FormPad->FontWin.Space = 0;

	/* Edit���� */
	FormPad->FontEdit.FontCode = FC_ST_16;
	FormPad->FontEdit.BackColor = CL_MASK;	/* ͸��ɫ */
	FormPad->FontEdit.FrontColor = CL_RED;
	FormPad->FontEdit.Space = 0;

	/* win */
	FormPad->Win.Font = &FormPad->FontWin;;
	FormPad->Win.Left = WIN_NUM_X;
	FormPad->Win.Top = WIN_NUM_Y;
	FormPad->Win.Height = WIN_NUM_H;
	FormPad->Win.Width = WIN_NUM_W;
	FormPad->Win.pCaption = FormPad->pTitle;	/* �βΣ�ȫ�ֵ� */
	
	/* Edit */
	FormPad->Edit.Font = &FormPad->FontEdit;
	FormPad->Edit.Left = EDIT_NUM_X;
	FormPad->Edit.Top = EDIT_NUM_Y;
	FormPad->Edit.Height = EDIT_NUM_H;
	FormPad->Edit.Width = EDIT_NUM_W;	
	FormPad->Edit.pCaption = FormPad->pEditText;

	/* ��ť */
	// LCD_InitButton(BUTTON_T *_btn, uint16_t _x, uint16_t _y, uint16_t _h, uint16_t _w, char *_pCaption, FONT_T *_pFont);
	LCD_InitButton(&FormPad->Btn0,  BTN0_X,  BTN0_Y,  BTN_H, BTN_W, "0", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->Btn1,  BTN1_X,  BTN1_Y,  BTN_H, BTN_W, "1", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->Btn2,  BTN2_X,  BTN2_Y,  BTN_H, BTN_W, "2", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->Btn3,  BTN3_X,  BTN3_Y,  BTN_H, BTN_W, "3", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->Btn4,  BTN4_X,  BTN4_Y,  BTN_H, BTN_W, "4", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->Btn5,  BTN5_X,  BTN5_Y,  BTN_H, BTN_W, "5", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->Btn6,  BTN6_X,  BTN6_Y,  BTN_H, BTN_W, "6", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->Btn7,  BTN7_X,  BTN7_Y,  BTN_H, BTN_W, "7", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->Btn8,  BTN8_X,  BTN8_Y,  BTN_H, BTN_W, "8", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->Btn9,  BTN9_X,  BTN9_Y,  BTN_H, BTN_W, "9", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->BtnA,  BTNA_X,  BTNA_Y,  BTN_H, BTN_W, ".", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->BtnB,  BTNB_X,  BTNB_Y,  BTN_H, BTN_W, "-", &FormPad->FontBtnNum);
	LCD_InitButton(&FormPad->BtnF1, BTNF1_X, BTNF1_Y, BTN_H, BTN_W, "�˸�", &FormPad->FontBtn);
	LCD_InitButton(&FormPad->BtnF2, BTNF2_X, BTNF2_Y, BTN_H, BTN_W, "���", &FormPad->FontBtn);
	LCD_InitButton(&FormPad->BtnF3, BTNF3_X, BTNF3_Y, BTN_H, BTN_W, "����", &FormPad->FontBtn);
	LCD_InitButton(&FormPad->BtnF4, BTNF4_X, BTNF4_Y, BTN_H, BTN_W, "ȷ��", &FormPad->FontBtn);	
}

/*
*********************************************************************************************************
*	�� �� ��: SetBtnText
*	����˵��: �л����밴ť�����֡� 0-9 ��12����ť��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void SetBtnText(uint8_t _mode)
{	
	if (_mode == PAD_DIGIT)		/* ������ */
	{
		FormPad->Btn0.pCaption = "0";
		FormPad->Btn1.pCaption = "1";
		FormPad->Btn2.pCaption = "2";
		FormPad->Btn3.pCaption = "3";
		FormPad->Btn4.pCaption = "4";
		FormPad->Btn5.pCaption = "5";
		FormPad->Btn6.pCaption = "6";
		FormPad->Btn7.pCaption = "7";
		FormPad->Btn8.pCaption = "8";
		FormPad->Btn9.pCaption = "9";
		FormPad->BtnA.pCaption = ".";
		FormPad->BtnB.pCaption = "-";		
	}
	else if (_mode == PAD_ABC)	/* ���ֺ��ַ���� */
	{
		FormPad->Btn0.pCaption = "0 ����";
		FormPad->Btn1.pCaption = "1 ����";
		FormPad->Btn2.pCaption = "2 abc";
		FormPad->Btn3.pCaption = "3 def";
		FormPad->Btn4.pCaption = "4 ghi";
		FormPad->Btn5.pCaption = "5 jkl";
		FormPad->Btn6.pCaption = "6 mno";
		FormPad->Btn7.pCaption = "7 pqrs";
		FormPad->Btn8.pCaption = "8 tuv";
		FormPad->Btn9.pCaption = "9 wxyz";
		FormPad->BtnA.pCaption = ".";
		FormPad->BtnB.pCaption = "-";		
	}
	else if (_mode >= PAD_ABC0 && _mode <= PAD_ABCB)	/* ���ֺ��ַ���� 0 ���º�Ķ������� */
	{
		uint8_t index1;
		
		index1 = _mode - PAD_ABC0;
		
		FormPad->Btn0.pCaption = (char *)g_key_tab[index1 * 12];
		FormPad->Btn1.pCaption = (char *)g_key_tab[index1 * 12 + 1];
		FormPad->Btn2.pCaption = (char *)g_key_tab[index1 * 12 + 2];
		FormPad->Btn3.pCaption = (char *)g_key_tab[index1 * 12 + 3];
		FormPad->Btn4.pCaption = (char *)g_key_tab[index1 * 12 + 4];
		FormPad->Btn5.pCaption = (char *)g_key_tab[index1 * 12 + 5];
		FormPad->Btn6.pCaption = (char *)g_key_tab[index1 * 12 + 6];
		FormPad->Btn7.pCaption = (char *)g_key_tab[index1 * 12 + 7];
		FormPad->Btn8.pCaption = (char *)g_key_tab[index1 * 12 + 8];
		FormPad->Btn9.pCaption = (char *)g_key_tab[index1 * 12 + 9];
		FormPad->BtnA.pCaption = (char *)g_key_tab[index1 * 12 + 10];
		FormPad->BtnB.pCaption = (char *)g_key_tab[index1 * 12 + 11];
		
		/* ֻ��ʾ���ֲ��� */
		LCD_DrawButton(&FormPad->Btn0);	
		LCD_DrawButton(&FormPad->Btn1);
		LCD_DrawButton(&FormPad->Btn2);
		LCD_DrawButton(&FormPad->Btn3);
		LCD_DrawButton(&FormPad->Btn4);
		LCD_DrawButton(&FormPad->Btn5);
		LCD_DrawButton(&FormPad->Btn6);
		LCD_DrawButton(&FormPad->Btn5);
		LCD_DrawButton(&FormPad->Btn6);
		LCD_DrawButton(&FormPad->Btn7);
		LCD_DrawButton(&FormPad->Btn8);
		LCD_DrawButton(&FormPad->Btn9);
		LCD_DrawButton(&FormPad->BtnA);
		LCD_DrawButton(&FormPad->BtnB);						
	}
}

/*
*********************************************************************************************************
*	�� �� ��: GetCharPad
*	����˵��: ���ݲ˵�ID��þ�����ַ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static uint8_t GetCharPad(uint8_t _key1, uint8_t _key2)
{
	uint8_t index1, index2;
	char *p;
	
	if (_key1 >= '0' && _key1 <= '9')
	{
		index1 = _key1 - '0';
	}
	else if (_key1 == 'A')
	{
		index1 = 10;
	}
	else if (_key1 == 'B')
	{
		index1 = 11;
	}	
	else
	{
		index1 = 0;
	}

	if (_key2 >= '0' && _key2 <= '9')
	{
		index2 = _key2 - '0';
	}
	else if (_key2 == 'A')
	{
		index2 = 10;
	}
	else if (_key2 == 'B')
	{
		index2 = 11;
	}	
	else
	{
		index2 = 0;
	}	
	
	p = (char *)g_key_tab[index1 * 12 + index2];
			
	return p[0];
}

/*
*********************************************************************************************************
*	�� �� ��: DispAllBtn
*	����˵��: ��ʾ���еİ�ť�ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispAllBtn(void)
{
	/* ��ť */
	LCD_DrawButton(&FormPad->Btn0);	
	LCD_DrawButton(&FormPad->Btn1);
	LCD_DrawButton(&FormPad->Btn2);
	LCD_DrawButton(&FormPad->Btn3);
	LCD_DrawButton(&FormPad->Btn4);
	LCD_DrawButton(&FormPad->Btn5);
	LCD_DrawButton(&FormPad->Btn6);
	LCD_DrawButton(&FormPad->Btn5);
	LCD_DrawButton(&FormPad->Btn6);
	LCD_DrawButton(&FormPad->Btn7);
	LCD_DrawButton(&FormPad->Btn8);
	LCD_DrawButton(&FormPad->Btn9);
	LCD_DrawButton(&FormPad->BtnA);
	LCD_DrawButton(&FormPad->BtnB);
	LCD_DrawButton(&FormPad->BtnF1);
	LCD_DrawButton(&FormPad->BtnF2);
	LCD_DrawButton(&FormPad->BtnF3);
	LCD_DrawButton(&FormPad->BtnF4);
}

/*
*********************************************************************************************************
*	�� �� ��: DispFormPad
*	����˵��: ��ʾ���еľ�̬�ؼ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispFormPad(void)
{
	LCD_DrawWin(&FormPad->Win);
	DispAllBtn();
	LCD_DrawEdit(&FormPad->Edit);
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
