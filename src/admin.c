#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include"admin.h"
#include"user.h"
#include"menuView.h"
#include"control.h"
#include"issueLink.h"
#include"notary.h"

/*管理员登录*/
void adminLogin() {
	int err = 0;
	while (1) {
		printf("请输入管理员帐号：");
		char adname[20] = { 0 }, adpwd[20] = { 0 };
		getstr(adname, 20);
		printf("请输入管理员密码：");
		getPassword(adpwd);
		if (strcmp(adname, "admin") != 0 || strcmp(adpwd, "12345") != 0) {
			printf("帐号或密码错误！\n");
			err++;
			if (err > 5) {
				printf("检测到您多次输入错误，将自动为您退出登录界面");
				getchar();
				return;
			}
		}
		else break;
	}
	identifyCode();//输入验证码
	conAdm();
}

/*创建管理员头节点*/
Adm* creAdmhead() {
	Adm* admhead = (Adm*)malloc(sizeof(Adm));
	memset(admhead, 0, sizeof(Adm));
	admhead->next = admhead;
	admhead->pre = admhead;
	return admhead;
}

/*创建管理员节点*/
Adm* creAdmNode(int id, char* pwd, char prop) {
	Adm* admnode = (Adm*)malloc(sizeof(Adm));
	admnode->id = id;
	strcpy(admnode->pwd, pwd);
	admnode->property = prop;
	return admnode;
}

/*管理员主控函数*/
void conAdm() {
	Adm* admhead = creAdmhead();
	getAdmInfo(admhead);//读取普通管理员信息
	int choose = 0, isRun = 1;
	while (isRun) {
		choose = inputChoose(choose, viewAdminModeChose);
		switch (choose) {
		case 1:
			conSuperAdm(admhead);
			break;
		case 2:
			conNormalAdm(admhead);
			break;
		case 3:
			conNotary(admhead);
			break;
		case 0:
			isRun = 0;
			break;
		default:
			printf("没有该选项!\n");
			getchar();
		}
	}
	saveSpInfo(admhead);
}

/*尾插新的管理员*/
void insertbytailAdm(Adm* admhead, Adm* node) {//后期想办法改进
	assert(admhead && node);
	admhead->pre->next = node;
	node->pre = admhead->pre;
	admhead->pre = node;
	node->next = admhead;
}

/*输出管理员信息*/
void outputAdm(Adm* head) {
	assert(head);
	printf("id\t密码\t\t管理员类型\n----------------------------------\n");
	printf("%d\t%s\t\t超级管理员\n", head->id, head->pwd);
	Adm* p = head->next;
	while (p != head) {
		printf("%d\t%s\t\t", p->id, p->pwd);
		switch (p->property) {
		case 'n':printf("公证员\n"); break;
		case 'g':printf("普通管理员\n"); break;
		default:printf("该管理员信息存在问题，请及时修正！\n"); break;
		}
		p = p->next;
	}
	getchar();
}

/*添加管理员*/
void addAdmin(Adm* admhead) {
	assert(admhead);
	Adm* adm = (Adm*)malloc(sizeof(Adm));
	if (adm != NULL)
		adm->id = admhead->pre->id + 1;
	else {
		TYPE_ERROR();
	}
	printf("请输入该管理员的属性(n为公证员，g为普通管理员)：");
	int check = scanf("%c", &adm->property);
	CLEAR();
	while ((adm->property != 'n' && adm->property != 'g') || check < 1) {
		printf("输入错误，请重新输入！\n");
		printf("请输入该管理员的属性(n为公证员，g为普通管理员)：");
		check = scanf("%c", &adm->property);
		CLEAR();
	}
	printf("请初始化该管理员账号的密码：");
	getPassword(adm->pwd);
	printf("请再确认一遍该密码：");
	char repwd[20];
	getPassword(repwd);
	while (strcmp(repwd, adm->pwd) != 0) {
		printf("两遍密码输入不一致，请再次输入！");
		printf("请初始化该管理员账号的密码：");
		getPassword(adm->pwd);
		printf("请再确认一遍该密码：");
		getPassword(repwd);
	}
	insertbytailAdm(admhead, adm);
	saveAdmInfo(admhead);
	printf("*******新管理员添加成功！*********\n");
	getchar();
}

