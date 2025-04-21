#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void swap(BYTE* a, BYTE* b) {
	BYTE temp = *a;
	*a = *b;
	*b = temp;
}

BYTE Median(BYTE* arr, int size) {

	for (int i = 0; i < size - 1; i++) {
		for (int j = 1; j < size; j++) {
			if (arr[i] > arr[j]) swap(&arr[i], &arr[j]);
		}
	}
	return arr[size / 2];
}

int main() {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE* fp;

	fp = fopen("lenna_impulse.bmp", "rb");
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

	//Image process

	int Length = 5;
	int Margin = Length / 2;
	int WSize = Length * Length;
	BYTE* temp = (BYTE*)malloc(sizeof(BYTE)*WSize);

	for (int i = Margin; i < H - Margin; i++) {
		for (int j = Margin; j < W - Margin; j++) {
			for(int m = -Margin; m <= Margin; m++) {
				for (int n = -Margin; n <= Margin; n++) {
					temp[(m + Margin) * Length + n + Margin] = Image[(i + m) * W + j + n];
				}
			}
			Output[i * W + j] = Median(temp, WSize);
		}
	}
	

	fp = fopen("Median_img.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	free(Image);
	free(Output);

	return 0;
}