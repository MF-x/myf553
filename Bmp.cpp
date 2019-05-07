#include "stdafx.h"
#include <stdio.h>
#include <math.h>

BITMAPINFO *lpBitsInfo = NULL;
int isGray = FALSE;
int H[256] = {0, };
int showHistogram;

int checkGray() {

	if (lpBitsInfo->bmiHeader.biBitCount != 8) {
		return FALSE;
	}
	
	int flag = TRUE;
	BYTE R, G, B;
	for (int i = 0; i < 256; i++) {
		R = lpBitsInfo->bmiColors[i].rgbRed;
		G = lpBitsInfo->bmiColors[i].rgbGreen;
		B = lpBitsInfo->bmiColors[i].rgbBlue;
		if (R != G || R != B || G != B) {
			flag = FALSE;
			break;
		}
	}

	return flag;
}

// ��ȡͼƬ
BOOL loadBmpFile(char * bmpFileName) {

	// ����ͼƬ
	FILE *fp = fopen(bmpFileName,"rb");

	// ��ȡʧ��
	if (fp == NULL)
		return 2;
	
	// ��ȡ�ļ�ͷ
	BITMAPFILEHEADER *bf = (BITMAPFILEHEADER *)malloc(sizeof(BITMAPFILEHEADER));
	fread(bf, sizeof(BITMAPFILEHEADER), 1, fp);
	
	// ����bmp�ļ�
	if (bf->bfType != 0x4d42) {
		// not a bit map file
		return 3;
	}

	// ��ȡ��Ϣͷ
	BITMAPINFOHEADER *bi = (BITMAPINFOHEADER *)malloc(sizeof(BITMAPINFOHEADER));
	fread(bi, sizeof(BITMAPINFOHEADER), 1, fp);


	// �����ɫ������
	int numOfColors = 0;
	if (bi->biBitCount != 24) {
		if (bi->biClrUsed == 0) {
			numOfColors = (int)pow(2, bi->biBitCount);
		}else {
			numOfColors = bi->biClrUsed;
		}
	}

	//����ÿһ�е�����ռ�����ֽ�
	int lineBytes = (bi->biWidth * bi->biBitCount + 31) / 32 * 4;

	//����ʵ��ͼ������ռ�õ��ֽ���
	int imgSize = lineBytes * bi->biHeight;

	// ������ļ�ͷ��Ĵ�С
	int size = sizeof(BITMAPINFOHEADER) + numOfColors * sizeof(RGBQUAD) + imgSize;

	// ΪlpBitsInfo����ռ�
	free(lpBitsInfo);		// ��free����ֹ�ڴ�й©
	lpBitsInfo = (LPBITMAPINFO)malloc(size);

	// ����ʧ��
	if(lpBitsInfo == NULL){
		return 4;
	}

	// ���ļ�ָ�������Ϣͷ
	fseek(fp, sizeof(BITMAPFILEHEADER), SEEK_SET);

	// ����lpBitsInfo����
	fread((char *)lpBitsInfo, size, 1, fp);

	// �޸�Ϊ��ȷ��biClrUsed
	lpBitsInfo->bmiHeader.biClrUsed = numOfColors;

	// �ͷ��ڴ�
	free(bf);
	free(bi);
	fclose(fp);

	isGray = checkGray();
	showHistogram = FALSE;
	return TRUE;
}

