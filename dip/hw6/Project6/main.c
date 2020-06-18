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
void  Bilateral_Filtering();

int main(int argc, char*argv[]) {
	unsigned char* file_temp;
	FILE* fpbmp;
	FILE* fpout;
	fpbmp = openFile("timg.bmp");		//打开原图片
	fpout = writeFile("25.bmp");	//分别输出双边滤波结果
	//实验发现多次进行双边滤波，效果较佳
	readBMP(fpbmp);
	addHeader(fpbmp, fpout);

	//RGBtoYUV();
	//*-----------------my functions--------------*//
	Bilateral_Filtering();//进行双边滤波

	//YUVtoRGB();//本次实验中不需要进行yuv转换
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

void  Bilateral_Filtering() {
	float wr,wg,wb;	//权重系数w
	float d;	//定义域核d
	float rr,rg,rb;	//值域核r
	int i, j;	//目标像素位置
	int k, l;	//mask各个像素位置
	int num_pixs;	//mask边长
	num_pixs = 25;
	for (j = 0; j < height; j++) {
		for (i = 0; i < width; i++) {
			if (i <= num_pixs / 2 || j <= num_pixs / 2 || i >= width - 1 - num_pixs / 2 || j == height - 1 - num_pixs / 2) {
				out_R[j][i] = R[j][i];
				out_G[j][i] = G[j][i];
				out_B[j][i] = B[j][i];	//输出边缘保持原图像不变
			}
			else {
				float fw_r, fw_g, fw_b;
				float totalw_r, totalw_g, totalw_b;
				fw_r = 0; fw_g = 0; fw_b = 0;
				totalw_r = 0; totalw_g = 0; totalw_b = 0;
				for (l = j - num_pixs / 2; l <= j + num_pixs / 2; l++) {
					for (k = i - num_pixs / 2; k <= i - num_pixs / 2; k++) {
						d = exp(-((l - j) ^ 2 + (k - i) ^ 2)*1.0/ (2 * num_pixs*num_pixs));
						rr = exp(-((R[j][i] - R[l][k]) ^ 2)*1.0 / (2 * num_pixs*num_pixs));
						rg = exp(-((G[j][i] - G[l][k]) ^ 2)*1.0 / (2 * num_pixs*num_pixs));
						rb = exp(-((B[j][i] - B[l][k]) ^ 2)*1.0 / (2 * num_pixs*num_pixs));
						wr = d * rr;
						wg = d * rg;
						wb = d * rb;
						fw_r = fw_r + R[l][k] * wr;
						fw_g = fw_g + G[l][k] * wg;
						fw_b = fw_b + B[l][k] * wb;
						totalw_r = totalw_r + wr;
						totalw_g = totalw_g + wg;
						totalw_b = totalw_b + wb;
					}
				}
				out_R[j][i] = (int)(fw_r / totalw_r);
				out_G[j][i] = (int)(fw_g / totalw_g);
				out_B[j][i] = (int)(fw_b / totalw_b);
			}
		}
	}
}

