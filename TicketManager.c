#define _CRT_SECURE_NO_WARNINGS 1//编译器禁用安全警告
#include <stdio.h>//标准输入输出
#include "TicketManagerSystemlib.h"//自定义头文件，存放车票系统所需大部分函数功能
#include <stdlib.h>//清空控制台要用system("cls");

//等待用户在主菜单选择功能，调用该函数前一般与 "TicketManagerSystemlib.h" 中的 void Welcome() 一起调用
void MainMenu()
{
	while (1)
	{
		char order[2];
		scanf("%s", &order);
		if (order[1] == '\0')
		{
			switch (order[0])
			{
			case 'w':
				system("cls");
				OutPutWithTime("您选择了功能 [录入班次信息] \n");
				WriteBusDataFunction();

				system("cls");
				Welcome();
				break;
			case 'v':
				system("cls");
				OutPutWithTime("您选择了功能 [浏览班次信息] \n");
				OutPutAllBusTime();
				printf("按回车键返回...");
				getchar();
				getchar();

				system("cls");
				Welcome();
				break;
			case 'c':
				system("cls");
				OutPutWithTime("您选择了功能 [查询路线] \n");
				SearchBusFunction();

				system("cls");
				Welcome();
				break;
			case 'p':
				system("cls");
				OutPutWithTime("您选择了功能 [售票与退票] \n");
				TradeTicket();

				system("cls");
				Welcome();
				break;
			default:
				printf("指令非法，请重试:");
				break;
			}
		}
		else
		{
			printf("指令非法，请重试:");
		}
	}
}
//程序入口
int main()
{
	Welcome();
	MainMenu();
}