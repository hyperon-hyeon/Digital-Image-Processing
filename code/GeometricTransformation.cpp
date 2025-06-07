#pragma warning(disable:4996)
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <math.h>

void SaveBMPFile(BITMAPFILEHEADER hf, BITMAPINFOHEADER hInfo, RGBQUAD* hRGB, BYTE* Output, int W, int H, const char* FileName) {
	FILE* fp = fopen(FileName, "wb");
	fwrite(&hf, sizeof(BYTE), sizeof(BITMAPFILEHEADER), fp);
	fwrite(&hInfo, sizeof(BYTE), sizeof(BITMAPINFOHEADER), fp);
	fwrite(hRGB, sizeof(RGBQUAD), 256, fp);
	fwrite(Output, sizeof(BYTE), W * H, fp);
}

void Translation(BYTE* Image, BYTE* Output,int W, int H, int Tx, int Ty) {
	Ty *= -1; //Y축 반대로 되어 있음
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			if( ( i+Ty<H&& i+Ty >=0 ) && ( j+Tx<W && j+Tx>=0 ))
				Output[(i + Ty) * W + (j + Tx)] = Image[i * W + j];
		}
	}
}

void Scaling(BYTE* Image, BYTE* Output, int W, int H, int SFx, int SFy) {
	int tmpX, tmpY;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			tmpX = (int)(j / SFx); //순방향 사상: tmpX=j*SFx;
			tmpY = (int)(i / SFy); //순방향 사상: tmpY=i*SFy;
			if (tmpX < W && tmpY < H) {
				Output[i * W + j] = Image[tmpY * W + tmpX]; //역방향 사상
				//Output[tmpY*W+j]=Image[i*W+j] <- 순방향 사상
			}
		}
	}
}

void Rotation(BYTE* Image, BYTE* Output, int W, int H, int Angle) {
	double Radian = Angle * 3.141592 / 180.0;
	int tmpX, tmpY;
	int centerX = (int)W / 2, centerY = (int)H / 2;
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			tmpX = (int)(cos(Radian) * (j-centerX) - sin(Radian) * (i-centerY))+centerX;
			tmpY = (int)(sin(Radian) * (j - centerX) + cos(Radian) * (i - centerY))+centerY;
			if (tmpX >= 0 && tmpX < W && tmpY >= 0 && tmpY < H) {
				Output[i * W + j] = Image[tmpY * W + tmpX];
			}
		}
	}
}

void VerticalFlip(BYTE* Image, BYTE* Output,int W, int H) {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			Output[(H - i - 1) * W + j] = Image[i * W + j];
		}
	}
}

void HorizontalFlip(BYTE* Image, BYTE* Output, int W, int H) {
	for (int i = 0; i < H; i++) {
		for (int j = 0; j < W; j++) {
			Output[i * W + (W - j - 1)] = Image[i * W + j];
		}
	}
}

int main() {
	BITMAPFILEHEADER hf;
	BITMAPINFOHEADER hInfo;
	RGBQUAD hRGB[256];
	FILE* fp;
	fp = fopen("Lenna.bmp", "rb");
	if (fp == NULL) {
		printf("File is not found\n");
		return -1;
	}
	fread(&hf, sizeof(BITMAPFILEHEADER), 1, fp);
	fread(&hInfo, sizeof(BITMAPINFOHEADER), 1, fp);
	fread(&hRGB, sizeof(RGBQUAD), 256, fp);
	int ImgSize = hInfo.biWidth * hInfo.biHeight;
	int W = hInfo.biWidth, H = hInfo.biHeight;
	BYTE* Image = (BYTE*)malloc(ImgSize);
	BYTE* Output = (BYTE*)malloc(ImgSize);
	fread(Image, sizeof(BYTE), ImgSize, fp);
	fclose(fp);

	//Translation(Image,Output,W,H,30,30);
	//Scaling(Image, Output, W, H, 2,2);
	//Rotation(Image, Output, W, H, 60);
	//VerticalFlip(Image, Output, W, H);
	HorizontalFlip(Image, Output, W, H);

	SaveBMPFile(hf, hInfo, hRGB, Output, W, H, "horizontalLenna.bmp");
	free(Image);
	free(Output);
	return 0;
}