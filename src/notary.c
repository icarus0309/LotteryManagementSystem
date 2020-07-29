#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
#include"notary.h"
#include"user.h"
#include"admin.h"
#include"issueLink.h"
#include"control.h"

/*创建购买彩票头节点*/
Plot* crePlotHead() {
	Plot* plothead = (Plot*)malloc(sizeof(Plot));
	memset(plothead, 0, sizeof(Plot));
	plothead->singlenum = 0;
	plothead->next = plothead;
	plothead->pre = plothead;
	return plothead;
}

/*创建购买彩票节点*/
Plot* crePlotNode(int issno, int sinnum, int* lot, int usrid,char* usrname, int islucky, float bonus) {
	Plot* plotnode = (Plot*)malloc(sizeof(Plot));
	plotnode->issueNo = issno;
	plotnode->singlenum = sinnum;
	memcpy(plotnode->lotnum, lot, sizeof(int) * 3);
	plotnode->usrid=usrid;
	strcpy(plotnode->usrname, usrname);
	plotnode->islucky = islucky;
	plotnode->bonus = bonus;
	return plotnode;
}

/*彩票节点的尾插*/
void insertbytailPlot(Plot* plothead, Plot* node) {
	assert(plothead && node);
	plothead->pre->next = node;
	node->pre = plothead->pre;
	plothead->pre = node;
	node->next = plothead;
}

/*读取个人彩票信息链表*/
Plot* getPlotInfo() {
	/*
	每个用户每购买一次彩票就需要将链表从头至尾全部读取一次，
	程序这样写有个很大的弊端：当数据量过于庞大时程序会很慢.
	想过解决办法，即写入时用追加，这样就不需要读取了，除非一些特殊情况。
	但是考虑到singlenum的唯一性，还是读取进来操作比较方便一点。
	先就这样吧。
	*/
	Plot* plothead = crePlotHead();
	FILE* f;
	if (NULL == (f = fopen("personalLotteryInfo.txt", "r"))) {
		fprintf(stderr, "Can not open file!\n");
		exit(-1);
	}
	char buf[100] = "\0", usrname[20] = "\0";
	float bonus;
	int r = 0, issueNo = 0, singlenum = 0, islucky = 0, usrid,lotnum[3] = { 0 };
	while (1) {
		if (NULL == fgets(buf, 100, f)){
			break;
		}
		if ('\n' == buf[0]) continue;
		r = sscanf(buf, "%d%d%d%d%d%d%s%d%f", &issueNo, &singlenum, &lotnum[0], &lotnum[1], &lotnum[2], &usrid,usrname, &islucky, &bonus);
		if (r == 9) {
			Plot* node = crePlotNode(issueNo, singlenum, lotnum,usrid, usrname, islucky, bonus);
			insertbytailPlot(plothead, node);
		}
	}
	fclose(f);
	f=NULL;
	free(f);
	return plothead;
}

/*保存彩票信息*/
void savePlotInfo(Plot * plothead) {
	assert(plothead);
	FILE* fp;
	if (NULL == (fp = fopen("personalLotteryInfo.txt", "w"))) {
		free(fp);		
		fprintf(stderr, "Can not open file!\n");
		exit(-1);
	}
	Plot* p = plothead->next;
	while (p != plothead) {
		fprintf(fp, "%d\t%d\t%d %d %d\t%d\t%s\t%d\t%.2f\n", p->issueNo, p->singlenum, \
			p->lotnum[0], p->lotnum[1], p->lotnum[2], p->usrid,p->usrname, p->islucky, p->bonus);
		p = p->next;
	}
	p=NULL;
	free(p);
	fp=NULL;
	free(fp);
	fclose(fp);
}

