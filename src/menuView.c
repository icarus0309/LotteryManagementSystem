#include<stdio.h>
#include<stdlib.h>
#include"menuView.h"

void viewUsr() {
	system("clear");
	printf("**********欢迎来到博彩俱乐部***********\n");
	printf("*             [1]用户登录             *\n");
	printf("*             [2]用户注册             *\n");
	printf("*             [3]管理员登录           *\n");
	printf("*             [0]退出                 *\n");
	printf("***************************************");
	printf("\n\n请输入：");
}

void viewUsrlogined(int isPoor) {
	system("clear");
	printf("***************************************\n");
	printf("*           [1]查看个人信息           *\n");
	printf("*           [2]修改个人信息           *\n");
	printf("*           [3]注销个人信息           *\n");
	printf("*           [4]充值                   *\n");
	printf("*           [5]查询中奖记录           *\n");
	printf("*           [6]买票                   *\n");
	printf("*           [7]查询购票记录           *\n");	
	printf("*           [8]猜拳游戏               *\n");
	printf("*           [9]提现                   *\n");	
	printf("*           [0]返回上一级             *\n");
	printf("***************************************");
	if(isPoor){
		printf("\n温馨提示：您当前余额已经不足5元了哦！\n");//余额不足时提醒
	}
	printf("\n\n请输入：");
}

void viewModiusr(){
	system("clear");
	printf("***********欢迎来到博彩俱乐部**********\n");
	printf("*            [1]修改用户名称          *\n");
	printf("*            [2]修改用户密码          *\n");
	printf("*            [0]返回上一级            *\n");
	printf("***************************************");
	printf("\n\n请输入：");
}

void viewSuperadmin() {
	system("clear");
	printf("**********欢迎进入超级管理员界面*******\n");
	printf("*           [1]打印管理员列表         *\n");
	printf("*           [2]增加管理员             *\n");
	printf("*           [3]删除管理员             *\n");
	printf("*           [4]修改管理员             *\n");
	printf("*           [5]查看管理员             *\n");
	printf("*           [0]返回上一级 	      *\n");
	printf("***************************************");
	printf("\n\n请输入：");
}

void viewNormaladmin() {
	system("clear");
	printf("**********欢迎进入管理员界面***********\n");
	printf("            [1]发行彩票            \n");
	printf("            [2]按用户帐号查询       \n");
	printf("            [3]按用户余额排序       \n");
	printf("            [0]返回上一级	          \n");
	printf("***************************************");
	printf("\n\n请输入：");
}

void viewNotary() {
	system("clear");
	printf("***********欢迎进入公证员界面**********\n");
	printf("            [1]开奖                \n");
	printf("            [2]查看历史发行         \n");
	printf("            [0]返回上一级	          \n");
	printf("***************************************");
	printf("\n\n请输入：");
}

void viewAdminModeChose(){
	system("clear");
	printf("*************请选择管理员模式**************\n");
	printf("            [1]超级管理员登录           \n");
	printf("            [2]普通管理员登录           \n");
	printf("            [3]公证员登录              \n");
	printf("            [0]返回上一级              \n");
	printf("*******************************************");
	printf("\n\n请输入：");
}

void viewModiadmin(){
	system("clear");
	printf("*************请选择管理员模式**************\n");
	printf("            [1]修改管理员密码         \n");
	printf("            [2]修改管理员属性         \n");
	printf("            [0]返回上一级              \n");
	printf("*******************************************");
	printf("\n\n请输入：");
}
