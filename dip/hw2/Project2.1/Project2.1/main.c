

#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include"definition.h"
#include"fundation.h"
#include"operations.h"

int main(int argc, char* argv[])
{
	unsigned char *fp_temp;

	FILE *fpbmp;
	FILE *fpout;
	Initialization();

	fpbmp = openfile("input.bmp");	//read the bmp
	bmpDataPart(fpbmp);

	fpout = writefile("binar.bmp");	//the name decided by the operations
	addHeadertofile(fpbmp, fpout);	//printf the fileinfo and header in the outfile
	RGBtoYUV();	//turn the RGBinfo to YUVinfo

	//------------here to choose which operation you wanna to use---
	Binarization();
	//Dilation();
	//Erosion();
	//Opening();
	//Closing();

	YUVtoRGB();	//turn back to RGBinfo
	bmpoutput(fpout);	//print the answer

	fclose(fpbmp);	//close the input file
	fclose(fpout);	//close the output file

}