// ͼ��ҶȻ�
void gray() {

	// �������ļ���Size
	int lineBytes = (lpBitsInfo->bmiHeader.biWidth * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	int imgSize = lineBytes * lpBitsInfo->bmiHeader.biHeight;

	int size = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * 256 + imgSize;

	// ����ռ�
	BITMAPINFO *newLpBitsInfo = (LPBITMAPINFO)malloc(size);

	// ������Ϣͷ
	memcpy(&newLpBitsInfo->bmiHeader, &lpBitsInfo->bmiHeader, sizeof(BITMAPINFOHEADER));

	// �޸���Ϣͷ�в�����Ϣ
	newLpBitsInfo->bmiHeader.biBitCount = 8;
	newLpBitsInfo->bmiHeader.biClrUsed = 256;
	
	
	// ���õ�ɫ��
	int i ,j;

	for(i = 0; i < 256; i++) {
		newLpBitsInfo->bmiColors[i].rgbBlue = i;
		newLpBitsInfo->bmiColors[i].rgbGreen = i;
		newLpBitsInfo->bmiColors[i].rgbRed = i;
		newLpBitsInfo->bmiColors[i].rgbReserved = 0;
	}

	// ת��ʵ����ɫ��Ϣ
	BYTE *lpBits = (BYTE *)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	BYTE *newLpBits = (BYTE *)&newLpBitsInfo->bmiColors[newLpBitsInfo->bmiHeader.biClrUsed];
	
	for(i = 0, j = 0; i < imgSize; i+=3, j++) {
		newLpBits[j] = (lpBits[i] + lpBits[i + 1] + lpBits[i + 2]) / 3;
	}

	// ����ָ��
	free(lpBitsInfo);
	lpBitsInfo = newLpBitsInfo;

	isGray = TRUE;
}

void pixel(int x, int y, char *str) {

	int h = lpBitsInfo->bmiHeader.biHeight;
	int lineBytes = (lpBitsInfo->bmiHeader.biWidth * lpBitsInfo->bmiHeader.biBitCount + 31)/32 * 4;
	BYTE *lpBits = (BYTE *)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	BYTE *pixel, p;
	int r, g, b;

	switch(lpBitsInfo->bmiHeader.biBitCount) {
		case 1:
			pixel = &lpBits[lineBytes * (h - x - 1) + y / 8];
			p = *pixel & (1 << (7 - y % 8));
			if(p == 0) {
				r = g = b = 0;
			}else {
				r = g = b = 255;
			}
			break;
		case 4:
			pixel = &lpBits[lineBytes * (h - x - 1) + y / 2];
			if(y % 2 == 0) {
				p = *pixel >> 4;
			}else {
				p = *pixel & 0xf;
			}
			r = lpBitsInfo->bmiColors[p].rgbRed;
			g = lpBitsInfo->bmiColors[p].rgbGreen;
			b = lpBitsInfo->bmiColors[p].rgbBlue;
			break;
		case 8:
			pixel = &lpBits[lineBytes * (h - x - 1) + y];
			r = lpBitsInfo->bmiColors[*pixel].rgbRed;
			g = lpBitsInfo->bmiColors[*pixel].rgbGreen;
			b = lpBitsInfo->bmiColors[*pixel].rgbBlue;
			break;
		case 24:
			pixel = &lpBits[lineBytes * (h - x - 1) + y * 3];
			r = pixel[2];
			g = pixel[1];
			b = pixel[0];
			break;
		default:
			return;
	}
	sprintf(str, "RGB( %3d, %3d, %3d)", r, g, b);
}

void histogram() {
	
	int lineBytes = (lpBitsInfo->bmiHeader.biWidth * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	int imgSize = lineBytes * lpBitsInfo->bmiHeader.biHeight;

	BYTE *lpBits = (BYTE *)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	int i;
	for (i = 0; i < 256; i++) {
		H[i] = 0;
	}

	for (i = 0; i < imgSize; i++) {
		H[lpBits[i]]++;
	}

}

void linearTrans(double a, int b){
	
	int lineBytes = (lpBitsInfo->bmiHeader.biWidth * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	int imgSize = lineBytes * lpBitsInfo->bmiHeader.biHeight;

	BYTE *lpBits = (BYTE *)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];

	double pixel;
	for (int i = 0; i < imgSize; i++) {
		pixel = lpBits[i] * a + b +0.5;
		if(pixel > 255) {
			pixel = 255;
		}else if(pixel < 0) {
			pixel = 0;
		}
		lpBits[i] = (BYTE)pixel;
	}

	histogram();
}

void equalize() {

	int lineBytes = (lpBitsInfo->bmiHeader.biWidth * lpBitsInfo->bmiHeader.biBitCount + 31) / 32 * 4;
	int imgSize = lineBytes * lpBitsInfo->bmiHeader.biHeight;

	BYTE *lpBits = (BYTE *)&lpBitsInfo->bmiColors[lpBitsInfo->bmiHeader.biClrUsed];
	int i;

	histogram();

	double cH[256];
	cH[0] = 1.0 * H[0] / imgSize;
	for(i = 1; i < 256; i++) {
		cH[i] = 1.0 * H[i] / imgSize + cH[i - 1];
	}

	int D[255];
	for(i = 0; i < 256; i++) {
		D[i] = (int)(cH[i] * 255 + 0.5);
	}


	for(i = 0; i < imgSize; i++) {
		lpBits[i] = D[lpBits[i]];
	}

	histogram();
}
