#ifndef COMPILE_FUNDATION
#define COMPILE_FUNDATION

FILE *openfile(char filename[]);
FILE *writefile(char filename[]);

void RGBtoYUV();	//turn the RGBinfo to YUVinfo
void YUVtoRGB();	//turn back and be ready to print
void bmpoutput(FILE *fpout);	//print out the file
void bmpDataPart(FILE* fpbmp);
void Initialization();
void addHeadertofile(FILE *input, FILE *output);	//add the standard data to the head of the file
void bmpFileTest(FILE* fpbmp);		//test that if it is a BM
void bmpHeaderPartLength(FILE* fpbmp);	//get the length of the header
void BmpWidthHeight(FILE* fpbmp);	//get the width and height of the header

#endif // !COMPILE_FUNDATION

