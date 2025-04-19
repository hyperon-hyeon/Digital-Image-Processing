#pragma warning(disable:4996) //safety issue cause version upgrade 
#include <stdio.h>
#include<stdlib.h> //malloc
#include <Windows.h> //bitmap header structure

int main() {
	BITMAPFILEHEADER hf; //14 bytes
	BITMAPINFOHEADER hInfo; //40 bytes
	RGBQUAD hRGB[256]; //palette, 256*4=1024bytes
	FILE* fp;
	fp = fopen("lenna.bmp", "rb"); // "rb"=read binary file
	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}
	// 1. file -> memory
	
	//fileHeader, infoHeader, palette
	fread(&hf,sizeof(BITMAPFILEHEADER),1,fp); // file information save memory variable.(saving at fileheader(pointer),14bytes,once reading, atfile pointer)
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);//(array's name is address(not pointer),RGBQuard_4bytes, 256 times, at file pointer)

	//pixel
	int ImgSize = hInfo.biWidth * hInfo.biHeight; //malloc as ImgSize
	BYTE * Image= (BYTE*) malloc(ImgSize); // BYTE Image[ImgSize]<-not available, return start point 
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);

	fclose(fp);

	//2. memory -> img process
	for (int i = 0; i < ImgSize; i++) {
		Output[i] = 255 - Image[i]; // reverse Img, if Output[i]>255 - overflow (need clipping!)
	}

	//3. save reverse img
	fp = fopen("output.bmp", "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp); //1byte(writing binary)*14times
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	//malloc is not free -> *memory leak* error(heap memory still use)
	free(Image);
	free(Output);

	return 0;
}