
#include <stdio.h>
#include <stdlib.h> //malloc free
#include<math.h>

#include "bmp.h"


//定义画布长宽值
#define BMP_WIDTH  1000
#define BMP_HEIGHT 1000

#define BMP_PERW   3
#define BM 19778
#define MAXSIZE 10000

//输出像素值的rgb值数组
//所以在对坐标[x,y]定点时,应该这样调用 buff[y][x][..]
//第三维：0->R  1->G  2->B
unsigned char buff[BMP_HEIGHT][BMP_WIDTH][BMP_PERW];

//输入像素值的rgb值数组
unsigned char R[MAXSIZE][MAXSIZE], G[MAXSIZE][MAXSIZE], B[MAXSIZE][MAXSIZE];

//指针偏移量
unsigned int OffSet = 0;

//输入图片的长款值
int width, height;


//打开源文件
FILE* openFile(char name[]);

//原图信息的读取
void readBMP(FILE *fpbmp);

//平移函数
//将画布n*n等分，new_x与new_y对应新坐标
void translation(int new_x, int new_y);

//旋转函数
//angle代表顺时针旋转angle
//angle是弧度制
void rotation(double angle);

//缩放
//coeffx与coeffy对应缩放倍数
//大于1时表示放大，小于1时表示缩小
void scale(double coeffx, double coeffy);

//错切
void shear(double k1, double k2);

//对称
//flag1为1时表示x方向对称
//flag2为1时表示y方向对称
void mirror(int flag1, int flag2);


int main(int argc, char *argv[])
{
	int x, y;
	FILE* fpbmp;
	fpbmp = openFile("input.bmp");
	readBMP(fpbmp);

	//---------- 创建一张图片 ----------

	//initial
	for (x = 0; x < BMP_WIDTH; x++)
	{
		for (y = 0; y < BMP_HEIGHT; y++)
		{
			buff[y][x][0] = R[y*height / BMP_HEIGHT][20];
			buff[y][x][1] = G[y*height / BMP_HEIGHT][20];
			buff[y][x][2] = B[y*height / BMP_HEIGHT][20];
		}
	}
	
	
	//painting

	//--------------1、translation------------------//
	//translation(1, 1);

	//--------------2、rotation---------------------//
	//double angle;
	//angle = 3.1415926/6;
	//rotation(angle);

	//--------------3、Scale---------------------//
	//double coeffx,coeffy;//缩放倍数，放大则大于1，缩小则小于1
	//coeffx = 0.5;
	//coeffy = 0.8;
	//scale(coeffx,coeffy);

	//--------------4、Shear---------------------//
	//double kx,ky;//x方向错切系数与y方向错切系数
	//kx = 0.2;
	//ky = 0.8;
	//shear(kx,ky);

	//--------------5、Mirror---------------------//
	//mirror(1,1);//第一个1表示水平对称，第二个1表示垂直对称

	bmp_create("test.bmp", (unsigned char *)buff, BMP_WIDTH, BMP_HEIGHT, BMP_PERW);
	return 0;
}


FILE* openFile(char name[]) {
	FILE* fpbmp;
	fpbmp = fopen(name, "rb");
	if (fpbmp == NULL) {
		printf("open failed(；′⌒`)\n");
		exit(1);
	}
	//test if it is a bmp
	unsigned short bfType = 0;
	fseek(fpbmp, 0L, SEEK_SET);
	fread(&bfType, sizeof(char), 2, fpbmp);
	if (BM != bfType) {
		printf("it is not a bmp(；′⌒`)\n");
		exit(1);
	}

	//get the header length
	fseek(fpbmp, 10L, SEEK_SET);
	fread(&OffSet, sizeof(char), 4, fpbmp);
	printf("get the length of the header part %d\n", OffSet);

	//get width and height
	int size;
	fseek(fpbmp, 18L, SEEK_SET);
	fread(&width, sizeof(char), 4, fpbmp);
	printf("the bmp width is %d\n", width);
	fseek(fpbmp, 2L, SEEK_SET);
	fread(&size, sizeof(char), 4, fpbmp);
	fseek(fpbmp, 22L, SEEK_SET);
	fread(&height, sizeof(char), 4, fpbmp);
	printf("the bmp height is %d\n", height);

	fseek(fpbmp, 0L, SEEK_SET);
	return fpbmp;
}

