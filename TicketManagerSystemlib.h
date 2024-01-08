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
	// ��ȡ��ǰϵͳʱ��
	time_t currentTime;
	struct tm* localTime;

	time(&currentTime);
	localTime = localtime(&currentTime);

	// ���������ʱ���ַ���
	int inputHour, inputMinute;
	sscanf(inputTime, "%d:%d", &inputHour, &inputMinute);

	// �Ƚ�ʱ��
	if (localTime->tm_hour < inputHour || (localTime->tm_hour == inputHour && localTime->tm_min < inputMinute)) {
		return 0; // �����ʱ������ϵͳʱ��
	}
	else {
		return 1; // �����ʱ�����ڵ���ϵͳʱ��
	}
}

void OutPutAllBusTime()
{
	struct businfo lineinfo;
	FILE* file;
	char line[100]; // �洢�������ݵ��ַ�����
	int targetLine = 1; // Ŀ���к�
	file = fopen("data.bin", "r"); // ���ļ����ж�ȡ����
	fgets(line, sizeof(line), file);//������һ��
	printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "���", "����ʱ��", "���վ", "�յ�վ", "�г�ʱ��", "�����", "�Ѷ�Ʊ��", "|");
	while ((fgetc(file)) != EOF)
	{
		fgets(line, sizeof(line), file); // ���ζ�ȡÿһ��
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
			printf("�˰��ѷ���");
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
	char line[100]; // �洢�������ݵ��ַ�����
	int targetLine = 1; // Ŀ���к�
	file = fopen("data.bin", "r"); // ���ļ����ж�ȡ����
	fgets(line, sizeof(line), file);//������һ��
	int isHave = 0;
	printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "���", "����ʱ��", "���վ", "�յ�վ", "�г�ʱ��", "�����", "�Ѷ�Ʊ��", "|");
	while ((fgetc(file)) != EOF)
	{
		fgets(line, sizeof(line), file); // ���ζ�ȡÿһ��

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
			printf("�˰��ѷ���");
		}
		printf("\n");
		targetLine++;
	}
	fclose(file);
	if (isHave==0)
	{
		printf("δ�ҵ���Ч���\n");
	}
}

