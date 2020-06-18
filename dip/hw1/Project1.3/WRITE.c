#include <stdio.h>
#include <windows.h>
#include<string.h>
#include"RGB2YUV.h"
#include"TRANSCONST.h"
#include"READBMP.h"
#include"WRITE.h"
int WriteYUV(unsigned char* Y, unsigned char* U, unsigned char* V, unsigned long size, FILE* outFile) {
	if (fwrite(Y, 1, size, outFile) != size)return 0;
	if (fwrite(U, 1, size / 4, outFile) != size / 4)return 0;
	if (fwrite(V, 1, size / 4, outFile) != size / 4)return 0;
	return 1;
}


int WriteRGB(unsigned char* Y, unsigned char* U, unsigned char* V, unsigned long size, FILE* outFile) {
	if (fwrite(Y, 3, size, outFile) != size)return 0;
	return 1;
}