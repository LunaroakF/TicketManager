﻿#pragma once
#define _CRT_SECURE_NO_WARNINGS 1//编译器禁用安全警告
#include <time.h>//获取系统时间与比较
#include <stdio.h>//标准输入输出
#include <stdlib.h>//清空控制台要用system("cls");
#include <string.h>//用strcmp比较字符串之类的东西对字符串修改
#include "VerifyUser.h"//验证管理员身份

// 车次基本数据结构
struct businfo
{
	char starttime[10];//发车时间
	char startstation[20];//起点站
	char destination[20];//终点站
	float triplong;//行车时数
	int max;//额定载量
	int current;//已订票数
};

// 比较传入时间(传入数据如"8:00")与系统时间，传入的时间早于系统时间返回0(已发班)，晚于系统时间返回1(未发班)
int isLaterThanSystemTime(char* inputTime) {
	//获取当前系统时间
	time_t currentTime;
	struct tm* localTime;
	time(&currentTime);
	localTime = localtime(&currentTime);//转换为本地时间

	// 解析传入的时间字符串，从inputTime字符串中以':'分割开来，格式化存储数据方便后面比较
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
// 输出所有车次表 或 进入 [浏览班次信息] 功能
void OutPutAllBusTime()
{
	struct businfo lineinfo;
	FILE* file;
	char line[100]; //存储单行内容的字符数组
	int targetLine = 1; //目标行号
	file = fopen("data.bin", "r"); //打开文件进行读取操作
	fgets(line, sizeof(line), file);//跳过第一行
	printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "班次", "发车时间", "起点站", "终点站", "行车时数", "额定载量", "已订票数", "|");//表格标题
	while ((fgetc(file)) != EOF)//输出直到读不到东西
	{
		fgets(line, sizeof(line), file);//依次读取每一行
		//依次以'|'分隔读来的一行存储到lineinfo
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
			printf("此班已发出 ");
		}
		if (lineinfo.current>=lineinfo.max)
		{
			printf("已售罄");
		}
		printf("\n");
		targetLine++;
	}
	fclose(file);
}
// 按目的地输出所有匹配的车次表(传入数据如"十堰")
void OutPutAllBusTimeByName(char* name)
{
	struct businfo lineinfo;
	FILE* file;
	char line[100];//存储单行内容的字符数组
	int targetLine = 1;//目标行号
	file = fopen("data.bin", "r");//打开文件进行读取操作
	fgets(line, sizeof(line), file);//跳过第一行
	int isHave = 0;//目前找到了目标车次了吗(0=没有 1=有且>=1)
	printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "班次", "发车时间", "起点站", "终点站", "行车时数", "额定载量", "已订票数", "|");//表格标题
	while ((fgetc(file)) != EOF)
	{
		fgets(line, sizeof(line), file);//依次读取每一行
		//依次以'|'分隔读来的一行存储到lineinfo
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
		if (lineinfo.current >= lineinfo.max)
		{
			printf("已售罄");
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
// 将 结构体businfo 中的信息写入到文件指定行，进行强制替换的写法(传入数据如[ "data.bin", 2, &businfo])
void WriteToFileAtLine(char* filename, int lineNumber, struct businfo *newinfo) {
	//用临时文件覆盖的方法
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
			fputs("\n", tempFile); // 向缓存文件写入信息
			fprintf(tempFile, "|%s|%s|%s|%.1f|%d|%d\n", newinfo->starttime, newinfo->startstation, newinfo->destination, newinfo->triplong, newinfo->max, newinfo->current);
		}
		else {
			fprintf(tempFile, "%s", buffer);//将数据文件中的当前行丢到缓存文件里
		}
		currentLine++;
	}
	fclose(file);
	fclose(tempFile);
	remove(filename);
	rename("temp.bin", filename);
}
// 文件写入后可能会出现空白行，影响下次数据的写入，每次修改完文件后运行一次，删除所有的空白行(传入数据如"data.bin")
void removeEmptyLines(char* filename) {
	FILE* inputFile, * outputFile;
	char buffer[512];

	//打开原始文件
	inputFile = fopen(filename, "r");
	if (inputFile == NULL) {
		printf("无法打开文件 %s\n", filename);
		return;
	}

	//创建一个临时文件
	outputFile = fopen("temp.txt", "w");
	if (outputFile == NULL) {
		printf("无法创建临时文件\n");
		fclose(inputFile);
		return;
	}

	//逐行读取原始文件并复制到临时文件，跳过空白行
	while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
		//判断当前行是否为空白行
		int isEmpty = 1; //假设为空白行
		for (int i = 0; buffer[i] != '\0'; i++) {
			if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\n' && buffer[i] != '\r') {
				isEmpty = 0;
				break;
			}
		}

		//如果不是空白行，则写入临时文件
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
// 删除指定行的内容(传入数据如["data.bin", 1])
void deleteLine(char* filename, int line) {
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
		if (currentLine != line) {
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
// 输出带时间的日志，自带回车(传入数据如"害怕") (输出数据如"[2024-01-09 18:36:51] 害怕")
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
	time(&currenttime);//获取数据
	currentdate = localtime(&currenttime);//转换为本地时间
	strftime(output, 20, "%Y-%m-%d %H:%M:%S", currentdate);  //格式化2024-01-01 12:00:00
	printf("[%s] %s\n", output,data);
}
// 已经验证管理员身份后调用的添加车次交互
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
	printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "班次", "发车时间", "起点站", "终点站", "行车时数", "额定载量", "已订票数", "|");
	printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s", targetLine, newinfo.starttime, newinfo.startstation, newinfo.destination, newinfo.triplong, newinfo.max, newinfo.current, "|");
	if (isLaterThanSystemTime(newinfo.starttime))
	{
		printf("此班已发出");
	}
	if (newinfo.current >= newinfo.max)
	{
		printf("已售罄");
	}
	printf("\n");
	printf("请确认您的班车信息(输入1同意，0舍弃):\n");
	int answer;
	scanf("%d", &answer);
	if (answer == 1)
	{
		WriteToFileAtLine("data.bin", targetLine, &newinfo);
		removeEmptyLines("data.bin");
		OutPutWithTime("信息已保存");
	}
}
// 已经验证管理员身份后调用的删除车次交互
void DelBus()
{
	printf("请输入欲删除的班车班次序号:");
	int num;
	struct businfo delinfo;
	scanf("%d", &num);
	if (num <= 0)
	{
		printf("对不起，请输入正确的班次\n\n");
		DelBus();
		return;
	}
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
	if (targetLine != 0)
	{
		printf("对不起，未找到该班次\n\n");
		DelBus();
		return;
	}
	//依次以'|'分隔读来的一行存储到delinfo
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
	if (delinfo.current >= delinfo.max)
	{
		printf("已售罄");
	}
	printf("\n");
	int answer;
	scanf("%d", &answer);
	if (answer==1)
	{
		deleteLine("data.bin", num+1);
		OutPutWithTime("信息已保存");
	}
}
// 新手教程
void Welcome()
{
	printf("车票管理系统v1.1\n");
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
// 进入 [录入班次信息] 功能
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
// 进入 [查询路线] 功能
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
					fgets(line, sizeof(line), file); //依次读取每一行
					targetLine--;
				}
				fclose(file);
				//依次以'|'分隔读来的一行存储到currentinfo
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
				if (currentinfo.current >= currentinfo.max)
				{
					printf("已售罄");
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
// 进入 [售票与退票] 功能
void TradeTicket()
{
	printf("无账号登录系统的购票退票，仅限娱乐\n");
	while (1)
	{
		fflush(stdin);
		printf("您可输入c查询今日所有班次，输入1来购票，0来退票，r退出:");
		char answer[2];
		scanf("%s", &answer);
		if (answer[1] == '\0')
		{
			if (answer[0] == 'c')
			{
				OutPutAllBusTime();
			}
			else if (answer[0] == '1')
			{
				printf("您选择了购买车票，请输入您要购买的班次:");
				int num;
				scanf("%d", &num);
				if (num <= 0)
				{
					printf("对不起，请输入正确的班次\n\n");
					continue;
				}
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
				if (targetLine!=0)
				{
					printf("对不起，未找到该班次\n\n");
					continue;
				}
				//依次以'|'分隔读来的一行存储到currentinfo
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
				printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "班次", "发车时间", "起点站", "终点站", "行车时数", "额定载量", "已订票数", "|");
				printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s", num, currentinfo.starttime, currentinfo.startstation, currentinfo.destination, currentinfo.triplong, currentinfo.max, currentinfo.current, "|");
				if (isLaterThanSystemTime(currentinfo.starttime))
				{
					printf("此班已发出");
				}
				if (currentinfo.current >= currentinfo.max)
				{
					printf("已售罄");
				}
				printf("\n");
				printf("这是你即将购买的班车信息(输入您要购买的车票数量，0取消):\n");
				int as;
				scanf("%d", &as);
				if (as>0)
				{
					if (currentinfo.current+as>currentinfo.max)
					{
						printf("对不起，该班次车票不足\n\n");
					}
					else if(isLaterThanSystemTime(currentinfo.starttime))
					{
						printf("对不起，此班已发出\n\n");
					}
					else
					{
						currentinfo.current+=as;
						deleteLine("data.bin", num+1);
						WriteToFileAtLine("data.bin", num, &currentinfo);
						removeEmptyLines("data.bin");
						char cachedata[200];
						sprintf(cachedata, "恭喜您，车票已成功预定 %d 张,请您及时前往车站凭二代身份证取票", as);
						OutPutWithTime(cachedata);
					}
				}
				else
				{
					printf("对不起，请输入正确的票数\n\n");
				}
			}
			else if (answer[0] == '0')
			{
				printf("您选择了退票，请输入您欲退票的班次:");
				int num;
				scanf("%d", &num);
				if (num<=0)
				{
					printf("对不起，请输入正确的班次\n\n");
					continue;
				}
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
				if (targetLine != 0)
				{
					printf("对不起，未找到该班次\n\n");
					continue;
				}
				//依次以'|'分隔读来的一行存储到currentinfo
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
				printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "班次", "发车时间", "起点站", "终点站", "行车时数", "额定载量", "已订票数", "|");
				printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s", num, currentinfo.starttime, currentinfo.startstation, currentinfo.destination, currentinfo.triplong, currentinfo.max, currentinfo.current, "|");
				if (isLaterThanSystemTime(currentinfo.starttime))
				{
					printf("此班已发出");
				}
				if (currentinfo.current >= currentinfo.max)
				{
					printf("已售罄");
				}
				printf("\n");
				printf("这是你即将退票的班车信息(请输入退票票数，0取消):\n");
				int as;
				scanf("%d", &as);
				if (as >0)
				{
					if (currentinfo.current == 0)
					{
						printf("对不起，该班次车票为零\n\n");
					}
					else if (isLaterThanSystemTime(currentinfo.starttime))
					{
						printf("对不起，此班已发出\n\n");
					}
					else
					{
						currentinfo.current-=as;
						deleteLine("data.bin", num + 1);
						WriteToFileAtLine("data.bin", num, &currentinfo);
						removeEmptyLines("data.bin");
						OutPutWithTime("退票成功");
					}
				}
				else
				{
					printf("对不起，请输入正确的票数\n\n");
				}
			}
			else if(answer[0]=='r')
			{
				return;
			}
		}
	}
}
