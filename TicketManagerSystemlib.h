#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#include <time.h>
#include <conio.h>
#include <stdio.h>

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

void Welcome()
{
	printf("��Ʊ����ϵͳv1.0\n");
	printf("Powered by hehu\n\n");

	printf("��ӭ������Ʊ����ƽ̨��ͨ���򵥵���������Է���ع������ĳ�Ʊ��\n");
	printf("�����Լ����ݼ��������������:\n");
	for (int i = 0; i <18*4-3; i++)
	{
		printf("_");
	}
	printf("\n%-16s %-16s %-16s %-16s %s\n", "|¼������Ϣ","|��������Ϣ","|��ѯ·��","|��Ʊ����Ʊ","|");
	printf("%-16s %-16s %-16s %-16s %s\n", "|w", "|v", "|c", "|p", "|");
	for (int i = 0; i < 18 * 4 - 3; i++)
	{
		printf("-");
	}
	printf("\n��������Ҫ���� ��Ʊ����Ʊ ���ܣ�������p���س�");
	printf("\n���ڣ�����������Ҫ�����Ĵ���:");
}

void WriteBusDataFunction()
{
	printf("�ò�����Ҫ��֤��ݣ����¼\n");
	printf("Ĭ���û������붼Ϊadmin\n");
	printf("�û���:");
	char username[100];
	scanf("%s", &username);
	printf("����:");
	char password[100];
	int i = 0;
	while (1) {
		password[i] = _getch(); // ͨ��getch()������ȡ���ص��ַ�����
		if (password[i] == '\r') break; // ������Enter��ʱ��������
		putchar('*'); // ��ʾΪ�Ǻ�
		i++;
	}
	password[i] = '\0'; // ����ַ�����β��־'\0'
	//username �� password�û���Ϣ
}