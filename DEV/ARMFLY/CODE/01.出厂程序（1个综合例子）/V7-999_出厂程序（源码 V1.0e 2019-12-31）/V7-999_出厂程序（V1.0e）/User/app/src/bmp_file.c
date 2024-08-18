/*
*********************************************************************************************************
*
*	ģ������ : BMP�ļ�����ģ��
*	�ļ����� : bmp_file.c
*	��    �� : V1.0
*	˵    �� : ��Ҫʵ�ֽ���Ļ��ͼ����ΪBMP�ļ�
*
*	�޸ļ�¼ :
*		�汾��  ����        ����     ˵��
*		V1.0    2013-03-01 armfly  ��ʽ����
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "ff.h"
#include "bmp_file.h"
#include "ff_gen_drv.h"
#include "sd_diskio_dma.h"

/*
	BMP�ļ�����ɽṹ :

	λͼ�ļ�ͷ��bitmap-file header��
	λͼ��Ϣͷ��bitmap-information header��
	��ɫ��/��ɫ�壨color table��
	λͼ���ݣ�bitmap-data��

	���ͼ���ǵ�ɫ��16ɫ��256ɫ��������ŵ�ɫ�����λͼ���ݣ�λͼ������ָ���ɫ���������š�
	���λͼ��16λ��24λ��32λɫ����ͼ���ļ��в�������ɫ�壬�������ڵ�ɫ�壬ͼ�����ɫֱ����λͼ�����и�����
	16λͼ��ʹ��2�ֽڱ�����ɫֵ�����������ָ�ʽ��
		5λ�� 5λ�� 5λ�� ��555��ʽ��555��ʽֻʹ����15 λ�����һλ��������Ϊ0��
		5λ�� 6λ�� 5λ�� ��565��ʽ��
	24λͼ��ʹ��3�ֽڱ�����ɫֵ��ÿһ���ֽڴ���һ����ɫ�����졢�̡������С�
	32λͼ��ʹ��4�ֽڱ�����ɫֵ��ÿһ���ֽڴ���һ����ɫ������ԭ���ĺ졢�̡���������Alphaͨ������͸��ɫ��

*/

/* BMP�ļ�ͷ�ṹ �������__packed�ؼ��ֱ�ʾ�ṹ���Ա��������� */
__packed typedef struct
{
	/* 14�ֽ� λͼ�ļ�ͷ */
	uint16_t bfType;		/* 2�ֽ� λͼ��� ��Windows�У����ֶε�ֵ��Ϊ'BM'*/
	uint32_t bfSize;		/* bmp�ļ���С 4�ֽ� */
	uint16_t bfReserved1;	/* ������ÿ�ֽ���"00"��д 2�ֽ� */
	uint16_t bfReserved2; 	/* ͬ�� 2�ֽ� */
	uint32_t bfOffBits;		/* ��¼ͼ������������ʼλ��(ͼ������������ļ�ͷ�ֽڵ�ƫ����)�� 4�ֽ� */

	/* 40�ֽ� λͼ��Ϣͷ */
	uint32_t biSize;		/* 4�ֽ� ���ṹ�Ĵ�С����Windows�У���Ϊ28h��40�ֽ� */
	int32_t  biWidth;		/* 4�ֽ� BMPͼ��Ŀ�ȣ���λ���� */
	int32_t  biHeight;		/* 4�ֽ� BMPͼ��ĸ߶ȣ���λ���� */
	uint16_t biPlanes;		/* 2�ֽ� Ŀ���豸�ļ���(ɫ��ƽ����)���̶�Ϊ1 */
	uint16_t biBitCount;	/* 2�ֽ� BMPͼ���ɫ���һ�������ö���λ��ʾ���������� 1 4 8 16 24 32 */
	uint32_t biCompression;	/* 4�ֽ� ѹ�����ͣ�0(��ѹ��), 1(BI_RLE8), 2(BI_RLE4) */
	uint32_t biSizeImage;	/* 4�ֽ� ��ʾλͼ��������Ĵ�С���ֽ�Ϊ��λ */
	int32_t  biXPelsPerMeter;	/* 4�ֽ� ������/�ױ�ʾ��ˮƽ�ֱ��� */
	int32_t  biYPelsPerMeter;	/* 4�ֽ� ������/�ױ�ʾ�Ĵ�ֱ�ֱ��� */
	uint32_t biClrUsed;			/* 4�ֽ� ʵ��ʹ��ɫ����Ŀ������λ���� */
	uint32_t biClrImportant;	/* 4�ֽ� ͼ������Ҫ��ɫ����Ŀ��0��ʾ��ɫ�������е���ɫ������Ҫ��  */
}BMP_T;

