#ifndef COMPILE_DEFINITION
#define COMPILE_DEFINITION
#define BITMAPFILEHEADERLENGTH 14   // The bmp FileHeader length is 14
#define BM 19778                    // The ASCII code for BM
#define elementline 3				//designed by the operation erosion and dilation
#define MAXSIZE 10000
unsigned char Y[MAXSIZE][MAXSIZE], output_Y[MAXSIZE][MAXSIZE];
unsigned char U[MAXSIZE][MAXSIZE], output_U[MAXSIZE][MAXSIZE];
unsigned char V[MAXSIZE][MAXSIZE], output_V[MAXSIZE][MAXSIZE];

long width;          // The Width of the Data Part
long height;         // The Height of the Data Part
unsigned char r[MAXSIZE][MAXSIZE], output_r[MAXSIZE][MAXSIZE];
unsigned char g[MAXSIZE][MAXSIZE], output_g[MAXSIZE][MAXSIZE];
unsigned char b[MAXSIZE][MAXSIZE], output_b[MAXSIZE][MAXSIZE];
unsigned char records[MAXSIZE][MAXSIZE];

#endif // !COMPILE_DEFINITION
