#include "Pic.h"

//文件信息
unsigned short bfType;//文件类型,ASCII字符BM;
//像素数据
unsigned char ** ImgData;//二维数组

int PrintPicMessage(char* path)
{
	FILE* fp;
	errno_t err;
	unsigned int j, k;
	BitMapFileHeader FileHeader;
	BitMapInforHeader InfoHeader;
	RGBQuad *IpRGBQuad;
	
	OtherData  *Otherdata, *temp, *SavePreCursor;
	/*图像的读取顺序是从下到上,从左到右*/

	IpRGBQuad = (RGBQuad *)malloc(256 * sizeof(RGBQuad));//灰度图为8位的调色板数据为256个结构,1024个字节
	Otherdata = (OtherData*)malloc(sizeof(OtherData));
	SavePreCursor = Otherdata;//保存当前指针游标,目的就是保存链表头结点
	Otherdata->extradata = NULL;
	Otherdata->next = NULL;
	err = fopen_s(&fp, path, "rb");
	if (err != 0)
	{
		printf("cannot open this file");
		return 0;

	}
	//拥有1024个调色板数据

	//printf("%d\n", sizeof(FileHeader));
	//fread(&FileHeader, sizeof(FileHeader), 1, fp);//读取位图文件头
	fread(&bfType, sizeof(unsigned short), 1, fp);//读取位图文件头
	if (bfType == 0x4D42)
	{
		printf("\n\n------------------------位图文件头----------------------------------------\n\n");
		fread(&FileHeader, sizeof(FileHeader), 1, fp);//读取位图文件头


		printf("文件类型%X\n", bfType);
		//printf("%X\n", FileHeader.bfType);
		printf("文件大小(单位字节):%X\n", FileHeader.bfSize);
		printf("备用:%X\n", FileHeader.bfReserved1);
		printf("备用:%X\n", FileHeader.bfReserved2);
		printf("图像开始处的字节偏移:%X\n", FileHeader.bfOffBits);


		//读取位图信息头
		printf("\n\n-------------------------位图信息头----------------------------------------\n\n");
		fread(&InfoHeader, sizeof(InfoHeader), 1, fp);

		printf("信息头大小:%X\n", InfoHeader.biSize);
		printf("图像宽度:%d\n", InfoHeader.biWidth);
		printf("图像高度:%d\n", InfoHeader.biHeight);
		printf("位平面数:%X\n", InfoHeader.biPlanes);
		printf("每像素位数:%X\n", InfoHeader.biBitCount);
		printf("压缩类型:%X\n", InfoHeader.biCompression);
		printf("压缩图像大小的字节数:%X\n", InfoHeader.biSizeImage);
		printf("水平分辨率:%X\n", InfoHeader.biXpelspermeter);
		printf("垂直分辨率:%X\n", InfoHeader.biYpelspermeter);
		printf("使用的色彩数:%X\n", InfoHeader.biClrUsed);
		printf("重要色彩数:%X\n", InfoHeader.biClrImportant);


		//读取调色板数据

		printf("\n\n-----------------------------调色板------------------------------------------\n\n");
		printf("\t红\t绿\t蓝\n");

		fread(IpRGBQuad, sizeof(RGBQuad), 256, fp);
		for (int i = 0; i < 256; i++)
		{
			printf("\t%X\t%X\t%X\n", IpRGBQuad[i].rgbBlue, IpRGBQuad[i].rgbGreen, IpRGBQuad[i].rgbRed);
		}

		//读取像素数据

		InfoHeader.biHeight++;
		InfoHeader.biWidth += 2;
		ImgData = new unsigned char *[InfoHeader.biHeight];
		for (j = 0; j < InfoHeader.biHeight; j++)
		{
			ImgData[j] = new unsigned char[InfoHeader.biWidth];//定义像素数组ImgData[InfoHeader.biHeight][InfoHeader.biWidth]
		}

		for (j = 0; j < InfoHeader.biHeight; j++)
		{
			for (k = 0; k < InfoHeader.biWidth; k++)
			{
				fread(&ImgData[j][k], sizeof(unsigned char), 1, fp);
				//
			}

		}

		//读取其他数据

		temp = (OtherData*)malloc(sizeof(OtherData));
		while (!feof(fp))
		{
			temp = (OtherData*)malloc(sizeof(OtherData));

			fread(&temp->extradata, sizeof(unsigned char), 1, fp);

			Otherdata->next = temp;
			Otherdata = Otherdata->next;
		}
		Otherdata->next = NULL;//置末尾为空,判断条件
		Otherdata = SavePreCursor;//使Otherdata重新指向头部
	}
	fclose(fp);
	return 0;
}

int CreatGrayPic(char* data,char* path, int h,int w)
{
	FILE* fp;
	errno_t err;
	BitMapFileHeader FileHeader;
	BitMapInforHeader InfoHeader;
	memset(&FileHeader, 0, sizeof(BitMapFileHeader));
	memset(&InfoHeader, 0, sizeof(BitMapInforHeader));
	//构造灰度图的文件头
	bfType = 0x4D42;
	FileHeader.bfOffBits = (DWORD)sizeof(BitMapFileHeader) + (DWORD)sizeof(BitMapInforHeader) + sizeof(RGBQuad) * 256 + sizeof(unsigned short);
	FileHeader.bfSize = h * w + sizeof(RGBQuad) * 256 + sizeof(BitMapFileHeader) + sizeof(BitMapInforHeader) + sizeof(unsigned short);
	FileHeader.bfReserved1 = 0;
	FileHeader.bfReserved2 = 0;
	//构造灰度图的信息头
	InfoHeader.biBitCount = 8;
	InfoHeader.biSize = sizeof(BitMapInforHeader);
	InfoHeader.biHeight = h;
	InfoHeader.biWidth = w;
	InfoHeader.biPlanes = 1;
	InfoHeader.biCompression = BI_RGB;
	InfoHeader.biSizeImage = 0;
	InfoHeader.biXpelspermeter = 10012;
	InfoHeader.biYpelspermeter = 10012;
	InfoHeader.biClrImportant = 256;
	InfoHeader.biClrUsed = 256;

	//构造灰度图的调色版

	RGBQuad rgbquad[256];
	for (int iii = 0; iii < 256; iii++)
	{
		rgbquad[iii].rgbBlue = iii;
		rgbquad[iii].rgbGreen = iii;
		rgbquad[iii].rgbRed = iii;
		rgbquad[iii].rgbReserved = 0;
	}
	err = fopen_s(&fp, path, "w");
	if (err != 0)
	{
		printf("cannot creat this file");
		return 0;

	}
	fwrite(&bfType, sizeof(unsigned short), 1, fp);
	fwrite(&FileHeader, sizeof(BitMapFileHeader),1, fp);
	fwrite(&InfoHeader, sizeof(BitMapInforHeader),1, fp);
	fwrite(&rgbquad, sizeof(RGBQuad) * 256,1, fp);
	fwrite(data,1, h*w, fp);   //这里data的大小为h*w 自定义
	fclose(fp);
	return 0;
}