/*查找用户信息*/
void lookAcc() {
	Usr* usrhead = creUsrhead();
	getUsrinfo(usrhead);
	char ipname[20] = "\0";
	printf("请输入要查找的用户名：");
	getstr(ipname, 20);
	Usr* usr = findUsr(usrhead, ipname);
	if (usr == NULL) {
		printf("查无此人！\n");
		getchar();
		return;
	}
	else {
		outputsUsr(usr);
	}
}


/*根据账户余额排序*/
void sortAcc() {
	Usr* usrhead = creUsrhead();
	getUsrinfo(usrhead);
	printf("排序之前：\n");
	outputAllUsr(usrhead);
	int len = 0;
	Usr* p = usrhead->next;
	while (p != usrhead) {
		len++;
		p = p->next;
	}
	quickSort(usrhead->next, usrhead->pre);
	printf("排序之后：\n");
	outputAllUsr(usrhead);
	getchar();
}

/*换值four兄弟*/
void swapStrs(char *str1,char *str2) {
	char tmp[20] = {0};
	strcpy(tmp, str1);
	strcpy(str1,str2);
	strcpy(str2, tmp);
}

void swapData(float* da,float* db) {
	float tmp = 0;
	tmp = *da;
	*da = *db;
	*db = tmp;
}

void swapDataInt(int* da,int* db) {
	int tmp = 0;
	tmp = *da;
	*da = *db;
	*db = tmp;
}

void swapUsrs(Usr* pi,Usr* pj) {
	swapStrs(pi->name, pj->name);
	swapStrs(pi->pwd, pj->pwd);
	swapData(&(pi->money), &(pj->money));
	swapDataInt(&(pi->id),&(pj->id));
	swapDataInt(&(pi->islucky),&(pj->islucky));
}

/*快速排序中的分割子函数*/
Usr* partition(Usr* left,Usr* right) {
	assert(left&&right);
	Usr* pj = left;
	Usr* pi = left->pre;
	while (pj != right) {
		if (pj->money > right->money) {
			pi = pi->next;
			swapUsrs(pi, pj);
		}
		pj = pj->next;
	}
	swapUsrs(pi->next, right);
	return pi->next;
}

/*快速排序算法*/
void quickSort( Usr* start,Usr* end) {
	assert(start&&end);
	if(start==end||end->next==start||start->pre==end){
		return;
	}
	if (start != end) {
		Usr* medium = partition(start,end);
		quickSort(start,medium->pre);
		quickSort(medium->next,end);
	}
}

/*普通管理员主控函数*/
void conNormalAdm(Adm* admhead) {
	assert(admhead);
	int id = 0;
	int check = checkId(&id);
	CHECK();
	Adm* adm = findAdm(admhead, id);
	IF_ADM_NOT_EXIST();
	if (adm->property != 'g') {
		TYPE_ERROR();
	}
	char pwd[20];
	printf("请输入密码：");
	getPassword(pwd);
	if (strcmp(pwd, adm->pwd) != 0) {
		TYPE_ERROR();
	}
	int choose = 0, isRun = 1;
	while (isRun) {
		choose = inputChoose(choose, viewNormaladmin);
		switch (choose) {
		case 1:
			issueLottery();
			break;
		case 2:
			lookAcc();
			break;
		case 3:
			sortAcc();
			break;
		case 0:
			isRun = 0;
			break;
		default:
			printf("没有该选项!\n");
			getchar();
			break;
		}
	}
	adm=NULL;
	free(adm);
	saveAdmInfo(admhead);
}

