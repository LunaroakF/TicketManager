#pragma once
#define _CRT_SECURE_NO_WARNINGS 1//���������ð�ȫ����
#include <stdio.h>//��׼�������
#include <string.h>//��strcat�ں��ַ���֮��Ķ������ַ����޸�

// �Ƚϴ�����˺��������ļ��е��Ƿ�һ��
int checkinfo(char* username, char* password)
{
	FILE* file; // �����ļ�ָ�����
	char buffer[1024]; // ���ÿ�ζ�ȡ�����ַ���
	strcat(username, "|");
	strcat(username, password);
	strcat(username, "\n");//�����˺�������ڵ�һ�� ĩβ���лس������ԶԴ�����˺�����ϲ���ĩβ�ӻس���'\n'����strcmp��Ƚ�
	file = fopen("data.bin", "r"); 
	if (file == NULL)
	{
		return -1;
	}
	fgets(buffer, sizeof(buffer), file);
	fclose(file);
	return strcmp(buffer, username);
}
// ��֤����Ա��ݵ���
int verifyuser()
{
	printf("�ò�����Ҫ��֤�߼���ݣ����¼\n");
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

	if (checkinfo(username, password) == 0)
	{
		printf("\n");
		//OutPutWithTime("��֤�ɹ�");
		return 1;
	}
	else
	{
		printf("\n");
		//OutPutWithTime("�û������������");
		return 0;
	}
}