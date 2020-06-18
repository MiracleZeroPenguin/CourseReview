#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include"definition.h"
#include"fundation.h"
#include"operations.h"

unsigned int OffSet = 0;    // OffSet from Header part to Data Part

void addHeadertofile(FILE *input, FILE *output)
{
	unsigned char *fp_temp;

	fseek(input, 0L, SEEK_SET);
	fseek(output, 0L, SEEK_SET);

	fp_temp = malloc(OffSet);
	fread(fp_temp, 1, OffSet, input);//读取头文件
	fp_temp[18] = (int)width;	//write the width
	fp_temp[22] = (int)height;	//write the height
	fp_temp[2] = (int)(OffSet + height * width * 3);
	fp_temp[34] = (int)(height*((24 * width / 8 + 3) / 4 * 4));
	fwrite(fp_temp, 1, OffSet, output);//输出头文件
}


void bmpFileTest(FILE* fpbmp)
{
	unsigned short bfType = 0;

	fseek(fpbmp, 0L, SEEK_SET);//seek_set 起始位置
	fread(&bfType, sizeof(char), 2, fpbmp);
	if (BM != bfType)
	{
		printf("This file is not bmp file.!!!\n");
		exit(1);
	}
}

void bmpoutput(FILE* fpout)
{
	long i, j = 0;
	long stride;
	unsigned char* pixout = NULL;

	stride = (24 * width + 31) / 8;
	stride = stride / 4 * 4;
	pixout = malloc(stride);

	fseek(fpout, OffSet, SEEK_SET);

	for (j = 0; j < height; j++)
	{
		for (i = 0; i < width; i++)
		{
			pixout[i * 3 + 2] = output_r[height - 1 - j][i];
			pixout[i * 3 + 1] = output_g[height - 1 - j][i];
			pixout[i * 3] = output_b[height - 1 - j][i];
		}
		fwrite(pixout, 1, stride, fpout);
	}
}

FILE *openfile(char filename[])
{
	FILE *fpbmp;
	fpbmp = fopen(filename, "rb");
	if (fpbmp == NULL)
	{
		printf("Open bmp failed!!!\n");
		exit(1);
	}

	bmpFileTest(fpbmp);
	bmpHeaderPartLength(fpbmp);
	BmpWidthHeight(fpbmp);

	fseek(fpbmp, 0L, SEEK_SET);
	return fpbmp;
}

FILE *writefile(char filename[])
{
	FILE *fpout;
	fpout = fopen(filename, "wb+");
	if (fpout == NULL)
	{
		printf("Open out.bmp failed!!!\n");
		exit(1);
	}
	fseek(fpout, 0L, SEEK_SET);
	return fpout;

}


void bmpDataPart(FILE* fpbmp)
{	//clarify the data in the bmp
	int i, j = 0;
	int stride;
	unsigned char* pix = NULL;

	fseek(fpbmp, OffSet, SEEK_SET);		//get the data
	stride = (24 * width + 31) / 8;
	stride = stride / 4 * 4;
	pix = malloc(stride);

	for (j = 0; j < height; j++)
	{
		fread(pix, 1, stride, fpbmp);

		for (i = 0; i < width; i++)
		{
			r[height - 1 - j][i] = pix[i * 3 + 2];
			g[height - 1 - j][i] = pix[i * 3 + 1];
			b[height - 1 - j][i] = pix[i * 3];
			//init the output_info
			output_r[height - 1 - j][i] = 255;
			output_g[height - 1 - j][i] = 255;
			output_b[height - 1 - j][i] = 255;
		}
	}
}



/* To get the OffSet of header to data part */
void bmpHeaderPartLength(FILE* fpbmp)
{
	fseek(fpbmp, 10L, SEEK_SET);
	fread(&OffSet, sizeof(char), 4, fpbmp);
	printf("The Header Part is of length %d.\n", OffSet);
}

/* To get the width and height of the bmp file */
void BmpWidthHeight(FILE* fpbmp)
{
	int size;
	fseek(fpbmp, 18L, SEEK_SET);
	fread(&width, sizeof(char), 4, fpbmp);
	fseek(fpbmp, 2L, SEEK_SET);
	fread(&size, sizeof(char), 4, fpbmp);
	printf("The Size of the bmp file is %ld.\n", size);
	fseek(fpbmp, 22L, SEEK_SET);
	fread(&height, sizeof(char), 4, fpbmp);
	printf("The Width of the bmp file is %ld.\n", width);
	printf("The Height of the bmp file is %ld.\n", height);
}


void RGBtoYUV()
{
	//convert RGB to YUV
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y[j][i] = 0.299*r[j][i] + 0.587*g[j][i] + 0.114*b[j][i];
			U[j][i] = -0.147*r[j][i] - 0.289*g[j][i] + 0.436*b[j][i];
			V[j][i] = 0.615*r[j][i] - 0.515*g[j][i] - 0.100*b[j][i];
		}
	}
}



void YUVtoRGB()
{
	//convert YUV to GRB
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			output_r[j][i] = output_Y[j][i] + 1.140*output_V[j][i];
			output_g[j][i] = output_Y[j][i] - 0.394*output_U[j][i] - 0.581*output_V[j][i];
			output_b[j][i] = output_Y[j][i] + 2.032*output_U[j][i];
		}
	}
}

void Initialization()
{
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			output_Y[j][i] = 255;
			output_U[j][i] = 0;
			output_V[j][i] = 0;
			records[j][i] = 0;
		}
	}
}
