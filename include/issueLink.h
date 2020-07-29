#ifndef __ISSUELINK_H__
#define __ISSUELINK_H__

typedef struct issuelink {
	int issueNo;//from 202000
	float price;//5.0
	int drawStatus;//1 or 0
	int luckynum[3];//由于演示时数据库不可能很大，所以每一期彩票都只取3个数字，且都不大于10
	int sellnum;//卖了多少张彩票
	struct issuelink* pre;
	struct issuelink* next;
}Isu;

void issueLottery();//发布彩票

void creNewIssue(Isu* isuhead);//发布彩票的子函数

void getLotteryInfo(Isu* isuhead);//读取彩票信息

void saveLotteryInfo(Isu* isuhead);//存入彩票信息

Isu* creIsuHead();//创建彩票头节点

Isu* creIsuNode(int issueno, float price, int drawstatus, int* luckynum, int sellnum);//创建彩票节点

void insertbytailIsu(Isu* isuhead, Isu* node);//彩票信息尾插

void lookPassIsu();//review isu had been outed

#endif