void WriteToFileAtLine(char* filename, int lineNumber, struct businfo *newinfo) {
	FILE* file = fopen(filename, "r");
	FILE* tempFile = fopen("temp.bin", "w");

	if (file == NULL || tempFile == NULL) {
		printf("�޷����ļ��򴴽���ʱ�ļ���\n");
		exit(EXIT_FAILURE);
	}
	char buffer[1024];
	int currentLine = 1;
	while (fgets(buffer, sizeof(buffer), file) != NULL) {
		if (currentLine == lineNumber) {
			fprintf(tempFile, "%s", buffer);
			fputs("\n", tempFile); // ���ļ�д����Ϣ
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

	// ��ԭʼ�ļ�
	inputFile = fopen(filename, "r");
	if (inputFile == NULL) {
		printf("�޷����ļ� %s\n", filename);
		return;
	}

	// ����һ����ʱ�ļ�
	outputFile = fopen("temp.txt", "w");
	if (outputFile == NULL) {
		printf("�޷�������ʱ�ļ�\n");
		fclose(inputFile);
		return;
	}

	// ���ж�ȡԭʼ�ļ������Ƶ���ʱ�ļ��������հ���
	while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
		// �жϵ�ǰ���Ƿ�Ϊ�հ���
		int isEmpty = 1; // ����Ϊ�հ���
		for (int i = 0; buffer[i] != '\0'; i++) {
			if (buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\n' && buffer[i] != '\r') {
				isEmpty = 0;
				break;
			}
		}

		// ������ǿհ��У���д����ʱ�ļ�
		if (!isEmpty) {
			fputs(buffer, outputFile);
		}
	}

	// �ر��ļ�
	fclose(inputFile);
	fclose(outputFile);

	// ɾ��ԭʼ�ļ�
	remove(filename);

	// ��������ʱ�ļ�Ϊԭʼ�ļ�
	rename("temp.txt", filename);
}

void deleteLine(const char* filename, int totalLines) {
	FILE* inputFile, * outputFile;
	char buffer[512];
	int currentLine = 0;

	// ��ԭʼ�ļ�
	inputFile = fopen(filename, "r");
	if (inputFile == NULL) {
		printf("�޷����ļ� %s\n", filename);
		return;
	}

	// ����һ����ʱ�ļ�
	outputFile = fopen("temp.txt", "w");
	if (outputFile == NULL) {
		printf("�޷�������ʱ�ļ�\n");
		fclose(inputFile);
		return;
	}

	// ���ж�ȡԭʼ�ļ������Ƶ���ʱ�ļ����������һ��
	while (fgets(buffer, sizeof(buffer), inputFile) != NULL) {
		currentLine++;
		if (currentLine != totalLines) {
			fputs(buffer, outputFile);
		}
	}

	// �ر��ļ�
	fclose(inputFile);
	fclose(outputFile);

	// ɾ��ԭʼ�ļ�
	remove(filename);

	// ��������ʱ�ļ�Ϊԭʼ�ļ�
	rename("temp.txt", filename);
	
}

void OutPutWithTime(char* data)//��ʱ�������Ϣ
{
	struct date {
		int tm_sec;         //��
		int tm_min;         //��
		int tm_hour;        //ʱ
		int tm_mday;        //��
		int tm_mon;         //��
		int tm_year;        //��
	};
	time_t currenttime;
	struct date* currentdate;
	char output[20];
	time(&currenttime);               //��ȡ����
	currentdate = localtime(&currenttime);    //ת��Ϊ����ʱ��
	strftime(output, 20, "%Y-%m-%d %H:%M:%S", currentdate);  //��ʽ��2024-01-01 12:00:00
	printf("[%s] %s\n", output,data);
}

void AddNewBus()
{
	FILE* file;
	char line[100]; // �洢�������ݵ��ַ�����
	int targetLine = 1; // Ŀ���к�
	file = fopen("data.bin", "r"); // ���ļ����ж�ȡ����
	fgets(line, sizeof(line), file);//������һ��
	while ((fgetc(file)) != EOF)
	{
		fgets(line, sizeof(line), file); // ���ζ�ȡÿһ��
		targetLine++;
	}
	fclose(file);
	struct businfo newinfo;
	printf("�����೵ ����ʱ��:");
	scanf("%s", &newinfo.starttime);
	printf("�����೵ ���վ:");
	scanf("%s", &newinfo.startstation);
	printf("�����೵ �յ�վ:");
	scanf("%s", &newinfo.destination);
	printf("�����೵ �г�ʱ��(Сʱh):");
	scanf("%f", &newinfo.triplong);
	printf("�����೵ ��ؿ���:");
	scanf("%d", &newinfo.max);
	printf("�����೵ �Ѷ�Ʊ����:");
	scanf("%d", &newinfo.current);
	printf("��ȷ�����İ೵��Ϣ(����1ͬ�⣬0����):\n");
	printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "���", "����ʱ��", "���վ", "�յ�վ", "�г�ʱ��", "�����", "�Ѷ�Ʊ��", "|");
	printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s", targetLine, newinfo.starttime, newinfo.startstation, newinfo.destination, newinfo.triplong, newinfo.max, newinfo.current, "|");
	if (isLaterThanSystemTime(newinfo.starttime))
	{
		printf("�˰��ѷ���");
	}
	printf("\n");
	int answer;
	scanf("%d", &answer);
	if (answer == 1)
	{
		WriteToFileAtLine("data.bin", targetLine, &newinfo);
		removeEmptyLines("data.bin");
		OutPutWithTime("��Ϣ�ѱ���");
	}
}

