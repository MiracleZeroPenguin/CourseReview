#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include"RGB2YUV.h"
#include"TRANSCONST.h"
#include"READBMP.h"
void ReadRGB(FILE *pFile, const BITMAPFILEHEADER file_h, const BITMAPINFOHEADER info_h, char* rgbDataOut) {
	unsigned long Loop, iLoop, jLoop, width, height, w, h;
	unsigned char mask, *Index_Data, *Data;

	//保证图像大小是4字节整数倍
	if ((info_h.biWidth % 4) == 0)w = info_h.biWidth;
	else w = (info_h.biWidth*info_h.biBitCount + 31) / 32 * 4;

	if ((info_h.biHeight % 2) == 0)h = info_h.biHeight;
	else h = info_h.biHeight + 1;

	width = w / 8 * info_h.biBitCount;
	height = h;

	//倒序前数据缓存区
	Index_Data = (unsigned char*)malloc(height*width);//buffer大小与bmp中有效数据大小相同
	//倒序后数据缓存区
	Data = (unsigned char*)malloc(height*width);

	fseek(pFile, file_h.bfOffBits, 0);

	if (fread(Index_Data, 1, height*width, pFile) == 0) {
		printf("readfile error!");
		exit(0);
	}

	//存放倒序，最后输出图像上下颠倒，不知和此处是否有关
	for (iLoop = 0; iLoop < height; iLoop++) {
		for (jLoop = 0; jLoop < width; jLoop++) {
			Data[iLoop*width + jLoop] = Index_Data[(height - iLoop - 1)*width + jLoop];
		}
	}

	if (info_h.biBitCount == 24) {
		memcpy(rgbDataOut, Data, height*width);
		free(Index_Data);
		free(Data);
		return;
	}

	//若非24位，需对其进行解析

}