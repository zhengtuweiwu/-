#ifndef PCH_H
#define PCH_H

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include <windows.h>


//位图文件头   
typedef struct {
	unsigned long bfSize;//文件大小
	unsigned short bfReserved1;//备用
	unsigned short bfReserved2;//备用
	unsigned long bfOffBits;//图像开始处的字节偏移

}BitMapFileHeader;

//文图信息头
typedef struct {
	unsigned long biSize;//信息头大小
	unsigned long biWidth;//图像宽度,以像素为单位
	unsigned long biHeight;//图像高度,以像素为单位
	unsigned short biPlanes;//位平面数,为1;
	unsigned short biBitCount;//每像素位数,1,4,8,24
	unsigned long biCompression;//压缩类型,0为非压缩
	unsigned long biSizeImage;//压缩图像大小
	unsigned long biXpelspermeter;//水平分辨率
	unsigned long biYpelspermeter;//垂直分辨率
	unsigned long biClrUsed;//使用的色彩数
	unsigned long biClrImportant;//重要色彩数
}BitMapInforHeader;

typedef struct
{
	unsigned char rgbBlue; //该颜色的蓝色分量  
	unsigned char rgbGreen; //该颜色的绿色分量  
	unsigned char rgbRed; //该颜色的红色分量  
	unsigned char rgbReserved; //保留值  
} RGBQuad;

//其他数据
typedef struct OtherData
{
	unsigned char extradata;
	struct OtherData *next;

}OtherData;

int PrintPicMessage(char* path);
int CreatGrayPic(char* data, char* path, int h, int w);

#endif 
