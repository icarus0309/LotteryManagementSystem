#ifndef __NOTARY_H__
#define __NOTARY_H__

//每当一个用户购买一次彩票，就生成一个信息节点，管理员发行彩票之后，生成该链表的头节点
//开奖后，每一个信息节点都必须读进来比较一遍
typedef struct personalLottery {
	int issueNo;
	int singlenum;//该彩票唯一标识
	int lotnum[3];//每个数字都可以随机生成或手动输入，范围0～9&r
	int usrid;
	char usrname[20];//购买该彩票的用户名
	int islucky;//判断这张彩票是否中奖：0没有中奖，mark表示中几等奖
	float bonus;//中奖金额取决于几等奖
	struct personalLottery* pre;
	struct personalLottery* next;
}Plot;

/*
要考虑文件IO的时效性，现实中的系统不可避免地有多个用户同时操作，所以每次需要把用户购票信息
先写好，当即读取当即写入
*/

/*Plot节点的尾插*/
void insertbytailPlot(Plot* plothead, Plot* node);

/*创建个人彩票信息头节点*/
Plot* crePlotHead();

/*创建个人彩票信息节点*/
Plot* crePlotNode(int issno, int sinnum, int* lot, int usrid,char* usrname, int islucky, float bonus);

/*尾插个人彩票信息节点*/
void insertbytailPlot(Plot* plothead, Plot* node);

/*读取个人彩票信息链表*/
Plot* getPlotInfo();

/*保存个人彩票信息链表*/
void savePlotInfo(Plot* plothead);

/*开奖*/
void notaryIsu();

/*将钱打给用户*/
float giveMoneyToUsrs(int usrid,int mark,float price) ;

#endif