void readBMP(FILE* fpbmp) {
	int i = 0;
	int j = 0;
	unsigned char* p = NULL;
	fseek(fpbmp, OffSet, SEEK_SET);
	int s = ((24 * width + 31) / 8) / 4 * 4;
	p = malloc(s);
	for (i = 0; i < height; i++) {
		fread(p, 1, s, fpbmp);
		for (j = 0; j < width; j++) {
			R[height - 1 - i][j] = p[j * 3 + 2];
			G[height - 1 - i][j] = p[j * 3 + 1];
			B[height - 1 - i][j] = p[j * 3];
		}
	}
}

void translation(int new_x, int new_y) {
	int i0, j0;

	for (i0 = 0; i0 < width; i0++)
	{
		for (j0 = 0; j0 <height; j0++)
		{
			buff[new_x*height + j0][new_y*width + i0][0] = R[j0][i0];
			buff[new_x*height + j0][new_y*width + i0][1] = G[j0][i0];
			buff[new_x*height + j0][new_y*width + i0][2] = B[j0][i0];
		}
	}
}

void rotation(double angle) {

	double x1, y1, cos_a, sin_a, d1, d2, d;
	int m, n, color;
	double new_width, new_height;
	new_width = width * cos(angle) + height * sin(angle);
	new_height = height * cos(angle) + width * sin(angle);
	cos_a = cos(angle);
	sin_a = sin(angle);
	for (int j = 0; j < new_height; j++) {
		for (int i = 0; i < new_width; i++) {
			x1 = cos_a * (j - new_height / 2) - sin_a * (i - new_width / 2) + width / 2;
			y1 = sin_a * (j - new_height / 2) + cos_a * (i - new_width / 2) + height / 2;
			n = floor(x1);
			m = floor(y1);
			if (m >= 0 && n >= 0 && n < width&&m < height) {
				for (color = 0; color < 3; color++) {
					switch (color){
					case 0:
						d1 = R[m][n] * (n + 1 - x1) + R[m][n + 1] * (x1-n);
						d2 = R[m + 1][n] * (n + 1 - x1) + R[m + 1][n + 1] * (x1 - n);
						d = d1 * (m+1- y1) + d2 * (y1 -m);
						break;
					case 1:
						d1 = G[m][n] * (n + 1 - x1) + G[m][n + 1] * (x1 - n);
						d2 = G[m + 1][n] * (n + 1 - x1) + G[m + 1][n + 1] * (x1 - n);
						d = d1 * (m + 1 - y1) + d2 * (y1 - m);
						break;
					default:
						d1 = B[m][n] * (n + 1 - x1) + B[m][n + 1] * (x1 - n);
						d2 = B[m + 1][n] * (n + 1 - x1) + B[m + 1][n + 1] * (x1 - n);
						d = d1 * (m + 1 - y1) + d2 * (y1 - m);
						break;
					}
					buff[j][i][color] =d;
				}
			}
		}
	}
}