/*
*********************************************************************************************************
*	�� �� ��: MakeBmpHeader
*	����˵��: ����BMP�ļ�ͷ�ṹ�� �̶���24λɫ�� RGB = 888 �ṹ���洢���ֽ���ǰ��Bǰ��G�У�R��
*	��    ��: _pBmp : BMP�ļ�ͷ�ṹ��ָ��
*			 _Height : ͼ��߶�
*			 _Width :  ͼ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void MakeBmpHeader(BMP_T *_pBmp, uint16_t _Width, uint16_t _Height)
{
	/* 14�ֽ� λͼ�ļ�ͷ */
	_pBmp->bfType = 0x4D42;	/* 2�ֽ� λͼ��� ��Windows�У����ֶε�ֵ��Ϊ'BM'*/
	_pBmp->bfSize = (_Height * _Width * 3) + 54;	/* bmp�ļ���С 4�ֽ� */
	_pBmp->bfReserved1 = 0;	/* ������ÿ�ֽ���"00"��д 2�ֽ� */
	_pBmp->bfReserved2 = 0; /* ͬ�� 2�ֽ� */
	_pBmp->bfOffBits = 54;	/* ��¼ͼ������������ʼλ��(ͼ������������ļ�ͷ�ֽڵ�ƫ����)�� 4�ֽ� */

	/* 40�ֽ� λͼ��Ϣͷ */
	_pBmp->biSize = 40;		/* 4�ֽ� ���ṹ�Ĵ�С����Windows�У���Ϊ28h��40�ֽ� */
	_pBmp->biWidth = _Width;/* 4�ֽ� BMPͼ��Ŀ�ȣ���λ���� */
	_pBmp->biHeight = _Height;		/* 4�ֽ� BMPͼ��ĸ߶ȣ���λ���� */
	_pBmp->biPlanes = 1;			/* 2�ֽ� Ŀ���豸�ļ���(ɫ��ƽ����)���̶�Ϊ1 */
	_pBmp->biBitCount = 24;			/* 2�ֽ� BMPͼ���ɫ���һ�������ö���λ��ʾ���������� 1 4 8 16 24 32 */
	_pBmp->biCompression = 0;		/* 4�ֽ� ѹ�����ͣ�0(��ѹ��), 1(BI_RLE8), 2(BI_RLE4) */
	_pBmp->biSizeImage = (_Height * _Width * 3);	/* 4�ֽ� ��ʾλͼ��������Ĵ�С���ֽ�Ϊ��λ */
	_pBmp->biXPelsPerMeter = 0;	/* 4�ֽ� ������/�ױ�ʾ��ˮƽ�ֱ��� */
	_pBmp->biYPelsPerMeter = 0;	/* 4�ֽ� ������/�ױ�ʾ�Ĵ�ֱ�ֱ��� */
	_pBmp->biClrUsed = 0;		/* 4�ֽ� ʵ��ʹ��ɫ����Ŀ��0����λ���� */
	_pBmp->biClrImportant = 0;	/* 4�ֽ� ͼ������Ҫ��ɫ����Ŀ��0��ʾ��ɫ�������е���ɫ������Ҫ��  */
}

