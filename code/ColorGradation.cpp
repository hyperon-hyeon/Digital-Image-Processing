#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName) {
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

int main() {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE* fp;
	fp = fopen("tcasample.bmp", "rb");

	if (fp == NULL) {
		printf("File is not found\n");
		return -1;
	}

	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	int W = hInfo.biWidth, H = hInfo.biHeight;
	BYTE* Image;
	BYTE* Output;

	if (hInfo.biBitCount == 24) {
		Image = (BYTE*)malloc(ImgSize * 3);
		Output = (BYTE*)malloc(ImgSize * 3);
		fread(Image, sizeof(BYTE), ImgSize * 3, fp);
	}
	else {
		Image = (BYTE*)malloc(ImgSize);
		Output = (BYTE*)malloc(ImgSize);
		fread(Image, sizeof(BYTE), ImgSize, fp);
	}
	fclose(fp);

	double wt=0.0;
	for (int i = 0; i < 160; i++) {
		for (int j = 0; j < W; j++) {
			wt = j / (double)(W - 1);
			Output[i * W * 3 + j * 3] = (BYTE)(255 * (1 - wt));
			Output[i * W * 3 + j * 3 + 1] = (BYTE)(255 * wt);
			Output[i * W * 3 + j * 3 + 2] = (BYTE)(255 * wt);
		}
	}

	for (int i = 160; i < 320; i++) {
		for (int j = 0; j < W; j++) {
			wt = j / (double)(W - 1);
			Output[i * W * 3 + j * 3] = (BYTE)(255 * wt);
			Output[i * W * 3 + j * 3 + 1] = (BYTE)(255 * (1 - wt));
			Output[i * W * 3 + j * 3 + 2] = (BYTE)(255 * wt);
		}
	}

	for (int i = 320; i < 480; i++) {
		for (int j = 0; j < W; j++) {
			wt = j / (double)(W - 1);
			Output[i * W * 3 + j * 3] = (BYTE)(255 * wt);
			Output[i * W * 3 + j * 3 + 1] = (BYTE)(255 *wt);
			Output[i * W * 3 + j * 3 + 2] = (BYTE)(255 * (1 - wt));
		}
	}


	SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "Gradation.bmp");
	free(Image);
	free(Output);
}