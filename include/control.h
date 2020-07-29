#ifndef __CONTROL_H__
#define __CONTROL_H__

#define CLEAR()  while(getchar()!= '\n')

#define CHECK() if(check<=0){ \
	printf("输入不合规范！\n");	\
	return; \
} 
#define IF_ADM_NOT_EXIST() if (adm == NULL) {\
		system("clear"); \
		printf("该ID不存在！"); \
		getchar(); \
		return; \
}
#define TYPE_ERROR() printf("错误！自动返回上一级\n");\
		getchar();\
		return

/*修改密码*/
void modiPwd(char* nodepwd);

/*获得字符串*/
void getstr(char s[], int l);


#endif