/*
*********************************************************************************************************
*	�� �� ��: SaveScreenToBmp
*	����˵��: ����ǰ��Ļ����ΪBMP�ļ�ͷ�����FatFS�ļ�ϵͳʹ�á�
*	��    ��: _index : �ļ����
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void SaveScreenToBmp(uint16_t _index)
{
	__packed BMP_T tBmpHead;
	uint8_t LineData[800 * 3]; 	/* ����� 800���ؿ�ȷ��仺���� */
	uint16_t i,j;
	uint16_t pixel;

	/* ������ʹ�õľֲ�����ռ�ý϶࣬���޸������ļ�����֤��ջ�ռ乻�� */
	FRESULT result;
	FATFS fs;
	FIL file;
	uint32_t bw;
	char path[32];
	
	char DiskPath[4]; /* SD card logical drive path */

	FATFS_LinkDriver(&SD_Driver, DiskPath);
	
	bsp_LedOn(1);	/* ����LED1, ��ʾ��ʼ���� */

	MakeBmpHeader(&tBmpHead, LCD_GetWidth(), LCD_GetHeight());	/* ����BMP�ļ�ͷ */

	/* �ļ�ϵͳ���� */
	{
		/* �����ļ�ϵͳ */
		result = f_mount(&fs, DiskPath, 0);			/* Mount a logical drive�� 1��ʾ����ִ�� */
		if (result != FR_OK)
		{
			goto quit1;
		}

		/* ���ļ� */
		sprintf(path, "%s/%dx%d_%02d.bmp", DiskPath, tBmpHead.biWidth, tBmpHead.biHeight, _index);
		//result = f_opendir(&DirInf, path);
		result = f_open(&file, path, FA_CREATE_ALWAYS | FA_WRITE);
		if (result != FR_OK)
		{
			printf("����SD���ļ�%sʧ�� (%d)\r\n", path, result);
			goto quit1;
		}
	}

	/* дbmp�ļ�ͷ */
	result = f_write(&file, (char *)&tBmpHead, 54, &bw);
	if (result != FR_OK)
	{
		printf("%s �ļ�д��ʧ��\r\n", path);
		goto quit1;
	}

	/* ��ʼ��ȡ��Ļ���ݣ���д���ļ� */
	for (i = 0; i <  tBmpHead.biHeight; i++)
	{
		/* ��ȡ1�����ݵ������� (ע�⣺BMP�ļ�ɨ����򣬴����ң����µ��ϡ���LCD�߼������Ǵ�ֱ��ת��) */
		for (j = 0; j < tBmpHead.biWidth; j++)
		{
			pixel = LCD_GetPixel(j, tBmpHead.biHeight - i - 1);		/* ��ȡ��Ļ1������ ��RGB = 565�ṹ��*/
			
			#if 1
			LineData[3 * j + 2] = ((pixel & 0xF800) >> 8);		/* R */
			LineData[3 * j + 1] = ((pixel & 0x07E0) >> 3);		/* G */
			LineData[3 * j + 0] = ((pixel & 0x001F) << 3);		/* B */
			#else
			if (_index == 0)
			{
				LineData[3 * j + 2] = 255;
				LineData[3 * j + 1] = 0;
				LineData[3 * j + 0] = 0;
			}
			else if (_index == 1)
			{
				LineData[3 * j + 2] = 0;
				LineData[3 * j + 1] = 255;
				LineData[3 * j + 0] = 0;
			}
			else
			{
				LineData[3 * j + 2] = 0;
				LineData[3 * j + 1] = 0;
				LineData[3 * j + 0] = 255;
			}			
			#endif
		}

		#if 0
		/* д1������ */
		result = f_write(&file, LineData, tBmpHead.biWidth * 3, &bw);
		if (result != FR_OK)
		{
			printf("%s �ļ�д��ʧ��\r\n", path);
			goto quit1;
		}
		#else
		
		for (j = 0 ;j < tBmpHead.biWidth * 3 / 480; j++)
		{
			/* д1������ */
			result = f_write(&file, &LineData[480 * j], 480, &bw);
			if (result != FR_OK)
			{
				printf("%s �ļ�д��ʧ��\r\n", path);
				goto quit1;
			}
		}		
		#endif
	}

quit1:
	/* �ر��ļ�*/
	f_close(&file);

	/* ж���ļ�ϵͳ */
	//f_mount(ucVolume, NULL);
	f_mount(NULL, DiskPath, 0);

	FATFS_UnLinkDriver(DiskPath);	/* ж������ */
	
	bsp_LedOff(1);	/* �ر�LED1, ��ʾ��ʼ�������� */
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
