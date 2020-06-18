#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define BM 19778
#define BITHeaderLength 14
#define MAXSIZE 10000


unsigned char Y[MAXSIZE][MAXSIZE], U[MAXSIZE][MAXSIZE], V[MAXSIZE][MAXSIZE];
unsigned char out_Y[MAXSIZE][MAXSIZE], out_U[MAXSIZE][MAXSIZE], out_V[MAXSIZE][MAXSIZE];
unsigned char R[MAXSIZE][MAXSIZE], G[MAXSIZE][MAXSIZE], B[MAXSIZE][MAXSIZE];
unsigned char out_R[MAXSIZE][MAXSIZE], out_G[MAXSIZE][MAXSIZE], out_B[MAXSIZE][MAXSIZE];
unsigned int OffSet = 0;
long width, height;

FILE* openFile(char name[]);
FILE* writeFile(char name[]);
void readBMP(FILE* fpbmp);
void addHeader(FILE* fpbmp, FILE* fpout);
void RGBtoYUV();
void YUVtoRGB();
void outBMP(FILE* FPOUT);
void MeanFiltering();
void Laplacian();
void Laplacian_enhencement();

int main(int argc, char*argv[]) {
	unsigned char* file_temp;
	FILE* fpbmp;
	FILE* fpout;
	fpbmp = openFile("input_mai.bmp");		//打开原图片
	fpout = writeFile("meanfiltering_mai_25.bmp");	//分别输出meanfiltering，laplacian,laplacian_enhancement

	readBMP(fpbmp);
	addHeader(fpbmp, fpout);

	//RGBtoYUV();
	//*-----------------my functions--------------*//
	MeanFiltering();	//均值滤波函数
	//Laplacian();		//拉普拉斯变换获得边缘图像
	//Laplacian_enhencement();	//拉普拉斯图像增强

	//YUVtoRGB();
	outBMP(fpout);

	fclose(fpbmp);
	fclose(fpout);
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

FILE* writeFile(char name[]) {
	FILE* fpout;
	fpout = fopen(name, "wb+");
	if (fpout == NULL) {
		printf("open output file failed(；′⌒`)\n");
		exit(1);
	}
	fseek(fpout, 0L, SEEK_SET);
	return fpout;
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
			out_R[height - 1 - i][j] = 255;
			out_G[height - 1 - i][j] = 255;
			out_B[height - 1 - i][j] = 255;
		}
	}
}

void addHeader(FILE* fpbmp, FILE* fpout) {
	unsigned char* fp_temp;
	fseek(fpbmp, 0L, SEEK_SET);
	fseek(fpout, 0L, SEEK_SET);
	fp_temp = malloc(OffSet);
	fread(fp_temp, 1, OffSet, fpbmp);

	fp_temp[2] = (int)(height*width * 3 + OffSet);
	fp_temp[18] = (int)width;
	fp_temp[22] = (int)height;
	fp_temp[34] = (int)(height*((24 * width / 8 + 3) / 4 * 4));
	fwrite(fp_temp, 1, OffSet, fpout);
}

void RGBtoYUV() {
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			Y[i][j] = 0.299*R[i][j] + 0.587*G[i][j] + 0.114*B[i][j];
			U[i][j] = -0.147*R[i][j] - 0.289*G[i][j] + 0.436*B[i][j];
			V[i][j] = 0.615*R[i][j] - 0.515*G[i][j] - 0.100*B[i][j];
		}
	}
}

void YUVtoRGB() {
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			out_R[i][j] = out_Y[i][j] + 1.140*out_V[i][j];
			out_G[i][j] = out_Y[i][j] - 0.394*out_U[i][j] - 0.581*out_V[i][j];
			out_B[i][j] = out_Y[i][j] + 2.032*out_U[i][j];
		}
	}
}

void outBMP(FILE* fpout) {
	int i = 0;
	int j = 0;
	unsigned char* p_out = NULL;
	fseek(fpout, OffSet, SEEK_SET);
	int s = ((24 * width + 31) / 8) / 4 * 4;
	p_out = malloc(s);

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			p_out[j * 3 + 2] = out_R[height - 1 - i][j];
			p_out[j * 3 + 1] = out_G[height - 1 - i][j];
			p_out[j * 3] = out_B[height - 1 - i][j];
		}
		fwrite(p_out, 1, s, fpout);
	}
}

