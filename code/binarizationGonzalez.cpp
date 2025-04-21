#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void binarization(BYTE* Img, BYTE* Out, int W, int H, int T){
	int ImgSize = W * H;
	for (int i = 0; i < ImgSize; i++) {
		if (Img[i] >= T) Out[i] = 255;
		else Out[i] = 0;
	}
}

int gonzalezThreshold(BYTE* Img, int* Histo, int W, int H){
	int ImgSize = W * H;

	int T, T1=-1, T2=-1, nextT=0;
	int G1=0, G2=0, m1, m2, cnt1=0, cnt2=0;
	int e = 3;
	int condition = 0;

	//initial T
	for (int i = 0; i < 256; i++) {
		if (Histo[i] != 0) {
			T1 = i;
			break;
		}
	}
	for (int i = 255; i >= 0; i--) {
		if (Histo[i] != 0) {
			T2 = i;
			break;
		}
	}
	T = (T1 + T2) / 2;

	while (condition!=1) {
		for (int i = 0; i < ImgSize; i++) {
			if (Img[i] >= T) {
				G1 += Img[i];
				cnt1++;
			}
			else {
				G2 += Img[i];
				cnt2++;
			}
		}
		
		if (cnt1 == 0) cnt1 = 1;
		if (cnt2 == 0) cnt2 = 1;

		m1 = G1 / cnt1;
		m2 = G2 / cnt2;

		nextT = (m1 + m2) / 2;
		if (abs(nextT - T) < e) condition = 1;
		//printf("%d %d \n", T, nextT);
		T = nextT;
	}
	return nextT;
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
	int T;

	//Image process
	obtainHisto(Image, Histo, W, H);
	T=gonzalezThreshold(Image, Histo, W, H);
	binarization(Image, Output, W, H, T);

	fp = fopen("binarizationGonzalez_img.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	free(Image);
	free(Output);

	return 0;
}