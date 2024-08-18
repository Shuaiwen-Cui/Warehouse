/*
*********************************************************************************************************
*
*	ģ������ : Ӳ�����Խ��档
*	�ļ����� : hard_test.c
*	��    �� : V1.0
*	˵    �� : ��ʾӲ����Ϣ���Զ�����һЩоƬ��
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-01-01 armfly  ��ʽ����
*
*	Copyright (C), 2012-2013, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"			/* printf����������������ڣ����Ա����������ļ� */
#include "form_hard_test.h"

/* ���ذ�ť������(��Ļ���½�) */
#define BUTTON_RET_H	32
#define BUTTON_RET_W	60
#define	BUTTON_RET_X	(g_LcdWidth - BUTTON_RET_W - 4)
#define	BUTTON_RET_Y	(g_LcdHeight - BUTTON_RET_H - 4)

/*
*********************************************************************************************************
*	�� �� ��: HardInfo
*	����˵��: �Զ����Ӳ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void HardInfo(void)
{
	uint8_t ucKeyCode;		/* �������� */
	uint8_t ucTouch;		/* �����¼� */
	uint8_t fRefresh;		/* ˢ�������־,1��ʾ��Ҫˢ�� */
	FONT_T tFont, tFontBtn;	/* ����һ������ṹ���������������������� */
	char buf[128];
	uint16_t x, y;
	uint16_t usLineCap = 18;

	int16_t tpX, tpY;
	BUTTON_T tBtn;

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
	}
	
	x = 5;
	y = 3;
	LCD_DispStr(x, y, "������STM32-V7������Ӳ����Ϣ", &tFont);			/* ��(8,3)���괦��ʾһ������ */
	y += usLineCap;

	/* ���CPU ID */
	{
		uint32_t id[3];

		bsp_GetCpuID(id);

		printf("\r\nCPU : STM32H743, ��Ƶ: %dMHz\r\n", SystemCoreClock / 1000000);
		
		LCD_DispStr(x, y, "CPU : STM32H743", &tFont);			
		y += usLineCap;				
		
		sprintf(buf, " UID = %08X %08X %08X"
			, id[0], id[1], id[2]);
		printf("%s\r\n", buf);
		LCD_DispStr(x, y, buf, &tFont);
		y += usLineCap;		
	}
	
	/* ��ʾTFT�������ͺź���Ļ�ֱ��� */
	{
		strcpy(buf, "TFT Driver : ");
		LCD_GetChipDescribe(&buf[strlen(buf)]);	/* ��ȡTFT����оƬ�ͺ� */
		sprintf(&buf[strlen(buf)], "   %d x %d", LCD_GetWidth(), LCD_GetHeight());
		LCD_DispStr(x, y, buf, &tFont);
		y += usLineCap;
	}

	/* ����SRAM */
	{
		sprintf(buf, "SDRAM Model ... ");

		LCD_DispStr(x, y, buf, &tFont);		
		if (bsp_TestExtSDRAM2() == 0)
		{
			sprintf(buf, "SDRAM, 32Bit, Test OK");
			printf("%s\r\n", buf);

			LCD_DispStr(x, y, buf, &tFont);
		}
		else
		{
			sprintf(buf, " SDRAM, Test Err");
			printf("%s\r\n", buf);

			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;
	}

	/* ����NADN Flash */
	#if 1
	{
		uint32_t id;

		NAND_Init();	/* ��ʼ��NAND */
		
		id = NAND_ReadID();

		sprintf(buf, "NAND Flash ID = 0x%04X, Type = ", id);
		if (id == HY27UF081G2A)
		{
			sprintf(buf, "NAND Flash ID = 0x%04X, Model = HY27UF081G2A, OK", id);
			printf("%s\r\n", buf);
			LCD_DispStr(x, y, buf, &tFont);
		}
		else if (id == H27U1G8F2BTR)
		{
			sprintf(buf, "NAND Flash ID = 0x%04X, Model = H27U1G8F2BTR, OK", id);
			printf("%s\r\n", buf);
			LCD_DispStr(x, y, buf, &tFont);
		}		
		else if (id == H27U4G8F2DTR)
		{
			sprintf(buf, "NAND Flash ID = 0x%04X, Model = H27U4G8F2DTR, OK", id);
			printf("%s\r\n", buf);
			LCD_DispStr(x, y, buf, &tFont);
		}		
		else
		{
			sprintf(buf, "NAND Flash ID = 0x%04X, Model = Unknow, Err", id);
			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;
		//NAND_DispBadBlockInfo();	/* �򴮿�1��ӡNAND Flash������Ϣ (�˺�����ͷ��ʼFSMC) */
	}
	#endif
	/* ����I2C�豸 */
	{
		if (i2c_CheckDevice(EE_DEV_ADDR) == 0)
		{
			sprintf(buf, "AT24C128 Ok (0x%02X)", EE_DEV_ADDR);
			printf("%s\r\n", buf);
			LCD_DispStr(x, y, buf, &tFont);
		}
		else
		{
			sprintf(buf, "AT24C128 Err (0x%02X)", EE_DEV_ADDR);
			printf("%s\r\n", buf);

			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;

		if (i2c_CheckDevice(I2C_ADDR_SI4730_W) == 0)
		{
			sprintf(buf, "Si4730/4704 Ok (0x%02X)", I2C_ADDR_SI4730_W);
			printf("%s\r\n", buf);
			LCD_DispStr(x, y, buf, &tFont);
		}
		else
		{
			sprintf(buf, "Si4730/4704 Err (0x%02X)", I2C_ADDR_SI4730_W);
			printf("%s\r\n", buf);

			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;
#if 0
		if (i2c_CheckDevice(HMC5883L_SLAVE_ADDRESS) == 0)
		{
			sprintf(buf, "HMC5883L Ok (0x%02X)", HMC5883L_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
			LCD_DispStr(x, y, buf, &tFont);
		}
		else
		{
			sprintf(buf, "HMC5883L / MAG3110 Err");
			printf("%s\r\n", buf);

			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;
#endif		

		if (i2c_CheckDevice(MPU6050_SLAVE_ADDRESS) == 0)
		{
			sprintf(buf, "MPU6050 Ok (0x%02X)", MPU6050_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
			LCD_DispStr(x, y, buf, &tFont);
		}
		else
		{
			sprintf(buf, "MPU6050 Err (0x%02X)", MPU6050_SLAVE_ADDRESS);
			printf("%s\r\n", buf);

			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;

		if (i2c_CheckDevice(BMP085_SLAVE_ADDRESS) == 0)
		{
			sprintf(buf, "BMP180 Ok (0x%02X)", BMP085_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
			LCD_DispStr(x, y, buf, &tFont);
		}
		else
		{
			sprintf(buf, "BMP180 Err (0x%02X)", BMP085_SLAVE_ADDRESS);
			printf("%s\r\n", buf);

			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;

		if (i2c_CheckDevice(WM8978_SLAVE_ADDRESS) == 0)
		{
			sprintf(buf, "WM8978 Ok (0x%02X)", WM8978_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
			LCD_DispStr(x, y, buf, &tFont);
		}
		else
		{
			sprintf(buf, "WM8978 Err (0x%02X)", WM8978_SLAVE_ADDRESS);
			printf("%s\r\n", buf);

			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;

		if (i2c_CheckDevice(BH1750_SLAVE_ADDRESS) == 0)
		{
			sprintf(buf, "BH1750 Ok (0x%02X)", BH1750_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
			LCD_DispStr(x, y, buf, &tFont);
		}
		else
		{
			sprintf(buf, "BH1750 Err (0x%02X)", BH1750_SLAVE_ADDRESS);
			printf("%s\r\n", buf);

			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;
	}

	/* ����SPI Flash */
	{
		bsp_InitSFlash();	/* ��ʼ��SPIӲ�� */

		sf_ReadInfo();
		
		/* ��⴮��Flash OK */
		sprintf(buf, "SPI Flash ID = %08X, Model = %s",g_tSF.ChipID , g_tSF.ChipName);
		printf("%s\r\n", buf);

		if (g_tSF.ChipID == W25Q64_ID)
		{
			LCD_DispStr(x, y, buf, &tFont);
		}
		else
		{
			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}
		y += usLineCap;
	}
	
	/* QSPI��� */
	{
		uint32_t id;
		char name[32];
		
		bsp_InitQSPI_W25Q256();
		
		id = QSPI_ReadID();
		if (id == W25Q64_ID)
		{
			strcpy(name, "W25Q64");
		}
		else if (id == W25Q128_ID)
		{
			strcpy(name, "W25Q128");
		}
		else if (id == W25Q256_ID)
		{
			strcpy(name, "W25Q256");
		}		
		else
		{
			strcpy(name, "UNKNOW");
		}
			
		/* ��⴮��Flash OK */
		sprintf(buf, "QSPI Flash ID = %08X, Model = %s", id, name);
		printf("%s\r\n", buf);

		if (id == W25Q256_ID)
		{
			LCD_DispStr(x, y, buf, &tFont);
		}
		else
		{
			tFont.FrontColor = CL_RED;
			LCD_DispStr(x, y, buf, &tFont);
			tFont.FrontColor = CL_WHITE;
		}		
	}

	fRefresh = 1;	/* 1��ʾ��Ҫˢ��LCD */
	bsp_StartAutoTimer(0, 1000);
	while (1)
	{
		bsp_Idle();

		/* ��ʾʱ�� */
		if (bsp_CheckTimer(0))
		{
			uint16_t x, y;
			
			tFont.FontCode = FC_ST_16;	/* ������� 16���� */
			tFont.FrontColor = CL_WHITE;	/* ������ɫ */
			tFont.BackColor = CL_BLUE;	/* ���ֱ�����ɫ */
			tFont.Space = 0;				/* ���ּ�࣬��λ = ���� */
			
			RTC_ReadClock();	/* ��ʱ�ӣ������ g_tRTC */
			
			x = 5;
			y = LCD_GetHeight() - 20;
			
			sprintf(buf, "%4d-%02d-%02d %02d:%02d:%02d",
				g_tRTC.Year, g_tRTC.Mon, g_tRTC.Day, g_tRTC.Hour, g_tRTC.Min, g_tRTC.Sec);
			LCD_DispStr(x, y, buf, &tFont);
		}		
		
		if (fRefresh)
		{
			fRefresh = 0;

			/* ��ʾ��ť */
			{
				tBtn.Font = &tFontBtn;

				tBtn.Left = BUTTON_RET_X;
				tBtn.Top = BUTTON_RET_Y;
				tBtn.Height = BUTTON_RET_H;
				tBtn.Width = BUTTON_RET_W;
				tBtn.Focus = 0;	/* ʧȥ���� */
				tBtn.pCaption = "����";
				LCD_DrawButton(&tBtn);
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
						tBtn.Font = &tFontBtn;

						tBtn.Left = BUTTON_RET_X;
						tBtn.Top = BUTTON_RET_Y;
						tBtn.Height = BUTTON_RET_H;
						tBtn.Width = BUTTON_RET_W;
						tBtn.Focus = 1;	/* ���� */
						tBtn.pCaption = "����";
						LCD_DrawButton(&tBtn);
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
						tBtn.pCaption = "����";
						LCD_DrawButton(&tBtn);

						return;		/* ���� */
					}
					else	/* ��ťʧȥ���� */
					{
						tBtn.Font = &tFontBtn;

						tBtn.Left = BUTTON_RET_X;
						tBtn.Top = BUTTON_RET_Y;
						tBtn.Height = BUTTON_RET_H;
						tBtn.Width = BUTTON_RET_W;
						tBtn.Focus = 0;	/* ���� */
						tBtn.pCaption = "����";
						LCD_DrawButton(&tBtn);

					}
			}
		}

		ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			/* �м����� */
			switch (ucKeyCode)
			{
				case JOY_DOWN_OK:		/* ҡ��OK�� */
					break;

				default:
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: PrintfHardInfo
*	����˵��: �Զ����Ӳ��. printf�����ڡ�����ʾ��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
extern uint32_t dm9k_ReadID(void);
void PrintfHardInfo(void)
{
	char buf[128];

	/* ���CPU ID */
	{
		uint32_t id[3];

		bsp_GetCpuID(id);

		printf("\r\nCPU : STM32H743, ��Ƶ: %dMHz\r\n", SystemCoreClock / 1000000);
		sprintf(buf, " UID = %08X %08X %08X"
			, id[0], id[1], id[2]);
		printf("%s\r\n", buf);
	}
	
	/* ��ʾTFT�������ͺź���Ļ�ֱ��� */
	{
		strcpy(buf, "TFT Driver : ");
		LCD_GetChipDescribe(&buf[strlen(buf)]);	/* ��ȡTFT����оƬ�ͺ� */
		sprintf(&buf[strlen(buf)], "   %d x %d", LCD_GetWidth(), LCD_GetHeight());
		printf("%s\r\n", buf);
	}

	/* ����SRAM */
	#if 0
	{
		if (bsp_TestExtSDRAM2() == 0)
		{
			sprintf(buf, "SDRAM Test OK");
			printf("%s\r\n", buf);
		}
		else
		{
			sprintf(buf, "SDRAM Test Err");
			printf("%s\r\n", buf);
		}
	}
	#endif

	/* ����NADN Flash */
	#if 1
	{
		uint32_t id;

		NAND_Init();	/* ��ʼ��NAND */
		
		id = NAND_ReadID();

		sprintf(buf, "NAND Flash ID = 0x%04X, Type = ", id);
		if (id == HY27UF081G2A)
		{
			sprintf(buf, "NAND Flash ID = 0x%04X, Model = HY27UF081G2A, OK", id);
			printf("%s\r\n", buf);
		}
		else if (id == H27U1G8F2BTR)
		{
			sprintf(buf, "NAND Flash ID = 0x%04X, Model = H27U1G8F2BTR, OK", id);
			printf("%s\r\n", buf);
		}			
		else if (id == H27U4G8F2DTR)
		{
			sprintf(buf, "NAND Flash ID = 0x%04X, Model = H27U4G8F2DTR, OK", id);
			printf("%s\r\n", buf);
		}
		else
		{
			sprintf(buf, "NAND Flash ID = 0x%04X, Model = Unknow, Err", id);
			printf("%s\r\n", buf);
		}
	}
	#endif
	/* ����I2C�豸 */
	{
		if (i2c_CheckDevice(EE_DEV_ADDR) == 0)
		{
			sprintf(buf, "AT24C128 Ok (0x%02X)", EE_DEV_ADDR);
			printf("%s\r\n", buf);
		}
		else
		{
			sprintf(buf, "AT24C128 Err (0x%02X)", EE_DEV_ADDR);
			printf("%s\r\n", buf);
		}

		if (i2c_CheckDevice(I2C_ADDR_SI4730_W) == 0)
		{
			sprintf(buf, "Si4730 Ok (0x%02X)", I2C_ADDR_SI4730_W);
			printf("%s\r\n", buf);
		}
		else
		{
			sprintf(buf, "Si4730 Err (0x%02X)", I2C_ADDR_SI4730_W);
			printf("%s\r\n", buf);
		}
#if 0
		if (i2c_CheckDevice(HMC5883L_SLAVE_ADDRESS) == 0)
		{
			sprintf(buf, "HMC5883L Ok (0x%02X)", HMC5883L_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
		}
		else
		{
			sprintf(buf, "HMC5883L Err (0x%02X)", HMC5883L_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
		}
#endif

		if (i2c_CheckDevice(MPU6050_SLAVE_ADDRESS) == 0)
		{
			sprintf(buf, "MPU6050 Ok (0x%02X)", MPU6050_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
		}
		else
		{
			sprintf(buf, "MPU6050 Err (0x%02X)", MPU6050_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
		}

		if (i2c_CheckDevice(BMP085_SLAVE_ADDRESS) == 0)
		{
			sprintf(buf, "BMP180 Ok (0x%02X)", BMP085_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
		}
		else
		{
			sprintf(buf, "BMP180 Err (0x%02X)", BMP085_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
		}

		if (i2c_CheckDevice(WM8978_SLAVE_ADDRESS) == 0)
		{
			sprintf(buf, "WM8978 Ok (0x%02X)", WM8978_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
		}
		else
		{
			sprintf(buf, "WM8978 Err (0x%02X)", WM8978_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
		}

		if (i2c_CheckDevice(BH1750_SLAVE_ADDRESS) == 0)
		{
			sprintf(buf, "BH1750 Ok (0x%02X)", BH1750_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
		}
		else
		{
			sprintf(buf, "BH1750 Err (0x%02X)", BH1750_SLAVE_ADDRESS);
			printf("%s\r\n", buf);
		}
	}

	/* ����SPI Flash */
	{
		//bsp_InitSFlash();	/* ��ʼ��SPIӲ��, bsp.c ���Ѿ���ʼ�� */

		/* ��⴮��Flash OK */
		sprintf(buf, "SPI Flash ID= = %08X, Model = %s",g_tSF.ChipID , g_tSF.ChipName);
		printf("%s\r\n", buf);
	}
	
	/* ���DM9000 */
	#if 0
	{
		uint32_t id;
		
		id = dm9k_ReadID();
		
		if (id == 0x0A469000)
		{
			printf("DM9000AEP OK,  ID = %8X\r\n", id);
		}
		else
		{
			printf("DM9000AEP Error, ID = %8X\r\n", id);
		}
	}
	#endif
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
