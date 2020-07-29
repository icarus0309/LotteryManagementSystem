#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h> 
#include<time.h>
#include<math.h>
#include<termios.h> 
#include<assert.h>
#include"user.h"
#include"control.h"

/*用来在每个模块的主控函数中显示界面的函数*/
int inputChoose(int choose, void(*viewfunc)()) {
	int flag = 0, check = 0;
	do {
		flag = 0;
		viewfunc();
		check = scanf("%d", &choose);
		if (getchar() != '\n' || check < 1) {
			flag = 1;
			CLEAR();
			printf("输入不合法！\n");
			getchar();
		}
	} while (flag);
	return choose;
}

/*修改密码*/
void modiPwd(char* nodepwd){
	printf("请输入原密码：");
	int err=0;
	char oldpwd[20],newpwd[20],renewpwd[20];
	getPassword(oldpwd);
	if(strcmp(oldpwd,nodepwd)==0){
		while(1){
			printf("请输入新密码：");
			getPassword(newpwd);
			printf("请再确认一遍：");
			getPassword(renewpwd);
			if(strcmp(newpwd,renewpwd)==0){
				strcpy(nodepwd,newpwd);
				printf("\n*******密码修改成功!*********\n");
				getchar();				
				break;			
			}	
			else{
				err++;
				if(err>5){
					printf("已经检测到您多次修改失败，是否退出修改？(y)");			
					if(getchar()=='y'){
						CLEAR();
						break;
					}	
				}
				printf("两次输入不一致，请重新输入！\n\n");
			}
		}
	}
	else{
		printf("原密码不正确！退出修改！\n");	
		getchar();
	}
}

/*获取字符串*/
void getstr(char s[], int l) {
	fgets(s, l, stdin);
	int len = strlen(s) - 1;
	if (s[len] == '\n'){
		s[len] = '\0';
	}
	else CLEAR();
}

/*隐藏输入*/
int getch(){
	int c = 0;
	struct termios org_opts, new_opts;
	int res = 0;
	res = tcgetattr(STDIN_FILENO, &org_opts);
	assert(res == 0);
	memcpy(&new_opts, &org_opts, sizeof(new_opts));
	new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
	c = getchar();
	res = tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
	assert(res == 0);
	return c;
}

/*获取密码*/
void getPassword(char* pcPWD){
	int ch = 0;
	int i = 0;
	for (i = 0; i < 20;){
		ch = getch();
		if (ch == -1){ //当无键入时，跳出当前循环继续下一个循环
			continue;
		}
		//127在键盘上是delete  8代表退格键backspace
		if ((ch == 127 || ch == 8) && i > 0){ //当有键入时，按键delete或backspace时，删除一位
			putchar('\b');	//这里是删除一个，通过输出回撤符 /b，回撤一格，
			putchar(' ');	//再显示空格符把刚才的*给盖住，
			putchar('\b');	//然后再 回撤一格等待录入。
			i--;			//减少一位
		}
		else if (ch == 10 || ch == 13){ //当键入的为换行时
			pcPWD[i] = '\0';//将字符串以'\0'结尾
			putchar('\n');
			return;
		}
		else if (i == 0 && (ch == 127 || ch == 8)){//当无键入时，按键delete或backspace时，产生警报
			printf("\a");
		}
		else if (i < 19){ //当键入的数少于19位时每键入一个数打印一个星号覆盖
			pcPWD[i] = (char)ch;
			putchar('*');
			i++;
		}
		else{ //当键入的数超过19位时，再次键入产生响铃
			printf("\a");//响铃
		}
	}
}

/*
	function:验证码
	return:正确返回1，否则返回0
*/
int identifyCode(void) {
	char array[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	char a[5] = "";
	char b[5] = "";
	while (1) {
		//得到验证码
		srand(time(NULL));
		a[0] = array[rand() % 26];
		a[1] = array[rand() % 26 + 26];
		a[2] = array[rand() % 10 + 52];
		a[3] = array[rand() % 10 + 52];
		a[4] = '\0';
		printf("验证码:");
		printf("%s\n", a);
		printf("请输入验证码（Esc强制退回）：");
		scanf("%5s", b);
		CLEAR();
		if (strcmp(a, b) != 0) {
			printf("验证码错误，请重新输入！\n");
			if (b[0] == 27)	return 0;
		}
		else {
			return 1;
		}
	}
}


/*判断输入ID是否合法*/
int checkId(int* id) {
	int err = 0, check = 0;
	while (check <= 0) {
		printf("请输入操作对象的ID：");
		check = scanf("%d", id);
		CLEAR();
		if (check > 0) {
			break;
		}
		else {
			printf("请勿输入非法字符！您还有%d次机会\n", 3 - err);
			err++;
		}
		if (err > 3) {
			return check;
		}
	}
	return check;
}

