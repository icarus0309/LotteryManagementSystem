#ifndef __USER_H__
#define __USER_H__


typedef struct User {
	int id;//唯一
	char name[20];
	char pwd[20];
	float money;
	int islucky;//显示一次后立刻归零
	struct User* pre;
	struct User* next;
}Usr;

int getch();
void getPassword(char* pcPWD);
void getName(Usr* head,char* name,char* flag);
int identifyCode(void);
Usr* creUsrhead();
Usr* creUsrnode(int id,char* name, char* pwd,float money,int islucky);
void insertbytail(Usr * usrhead, Usr * node);
int checkRpeatname(Usr* head, char* name);
void usrRegist();
void usrLogin();
void getUsrinfo(Usr* head);
void saveUsrinfo(Usr* head);
void conUsr(Usr* head,Usr* usr);
void outputsUsr(Usr* usr);
void Recharge(Usr* head,Usr* usr);
void modiInfo(Usr*head,Usr* usr);
void modiName(Usr*head,Usr* usr);
void modiUsrPwd(Usr* usrhead,Usr* usr);
int cancelAcc(Usr* usrhead,Usr* usr);
Usr* findUsr(Usr * usrhead, char* ipname);
Usr* findUsrbyId(Usr* usrhead,int usrid);
void quickSort( Usr* start,Usr* end);
Usr* partition(Usr* left,Usr* right);
void getMoney(Usr* usr);
void suprise(Usr* usr);
void swapStrs(char *str1,char *str2);//换值
void swapData(float* da,float* db);
void swapDataInt(int* da,int* db);
void swapUsrs(Usr* pi,Usr* pj) ;
Usr* partition(Usr* left,Usr* right);//快速排序中的分割子函数
void quickSort( Usr* start,Usr* end);//快速排序算法
void lookLuckyRecord(Usr* usr);
void lookBuyed(Usr* usr);
void gamble(Usr* usr);
void outputAllUsr(Usr* usrhead);

#endif