void DelBus()
{
	printf("��������ɾ���İ೵������:");
	int num;
	struct businfo delinfo;
	scanf("%d", &num);
	FILE* file;
	char line[100]; // �洢�������ݵ��ַ�����
	int targetLine = num; // Ŀ���к�
	file = fopen("data.bin", "r"); // ���ļ����ж�ȡ����
	fgets(line, sizeof(line), file);//������һ��
	while ((fgetc(file)) != EOF&&targetLine>0)
	{
		fgets(line, sizeof(line), file); // ���ζ�ȡÿһ��
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
	printf("�����㼴��ɾ���İ೵��Ϣ(����1ɾ����0ȡ��):\n");
	printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "���", "����ʱ��", "���վ", "�յ�վ", "�г�ʱ��", "�����", "�Ѷ�Ʊ��", "|");
	printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s", num, delinfo.starttime, delinfo.startstation, delinfo.destination, delinfo.triplong, delinfo.max, delinfo.current, "|");
	if (isLaterThanSystemTime(delinfo.starttime))
	{
		printf("�˰��ѷ���");
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
	printf("��Ʊ����ϵͳv1.0\n");
	printf("Powered by NortzWolfy&Hehu\n\n");

	printf("��ӭ������Ʊ����ƽ̨��ͨ���򵥵���������Է���ع������ĳ�Ʊ��\n");
	printf("�����Լ����ݼ��������������:\n");
	for (int i = 0; i <18*4+1; i++)
	{
		printf("_");
	}
	printf("\n|%-16s |%-16s |%-16s |%-16s %s\n", "¼������Ϣ","��������Ϣ","��ѯ·��","��Ʊ����Ʊ","|");
	printf("|%-16s |%-16s |%-16s |%-16s %s\n", "w", "v", "c", "p", "|");
	for (int i = 0; i < 18 * 4+1; i++)
	{
		printf("-");
	}
	printf("\n��������Ҫ���� ��Ʊ����Ʊ ���ܣ�������p���س�");
	printf("\n���ڣ�����������Ҫ�����Ĵ���:");
}

void WriteBusDataFunction()
{
	if (verifyuser())
	{
		system("cls");
		OutPutWithTime("��֤�ɹ�");
		while (1)
		{

			printf("��Ҫ��ѯ���а������c�����ӻ�ɾ���������1��0������r����\n");
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
					OutPutWithTime("��������������д��ڵİ��:");
					OutPutAllBusTime();
					break;
				case '1':
					OutPutWithTime("��ѡ������һ��೵�������ָ����д�೵��Ϣ:");
					AddNewBus();
					break;
				case '0':
					OutPutWithTime("��ѡ��ɾ��һ��೵�����������ָ��ɾ���೵��Ϣ:");
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
		OutPutWithTime("�û������������");
		printf("�Ƿ�����?(����1��0):\n");
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
		printf("���ɰ���κŲ�ѯ(����1)���ɰ��յ�վ��ѯ(����2)������r����:\n");
		char answer[2];
		scanf("%s", &answer);
		if (answer[1] == '\0')
		{
			if (answer[0] == '1')
			{
				printf("�������κ�:");
				int num;
				scanf("%d", &num);

				struct businfo currentinfo;
				FILE* file;
				char line[100]; // �洢�������ݵ��ַ�����
				int targetLine = num; // Ŀ���к�
				file = fopen("data.bin", "r"); // ���ļ����ж�ȡ����
				fgets(line, sizeof(line), file);//������һ��
				while ((fgetc(file)) != EOF && targetLine > 0)
				{
					fgets(line, sizeof(line), file); // ���ζ�ȡÿһ��
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
				printf("��������ѯ�İ೵��Ϣ:\n");
				printf("|%-6s |%-10s |%-8s |%-8s |%-10s |%-6s |%-6s %s\n", "���", "����ʱ��", "���վ", "�յ�վ", "�г�ʱ��", "�����", "�Ѷ�Ʊ��", "|");
				printf("|%-6d |%-10s |%-8s |%-8s |%-10.1lf |%-8d |%-8d %s", num, currentinfo.starttime, currentinfo.startstation, currentinfo.destination, currentinfo.triplong, currentinfo.max, currentinfo.current, "|");
				if (isLaterThanSystemTime(currentinfo.starttime))
				{
					printf("�˰��ѷ���");
				}
				printf("\n");
			}
			else if (answer[0] == '2')
			{
				printf("�������յ�վ����:");
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