void MeanFiltering() {
	int num_pixs = 25;		//定义mask大小，num_pix为mask总像素数量
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i<=num_pixs/2 || j <=num_pixs/2 || i >= width-1-num_pixs/2 || j == height-1-num_pixs/2) {
				out_R[j][i] = R[j][i];
				out_G[j][i] = G[j][i];
				out_B[j][i] = B[j][i];	//输出边缘保持原图像不变
			}
			else {
				int totalR, totalG, totalB,totalY;
				totalR = 0; totalG = 0; totalB = 0; totalY = 0;
				int k = 0;
				for (int ki = 0; ki < num_pixs; ki++) {
					for (int kj = 0; kj < num_pixs; kj++) {
						totalR = totalR + R[j + kj-num_pixs/2][i + ki-num_pixs/2];
						totalG = totalG + G[j + kj-num_pixs/2][i + ki-num_pixs/2];
						totalB = totalB + B[j + kj-num_pixs/2][i + ki-num_pixs/2];	//计算mask大小内原图像rgb各值之和
						k++;
					}
				}
				out_R[j][i] = totalR / (num_pixs*num_pixs);
				out_G[j][i] = totalG / (num_pixs*num_pixs);
				out_B[j][i] = totalB / (num_pixs*num_pixs);		//图像均值滤波变化
			}
		}
	}
}

void Laplacian() {
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i <= 0 || j <= 0 || i == width - 1 || j == height ) {
				out_R[j][i] = R[j][i];
				out_G[j][i] = G[j][i];
				out_B[j][i] = B[j][i];
			}
			else {
				out_R[j][i] = abs(R[j-1][i-1]+ R[j - 1][i + 1] + R[j + 1][i - 1] + R[j + 1][i + 1] + R[j - 1][i] + R[j + 1][i] + R[j][i - 1] + R[j][i + 1] - 8 * R[j][i]);
				out_G[j][i] = abs(G[j - 1][i - 1] + G[j - 1][i + 1] + G[j + 1][i - 1] + G[j + 1][i + 1]+G[j - 1][i] + G[j + 1][i] + G[j][i - 1] + G[j][i + 1] - 8 * G[j][i]);
				out_B[j][i] = abs(B[j - 1][i - 1] + B[j - 1][i + 1] + B[j + 1][i - 1] + B[j + 1][i + 1]+B[j - 1][i] + B[j + 1][i] + B[j][i - 1] + B[j][i + 1] - 8 * B[j][i]);
			}
		}
	}
}

void Laplacian_enhencement() {
	int ddr, ddg, ddb,judge;
	judge = 0;
	float index;//拉普拉斯图像增强算子系数
	index = 0.2;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i <= 0 || j <= 0 || i == width - 1 || j == height) {
				out_R[j][i] = R[j][i];
				out_G[j][i] = G[j][i];
				out_B[j][i] = B[j][i];
			}
			else {
				ddr = abs(R[j - 1][i - 1] + R[j - 1][i + 1] + R[j + 1][i - 1] + R[j + 1][i + 1] + R[j - 1][i] + R[j + 1][i] + R[j][i - 1] + R[j][i + 1] - 8 * R[j][i]);
				ddg = abs(G[j - 1][i - 1] + G[j - 1][i + 1] + G[j + 1][i - 1] + G[j + 1][i + 1] + G[j - 1][i] + G[j + 1][i] + G[j][i - 1] + G[j][i + 1] - 8 * G[j][i]);
				ddb = abs(B[j - 1][i - 1] + B[j - 1][i + 1] + B[j + 1][i - 1] + B[j + 1][i + 1] + B[j - 1][i] + B[j + 1][i] + B[j][i - 1] + B[j][i + 1] - 8 * B[j][i]);
				if (ddr != 0)
					judge = 1;
				else
					judge = 0;
				out_R[j][i] =( R[j][i] + index*abs(ddr))>255?255: (R[j][i] + index*abs(ddr));
				out_G[j][i] =( G[j][i] + index*abs(ddg))>255?255: (G[j][i] + index*abs(ddr));
				out_B[j][i] =( B[j][i] + index*abs(ddb))>255?255: (B[j][i] + index*abs(ddr));
				//存在输出值计算所得大于255，避免出现溢出情况
			}
		}
	}
}