#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include "VerifyUser.h"



void OutPutWithTime(char* data)//带时间输出信息
{
	struct date {
		int tm_sec;         //秒
		int tm_min;         //分
		int tm_hour;        //时
		int tm_mday;        //日
		int tm_mon;         //月
		int tm_year;        //年
	};
	time_t currenttime;
	struct date* currentdate;
	char output[20];
	time(&currenttime);               //获取数据
	currentdate = localtime(&currenttime);    //转换为北京时间
	strftime(output, 20, "%Y-%m-%d %H:%M:%S", currentdate);  //格式化2024-01-01 12:00:00
	printf("[%s] %s\n", output,data);
}

void Welcome()
{
	printf("车票管理系统v1.0\n");
	printf("Powered by hehu\n\n");

	printf("欢迎来到车票服务平台，通过简单的命令，您可以方便地管理您的车票：\n");
	printf("您可以键入快捷键来进行所需操作:\n");
	for (int i = 0; i <18*4-3; i++)
	{
		printf("_");
	}
	printf("\n%-16s %-16s %-16s %-16s %s\n", "|录入班次信息","|浏览班次信息","|查询路线","|售票与退票","|");
	printf("%-16s %-16s %-16s %-16s %s\n", "|w", "|v", "|c", "|p", "|");
	for (int i = 0; i < 18 * 4 - 3; i++)
	{
		printf("-");
	}
	printf("\n例如我需要进入 售票与退票 功能，请输入p并回车");
	printf("\n现在，请输入您需要操作的代码:");
}


void WriteBusDataFunction()
{
	if (verifyuser())
	{
		system("cls");
		OutPutWithTime("验证成功");

	}
	else
	{
		OutPutWithTime("用户名或密码错误");
		printf("是否重试?(输入1或0):\n");
		int data;
		scanf("%d", &data);
		if (data==1)
		{
			WriteBusDataFunction();
		}
		else
		{
			system("cls");
			Welcome();
		}
	}

}