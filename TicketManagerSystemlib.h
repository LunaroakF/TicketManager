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

int isLaterThanSystemTime(const char* inputTime) {
	// 获取当前系统时间
	time_t currentTime;
	struct tm* localTime;

	time(&currentTime);
	localTime = localtime(&currentTime);

	// 解析传入的时间字符串
	int inputHour, inputMinute;
	sscanf(inputTime, "%d:%d", &inputHour, &inputMinute);

	// 比较时间
	if (localTime->tm_hour < inputHour || (localTime->tm_hour == inputHour && localTime->tm_min < inputMinute)) {
		return 0; // 传入的时间早于系统时间
	}
	else {
		return 1; // 传入的时间晚于等于系统时间
	}
}

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
		printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s", targetLine, lineinfo.starttime, lineinfo.startstation, lineinfo.destination, lineinfo.triplong, lineinfo.max, lineinfo.current, "|");
		if (isLaterThanSystemTime(lineinfo.starttime))
		{
			printf("此班已发出");
		}
		printf("\n");
		targetLine++;
	}
	fclose(file);
}

void OutPutAllBusTimeByName(char* name)
{
	struct businfo lineinfo;
	FILE* file;
	char line[100]; // 存储单行内容的字符数组
	int targetLine = 1; // 目标行号
	file = fopen("data.bin", "r"); // 打开文件进行读取操作
	fgets(line, sizeof(line), file);//跳过第一行
	int isHave = 0;
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

		if (strcmp(lineinfo.destination,name)!=0)
		{
			targetLine++;
			continue;
		}
		isHave = 1;
		printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s", targetLine, lineinfo.starttime, lineinfo.startstation, lineinfo.destination, lineinfo.triplong, lineinfo.max, lineinfo.current, "|");
		if (isLaterThanSystemTime(lineinfo.starttime))
		{
			printf("此班已发出");
		}
		printf("\n");
		targetLine++;
	}
	fclose(file);
	if (isHave==0)
	{
		printf("未找到有效班次\n");
	}
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
			fprintf(tempFile, "|%s|%s|%s|%.1f|%d|%d", newinfo->starttime, newinfo->startstation, newinfo->destination, newinfo->triplong, newinfo->max, newinfo->current);
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

void removeEmptyLines(const char* filename) {
	FILE* inputFile, * outputFile;
	char buffer[512];

	// 打开原始文件
	inputFile = fopen(filename, "r");
	if (inputFile == NULL) {
		printf("无法打开文件 %s\n", filename);
		return;
	}

	// 创建一个临时文件
	outputFile = fopen("temp.txt", "w");
	if (outputFile == NULL) {
		printf("无法创建临时文件\n");
		fclose(inputFile);
		return;
	}

	// 逐行读取原始文件并复制到临时文件，跳过空白行
	while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
		// 判断当前行是否为空白行
		int isEmpty = 1; // 假设为空白行
		for (int i = 0; buffer[i] != '\0'; i++) {
			if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\n' && buffer[i] != '\r') {
				isEmpty = 0;
				break;
			}
		}

		// 如果不是空白行，则写入临时文件
		if (!isEmpty) {
			fputs(buffer, outputFile);
		}
	}

	// 关闭文件
	fclose(inputFile);
	fclose(outputFile);

	// 删除原始文件
	remove(filename);

	// 重命名临时文件为原始文件
	rename("temp.txt", filename);
}

void deleteLine(const char* filename, int totalLines) {
	FILE* inputFile, * outputFile;
	char buffer[512];
	int currentLine = 0;

	// 打开原始文件
	inputFile = fopen(filename, "r");
	if (inputFile == NULL) {
		printf("无法打开文件 %s\n", filename);
		return;
	}

	// 创建一个临时文件
	outputFile = fopen("temp.txt", "w");
	if (outputFile == NULL) {
		printf("无法创建临时文件\n");
		fclose(inputFile);
		return;
	}

	// 逐行读取原始文件并复制到临时文件，跳过最后一行
	while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
		currentLine++;
		if (currentLine != totalLines) {
			fputs(buffer, outputFile);
		}
	}

	// 关闭文件
	fclose(inputFile);
	fclose(outputFile);

	// 删除原始文件
	remove(filename);

	// 重命名临时文件为原始文件
	rename("temp.txt", filename);
	
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
	printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s", targetLine, newinfo.starttime, newinfo.startstation, newinfo.destination, newinfo.triplong, newinfo.max, newinfo.current, "|");
	if (isLaterThanSystemTime(newinfo.starttime))
	{
		printf("此班已发出");
	}
	printf("\n");
	int answer;
	scanf("%d", &answer);
	if (answer == 1)
	{
		WriteToFileAtLine("data.bin", targetLine, &newinfo);
		removeEmptyLines("data.bin");
		OutPutWithTime("信息已保存");
	}
}