/*开奖*/
void notaryIsu() {
	Isu* isuhead = creIsuHead();
	getLotteryInfo(isuhead);	
	Isu* isunow = isuhead->pre;
	if(isunow->drawStatus){
		printf("暂无彩票可开奖！\n");
		getchar();
		return;
	}
	int isunum = isunow->issueNo;
	int luck[3] = { 0 }, i = 0;
	for (; i < 3; i++) {
		luck[i] = rand() % 10;
	}
	printf("这一期的中奖号码是：%d %d %d\n",luck[0], luck[1], luck[2]);
	printf("*************************\n");
	printf("第%d期购买的彩票情况如下：\n",isunum);
	float price = isunow->price;
	Plot* plothead = getPlotInfo();
	Plot* p = plothead->next;
	while (p != plothead) {
		if (p->issueNo == isunum) {
			printf("购买用户：%s所购彩票情况：%d %d %d\n",p->usrname,\
						p->lotnum[0],p->lotnum[1],p->lotnum[2]);
			int mark = 0,sum=0,mul=0;
			mark += p->lotnum[0]==luck[0]?1:0;
			mark += p->lotnum[1]==luck[1]?1:0;
			mark += p->lotnum[2]==luck[2]?1:0;
			sum=p->lotnum[0]+p->lotnum[1]+p->lotnum[2];
			mul=(p->lotnum[0]+1)*(p->lotnum[0]+1)*(p->lotnum[0]+1);
			if(mark < 2  &&  sum==(luck[0]+luck[1]+luck[2])  &&  mul==(luck[0]+1)*(luck[0]+1)*(luck[0]+1) ){
				mark+=4;//single special:mark=4; third place+special:mark=5		
			}
			p->islucky=mark;
			p->bonus = giveMoneyToUsrs(p->usrid,mark,price);
		}
		p=p->next;
	}
	memcpy(isunow->luckynum, luck, sizeof(int) * 3);
	isunow->drawStatus = 1;
	savePlotInfo(plothead);
	saveLotteryInfo(isuhead);
	getchar();
	plothead=NULL;
	free(plothead);
	isuhead=NULL;
	free(isuhead);
	p=NULL;
	free(p);
	isunow=NULL;
	free(isunow);
}

/*将奖金发给用户*/
float giveMoneyToUsrs(int usrid,int mark,float price) {
	Usr* usrhead = creUsrhead();
	getUsrinfo(usrhead);
	Usr* usr=findUsrbyId(usrhead,usrid);
	usr->islucky = usr->islucky<0?0:usr->islucky;//消除限制注数时的影响
	if(usr==NULL){
		return 0;
	}
	int mul=0;
	switch(mark){
		case 0:
			break;
		case 1:
			mul=1;
			mark=1;
			break;
		case 2:
			mul=5;
			mark=10;
			break;
		case 3:
			mul=100;
			mark=100;
			break;
		case 4:
			mul=50;
			mark=1000;
			break;
		case 5:
			mul=51;
			mark=1001;
			break;
		default:
			printf("system error!\n");
			break;
	}
	usr->islucky += mark;
	usr->money+=mul*price;
	saveUsrinfo(usrhead);
	usrhead=NULL;
	free(usrhead);
	usr=NULL;
	free(usr);
	return mul*price;
}

/*查看已经发行的彩票*/
void lookPassIsu(){
	Isu* isuhead = creIsuHead();
	getLotteryInfo(isuhead);
	Isu* p=isuhead->next;
	printf(" 发行期号\t单价\t是否开奖\t中奖号码\t售出数量\n");
	printf("*----------------------------------------------------------------*\n");
	char str[4]="\0";
	char *YorN=str;
	while(p!=isuhead){
		YorN = p->drawStatus?"是":"否";
		printf(" %d\t\t%.2f\t  %s\t\t%d %d %d\t\t%d\n",p->issueNo,p->price,YorN,p->luckynum[0],p->luckynum[1],p->luckynum[2],p->sellnum);
		p=p->next;	
	}
	isuhead=NULL;
	free(isuhead);
	getchar();
}





