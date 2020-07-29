#include<stdio.h>
#include<string.h>
#include<assert.h>
#include<stdlib.h>
#include"issueLink.h"
#include"control.h"

void issueLottery() {
	Isu* isuhead = creIsuHead();
	getLotteryInfo(isuhead);
	if (isuhead->pre == isuhead) { //当前链表中仅存在头结点
		creNewIssue(isuhead);
		saveLotteryInfo(isuhead);
		return;
	}
	if (!isuhead->pre->drawStatus) {
		printf("上一期彩票尚未开奖，无法执行操作!\n");
		getchar();
		return;
	}
	else {
		creNewIssue(isuhead);
		saveLotteryInfo(isuhead);
	}
	isuhead=NULL;
	free(isuhead);
}

void creNewIssue(Isu* isuhead) {
	assert(isuhead);
	float price;
	int luckynum[3] = {0,0,0}; //luckynum由公证员开奖
	int draw = 0;
	printf("请输入本期彩票单张价格：");
	scanf("%f",&price);
	CLEAR();
	Isu* node = creIsuNode(isuhead->pre->issueNo+1,price,draw,luckynum,0);
	insertbytailIsu(isuhead, node);
	printf("\n*********彩票发行成功！*********\n");
	printf("此次彩票期数：%d 单价：%.2f元\n",node->issueNo,price);
	printf("********************************\n");
	node=NULL;
	free(node);	
	getchar();
}

void getLotteryInfo(Isu* isuhead) {
	assert(isuhead);
	char buf[100];
	int issueno=0,luckynum[3]={0,0,0},sellnum=0,drawstatus = 0, r=0;
	float price;
	FILE* f;
	if (NULL == (f = fopen("isuInfo.txt", "r"))) {
		fprintf(stderr, "Can not open file!\n");
		return;
	}
	while (1) {
		if (NULL == fgets(buf, 100, f)) {
			break;
		}
		if ('\n' == buf[0]) {
			continue;
		}
		r = sscanf(buf, "%d%f%d%d%d%d%d",\
			&issueno, &price,&drawstatus,&luckynum[0],&luckynum[1],&luckynum[2],&sellnum);
		if (r == 7) {
			Isu* node = creIsuNode(issueno, price, drawstatus, luckynum, sellnum);
			insertbytailIsu(isuhead, node);
		}
	}
	fclose(f);
}

void saveLotteryInfo(Isu* isuhead) {
	assert(isuhead);
	FILE* fp;
	if (NULL == (fp = fopen("isuInfo.txt", "w"))) {
		fprintf(stderr, "Can not open file!\n");
		return;
	}
	Isu* p = isuhead->next;
	while (p != isuhead) {
		fprintf(fp, "%d\t%f\t%d\t%d %d %d\t%d\n", p->issueNo, p->price,\
			p->drawStatus, p->luckynum[0],p->luckynum[1],p->luckynum[2], p->sellnum);
		p = p->next;
	}
	fclose(fp);
}

/*创建头结点*/
Isu* creIsuHead() {
	Isu* isuhead = (Isu*)malloc(sizeof(Isu));
	memset(isuhead, 0, sizeof(Isu));
	isuhead->issueNo = 2000;
	isuhead->next = isuhead;
	isuhead->pre = isuhead;
	isuhead->drawStatus = 1;//头节点已“开奖”
	return isuhead;
}

/*创建结点*/
Isu* creIsuNode(int issueno, float price, int drawstatus, int *luckynum, int sellnum) {
	Isu* isunode = (Isu*)malloc(sizeof(Isu));
	isunode->issueNo = issueno;
	isunode->drawStatus = drawstatus;
	memcpy(isunode->luckynum,luckynum,sizeof(int)*3);
	isunode->price = price;
	isunode->sellnum = sellnum;
	return isunode;
}

/*彩票信息尾插*/
void insertbytailIsu(Isu* isuhead, Isu* node) {
	assert(isuhead && node);
	isuhead->pre->next = node;
	node->pre = isuhead->pre;
	isuhead->pre = node;
	node->next = isuhead;
}