/*公证员*/
void conNotary(Adm* admhead){
	assert(admhead);
	int id = 0;
	int check = checkId(&id);
	CHECK();
	Adm* adm = findAdm(admhead, id);
	IF_ADM_NOT_EXIST();
	if (adm->property != 'n') {
		TYPE_ERROR();
	}
	char pwd[20];
	printf("请输入密码：");
	getPassword(pwd);
	if (strcmp(pwd, adm->pwd) != 0) {
		TYPE_ERROR();
	}
	int choose = 0, isRun = 1;
	while (isRun) {
		choose = inputChoose(choose, viewNotary);
		switch (choose) {
		case 1:
			notaryIsu();
			break;
		case 2:
			lookPassIsu();
			break;
		case 0:
			isRun = 0;
			break;
		default:
			printf("没有该选项!\n");
			getchar();
		}
	}
	adm=NULL;
	free(adm);
}

/*超级管理员主控函数*/
void conSuperAdm(Adm* admhead) {
	getSpAdmInfo(admhead);
	printf("请输入超级管理员通行码：");
	char sppwd[20]="\0";
	getPassword(sppwd);
	if (strcmp(sppwd, admhead->pwd) != 0) {
		TYPE_ERROR();
	}
	int choose = 0, isRun = 1;
	while (isRun) {
		choose = inputChoose(choose, viewSuperadmin);
		switch (choose) {
		case 1:
			outputAdm(admhead);
			break;
		case 2:
			addAdmin(admhead);
			break;
		case 3:
			delAdmin(admhead);
			break;
		case 4:
			modiAdmin(admhead);
			break;
		case 5:
			lookAdmin(admhead);
			break;
		case 0:
			isRun = 0;
			break;
		default:
			printf("没有该选项!\n");
			getchar();
			break;
		}
	}
	saveSpInfo(admhead);
}

/*查找具体管理员的信息*/
void lookAdmin(Adm* admhead) {
	assert(admhead);
	int id = 0;
	int check = checkId(&id);
	CHECK();
	Adm* adm = findAdm(admhead, id);
	IF_ADM_NOT_EXIST();
	printf("ID:%d\n密码:%s\n类型：", adm->id, adm->pwd);
	switch (adm->property) {
		case 's':
			printf("超级管理员\n");
			break;
		case 'g':
			printf("普通管理员\n");
			break;
		case 'n':
			printf("公证员\n");
			break;
		default:
			printf("该管理员信息异常，请及时排查原因！\n");
			break;
	}
	adm=NULL;
	free(adm);
	getchar();
}

