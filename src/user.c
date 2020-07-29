#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include"user.h"
#include"menuView.h"
#include"control.h"
#include"issueLink.h"
#include"notary.h"
#include"admin.h"

/*创建用户头节点*/
Usr* creUsrhead() {
	Usr* usrhead = (Usr*)malloc(sizeof(Usr));
	assert(usrhead);
	memset(usrhead, 0, sizeof(Usr));
	usrhead->id = 101;
	usrhead->pre = usrhead;
	usrhead->next = usrhead;
	usrhead->money = 0;//用usrhead->money存储所有用户充值的金额
	return usrhead;
}

/*创建用户节点*/
Usr* creUsrnode(int id, char* name, char* pwd,float money, int islucky) {
	Usr* node = (Usr*)malloc(sizeof(Usr));
	assert(node);
	node->id=id;
	strcpy(node->name, name);
	strcpy(node->pwd, pwd);
	node->money = money;
	node->islucky=islucky;
	return node;
}

/*用户链表尾插*/
void insertbytail(Usr * usrhead, Usr * node) {
	assert(usrhead && node);
	usrhead->pre->next = node;
	node->pre = usrhead->pre;
	usrhead->pre = node;
	node->next = usrhead;
}

/*检测是否重复*/
int checkRpeatname(Usr * usrhead, char* name) {
	assert(usrhead);
	Usr* p = usrhead->next;
	while (p != usrhead) {
		if (strcmp(p->name, name) == 0) {
			return 1;
		}
		p = p->next;
	}
	p=NULL;
	free(p);
	return 0;
}

/*注册姓名，并防止重复*/
void getName(Usr * usrhead, char* name, char* flag) {
	int err = 0;
	while (1) {
		printf("请输入用户名(不多于20个字符)：");
		getstr(name, 20);
		if (checkRpeatname(usrhead, name)) {//有时间的话写个哈希函数
			printf("该名称已被注册！\n");
		}
		else break;
		err++;
		if (err > 5) {
			printf("检测到当前失败次数过多，是否退出注册？(y确认，其余任意键继续)");
			scanf("%c", flag);
			CLEAR();
			if (*flag == 'y'){
				break;
			}
		}
	}
}

/*用户注册*/
void usrRegist() {
	Usr* usrhead = creUsrhead();
	getUsrinfo(usrhead);
	char name[20], flag = '\0';
	getName(usrhead, name, &flag);
	if (flag == 'y') {
		return;//退出注册，注册失败
	}
	char pwd[20], repwd[20];
	while (1) {
		printf("请输入密码(不多于20个字符):");
		getPassword(pwd);
		printf("请确认一遍密码:");
		getPassword(repwd);
		if (strcmp(repwd, pwd) == 0) {
			break;
		}
		else {
			printf("两遍密码不一致！\n\n");
		}
	}
	int id=(++usrhead->id);
	Usr* node = creUsrnode(id,name, pwd,0,0);
	insertbytail(usrhead, node);
	saveUsrinfo(usrhead);
	printf("*******注册成功！******\n");
	node=NULL;
	usrhead=NULL;
	free(node);
	free(usrhead);
	getchar();
}

