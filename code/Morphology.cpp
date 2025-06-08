#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName)
{
	FILE* fp = fopen(FileName, "wb");
	if (hInfo.biBitCount == 24) {
		fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
		fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
		fwrite(Output, sizeof(BYTE), W * H * 3, fp);

	}
	else if (hInfo.biBitCount == 8) {
		fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
		fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
		fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
		fwrite(Output, sizeof(BYTE), W * H, fp);
	}
	fclose(fp);
}

void InverseImage(BYTE* Img, BYTE* Out, int W, int H)
{
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++)
	{
		Out[i] = 255 - Img[i];
	}
}

void Dilation(BYTE* Image, BYTE* Output, int W, int H) { //팽창
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (Image[i * W + j] == 0) {
				if (!(Image[i * W + j - 1] == 0 && Image[i * W + j + 1] == 0
					&& Image[(i - 1) * W + j] == 0 && Image[(i + 1) * W + j]==0))
					Output[i * W + j] = 255;
				else Output[i * W + j] = 0;
			}
			else Output[i * W + j] = 255;
		}
	}
}

void Erosion(BYTE* Image, BYTE* Output, int W, int H) { // 침식
	for (int i = 1; i < H - 1; i++) {
		for (int j = 1; j < W - 1; j++) {
			if (Image[i * W + j] == 255) {
				if (!(Image[i * W + j - 1] == 255 && Image[i * W + j + 1]==255
					&& Image[(i - 1) * W + j] == 255 && Image[(i + 1) * W + j] == 255))
					Output[i * W + j] = 0;
				else Output[i * W + j] = 255;
			}
			else Output[i * W + j] = 0;
		}
	}

}

int main()
{
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE* fp;
	fp = fopen("dilation.bmp", "rb");

	if (fp == NULL) {
		printf("File not found!\n");
		return -1;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	int H = hInfo.biHeight, W = hInfo.biWidth;
	BYTE* Image;
	BYTE* Output;

	if (hInfo.biBitCount == 24) { 
		Image = (BYTE*)malloc(ImgSize * 3);
		Output = (BYTE*)malloc(ImgSize * 3);
		fread(Image, sizeof(BYTE), ImgSize * 3, fp);
	}
	else { 
		fread(hRGB, sizeof(RGBQUAD), 256, fp);
		Image = (BYTE*)malloc(ImgSize);
		Output = (BYTE*)malloc(ImgSize);
		fread(Image, sizeof(BYTE), ImgSize, fp);
	}
	//opening - Dilation->Erosion
	Dilation(Image, Output, W, H);
	Dilation(Output, Image, W, H);
	Dilation(Image, Output, W, H);
	Erosion(Image, Output, W, H);
	Erosion(Output, Image, W, H);
	Erosion(Image, Output, W, H);
	InverseImage(Image, Image, W, H);

	SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "openingMorphology.bmp");
	fclose(fp);
	free(Image);
	free(Output);
}