/*根据id寻找管理员*/
Adm* findAdm(Adm* admhead, int admid) {
	assert(admhead);
	if (admid == admhead->id) {
		return admhead;
	}
	Adm* p = admhead->next;
	while (p != admhead) {
		if (p->id == admid) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}


/*删除管理员*/
void delAdmin(Adm * admhead) {
	int deleid = 0;
	printf("您已进入删除管理员操作，");
	int check = checkId(&deleid);
	CHECK();
	Adm* adm = findAdm(admhead, deleid);
	if (adm == NULL || adm == admhead) {
		system("clear");
		if (adm == NULL) {
			printf("该ID不存在！");
		}
		else {
			printf("超级管理员不可删除！您可以尝试修改密码\n");
		}
		getchar();
		return;
	}
	adm->pre->next = adm->next;
	adm->next->pre = adm->pre;
	adm = NULL;
	free(adm);
	saveAdmInfo(admhead);
	printf("******删除成功！*******");
	getchar();
}

/*修改管理员信息*/
void modiadmFun(Adm * admhead, void(*func)(Adm*)) {
	assert(admhead);
	int modid = 0;
	printf("您已进入修改操作，");
	int check = checkId(&modid);
	CHECK();
	Adm* adm = findAdm(admhead, modid);
	IF_ADM_NOT_EXIST();
	func(adm);
	saveAdmInfo(admhead);
}

/*修改管理员密码*/
void modiAdmpwd(Adm * adm) {
	assert(adm);
	modiPwd(adm->pwd);
}

/*修改管理员属性*/
void modiAdmpro(Adm * adm) {
	assert(adm);
	if (adm->property == 's') {
		printf("错误原因：超管属性不可修改！\n");
		TYPE_ERROR();
	}
	printf("当前该管理员属性为：%c\n", adm->property);
	char newpro = '\0';
	while (1) {
		printf("您想要将其修改为(n代表公证员，g代表普通管理员)：");
		int check = scanf("%c", &newpro);
		CLEAR();
		if (check <= 0 || (newpro != 'n' && newpro != 'g')) {
			printf("输入错误，请重新输入！\n");
		}
		else {
			break;
		}
	}
	adm->property = newpro;
	printf("******修改成功！******");
	getchar();
}

/*修改管理员信息*/
void modiAdmin(Adm * admhead) {
	int choose = 0, isRun = 1;
	while (isRun) {
		choose = inputChoose(choose, viewModiadmin);
		switch (choose) {
		case 1:
			modiadmFun(admhead, modiAdmpwd);
			break;
		case 2:
			modiadmFun(admhead, modiAdmpro);
			break;
		case 0:
			isRun = 0;
			break;
		default:
			printf("没有该选项!\n");
			getchar();
		}
	}
	saveSpInfo(admhead);
	saveAdmInfo(admhead);
	//getchar();
}

/*防止存储超管的文本不存在,自动生成超管信息给头结点*/
/*也可在程序第一次运行时起作用*/
void getSuperifwrong(Adm * admhead) {
	admhead->id = 1001;
	strcpy(admhead->pwd, "admin_S");
	admhead->property = 's';
	printf("由于超管帐号遗失，系统自动生成新的超管帐号，初始id为1001，密码为admin_S\n");
	getchar();
}

/*将超管信息写入文件*/
void saveSpInfo(Adm * admhead) {
	assert(admhead);
	FILE* fp;
	if (NULL == (fp = fopen("superadmInfo.txt", "w"))) {
		fprintf(stderr, "Can not open file!\n");
		exit(-1);
	}
	fprintf(fp, "%d\t%s\t%c\n", 1001, admhead->pwd, 's');
	fclose(fp);
}

/*将普管信息写入文件*/
void saveAdmInfo(Adm * admhead) {
	assert(admhead);
	FILE* fp;
	if (NULL == (fp = fopen("admInfo.txt", "w"))) {
		fprintf(stderr, "Can not open file!\n");
		return;
	}
	Adm* p = admhead->next;
	while (p != admhead) {
		fprintf(fp, "%d %s %c\n", p->id, p->pwd, p->property);
		p = p->next;
	}
	fclose(fp);
}

/*读取超管信息*/
void getSpAdmInfo(Adm * admhead) {
	assert(admhead);
	char buf[100], pwd[20]="1";
	int id=1, r;
	FILE* f;
	if (NULL == (f = fopen("superadmInfo.txt", "r"))) {
		fprintf(stderr, "Can not open file!\n");
		getSuperifwrong(admhead);//有可能发生人为篡改本地文件情况
		saveSpInfo(admhead);//分配完成后立即存入本地文件
	}
	else {
		if (NULL == fgets(buf, 100, f)){
			getSuperifwrong(admhead);//有可能发生人为篡改本地文件情况
			saveSpInfo(admhead);//分配完成后立即存入本地文件
			return;	
		}
		r = sscanf(buf, "%d%20s", &id, pwd);
		if (r == 2) {
			admhead->id = id;
			admhead->property = 's';
			strcpy(admhead->pwd, pwd);
		}
		fclose(f);
	}
}

/*读取普管和公证员信息*/
void getAdmInfo(Adm * admhead) {
	assert(admhead);
	char buf[100], pwd[100], prop = '\0';
	int id, r;
	FILE* f;
	if (NULL == (f = fopen("admInfo.txt", "r"))) {
		fprintf(stderr, "Can not open file!\n");
		return;
	}
	while (1) {
		if (NULL == fgets(buf, 100, f)) break;
		if ('\n' == buf[0]) continue;
		r = sscanf(buf, "%d%20s%s", &id, pwd, &prop);
		if (r == 3) {
			Adm* node = creAdmNode(id, pwd, prop);
			insertbytailAdm(admhead, node);
		}
	}
	fclose(f);
}

