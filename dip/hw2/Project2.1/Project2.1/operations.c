#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include"definition.h"
#include"fundation.h"
#include"operations.h"

void Binarization()//data from Y[] to output_Y[]
{
	//get Threshold
	unsigned char T = 0, perfect_T = 0;
	double w_below, w_above, u_below, u_above, u;
	long number_below_T, number_above_T;

	for (int num = 0; num <= 255; num++) {
		long all_below_T = 0L, all_above_T = 0L;
		number_below_T = 0L, number_above_T = 0L;

		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				if (T >= Y[j][i]) {
					number_below_T++;
					all_below_T += Y[j][i];
				}
				else {
					number_above_T++;
					all_above_T += Y[j][i];
				}
			}
		}
		//if it is zero ,this loop is useless
		if (number_below_T == 0 || number_above_T == 0) continue;

		//密度
		u_below = (double)all_below_T / number_below_T;
		u_above = (double)all_above_T / number_above_T;

		w_below = (double)number_below_T / (height*width);
		w_above = (double)number_above_T / (height*width);

		//图像平均总灰度
		u = w_below * u_below + w_above * u_above;

		double fc[255] = { 0.0 }, max_fc = 0.0;
		fc[num] = w_below * pow(abs(u_below - u), 2.0) + w_above * pow(abs(u_above - u), 2.0);
		//循环比较获得最大方差，对应的记为最佳阈值
		if (fc[num] > max_fc) {
			perfect_T = T;
			max_fc = fc[num];
		}
		T++;
	}

	//对像素进行二值化处理
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			if (Y[j][i] < perfect_T)
				output_Y[j][i] = 0;
			else
				output_Y[j][i] = 255;
			output_U[j][i] = 0;
			output_V[j][i] = 0;
		}
	}
	printf("Binarization finished!\n");
}


void Dilation() {
	//Binarization();
	int Struture_Element[elementline][elementline] = { {0} };//假设是膨胀模型十字型
	int centre = (elementline - 1) / 2;
	/*生成十字形element*/
	for (int j = 0; j < elementline; j++)
	{
		for (int i = 0; i < elementline; i++)
		{
			if (j == centre || i == centre)
				Struture_Element[j][i] = 1;
			else
				Struture_Element[j][i] = 0;
		}
	}

	//传入经过binary后的二值图数据
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			output_Y[j][i] = output_Y[j][i];//*****//
			output_U[j][i] = 0;
			output_V[j][i] = 0;
		}
	}

	/*膨胀一小圈*/
	for (int j = 0; j < height - elementline; j++)
	{
		for (int i = 0; i < width - elementline; i++)
		{
			int Istrue = 0;
			for (int i1 = 0; i1 < elementline; i1++)
				for (int j1 = 0; j1 < elementline; j1++)
				{
					if (Struture_Element[i1][j1] == 1 && Y[j + j1][i + i1] == 0)
						Istrue = 1;
				}
			/*大概，YUV中黑色是。。。Y=0 （Ｔ＾Ｔ）*/
			if (Istrue == 1)
				output_Y[j + centre][i + centre] = 0;

		}
	}
	printf("Dilation successed!\n");
}


void Erosion()
{
	//Binarization();
	int Struture_Element[elementline][elementline] = { {0} };
	int centre = (elementline - 1) / 2;
	//生成十字形element，也可更改参数生成不同形状的element
	for (int j = 0; j < elementline; j++)
	{
		for (int i = 0; i < elementline; i++)
		{
			if (j == centre || i == centre)
				Struture_Element[j][i] = 1;
			else
				Struture_Element[j][i] = 0;
		}
	}

	//导入图片数据
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			output_Y[j][i] = output_Y[j][i];
			output_U[j][i] = 0;
			output_V[j][i] = 0;
		}
	}

	//腐蚀一小圈
	for (int j = 0; j < height - elementline; j++)
	{
		for (int i = 0; i < width - elementline; i++)
		{
			int Istrue = 0;
			for (int i1 = 0; i1 < elementline; i1++)
				for (int j1 = 0; j1 < elementline; j1++)
				{
					if (Struture_Element[i1][j1] == 0)
						continue;
					else if (Struture_Element[i1][j1] == 1 && output_Y[j + i1][i + j1] != 0)//不同时黑色
						Istrue = 1;//不全等
				}
			
			if (Istrue == 0)
				output_Y[j + centre][i + centre] = 0;
		}
	}
	printf("Erosion successed!\n");
}

void Opening() {
	Erosion();
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y[j][i] = output_Y[j][i];
			U[j][i] = output_U[j][i];
			V[j][i] = output_V[j][i];
		}
	}
	Dilation();
	printf("Opening finished, Now outer noises are removed!\n");
}

void Closing() {
	Dilation();
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			Y[j][i] = output_Y[j][i];
			U[j][i] = output_U[j][i];
			V[j][i] = output_V[j][i];
		}
	}
	Erosion();
	printf("Closing finished, Now inner noises are removed!\n");
}


