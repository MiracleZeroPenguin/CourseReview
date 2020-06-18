#include<stdio.h>
#include<Windows.h>
#include<stdlib.h>
#include"RGB2YUV.h"
#include"TRANSCONST.h"
void initLookupTable()
{
	for (int i = 0; i < 256; i++)
	{
		RGBYUV02990[i] = (float)0.2990 * i;
		RGBYUV05870[i] = (float)0.5870 * i;
		RGBYUV01140[i] = (float)0.1140 * i;
		RGBYUV01684[i] = (float)0.1684 * i;
		RGBYUV03316[i] = (float)0.3316 * i;
		RGBYUV04187[i] = (float)0.4187 * i;
		RGBYUV00813[i] = (float)0.0813 * i;
		YUVRGB00039[i] = (float)0.39*i;
		YUVRGB00058[i] = (float)0.58*i;
		YUVRGB00114[i] = (float)1.14*i;
		YUVRGB00203[i] = (float)2.03*i;
	}
}