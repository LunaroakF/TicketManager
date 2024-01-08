#include <stdio.h>
#include "TicketManagerSystemlib.h"
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS 1

char cwd[1024];

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

int main()
{
	getcwd(cwd, sizeof(cwd));//获取程序运行目录
	Welcome();
	MainMenu();
}