void scale(double coeffx,double coeffy) {
	double x1, y1, d1, d2, d;
	int m, n;
	double new_height, new_width;
	new_height = height * coeffy;
	new_width = width * coeffx;
	for (int j = 0; j < new_height; j++) {
		for (int i = 0; i < new_width; i++) {
			y1 = j / coeffy;
			x1 = i / coeffx;
			m = floor(y1);
			n = floor(x1);
			if (m >= 0 && n >= 0 && n < width&&m < height) {
				for (int color = 0; color < 3; color++) {
					switch (color) {
					case 0:
						d1 = R[m][n] * (n + 1 - x1) + R[m][n + 1] * (x1 - n);
						d2 = R[m + 1][n] * (n + 1 - x1) + R[m + 1][n + 1] * (x1 - n);
						d = d1 * (m + 1 - y1) + d2 * (y1 - m);
						break;
					case 1:
						d1 = G[m][n] * (n + 1 - x1) + G[m][n + 1] * (x1 - n);
						d2 = G[m + 1][n] * (n + 1 - x1) + G[m + 1][n + 1] * (x1 - n);
						d = d1 * (m + 1 - y1) + d2 * (y1 - m);
						break;
					default:
						d1 = B[m][n] * (n + 1 - x1) + B[m][n + 1] * (x1 - n);
						d2 = B[m + 1][n] * (n + 1 - x1) + B[m + 1][n + 1] * (x1 - n);
						d = d1 * (m + 1 - y1) + d2 * (y1 - m);
						break;
					}
					buff[j][i][color] = d;
				}
			}
		}
	}
}

void shear(double k1, double k2) {
	double x1, y1, d1, d2, d;
	int m, n;
	double new_height, new_width;
	new_height = height+k2*width;
	new_width = width+k1*height;
	for (int j = 0; j < new_height; j++) {
		for (int i = 0; i < new_width; i++) {
			y1 = (k2*i - j) / (k1*k2 - 1);
			x1 = (k1*j - i) / (k1*k2 - 1);
			m = floor(y1);
			n = floor(x1);
			if (m >= 0 && n >= 0 && n < width&&m < height) {
				for (int color = 0; color < 3; color++) {
					switch (color) {
					case 0:
						d1 = R[m][n] * (n + 1 - x1) + R[m][n + 1] * (x1 - n);
						d2 = R[m + 1][n] * (n + 1 - x1) + R[m + 1][n + 1] * (x1 - n);
						d = d1 * (m + 1 - y1) + d2 * (y1 - m);
						break;
					case 1:
						d1 = G[m][n] * (n + 1 - x1) + G[m][n + 1] * (x1 - n);
						d2 = G[m + 1][n] * (n + 1 - x1) + G[m + 1][n + 1] * (x1 - n);
						d = d1 * (m + 1 - y1) + d2 * (y1 - m);
						break;
					default:
						d1 = B[m][n] * (n + 1 - x1) + B[m][n + 1] * (x1 - n);
						d2 = B[m + 1][n] * (n + 1 - x1) + B[m + 1][n + 1] * (x1 - n);
						d = d1 * (m + 1 - y1) + d2 * (y1 - m);
						break;
					}
					buff[j][i][color] = d;
				}
			}
		}
	}
}

void mirror(int flag1, int flag2) {
	if (flag1 == 1&&flag2==0) {
		for (int j = 0; j < height; j++) {
			for (int i = width; i < 2*width; i++) {
				buff[j][i][0] = R[j][2 * width - 1 - i];
				buff[j][i][1] = G[j][2 * width - 1 - i];
				buff[j][i][2] = B[j][2 * width - 1 - i];
			}
		}
	}
	if (flag2 == 1&&flag1==0) {
		for (int j = height; j < 2 * height; j++) {
			for (int i = 0; i < width; i++) {
				buff[j][i][0] = R[2 * height - 1 - j][i];
				buff[j][i][1] = G[2 * height - 1 - j][i];
				buff[j][i][2] = B[2 * height - 1 - j][i];
			}
		}
	}
	if (flag1 == 1 && flag2 == 1) {
		for (int j = height; j < 2 * height; j++) {
			for (int i = width; i < 2*width; i++) {
				buff[j][i][0] = R[2 * height - 1 - j][2 * width - 1 - i];
				buff[j][i][1] = G[2 * height - 1 - j][2 * width - 1 - i];
				buff[j][i][2] = B[2 * height - 1 - j][2 * width - 1 - i];
			}
		}
	}
}