#pragma once
#define _CRT_SECURE_NO_WARNINGS 1//编译器禁用安全警告
#include <stdio.h>//标准输入输出
#include <string.h>//用strcat融合字符串之类的东西对字符串修改

// 比较传入的账号密码与文件中的是否一致
int checkinfo(char* username, char* password)
{
	FILE* file; // 定义文件指针变量
	char buffer[1024]; // 存放每次读取到的字符串
	strcat(username, "|");
	strcat(username, password);
	strcat(username, "\n");//由于账号密码存在第一行 末尾会有回车，所以对传入的账号密码合并后末尾加回车符'\n'再用strcmp相比较
	file = fopen("data.bin", "r"); 
	if (file == NULL)
	{
		return -1;
	}
	fgets(buffer, sizeof(buffer), file);
	fclose(file);
	return strcmp(buffer, username);
}
// 验证管理员身份调用
int verifyuser()
{
	printf("该操作需要验证高级身份，请登录\n");
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

	if (checkinfo(username, password) == 0)
	{
		printf("\n");
		//OutPutWithTime("验证成功");
		return 1;
	}
	else
	{
		printf("\n");
		//OutPutWithTime("用户名或密码错误");
		return 0;
	}
}