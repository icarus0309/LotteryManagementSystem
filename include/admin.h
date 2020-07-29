#ifndef __ADMIN_H__
#define __ADMIN_H__
typedef struct admin {
	int id;
	char pwd[20];//二级管理员密码，用以区分不同属性管理员的登录
	char property;//管理员分为3种属性：s-超管，g-普管，n-公证员
	struct admin* pre;
	struct admin* next;
}Adm;

void adminLogin();
void conSuperAdm(Adm* admhead);
void conAdm();
void insertbytailAdm(Adm * usrhead, Adm * node);
int checkId(int* id);//判定输入id是否为整型
int inputChoose(int choose,void(*viewfunc)());//control
/*超管*/
void outputAdm(Adm*);
void addAdmin(Adm*);
void delAdmin();
void modiAdmin(Adm*);
void lookAdmin(Adm*admhead);
void lookAllPriceInfo();
void getSpAdmInfo(Adm* admhead);
void getSuperifwrong(Adm* admhead);//防止超管文件被删除
void saveSpInfo(Adm* admhead);
void modiadmFun(Adm* admhead,void(*func)(Adm*));//修改管理员信息
void modiAdmpro(Adm* );//修改管理员属性
void modiAdmpwd(Adm* );//修改管理员密码
/*普管*/
void conNormalAdm(Adm* admhead);
void getAdmInfo(Adm* admhead);
void saveAdmInfo(Adm*admhead);
Adm* findAdm(Adm* admhead, int admid);//根据id寻找管理员
void lookAdm(Adm* admhead) ;//查找具体管理员的信息
void lookAcc();//查找用户信息
void sortAcc();//排序用户
/*公证员*/
void conNotary(Adm* admhead);


#endif
