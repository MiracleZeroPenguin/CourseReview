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
void addHeader(FILE* fpbmp,FILE* fpout);
void RGBtoYUV();
void YUVtoRGB();
void outBMP(FILE* FPOUT);
void Enhancement();
void Equalization_r();
void Equalization_g();
void Equalization_b();
void Equalization_y();

int main(int argc, char*argv[]){
	unsigned char* file_temp;
	FILE* fpbmp;
	FILE* fpout;
	fpbmp = openFile("input2.bmp");
	fpout = writeFile("equalization2.bmp");
	//fpout = writeFile("equalization2.bmp");
	readBMP(fpbmp);
	addHeader(fpbmp, fpout);

	
	
	Equalization_r();
	Equalization_g();
	Equalization_b();
	//RGBtoYUV();
	//Equalization_y();
	//Enhancement();

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
	if (BM!=bfType) {
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
		for (j = 0; j < width;j++) {
			R[height - 1 - i][j] = p[j * 3 + 2];
			G[height - 1 - i][j] = p[j * 3 + 1];
			B[height - 1 - i][j] = p[j * 3];
			out_R[height - 1 - i][j] = 255;
			out_G[height - 1 - i][j] = 255;
			out_B[height - 1 - i][j] = 255;
		}
	}
}

void addHeader(FILE* fpbmp,FILE* fpout) {
	unsigned char* fp_temp;
	fseek(fpbmp, 0L, SEEK_SET);
	fseek(fpout, 0L, SEEK_SET);
	fp_temp = malloc(OffSet);
	fread(fp_temp, 1, OffSet, fpbmp);
	
	fp_temp[2] = (int)(height*width * 3 +OffSet);
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
			U[i][j] =-0.147*R[i][j] - 0.289*G[i][j] + 0.436*B[i][j];
			V[i][j] = 0.615*R[i][j] - 0.515*G[i][j] - 0.100*B[i][j];
		}
	}
}

void YUVtoRGB() {
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			out_R[i][j] = out_Y[i][j] + 1.140*out_V[i][j];
			out_G[i][j] = out_Y[i][j] - 0.394*out_U[i][j] -0.581*out_V[i][j];
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

void Enhancement() {
	int max_L = 0;
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			if (Y[i][j] > max_L)max_L = Y[i][j];
		}
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			out_Y[i][j] = (unsigned char)255 * (log10(Y[i][j] + 1)) / (log10(max_L + 1));
			out_U[i][j] =0;
			out_V[i][j] =0;
		}
	}
}

void Equalization_y() {
	long n0[256] = { 0.0 };
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			n0[Y[i][j]]++;
		}
	}
	double p0[256] = { 0.0 };
	for (i = 0; i < 256; i++)
		p0[i] = n0[i] / (1.0*height*width);

	double s0[256] = { 0.0 };
	s0[0] = p0[0];
	for (i = 1; i < 256; i++)
		s0[i] = s0[i - 1] + p0[i];

	double minmize = 1.0;
	int min_number[256] = { 0 };
	double diff = 0.0;
	for (int j = 0; j < 256; j++)
	{
		//printf("!! == %f\n",s0[j]);
		diff = 0.0; minmize = 1.0;
		for (int i = 0; i < 256; i++)
		{
			double pix0 = 1.0*i / 256;
			diff = fabs(s0[j] - pix0);
			//printf("%d == %f\n",i,diff);
			if (diff < minmize) {
				minmize = diff;
				min_number[j] = i;//min_number[j] --> i ;
			}

		}
		//printf("%d == %d\n",j,min_number[j]);
	//	break;
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			out_Y[j][i] = min_number[Y[j][i]];
			//if (out_Y[j][i] == 0)out_Y[j][i] = 255;
			out_U[j][i] = 0;
			out_V[j][i] = 0;
		}
	}
}

void Equalization_r() {
	long n0[256] = { 0.0 };
	int i, j;
	for (i = 0;i < height; i++) {
		for (j = 0; j < width; j++) {
			n0[R[i][j]]++;
		}
	}
	double p0[256] = { 0.0 };
	for (i = 0; i < 256; i++)
		p0[i] = n0[i] / (1.0*height*width);
	
	double s0[256] = { 0.0 };
	s0[0] = p0[0];
	for (i = 1; i < 256; i++)
		s0[i] = s0[i - 1] + p0[i];

	double minmize = 1.0;
	int min_number[256] = { 0 };
	double diff = 0.0;
	for (int j = 0; j < 256; j++)
	{
		//printf("!! == %f\n",s0[j]);
		diff = 0.0; minmize = 1.0;
		for (int i = 0; i < 256; i++)
		{
			double pix0 = 1.0*i / 256;
			diff = fabs(s0[j] - pix0);
			//printf("%d == %f\n",i,diff);
			if (diff < minmize) {
				minmize = diff;
				min_number[j] = i;//min_number[j] --> i ;
			}

		}
		//printf("%d == %d\n",j,min_number[j]);
	//	break;
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			out_R[j][i] = min_number[R[j][i]];
		}
	}
}
void Equalization_g() {
	long n0[256] = { 0.0 };
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			n0[G[i][j]]++;
		}
	}
	double p0[256] = { 0.0 };
	for (i = 0; i < 256; i++)
		p0[i] = n0[i] / (1.0*height*width);

	double s0[256] = { 0.0 };
	s0[0] = p0[0];
	for (i = 1; i < 256; i++)
		s0[i] = s0[i - 1] + p0[i];

	double minmize = 1.0;
	int min_number[256] = { 0 };
	double diff = 0.0;
	for (int j = 0; j < 256; j++)
	{
		//printf("!! == %f\n",s0[j]);
		diff = 0.0; minmize = 1.0;
		for (int i = 0; i < 256; i++)
		{
			double pix0 = 1.0*i / 256;
			diff = fabs(s0[j] - pix0);
			//printf("%d == %f\n",i,diff);
			if (diff < minmize) {
				minmize = diff;
				min_number[j] = i;//min_number[j] --> i ;
			}

		}
		//printf("%d == %d\n",j,min_number[j]);
	//	break;
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			out_G[j][i] = min_number[G[j][i]];
		}
	}
}
void Equalization_b() {
	long n0[256] = { 0.0 };
	int i, j;
	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			n0[B[i][j]]++;
		}
	}
	double p0[256] = { 0.0 };
	for (i = 0; i < 256; i++)
		p0[i] = n0[i] / (1.0*height*width);

	double s0[256] = { 0.0 };
	s0[0] = p0[0];
	for (i = 1; i < 256; i++)
		s0[i] = s0[i - 1] + p0[i];

	double minmize = 1.0;
	int min_number[256] = { 0 };
	double diff = 0.0;
	for (int j = 0; j < 256; j++)
	{
		//printf("!! == %f\n",s0[j]);
		diff = 0.0; minmize = 1.0;
		for (int i = 0; i < 256; i++)
		{
			double pix0 = 1.0*i / 256;
			diff = fabs(s0[j] - pix0);
			//printf("%d == %f\n",i,diff);
			if (diff < minmize) {
				minmize = diff;
				min_number[j] = i;//min_number[j] --> i ;
			}

		}
		//printf("%d == %d\n",j,min_number[j]);
	//	break;
	}
	for (int j = 0; j < height; j++)
	{
		for (int i = 0; i < width; i++)
		{
			out_B[j][i] = min_number[B[j][i]];
		}
	}
}