#include <stdio.h>
#include <windows.h>
#include<string.h>
#include"RGB2YUV.h"
#include"TRANSCONST.h"
#include"READBMP.h"
#include"WRITE.h"



int main(int argc, char**argv){
	
	BITMAPFILEHEADER File_header;
	BITMAPINFOHEADER Info_header;
	FILE*bmpFile=NULL;		//读入的bmp图对应文件指针
	FILE*yuvFile=NULL;		//输出的yuv文件对应的指针
	FILE*grayFile = NULL;	//输出的灰度图对应的文件指针
	FILE*changeFile = NULL;	//输出的更改y值后的bmp图对应的文件指针

	unsigned char*rgbBuffer, *yBuffer, *uBuffer, *vBuffer,*changeBuffer,*grayBuffer;
	//原图rgb值的地址，原图yuv值的地址，更改y值后rgb值的地址，灰度图地址
	


	//新建一个yuv文件
	char name__[20];
	strcpy(name__, "output.yuv");
//	strcat(name__,  ".yuv");
	yuvFile=fopen(name__, "wb");

	if (!yuvFile){
		printf("yuvfile open error!\n");
		exit(0);
	}


	int i = 1;
		char str[20];
		char name_[20];

		strcpy(name_, "input.bmp");
		bmpFile=fopen(name_, "rb");
		if (!bmpFile){
			printf("bmpfile open error!\n");
			exit(0);
		}

		//读取位图文件头
		if (fread(&File_header, sizeof(BITMAPFILEHEADER), 1, bmpFile) != 1){
			printf("readfile header error!\n");
			exit(0);
		}

		//判断文件类型
		if (File_header.bfType != 0x4D42){
			printf("Not bmp file!\n");
			exit(0);
		}
		else{
			printf("this is a bmp file\n");
		}

		//读取位图信息头
		if (fread(&Info_header, sizeof(BITMAPINFOHEADER), 1, bmpFile) != 1){
			printf("read info header error!\n");
			exit(0);
		}
		
		//为了保证图像大小是4的倍数，每一行的像素数*每像素的位数=一行总位数
		//一行总位数必须是32位的整数倍。
		unsigned long width, height;
		if (Info_header.biWidth % 4 == 0)width = Info_header.biWidth;
		else width = (Info_header.biWidth*Info_header.biBitCount + 31) / 32 * (32 / 8);
		
		//保证列数是偶数
		if ((Info_header.biHeight % 2) == 0)height = Info_header.biHeight;
		else height = Info_header.biHeight + 1;


		//开辟缓冲区
		rgbBuffer = (unsigned char*)malloc(height*width * 3);
		changeBuffer = (unsigned char*)malloc(height*width * 3);
		grayBuffer = (unsigned char*)malloc(height*width * 3);
		yBuffer = (unsigned char*)malloc(height*width);
		uBuffer = (unsigned char*)malloc(height*width / 4);
		vBuffer = (unsigned char*)malloc(height*width / 4);


		//从bmp文件中读取rgb信息
		ReadRGB(bmpFile, File_header, Info_header, rgbBuffer);


		//核心步骤，将rgb数据转为yuv
		RGB2YUV(width, height, rgbBuffer, yBuffer, uBuffer, vBuffer, changeBuffer, grayBuffer);


		//将转好的yuv写到文件中
		if (WriteYUV(yBuffer, uBuffer, vBuffer, width*height, yuvFile))
			printf("writeYUV file successful!\n");
		else
			printf("writeYUV file failed!\n");
		
		


	//灰度图
	char name_gray[20];
	strcpy(name_gray, "gray.bmp");
	grayFile = fopen(name_gray, "wb");
	if (!grayFile) {
		printf("grayfile open error!\n");
		exit(0);
	}
	
	fwrite(&File_header, 1, sizeof(BITMAPFILEHEADER), grayFile);
	fwrite(&Info_header, 1, sizeof(BITMAPINFOHEADER), grayFile);
	
	if (WriteRGB(grayBuffer, uBuffer, vBuffer, width*height, grayFile))
		printf("writeGRAY file successful!\n");
	else
		printf("writeGRAY file failed!\n");
	

	//change
	char name_change[20];
	strcpy(name_change, "change.bmp");
	changeFile = fopen(name_change, "wb");
	if (!changeFile) {
		printf("changefile open error!\n");
		exit(0);
	}

	fwrite(&File_header, 1, sizeof(BITMAPFILEHEADER), changeFile);
	fwrite(&Info_header, 1, sizeof(BITMAPINFOHEADER), changeFile);

	if (WriteRGB(changeBuffer, uBuffer, vBuffer, width*height, changeFile))
		printf("writechange file successful!\n");
	else
		printf("writechange file failed!\n");

	//free时一定要小心，指针不要指向不是堆的区域
	if (rgbBuffer)
		free(rgbBuffer);
	if (yBuffer)
		free(yBuffer);
	if (uBuffer)
		free(uBuffer);
	if (vBuffer)
		free(vBuffer);
	if (grayBuffer)
		free(grayBuffer);
	if (changeBuffer)
		free(changeBuffer);


	fclose(bmpFile);
	fclose(yuvFile);
	fclose(changeFile);
	fclose(grayFile);
	return 0;

}

