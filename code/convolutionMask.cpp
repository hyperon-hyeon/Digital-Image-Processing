#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

void AverConv(BYTE* Img, BYTE* Out, int W, int H) {
	
	double kernel[3][3] = {
	0.1111, 0.1111, 0.1111,
	0.1111, 0.1111, 0.1111,
	0.1111, 0.1111, 0.1111
	};

	double sumProduct = 0.0;

	for (int i = 1; i < W - 1; i++) {
		for (int j = 1; j < H - 1; j++) {
			for (int m = -1; m <= 1; m++) {
				for (int n = -1; n <= 1; n++) {
					sumProduct += Img[(i + m) * W + j + n]*kernel[m+1][n+1];
				}
			}
			Out[i * W + j] = (BYTE)sumProduct;
			sumProduct = 0.0;
		}
	}
}

void LaplacianConv(BYTE* Img, BYTE* Out, int W, int H) {
	double kernel[3][3] = {
	-1.0,-1.0, -1.0,
	-1.0,8.0,-1.0,
	-1.0,-1.0,-1.0
	};
	double sumProduct = 0.0;

	for (int i = 1; i < W-1; i++) {
		for (int j = 1; j < H-1; j++) {
			for (int m = -1; m <= 1; m++) {
				for (int n = -1; n <= 1; n++) {
					sumProduct += Img[(i + m) * W + j + n] * kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = abs((long)sumProduct / 8);
			sumProduct = 0.0;
		}
	}
}

void Prewitt_X(BYTE* Img, BYTE* Out, int W, int H) { 
	double kernel[3][3] = {
	-1.0,0.0,1.0,
	-2.0,0.0,2.0,
	-1.0,0.0,1.0
	};
	double sumProduct = 0.0;

	for (int i = 1; i < W - 1; i++) {
		for (int j = 1; j < H - 1; j++) {
			for (int m = -1; m <= 1; m++) {
				for (int n = -1; n <= 1; n++) {
					sumProduct += Img[(i + m) * W + (j + n)] * kernel[m + 1][n + 1];
				}
			}
			Out[i * W + j] = (abs)((long)sumProduct / 4);
			sumProduct = 0.0;
		}
	}
}	

int main() {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE* fp;

	fp = fopen("LENNA.bmp", "rb");
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

	//AverConv(Image, Output, W, H);
	//LaplacianConv(Image, Output, W, H);
	Prewitt_X(Image, Output, W, H);

	fp = fopen("PrewittX_img.bmp", "wb");
	fwrite(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fwrite(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	free(Image);
	free(Output);

	return 0;
}