void DelBus()
{
	printf("请输入欲删除的班车班次序号:");
	int num;
	struct businfo delinfo;
	scanf("%d", &num);
	FILE* file;
	char line[100]; // 存储单行内容的字符数组
	int targetLine = num; // 目标行号
	file = fopen("data.bin", "r"); // 打开文件进行读取操作
	fgets(line, sizeof(line), file);//跳过第一行
	while ((fgetc(file)) != EOF&&targetLine>0)
	{
		fgets(line, sizeof(line), file); // 依次读取每一行
		targetLine--;
	}
	fclose(file);

	char* token = strtok(line, "|");
	strcpy(delinfo.starttime, token);
	token = strtok(NULL, "|");
	strcpy(delinfo.startstation, token);
	token = strtok(NULL, "|");
	strcpy(delinfo.destination, token);
	char triplong[20];
	token = strtok(NULL, "|");
	strcpy(triplong, token);
	delinfo.triplong = atof(triplong);
	char max[10];
	token = strtok(NULL, "|");
	strcpy(max, token);
	delinfo.max = atoi(max);
	char current[10];
	token = strtok(NULL, "|");
	strcpy(current, token);
	delinfo.current = atoi(current);
	printf("这是你即将删除的班车信息(输入1删除，0取消):\n");
	printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "班次", "发车时间", "起点站", "终点站", "行车时数", "额定载量", "已订票数", "|");
	printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s", num, delinfo.starttime, delinfo.startstation, delinfo.destination, delinfo.triplong, delinfo.max, delinfo.current, "|");
	if (isLaterThanSystemTime(delinfo.starttime))
	{
		printf("此班已发出");
	}
	printf("\n");
	int answer;
	scanf("%d", &answer);
	if (answer==1)
	{
		deleteLine("data.bin", num+1);
	}
}

void Welcome()
{
	printf("车票管理系统v1.0\n");
	printf("Powered by NortzWolfy&Hehu\n\n");

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

			printf("需要查询所有班次输入c，增加或删除班次输入1或0，输入r返回\n");
			char data[2];
			scanf("%s", &data);
			if (data[0] == 'r')
			{
				system("cls");
				Welcome();
				return;
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
					OutPutWithTime("您选择删除一班班车，请根据下列指引删除班车信息:");
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

void SearchBusFunction()
{
	while (1)
	{
		printf("您可按班次号查询(输入1)，可按终点站查询(输入2)，输入r返回:\n");
		char answer[2];
		scanf("%s", &answer);
		if (answer[1] == '\0')
		{
			if (answer[0] == '1')
			{
				printf("请输入班次号:");
				int num;
				scanf("%d", &num);

				struct businfo currentinfo;
				FILE* file;
				char line[100]; // 存储单行内容的字符数组
				int targetLine = num; // 目标行号
				file = fopen("data.bin", "r"); // 打开文件进行读取操作
				fgets(line, sizeof(line), file);//跳过第一行
				while ((fgetc(file)) != EOF && targetLine > 0)
				{
					fgets(line, sizeof(line), file); // 依次读取每一行
					targetLine--;
				}
				fclose(file);

				char* token = strtok(line, "|");
				strcpy(currentinfo.starttime, token);
				token = strtok(NULL, "|");
				strcpy(currentinfo.startstation, token);
				token = strtok(NULL, "|");
				strcpy(currentinfo.destination, token);
				char triplong[20];
				token = strtok(NULL, "|");
				strcpy(triplong, token);
				currentinfo.triplong = atof(triplong);
				char max[10];
				token = strtok(NULL, "|");
				strcpy(max, token);
				currentinfo.max = atoi(max);
				char current[10];
				token = strtok(NULL, "|");
				strcpy(current, token);
				currentinfo.current = atoi(current);
				printf("这是您查询的班车信息:\n");
				printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "班次", "发车时间", "起点站", "终点站", "行车时数", "额定载量", "已订票数", "|");
				printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s", num, currentinfo.starttime, currentinfo.startstation, currentinfo.destination, currentinfo.triplong, currentinfo.max, currentinfo.current, "|");
				if (isLaterThanSystemTime(currentinfo.starttime))
				{
					printf("此班已发出");
				}
				printf("\n");
			}
			else if (answer[0] == '2')
			{
				printf("请输入终点站名称:");
				char num[20];
				scanf("%s", &num);
				OutPutAllBusTimeByName(num);

			}
			else if (answer[0] == 'r')
			{
				return;
			}
		}
	}
}