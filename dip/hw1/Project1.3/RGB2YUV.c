#include <stdio.h>
#include <windows.h>
#include<string.h>
#include"RGB2YUV.h"
#include"TRANSCONST.h"
void RGB2YUV(unsigned long w, unsigned long h, unsigned char* rgbData, unsigned char* y, unsigned char* u, unsigned char* v, unsigned char* changeData, unsigned char*grayData) {
	initLookupTable();
	unsigned char* ytemp = NULL;
	unsigned char* utemp = NULL;
	unsigned char* vtemp = NULL;
	//ytemp = (unsigned char*)malloc(w*h);
	utemp = (unsigned char*)malloc(w*h);
	vtemp = (unsigned char*)malloc(w*h);

	unsigned long i, nr, ng, nb, nSize;
	//每个像素都需要rgb->yuv
	for (i = 0, nSize = 0; nSize < w*h * 3; nSize += 3) {
		nb = rgbData[nSize];
		ng = rgbData[nSize + 1];
		nr = rgbData[nSize + 2];
		y[i] = (unsigned char)(RGBYUV02990[nr] + RGBYUV05870[ng] + RGBYUV01140[nb]);
		utemp[i] = (unsigned char)(-RGBYUV01684[nr] - RGBYUV03316[ng] + nb / 2 + 128);
		vtemp[i] = (unsigned char)(nr / 2 - RGBYUV04187[ng] - RGBYUV00813[nb] + 128);
		i++;
	}
	//对uv信号采样
	int k = 0;
	for (i = 0; i < h; i += 2) {
		for (unsigned long j = 0; j < w; j += 2) {
			u[k] = (utemp[i*w + j] + utemp[(i + 1)*w + j] + utemp[i*w + j + 1] + utemp[(i + 1)*w + j + 1]) / 4;
			v[k] = (vtemp[i*w + j] + vtemp[(i + 1)*w + j] + vtemp[i*w + j + 1] + vtemp[(i + 1)*w + j + 1]) / 4;
			k++;
		}
	}
	for (i = 0; i < w*h; i++) {
		if (y[i] < 16)y[i] = 16;
		if (y[i] > 235)y[i] = 235;
	}
	for (i = 0; i < w*h / 4; i++) {
		if (u[i] < 16)u[i] = 16;
		if (u[i] > 240)u[i] = 240;
		if (v[i] < 16)v[i] = 16;
		if (v[i] > 240)v[i] = 240;
	}
	for (i = 0, nSize = 0; nSize < w*h * 3; nSize += 3) {
		grayData[nSize] = y[i];
		grayData[nSize + 1] = y[i];
		grayData[nSize + 2] = y[i];
		i++;
	}//gray

	for (i = 0, nSize = 0; nSize < w*h * 3; nSize += 3) {
		//y[i] = y[i] * 1.5;
		if (y[i] < 16)y[i] = 16;
		if (y[i] > 235)y[i] = 235;
		changeData[nSize] = (unsigned char)(y[i] + YUVRGB00114[(unsigned long)(vtemp[i]) - 128]);
		changeData[nSize + 1] = (unsigned char)(y[i] - YUVRGB00039[(unsigned long)(utemp[i]) - 128] - YUVRGB00058[(unsigned long)(vtemp[i])] - 128);
		changeData[nSize + 2] = (unsigned char)(y[i] + YUVRGB00203[(unsigned long)(utemp[i])] - 128);
		i++;
	}//change

	if (utemp)free(utemp);
	if (vtemp)free(vtemp);
}