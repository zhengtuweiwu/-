#include <iostream>
#include <string.h>
#include "Pic.h"
using namespace std;
//读取txt文件对每一行sql语句进行转换
#define MAX_LINE 500
#define H 20 //图片高度
#define W 20 //图片宽度
#pragma warning(disable:4996)
int SQLconvertPIC(char *path,char *output)
{
	int p=0;//行计数
	char buf[MAX_LINE];  /*缓冲区*/
	FILE *fp;            /*文件指针*/
	int len;             /*行字符个数*/
	int err;
	err = fopen_s(&fp, path, "r");
	if (err != 0)
	{
		printf("cannot open this file");
		return 0;

	}
	//p = 0;
	char data[H*W];
	char filePath[200];//目标生成全路径
	char filename[25];//目标文件名
	char str[1];
	//itoa(255, str, 10);//int转字符串
	while (fgets(buf, MAX_LINE, fp) != NULL)
	{
		
		memset(data, 0, H*W);
		memset(filePath, 0, 200);
		memset(filename, 0, 25);
		len = strlen(buf);
		buf[len - 1] = '\0';  /*去掉换行符*/
		//printf("%s %d \n", buf, len - 1);
		 
		//如果字符串不够，要补位
		if (len - 1< H*W)
		{
			memcpy(data, buf, len - 1);
			for (int ii = len - 1; ii < H*W; ii++)
			{
				strcat(data, "0");//不够位置的取0字符
			}

		}
		else
		{
			memcpy(data, buf, H*W);
		}
		p++;
		itoa(p, filename, 10);//int转字符串
		strcpy(filePath, output);
		strcat(filePath, "\\");//补全生成文件路径
		strcat(filePath, filename);//补全生成文件路径
		strcat(filePath, ".bmp");//补全生成文件路径
		CreatGrayPic(data, filePath,H,W);
		if (p == 1000)
		{
			break;
		}
	}
	fclose(fp);
	return 0;
}
int main()
{
	//char pathR[100] = "E:\\c-workspace\\GrayPic\\test.bmp";
	//PrintPicMessage(pathR);
	char path[100] = "E:\\c-workspace\\GrayPic\\sql-test\\6.txt";
	//char path[100] = "E:\\c-workspace\\GrayPic\\Last-Step.txt";
	char output[100] = "E:\\c-workspace\\GrayPic\\test";
	SQLconvertPIC(path,output);
	system("pause");
	return 0;

}


