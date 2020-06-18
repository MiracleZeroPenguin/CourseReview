#ifndef COMPILE_WRITE
#define COMPILE_WRITE
//输出yuv文件
int WriteYUV(unsigned char* Y, unsigned char* U, unsigned char* V, unsigned long size, FILE* outFile);
//输出rgb文件
int WriteRGB(unsigned char* Y, unsigned char* U, unsigned char* V, unsigned long size, FILE* outFile);
#endif // !COMPILE_WRITE
