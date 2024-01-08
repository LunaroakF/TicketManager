#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "VerifyUser.h"

struct businfo
{
	char starttime[10];
	char startstation[20];
	char destination[20];
	float triplong;
	int max;
	int current;
};

void OutPutAllBusTime()
{
	struct businfo lineinfo;
	FILE* file;
	char line[100]; // 存储单行内容的字符数组
	int targetLine = 1; // 目标行号
	file = fopen("data.bin", "r"); // 打开文件进行读取操作
	fgets(line, sizeof(line), file);//跳过第一行
	printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "班次", "发车时间", "起点站", "终点站", "行车时数", "额定载量", "已订票数", "|");
	while ((fgetc(file)) != EOF)
	{
		fgets(line, sizeof(line), file); // 依次读取每一行
		char* token = strtok(line, "|");
		strcpy(lineinfo.starttime, token);
		token = strtok(NULL, "|");
		strcpy(lineinfo.startstation, token);
		token = strtok(NULL, "|");
		strcpy(lineinfo.destination, token);
		char triplong[20];
		token = strtok(NULL, "|");
		strcpy(triplong, token);
		lineinfo.triplong = atof(triplong);
		char max[10];
		token = strtok(NULL, "|");
		strcpy(max, token);
		lineinfo.max = atoi(max);
		char current[10];
		token = strtok(NULL, "|");
		strcpy(current, token);
		lineinfo.current = atoi(current);
		printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s\n", targetLine, lineinfo.starttime, lineinfo.startstation, lineinfo.destination, lineinfo.triplong, lineinfo.max, lineinfo.current, "|");
		targetLine++;
	}
	fclose(file);
}

void WriteToFileAtLine(char* filename, int lineNumber, struct businfo *newinfo) {
	FILE* file = fopen(filename, "r");
	FILE* tempFile = fopen("temp.bin", "w");

	if (file == NULL || tempFile == NULL) {
		printf("无法打开文件或创建临时文件。\n");
		exit(EXIT_FAILURE);
	}
	char buffer[1024];
	int currentLine = 1;
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		if (currentLine == lineNumber) {
			fprintf(tempFile, "%s", buffer);
			fputs("\n", tempFile); // 向文件写入信息
			fprintf(tempFile, "|%s|%s|%s|%.2f|%d|%d", newinfo->starttime, newinfo->startstation, newinfo->destination, newinfo->triplong, newinfo->max, newinfo->current);
		}
		else {
			fprintf(tempFile, "%s", buffer);
		}
		currentLine++;
	}
	fclose(file);
	fclose(tempFile);
	remove(filename);
	rename("temp.bin", filename);
}

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

void AddNewBus()
{
	FILE* file;
	char line[100]; // 存储单行内容的字符数组
	int targetLine = 1; // 目标行号
	file = fopen("data.bin", "r"); // 打开文件进行读取操作
	fgets(line, sizeof(line), file);//跳过第一行
	while ((fgetc(file)) != EOF)
	{
		fgets(line, sizeof(line), file); // 依次读取每一行
		targetLine++;
	}
	fclose(file);
	struct businfo newinfo;
	printf("请键入班车 发车时间:");
	scanf("%s", &newinfo.starttime);
	printf("请键入班车 起点站:");
	scanf("%s", &newinfo.startstation);
	printf("请键入班车 终点站:");
	scanf("%s", &newinfo.destination);
	printf("请键入班车 行程时间(小时h):");
	scanf("%f", &newinfo.triplong);
	printf("请键入班车 额定载客量:");
	scanf("%d", &newinfo.max);
	printf("请键入班车 已订票人数:");
	scanf("%d", &newinfo.current);
	printf("请确认您的班车信息(输入1同意，0舍弃):\n");
	printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "班次", "发车时间", "起点站", "终点站", "行车时数", "额定载量", "已订票数", "|");
	printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s\n", targetLine, newinfo.starttime, newinfo.startstation, newinfo.destination, newinfo.triplong, newinfo.max, newinfo.current, "|");
	int answer;
	scanf("%d", &answer);
	if (answer == 1)
	{
		WriteToFileAtLine("data.bin", targetLine, &newinfo);
		OutPutWithTime("信息已保存");
	}
}

void DelBus()
{
	
}

void Welcome()
{
	printf("车票管理系统v1.0\n");
	printf("Powered by hehu\n\n");

	printf("欢迎来到车票服务平台，通过简单的命令，您可以方便地管理您的车票：\n");
	printf("您可以键入快捷键来进行所需操作:\n");
	for (int i = 0; i <18*4+1; i++)
	{
		printf("_");
	}
	printf("\n|%-16s |%-16s |%-16s |%-16s %s\n", "录入班次信息","浏览班次信息","查询路线","售票与退票","|");
	printf("|%-16s |%-16s |%-16s |%-16s %s\n", "w", "v", "c", "p", "|");
	for (int i = 0; i < 18 * 4+1; i++)
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
		while (1)
		{

			printf("需要查询所有班次输入c，增加或删除班次输入1或0\n");
			char data[2];
			scanf("%s", &data);
			if (data[1] != '\0')
			{
				printf("指令非法，请重试:");
			}
			else
			{
				switch (data[0])
				{
				case 'c':
					OutPutWithTime("下面输出今天所有存在的班次:");
					OutPutAllBusTime();
					break;
				case '1':
					OutPutWithTime("您选择增添一班班车，请根据指引填写班车信息:");
					AddNewBus();
					break;
				case '0':
					OutPutWithTime("您选择删除一班班车，请根据下列出的所有班次输入班次序号删除(输入序号):");
					DelBus();
					break;
				default:
					break;
				}
			}
		}
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