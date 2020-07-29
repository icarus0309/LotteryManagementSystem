#include<stdio.h>
#include"user.h"
#include"menuView.h"
#include"admin.h"
#include"control.h"

int main() {
	int choose = 0, flag = 0, isRun = 1, check = 0;
	while (isRun) {
		do {
			flag = 0;
			viewUsr();
			check = scanf("%d", &choose);
			if (getchar() != '\n' || check < 1) {
				flag = 1;
				CLEAR();
				printf("输入不合法！\n");
			}
		} while (flag);
		switch (choose) {
		case 1:
			usrLogin();
			break;
		case 2:
			usrRegist();
			break;
		case 3:
			adminLogin();
			break;
		case 0:
			isRun = 0;
			break;
		default:
			printf("没有该选项!\n");
		}
	}
	return 0;
}
