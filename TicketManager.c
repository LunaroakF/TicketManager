#include <stdio.h>
#include "TicketManagerSystemlib.h"
#include <stdlib.h>
#define _CRT_SECURE_NO_WARNINGS 1

char cwd[1024];

int main()
{
	getcwd(cwd, sizeof(cwd));//��ȡ��������Ŀ¼
	Welcome();

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
				OutPutWithTime("��ѡ���˹��� [¼������Ϣ] \n");
				WriteBusDataFunction();
				break;
			case 'v':
				system("cls");
				OutPutWithTime("��ѡ���˹��� [��������Ϣ] \n");

				break;
			case 'c':
				system("cls");
				OutPutWithTime("��ѡ���˹��� [��ѯ·��] \n");
				break;
			case 'p':
				system("cls");
				OutPutWithTime("��ѡ���˹��� [��Ʊ����Ʊ] \n");
				break;
			default:
				printf("ָ��Ƿ���������:");
				break;
			}
		}
		else
		{
			printf("ָ��Ƿ���������:");
		}
	}
}