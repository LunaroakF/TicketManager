#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <time.h>
#include <conio.h>
#include <stdio.h>

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
	printf("该操作需要验证身份，请登录\n");
	printf("默认用户名密码都为admin\n");
	printf("用户名:");
	char username[100];
	scanf("%s", &username);
	printf("密码:");
	char password[100];
	int i = 0;
	while (1) {
		password[i] = _getch(); // 通过getch()函数获取隐藏的字符输入
		if (password[i] == '\r') break; // 当按下Enter键时结束输入
		putchar('*'); // 显示为星号
		i++;
	}
	password[i] = '\0'; // 添加字符串结尾标志'\0'
	//username 与 password用户信息
}