/*通过账户ID查找用户*/
Usr* findUsrbyId(Usr* usrhead,int usrid){
	assert(usrhead);
	Usr* p = usrhead->next;
	while (p != usrhead) {
		if (p->id==usrid) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}

/*通过账户名查找用户*/
Usr* findUsr(Usr * usrhead, char* ipname) {
	assert(usrhead);
	if (ipname == NULL) {
		printf("请勿输入空名！");
		return NULL;
	}
	Usr* p = usrhead->next;
	while (p != usrhead) {
		if (strcmp(p->name, ipname) == 0) {
			return p;
		}
		p = p->next;
	}
	p=NULL;
	free(p);
	return NULL;
}

/*打印用户信息*/
void outputsUsr(Usr * usr) {
	assert(usr);
	printf("\n***************\nID\t%d\n账户名\t%s\n密码\t%s\n余额\t%.2f\n***************\n",\
			 usr->id,usr->name, usr->pwd, usr->money);
	printf("\n回车继续～\n");
	getchar();
}


/*打印所有用户*/
void outputAllUsr(Usr* usrhead){

	printf("+-------------------------------+\n\
|\t账户名\t     余额/元\t|\n+-------------------------------+\n");
	//新添加用户一定要将ID从最大的往上加一
	Usr* usr=usrhead->next;
	int len=0,i=0;
	while(usr!=usrhead){
		printf("|");
		len=20-strlen(usr->name);		
		for(i=0;i<len/2;i++){
			printf(" ");
		}
		printf("%s",usr->name);
		for(i=0;i<len/2;i++){
			printf(" ");
		}
		if(len%2==1){
			printf(" ");
		}
		printf("%.2f\t|\n",usr->money);
		
		usr=usr->next;
	}
	printf("+-------------------------------+\n");
}

/*用户登录*/
void usrLogin() {
	Usr* usrhead = creUsrhead();
	getUsrinfo(usrhead);
	system("clear");
	Usr* usr = NULL;
	int err = 0;
	while (usr == NULL || strcmp(usr->name, "admin") == 0) {
		//找到的账户为空，或为管理员帐号时，提示不合法
		if (err > 0) {
			printf("检测到该账户名不存在或不可使用！\n");
		}
		printf("请输入账户名：");
		char ipname[20];
		getstr(ipname, 20);
		if (strcmp(ipname, "\0") == 0){
			return;//输入为空，则自动返回上一层
		}
		usr = findUsr(usrhead, ipname);
		err++;
	}
	char pwd[100] = "\0";
	err = 0;
	while (strcmp(pwd, usr->pwd) != 0) {
		if (err > 0) {
			printf("密码错误！\n");
		}
		printf("请输入密码：");
		getPassword(pwd);
		err++;
		if (err > 4) {
			printf("检测到您已经输错五次密码，将退出登录操作\n");
			getchar();
			return;
		}
	}
	identifyCode();
	conUsr(usrhead, usr);
	usrhead=NULL;
	usr=NULL;
	free(usr);
	free(usrhead);
}

/*加载用户信息*/
void getUsrinfo(Usr * usrhead) {
	assert(usrhead);
	FILE* f;
	if (NULL == (f = fopen("usrInfo.txt", "r"))) {
		fprintf(stderr, "Can not open file!\n");
		exit(-1);
	}
	char buf[100], name[20], pwd[20];
	float money;
	int id,islucky,r;
	while (1) {
		if (NULL == fgets(buf, 100, f)) break;
		if ('\n' == buf[0]) continue;
		r = sscanf(buf, "%d%20s%20s%f%d",&id,name,pwd,&money,&islucky);
		if (r == 5) {
			if (strcmp(name, "admin") == 0) {
				usrhead->id=id;
				strcpy(usrhead->name, "admin");
				strcpy(usrhead->pwd, pwd);
				usrhead->money = money;
				usrhead->islucky = 0;
			}
			else {
				Usr* node = creUsrnode(id, name, pwd,money, islucky);
				insertbytail(usrhead, node);
			}
		}
	}
	fclose(f);
}

/*保存用户信息*/
void saveUsrinfo(Usr * usrhead) {
	assert(usrhead);
	FILE* fp;
	if (NULL == (fp = fopen("usrInfo.txt", "w"))) {
		fprintf(stderr, "Can not open file!\n");
		exit(-1);
	}
	Usr* p = usrhead->next;
	int numt =0, i=0;	
	while (p != usrhead) {
		if (strcmp(p->name, "admin") == 0) {
			p->money = usrhead->money; //保存时将头节点存储的总金额汇入管理员账户		
		}
		fprintf(fp, "%d\t%s", p->id,p->name);
		i = 20 - strlen(p->name); //我把i当苦力用了
		numt = i%4==0?(i/4-1):i/4;
		for(i=0; i<numt; i++){
			fprintf(fp,"\t");
		}
		fprintf(fp,"%s",p->pwd);
		i = 20 - strlen(p->pwd);
		numt = i%4==0?(i/4-1):i/4;
		for(i=0; i<numt; i++){
			fprintf(fp,"\t");
		}
		fprintf(fp,"%.2f\t%d\n",p->money,p->islucky);
		p = p->next;
	}
	fprintf(fp, "%d\t%s\t\t\t%s\t\t\t%.2f\t%d\n", p->id, p->name, p->pwd, p->money,p->islucky);//头节点放在末尾
	fclose(fp);
}


/*修改用户信息*/
void modiInfo(Usr * usrhead, Usr * usr) {
	int choose = 0, isRun = 1;
	while (isRun) {
		choose = inputChoose(choose, viewModiusr);
		switch (choose) {
		case 1:
			modiName(usrhead, usr);
			break;
		case 2:
			modiUsrPwd(usrhead, usr);
			break;
		case 0:
			isRun = 0;
			break;
		default:
			printf("没有该选项!\n");
			getchar();
		}
	}
}

/*修改用户名*/
void modiName(Usr * usrhead, Usr * usr) {
	assert(usr);
	char flag = '\0';
	printf("*******当前的账户名是%s,修改名请与原名不同*******\n", usr->name);
	char name[20];
	getName(usrhead, name, &flag);
	if (flag != 'y') {
		strcpy(usr->name, name);
		printf("******账户名修改成功！******");
		getchar();
	}
	saveUsrinfo(usrhead);
}


/*修改用户密码*/
void modiUsrPwd(Usr * usrhead, Usr * usr) {
	assert(usr && usrhead);
	modiPwd(usr->pwd);
	saveUsrinfo(usrhead);
}

/*用户注销*/
int cancelAcc(Usr * usrhead, Usr * usr) {
	assert(usrhead && usr);
	printf("您已进入注销程序，请谨慎操作！继续请按r，其余键则自动退出：");
	char isquit = getchar();
	CLEAR();
	if (isquit != 'r') {
		return 1;//1代表不执行删除操作
	}
	printf("请输入帐号密码：");
	char pwd[20] = {}, repwd[20] = {};
	getPassword(pwd);
	printf("请再确认一遍：");
	getPassword(repwd);
	if (strcmp(pwd, repwd) != 0) {
		printf("两遍密码不一致，已经为您退出注销程序！\n");
		return 1;
	}
	usr->pre->next = usr->next;
	usr->next->pre = usr->pre;
	printf("****账户已注销，感谢您一直以来的陪伴****");
	saveUsrinfo(usrhead);
	getchar();
	return 0;
}

/*用户充值*/
void Recharge(Usr * usrhead, Usr * usr) {
	float rechg = 0;
	int check = 0, flag = 0;
	do {
		flag = 0;
		printf("请输入需要充值的金额：");
		check = scanf("%f", &rechg);
		CLEAR();
		if (check < 1 || rechg <= 0) {
			printf("输入非法！请重新输入！\n");
			flag = 1;
		}
	} while (flag);
	usr->money += rechg;
	usrhead->money += rechg;
	printf("您已成功充值%.2f元，现在您的账户余额是%.2f元。\n", rechg, usr->money);
	printf("\n回车继续～\n");
	getchar();
}

/*买票*/
void buyIsu(Usr * usr) {
	assert(usr);
	Isu* isuhead = creIsuHead();
	getLotteryInfo(isuhead);
	Isu* isuNow = isuhead->pre;
	int num[3]={0};
	char choose='\0';
	if (isuNow!=isuhead && isuNow->drawStatus==1) {
		printf("\n*****小店已打佯，请您耐心等待～*****\n");
		getchar();
		return;
	}
	else {
		int issueNo = isuNow->issueNo;
		printf("现售彩票为第%d期，单价为%.2f元，是否确认购买？(y/else)",issueNo,isuNow->price);		
		choose=getchar();
		CLEAR();
		if(choose=='y'){
			printf("请问您要买几注(不可超过5注)？\n注意：任何无效字符将转化为整型后对5取余+1作为您的输入！所以请不要随便乱输入！\n请输入：");
			char buynum =getchar();
			CLEAR();
			int k = ((int)buynum-49)%5+1,i=0;	
			if(usr->money < k*(isuNow->price)){
				system("clear");
				printf("对不起，您的余额不足，无法购买！请及时充值。\n");
				getchar();				
				return;		
			}
			usr->money -= k*(isuNow->price);
			//生成此条用户彩票节点
			Plot* plothead=getPlotInfo();
			for(;i<k;i++){
				usr->islucky--;
				if(usr->islucky<-5){ //可能需要用负数，不然后面开奖时不知道怎么处理
					printf("您每期最多只能买5注彩票！已经达到上限!");
					getchar();
					break;				
				}
				num[0]=rand()%10;
				num[1]=rand()%10;
				num[2]=rand()%10;	
				printf("第%d注彩票号码是：%d %d %d\n",i+1,num[0],num[1],num[2]);
				Plot* node = crePlotNode(issueNo,plothead->pre->singlenum+1,num,usr->id,usr->name,0,0);	
				insertbytailPlot(plothead, node);		
			}
			isuNow->sellnum += k;
			savePlotInfo(plothead);//保存个人彩票信息
			plothead=NULL;			
			free(plothead);
		}
		else{
			return;		
		}
	}
	saveLotteryInfo(isuhead);
	printf("*******您已购买成功！*******\n");	
	isuhead=NULL;
	isuNow=NULL;
	free(isuhead);
	free(isuNow);
	getchar();
}

/*suprise*/
void suprise(Usr* usr){
	int i=0, tar = usr->islucky;
	usr->islucky= 0;
	int cow[4]={0,0,0,0};
	cow[0]=tar%10;
	cow[1]=(tar/10)%10;
	cow[2]=(tar/100)%10;
	cow[3]=tar/1000;
	char bee[5][20]={"三等","二等","一等","特别"};
	if(tar){
		system("clear");
		for(i=0;i<4;i++){
			if(cow[i]!=0){
				printf("恭喜你中了%d次%s奖！\n",cow[i],bee[i]);		
			}
		}
		getchar();	
	}
}

/*提现*/
void getMoney(Usr* usr){
	printf("您当前的金额是：%.2f元，",usr->money);
	printf("请输入需要提现的金额：");
	float get = 0;	
	int check = scanf("%f",&get);
	CLEAR();
	if(check<=0||get>usr->money||get<0){
		printf("金额错误！已经为您自动退出！\n");
		getchar();
		return;
	}
	usr->money-=get;
	printf("*******提现成功！******\n");
	getchar();
}

/*查询中奖记录*/
void lookLuckyRecord(Usr* usr){
	Plot* plothead = getPlotInfo();
	Plot* p = plothead->next;
	printf(" 彩票期号\t彩票序号\t中奖种类\t中奖金额\n*-----------------------------------------------------------*\n");	
	char a[20] ="\0";
	char* type=a;
	while (p != plothead) {
		if(p->usrid==usr->id && p->issueNo>2029 && p->islucky>0){//2029之前没有把中奖信息写入文件
			type= p->islucky==5?"三等奖+特别奖":(p->islucky==4?"特别奖":(p->islucky==3?"一等奖":(p->islucky==2?"二等奖":"三等奖")));		
			printf(" %d\t\t %d\t\t%s\t\t%.2f\n",p->issueNo,p->singlenum,type,p->bonus);
		}
		p=p->next;	
	}
	plothead=NULL;
	p=NULL;
	free(plothead);
	free(p);
	getchar();
}

/*查询购票记录*/
void lookBuyed(Usr* usr){
	Plot* plothead = getPlotInfo();
	Plot* p = plothead->next;
	printf(" 彩票期号\t彩票序号\t所购数字\n*----------------------------------------*\n");	
	while (p != plothead) {
		if(p->usrid==usr->id){
			printf(" %d\t\t %d\t\t%d %d %d\t\n",p->issueNo,p->singlenum,p->lotnum[0],p->lotnum[1],p->lotnum[2]);
		}
		p=p->next;	
	}
	p=NULL;
	free(p);
	plothead=NULL;
	free(plothead);
	getchar();
}

/*猜拳游戏*/
void gamble(Usr* usr){
	system("clear");
	printf("\n\n\n\n\t\t**********欢迎进入猜拳游戏！**********\n\n\n\n");
	int gam = 0,cho = 0,check=0,flag=0,comp=0;
	char quit='\0';
	while(1){
		printf("*您的账户余额为%.2f元*\n",usr->money);
		while(1){
			printf("\n请输入此轮猜拳的胜负赌注金额（不能超过10元且不能少于1元）：");
			check = scanf("%d",&gam);
			CLEAR();			
			if(check>0 && gam<usr->money && gam <= 10 && gam>0){
				break;			
			}
			else{
				printf("您的输入并不合理，请重新输入，谢谢！\n");
				continue;		
			}
		}
		printf("1代表石头，2代表剪刀，3代表布,除了这三个数字，摁下其余键则您将被无条件判负！\n");
		printf("您的出拳是：");
		check = scanf("%d",&cho);
		CLEAR();
		comp=rand()%3+1;
		printf("电脑的出拳是：%d\n",comp);
		if(check>0 && cho>0 && cho <4){
			if(comp>cho){
				flag= comp-cho==1?1:0;			
			}
			else if(comp<cho){
				flag= cho-comp==1?0:1;			
			}
			else{
				flag=-1;//平局			
			}
		}
		else {
			flag=0;					
		}
		if(flag==1){
			printf("您赢了！\n");
			usr->money+=gam;		
		}
		else if(!flag){
			printf("您输了！\n");
			usr->money-=gam;		
		}
		else{
			printf("平局！\n");		
		}
		printf("继续游戏请按1，其余任意键退出：");
		quit=getchar();
		CLEAR();
		if(quit!='1'){
			break;		
		}
	}
}

/*用户主函数*/
void conUsr(Usr * usrhead, Usr * usr) {
	suprise(usr);
	saveUsrinfo(usrhead);
	int choose = 0, flag = 0, isRun = 1, isPoor = 0, check = 0;
	while (isRun) {
		isPoor = (usr->money < 5);//isPoor用来在用户登录时提醒用户余额不足
		do {
			flag = 0;
			viewUsrlogined(isPoor);
			check = scanf("%d", &choose); //由于isPoor的存在，导致我没法开心地使用control.c里的inputChoose函数
			if (getchar() != '\n' || check < 1) {
				flag = 1;
				CLEAR();
				printf("输入不合法！\n");
			}
		} while (flag);
		switch (choose) {
		case 1:
			outputsUsr(usr);
			break;
		case 2:
			modiInfo(usrhead, usr);
			saveUsrinfo(usrhead);
			break;
		case 3:
			isRun=cancelAcc(usrhead, usr);
			break;
		case 4:
			Recharge(usrhead, usr);
			saveUsrinfo(usrhead);
			break;
		case 5:
			lookLuckyRecord(usr);
			break;
		case 6:
			buyIsu(usr);
			saveUsrinfo(usrhead);
			break;
		case 7:
			lookBuyed(usr);
			break;
		case 8:
			gamble(usr);
			break;		
		case 9:
			getMoney(usr);
			break;
		case 0:
			isRun = 0;
			break;
		default:
			printf("没有该选项!\n");
			getchar();
		}
		saveUsrinfo(usrhead);
	}
}

