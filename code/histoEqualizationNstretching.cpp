#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void obtainHisto(BYTE* Img, int* Histo, int W, int H) {
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) Histo[Img[i]]++;
}

void obtainAHisto(int*Histo, int*AHisto) {
	for (int i = 1; i < 256; i++) {
		AHisto[i] = AHisto[i - 1] + Histo[i];
	}
}

void histogramEqualization(BYTE* Img, BYTE* Out, int* AHisto, int W, int H) {
	int ImgSize = W * H;
	int Gmax = 255, Nt = W * H;
	double ratio = Gmax / (double)Nt;
	BYTE Norm[256];

	for (int i = 0; i < 256; i++) Norm[i] = (BYTE) (AHisto[i] * ratio); 
	for (int i = 0; i < ImgSize; i++)  Out[i] = Norm[Img[i]]; 
}

void histogramStretching(BYTE*Img, BYTE*Out, int* Histo, int W, int H) {
	int ImgSize = W * H;

	int min = -1, max=-1;

	for(int i=0;i<256;i++){
		if (Histo[i] != 0) {
			min = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (Histo[i] != 0) {
			max = i;
			break;
		}
	}

	for (int i = 0; i < ImgSize; i++) Out[i] = (BYTE) 255.0*(Img[i] - min) / (max - min) ;
}

int main() {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE* fp;

	fp = fopen("coin.bmp", "rb");
	if (fp == NULL) {
		printf("File not found\n");
		return -1;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(hRGB, sizeof(RGBQUAD), 256, fp);

	int W = hInfo.biWidth, H = hInfo.biHeight;
	int ImgSize = W * H;

	BYTE* Image = (BYTE*)malloc(ImgSize);
	BYTE* Output = (BYTE*)malloc(ImgSize);

	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	int Histo[256] = { 0 };
	int AHisto[256] = { 0 };

	//Image process
	obtainHisto(Image, Histo, W, H);
	obtainAHisto(Histo,AHisto);
	histogramEqualization(Image, Output, AHisto, W, H);
	//histogramStretching(Image, Output, Histo, W, H);

	fp = fopen("equalization_img.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	free(Image);
	free(Output);

	return 0;
}