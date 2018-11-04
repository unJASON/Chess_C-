#include <iostream>
#include <ctime>
#include <time.h>
#include<random>
#include<algorithm>
#include "jsoncpp/json.h"
#include <cstring>
//#define DEBUG

using namespace std;
int board[15][15];
int sec = 1000;
const double RuningTime = 0.95f;
float confident = 1.96;
int equivalence = 1000;
int x = -1;//最后一步下的地方
int y = -1;//最后一步下的地方

//扩展的点数
const int expand_CNT = 15;

//const double C = 1.0f;
const double C = 1.0f;

//产生随机数
default_random_engine engine(time(nullptr));
uniform_int_distribution<int> uidis(0, 224);

//引入score用于选点
struct Point {
	int x;
	int y;
	int score;
};
bool cmpScore(const Point & a, const Point & b) {
	return a.score > b.score;
}
//存储score
Point pointScore[256];

//棋型辨识数组 
int score[3][3][3][3][3][3];
//以下为各棋型的识别码  //权重
#define WIN 1 //4000
#define LOSE 2 //-4000
#define FLEX4 3 //2000
#define flex4 4 //-2000
#define BLOCK4 5 //1000
#define block4 6 //-1000 
#define FLEX3 7 //1000 ?
#define flex3 8 //-1000
#define BLOCK3 9 //400 ?
#define block3 10 //-600 ? 
#define FLEX2 11 //400 ?
#define flex2 12 //-600 ?
#define BLOCK2 13 //100 ?
#define block2 14 //-150 ?
#define FLEX1 15 //100 ?
#define flex1 16 //-150 ?
int init_score()
{
	//黑五连 AI胜 (这里的黑不是指先手，而是指AI)
	score[1][1][1][1][1][1] = WIN;
	score[1][1][1][1][1][0] = WIN;
	score[0][1][1][1][1][1] = WIN;
	score[1][1][1][1][1][2] = WIN;
	score[2][1][1][1][1][1] = WIN;
	//白五连 AI负
	score[2][2][2][2][2][2] = LOSE;
	score[2][2][2][2][2][0] = LOSE;
	score[0][2][2][2][2][2] = LOSE;
	score[2][2][2][2][2][1] = LOSE;
	score[1][2][2][2][2][2] = LOSE;
	//黑活四
	score[0][1][1][1][1][0] = FLEX4;
	//白活四
	score[0][2][2][2][2][0] = flex4;
	//黑活三
	score[0][1][1][1][0][0] = FLEX3;
	score[0][1][1][0][1][0] = FLEX3;
	score[0][1][0][1][1][0] = FLEX3;
	score[0][0][1][1][1][0] = FLEX3;
	//白活三
	score[0][2][2][2][0][0] = flex3;
	score[0][2][2][0][2][0] = flex3;
	score[0][2][0][2][2][0] = flex3;
	score[0][0][2][2][2][0] = flex3;
	//黑活二
	score[0][1][1][0][0][0] = FLEX2;
	score[0][1][0][1][0][0] = FLEX2;
	score[0][1][0][0][1][0] = FLEX2;
	score[0][0][1][1][0][0] = FLEX2;
	score[0][0][1][0][1][0] = FLEX2;
	score[0][0][0][1][1][0] = FLEX2;
	//白活二
	score[0][2][2][0][0][0] = flex2;
	score[0][2][0][2][0][0] = flex2;
	score[0][2][0][0][2][0] = flex2;
	score[0][0][2][2][0][0] = flex2;
	score[0][0][2][0][2][0] = flex2;
	score[0][0][0][2][2][0] = flex2;
	//黑活一
	score[0][1][0][0][0][0] = FLEX1;
	score[0][0][1][0][0][0] = FLEX1;
	score[0][0][0][1][0][0] = FLEX1;
	score[0][0][0][0][1][0] = FLEX1;
	//白活一
	score[0][2][0][0][0][0] = flex1;
	score[0][0][2][0][0][0] = flex1;
	score[0][0][0][2][0][0] = flex1;
	score[0][0][0][0][2][0] = flex1;


	int p1, p2, p3, p4, p5, p6, x, y, ix, iy;

	for (p1 = 0; p1 < 3; p1++)
	{
		for (p2 = 0; p2 < 3; p2++)
		{
			for (p3 = 0; p3 < 3; p3++)
			{
				for (p4 = 0; p4 < 3; p4++)
				{
					for (p5 = 0; p5 < 3; p5++)
					{
						for (p6 = 0; p6 < 3; p6++)
						{
							x = 0;
							y = 0;
							ix = 0;
							iy = 0;
							if (p1 == 1) x++;
							else if (p1 == 2) y++;
							if (p2 == 1) x++, ix++;
							else if (p2 == 2) y++, iy++;
							if (p3 == 1) x++, ix++;
							else if (p3 == 2) y++, iy++;
							if (p4 == 1) x++, ix++;
							else if (p4 == 2) y++, iy++;
							if (p5 == 1) x++, ix++;
							else if (p5 == 2) y++, iy++;
							if (p6 == 1) ix++;
							else if (p6 == 2) iy++;

							//黑冲四 
							if (x == 4 && y == 0 || ix == 4 && iy == 0)
							{
								if (score[p1][p2][p3][p4][p5][p6] == 0)
								{
									score[p1][p2][p3][p4][p5][p6] = BLOCK4;
								}
							}

							//白冲四 
							else if (x == 0 && y == 4 || ix == 0 && iy == 4)
							{
								if (score[p1][p2][p3][p4][p5][p6] == 0)
								{
									score[p1][p2][p3][p4][p5][p6] = block4;
								}
							}

							//黑眠三 
							else if (x == 3 && y == 0 || ix == 3 && iy == 0)
							{
								if (score[p1][p2][p3][p4][p5][p6] == 0)
								{
									score[p1][p2][p3][p4][p5][p6] = BLOCK3;
								}
							}

							//白眠三 
							else if (x == 0 && y == 3 || ix == 0 && iy == 3)
							{
								if (score[p1][p2][p3][p4][p5][p6] == 0)
								{
									score[p1][p2][p3][p4][p5][p6] = block3;
								}
							}

							//黑眠二
							else if (x == 2 && y == 0 || ix == 2 && iy == 0)
							{
								if (score[p1][p2][p3][p4][p5][p6] == 0)
								{
									score[p1][p2][p3][p4][p5][p6] = BLOCK2;
								}
							}

							//白眠二
							else if (x == 0 && y == 2 || ix == 0 && iy == 2)
							{
								if (score[p1][p2][p3][p4][p5][p6] == 0)
								{
									score[p1][p2][p3][p4][p5][p6] = block2;
								}
							}

						}
					}
				}
			}
		}
	}

	return 0;
}




static unsigned long x_random = 123456789, y_random = 362436069, z_random = 521288629;
inline unsigned long xorshf96(void) {          //period 2^96-1
	unsigned long t;
	x_random ^= x_random << 16;
	x_random ^= x_random >> 5;
	x_random ^= x_random << 1;

	t = x_random;
	x_random = y_random;
	y_random = z_random;
	z_random = t ^ x_random ^ y_random;

	return z_random;
}

//判断输赢需要用到的数组
int fx[4] = { 0,-1,-1,-1 };
int fy[4] = { -1,0,-1,1 };
int tx[4] = { 0,1,1,1 };
int ty[4] = { 1,0,1,-1 };

//黑色:1 自己,白色:2 ，对手
//节点数据结构
struct Node {
	int N, QB, QW;	//模拟次数,黑赢次数,白赢次数
	Node *first, *nxt;
	Node * newChild;
	Node * parent;
	int color;	//颜色，最后一步所下的颜色
	int x;	//下棋点
	int y;	//下棋点
	//unsigned int visited[15][15];
	int depth;	//探索深度
	int isTerminated;	//是否为终局
	int turn;	//当前应当下棋的颜色
	bool hasExpanded;	//是否已扩展
	unsigned int success;


	Node(int c, int t, int x, int y, Node * p = nullptr) {
		parent = p;
		color = c;
		this->x = x;
		this->y = y;
		first = NULL;
		nxt = NULL;
		newChild = NULL;
		N = QB = QW = 0;
		if (p) depth = p->depth + 1;
		else depth = 0;
		isTerminated = false;
		hasExpanded = false;
		turn = t;
		success = 0;
	}
};


Node* root;	//root 指针
//扩展
Node* expand(Node*);
//模拟
Node* simulate(Node*);

//反向传播
void packPropagation(Node*);

//UCT公式
double evaluate(Node *r);
//打印board
void printBoard();
//判断输赢，如果赢则返回颜色，如果未赢则返回0
int judge(int(*)[15], int, int, int);

Node * select(Node* n) {
	int own = n->color;
	int opp = 1;
	if (own == 1) opp = 2;
	++(n->N);
	//出现终局直接返回
	if (n->isTerminated) {
		if (n->success == 1) ++(n->QB);
		else if (n->success == 2) ++(n->QW);
		return n;
	}
	if (!n->hasExpanded) {
		expand(n);
		n->hasExpanded = true;
	}
	Node * leaf = nullptr;

	Node * bestChild = NULL;

	if (n->newChild == NULL) {
		double minVal = -10e3;
		for (Node * u = n->first; u; u = u->nxt) {
			double valCh = evaluate(u);
			if (valCh > minVal) {
				bestChild = u;
				minVal = valCh;
			}
		}

		if (bestChild) {
			int x = bestChild->x;
			int y = bestChild->y;
			board[x][y] = bestChild->color;
			leaf = select(bestChild);
			board[x][y] = 0;
		}

	}
	else if (n->newChild) {
		bestChild = n->newChild;
		n->newChild = bestChild->nxt;
		bestChild->nxt = n->first;
		n->first = bestChild;

		leaf = simulate(bestChild);

	}
	//反向传播带回输赢
	if (leaf->success == 1) {
		++(n->QB);
	}
	else if (leaf->success == 2) {
		++(n->QW);
	}
	return leaf;
}

//UCT 公式
double evaluate(Node *r) {
	//if (r->isTerminated) return r->isTerminated;
	if (r->N == 0) return -1;
	if (r->parent == NULL) return -1;


	double s1;
	if (r->color == 1)
		s1 = (double)(r->QB) / (double)(r->N);
	else {
		s1 = (double)(r->QW) / (double)(r->N);
	}

	double s2 = (double)(log((double)(r->parent->N))) / (double)(r->N);
	s2 = sqrt(s2)*C;

	return s1 + s2;
}
//评估局面返回最好的前N个点。
Node * scoreBoard(Node * r) {
	return NULL;
}
void judgeScore(int(*board)[15], int color, int x, int y);
const int SIZE = 15;
//返回评估后的点(需要通过评估函数进行剪枝运算)
Node * euldVis(Node * n) {

	int c = 1;
	if (n->color == 1) c = 2;
	int  cnt = 0;
	Node *u = nullptr;
	Node *v = nullptr;
	int dis1 = -1, dis2 = -1;

	for (int i = 0; i < SIZE; ++i) {
		for (int j = 0; j < SIZE; ++j) {
			pointScore[i*SIZE + j].x = i;
			pointScore[i*SIZE + j].y = j;
			pointScore[i*SIZE + j].score = -1000;
		}
	}
	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			if (board[i][j]) continue;
			if (true) {
				//先判断终局
				if (judge(board, c, i, j) == c) {
					Node *v = new Node(c, n->color, i, j, n);
					v->success = v->isTerminated = c;
					v->nxt = nullptr;
					v->hasExpanded = true;
					return v;
				}
			}
		}
	}

	//给各个点打分
	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			if (board[i][j]) continue;
			if (true) {
				//算分
				judgeScore(board, c, i, j);
			}
		}
	}


	sort(pointScore, pointScore + 15 * 15, cmpScore);
	for (int i = 0; i < expand_CNT; ++i) {
		u = new Node(c, n->color, pointScore[i].x, pointScore[i].y, n);
#ifdef DEBUG
		//cout <<"x:"<< pointScore[i].x<<"y:"<< pointScore[i].y <<"score:" << pointScore[i].score<<endl;
#endif 
		u->nxt = v;
		v = u;
	}
	//加上可能活三的点
	int final_exp = expand_CNT;
	while (true){
		if (pointScore[final_exp].score >= 222000){
			u = new Node(c, n->color, pointScore[final_exp].x, pointScore[final_exp].y, n);
			u->nxt = v;
			v = u;
			final_exp++;
		}else{
			break;
		}
	}
	return u;
}

//color 我的颜色
const int INITWEIGHT = 1000;
void judgeScore(int(*board)[15], int color, int x, int y) {
	//printBoard();
	int opp = 1;
	if (color == 1) opp = 2;
	int blankNum;	//连续空白不能太多
	int nx;
	int ny;
	int total_oppScore = 0;
	int total_selfScore = 0;
	int oppScore = INITWEIGHT;
	int selfScore = INITWEIGHT;
	int weight;	//记录联乘个数
	//记录4个对角的方向算score并求和
	for (int i = 0; i < 4; ++i) {
		//计算自己的分数
		nx = x;
		ny = y;
		blankNum = 0;
		weight = INITWEIGHT;
		for (int j = 0; j < 4; ++j) {
			nx += fx[i];
			ny += fy[i];
			//遇到边界停下并减分
			if (nx < 0 || nx>14 || ny < 0 || ny>14) {
				blankNum = 0;
				selfScore = selfScore / 10;
				break;
			}
			if (board[nx][ny] == color) {//遇到自己的颜色
				//遇到相同颜色则求和
				blankNum = 0;
				selfScore = selfScore + weight * 10;
				weight = weight * 10;
			}
			else if (board[nx][ny] == opp) {//遇到对手的颜色
			   //停下并减分
				blankNum = 0;
				selfScore = selfScore / 10;
				break;
			}
			else {	//没有棋子
				if (blankNum > 1) {
					break;
				}
				else {
					//连续空白最好不要太多
					blankNum++;
					weight = weight / 10;
					continue;
				}

			}
		}
		//另外一个方向重新计数
		nx = x;
		ny = y;
		blankNum = 0;
		for (int j = 0; j < 4; ++j) {
			nx += tx[i];
			ny += ty[i];
			//遇到边界停下并减分
			if (nx < 0 || nx>14 || ny < 0 || ny>14) {
				blankNum = 0;
				selfScore = selfScore / 10;
				break;
			}
			if (board[nx][ny] == color) {
				//遇到相同颜色则求和
				blankNum = 0;
				selfScore = selfScore + weight * 10;
				weight = weight * 10;
			}
			else if (board[nx][ny] == opp) {//遇到对手的颜色
			   //停下并减分
				blankNum = 0;
				selfScore = selfScore / 10;
				break;
			}
			else {
				if (blankNum > 1) {
					break;
				}
				else {
					//连续空白最好不要太多
					blankNum++;
					weight = weight / 10;
					continue;
				}
			}
		}

		//计算对手的分数
		nx = x;
		ny = y;
		blankNum = 0;
		weight = INITWEIGHT;
		for (int j = 0; j < 4; ++j) {
			nx += fx[i];
			ny += fy[i];
			//遇到边界停下并减分
			if (nx < 0 || nx>14 || ny < 0 || ny>14) {
				blankNum = 0;
				oppScore = oppScore / 10;
				break;
			}
			if (board[nx][ny] == opp) {//遇到对手的颜色
				//遇到相同颜色则求和
				blankNum = 0;
				oppScore = oppScore + weight * 10;
				weight = weight * 10;
			}
			else if (board[nx][ny] == color) {//遇到自己的颜色
				//停下并减分
				blankNum = 0;
				oppScore = oppScore / 10;
				break;
			}
			else {	//没有棋子
				if (blankNum > 1) {
					break;
				}
				else {
					//连续空白最好不要太多
					blankNum++;
					weight = weight / 10;
					continue;
				}

			}
		}
		nx = x;
		ny = y;
		//另外一个方向重新计数
		blankNum = 0;
		for (int j = 0; j < 4; ++j) {
			nx += tx[i];
			ny += ty[i];
			//遇到边界停下并减分
			if (nx < 0 || nx>14 || ny < 0 || ny>14) {
				blankNum = 0;
				oppScore = oppScore / 10;
				break;
			}
			if (board[nx][ny] == opp) {
				//遇到相同颜色则求和
				blankNum = 0;
				oppScore = oppScore + weight * 10;
				weight = weight * 10;
			}
			else if (board[nx][ny] == color) {//遇到自己的颜色减分跳出
				blankNum = 0;
				oppScore = oppScore / 10;
				break;
			}
			else {
				if (blankNum > 1) {//空白太多则跳出
					break;
				}
				else {
					//连续空白最好不要太多
					blankNum++;
					weight = weight / 10;
					continue;
				}
			}
		}

		total_oppScore = total_oppScore + oppScore;
		total_selfScore = total_selfScore + selfScore;
	}
	//pointScore[x*SIZE + y].score = selfScore > oppScore ? selfScore : oppScore;
	pointScore[x*SIZE + y].score = total_oppScore + total_selfScore;
}

//判断输赢，如果赢则返回颜色，如果未赢则返回0
int judge(int(*board)[15], int color, int x, int y) {
	int chains = 0;
	int nx;
	int ny;

	for (int i = 0; i < 4; ++i) {
		nx = x;
		ny = y;
		chains = 0;
		for (int j = 0; j < 4; ++j) {
			nx += fx[i];
			ny += fy[i];
			if (nx < 0 || nx>14 || ny < 0 || ny>14) break;
			if (board[nx][ny] == color) {
				++chains;
				if (chains >= 4) {
					return color;
				}
			}
			else break;
		}
		nx = x;
		ny = y;

		for (int j = 0; j < 4; ++j) {
			nx += tx[i];
			ny += ty[i];

			if (nx < 0 || nx>14 || ny < 0 || ny>14) break;
			if (board[nx][ny] == color) {
				++chains;
				if (chains >= 4) {
					return color;
				}
			}
			else break;
		}
	}
	return 0;
}
//初始化棋盘
void RecoverBoard(string str) {
	//解析Json数据
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);
	//分析input输入和之前的输出，Recover棋盘
	int turnNum_rsp = input["responses"].size();
	int turnNum_req = input["requests"].size();
	//对方先手
	int zero = 0;
	if (input["requests"][zero]["x"] != -1)
	{
		//己方第四回合是否选择了换手,第六回合才能从resopnses判别
		if (turnNum_rsp >= 2 && input["responses"][1]["x"] == -1)
		{
			//换手前下的子
			for (int i = 0; i < 2; i++)
			{
				//己方落子1
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 1;
				//对方落子2
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 2;
			}
			//换手后下的子
			for (int i = 2; i < turnNum_rsp; i++)
			{
				//对方落子2
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 2;
				//己方落子1
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 1;
			}
			if (input["requests"][turnNum_rsp]["x"].asInt() >= 0 && input["requests"][turnNum_rsp]["y"].asInt() >= 0)
				board[input["requests"][turnNum_rsp]["x"].asInt()][input["requests"][turnNum_rsp]["y"].asInt()] = 2;
			//保存对手走的最后一步棋到全局变量x,y
			x = input["requests"][turnNum_rsp]["x"].asInt();
			y = input["requests"][turnNum_rsp]["y"].asInt();
		}
		//未达第六回合或未换手
		else
		{
			for (int i = 0; i < turnNum_rsp; i++)
			{
				//对方落子2
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 2;
				//己方落子1
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 1;
			}
			if (input["requests"][turnNum_rsp]["x"].asInt() >= 0 && input["requests"][turnNum_rsp]["y"].asInt() >= 0)
				board[input["requests"][turnNum_rsp]["x"].asInt()][input["requests"][turnNum_rsp]["y"].asInt()] = 2;
			//保存对手走的最后一步棋到全局变量x,y
			x = input["requests"][turnNum_rsp]["x"].asInt();
			y = input["requests"][turnNum_rsp]["y"].asInt();
		}
	}
	//己方先手
	else
	{
		//对方选择了换手
		if (turnNum_req >= 3 && input["requests"][2]["x"] == -1)
		{
			//换手前下的子
			for (int i = 0; i < 2; i++)
			{
				//己方落子1
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 1;
				//对方落子2
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 2;
			}
			//换手后下的子
			for (int i = 2; i < turnNum_rsp; i++)
			{
				//对方落子2
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 2;
				//己方落子1
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 1;
			}
			if (input["requests"][turnNum_rsp]["x"].asInt() >= 0 && input["requests"][turnNum_rsp]["y"].asInt() >= 0)
				board[input["requests"][turnNum_rsp]["x"].asInt()][input["requests"][turnNum_rsp]["y"].asInt()] = 2;
			//保存对手走的最后一步棋到全局变量x,y
			//第五回合接收到的对方最后一手是-1,-1，需变换
			if (turnNum_req == 3)
			{
				x = input["responses"][1]["x"].asInt();
				y = input["responses"][1]["y"].asInt();
			}
			else
			{
				x = input["requests"][turnNum_rsp]["x"].asInt();
				y = input["requests"][turnNum_rsp]["y"].asInt();
			}
		}
		//未达到第五回合或未换手
		else
		{
			for (int i = 0; i < turnNum_rsp; i++)
			{
				//对方落子2
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 2;
				//己方落子1
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 1;
			}
			if (input["requests"][turnNum_rsp]["x"].asInt() >= 0 && input["requests"][turnNum_rsp]["y"].asInt() >= 0)
				board[input["requests"][turnNum_rsp]["x"].asInt()][input["requests"][turnNum_rsp]["y"].asInt()] = 2;
			//保存对手走的最后一步棋到全局变量x,y
			x = input["requests"][turnNum_rsp]["x"].asInt();
			y = input["requests"][turnNum_rsp]["y"].asInt();
		}
	}
}
//扩展 考虑扩展的时候返回的是否为终局节点
Node * expand(Node* n) {
	int c = 1;
	if (n->color == 1) c = 2;
	Node * u = euldVis(n);
	if (u->isTerminated) {
		n->newChild = u;
		return n;
	}
	n->newChild = u;
	//n->newChild = scoreBoard(n);

	return n;
}

//模拟
Node * simulate(Node* n) {
	++(n->N);
	if (n->isTerminated) {
		if (n->success == 1) {
			++(n->QB);
		}
		else if (n->success == 2) {
			++(n->QW);
		}
		return n;
	}

	int opp = n->color;
	int own = 1;
	if (n->color == 1) own = 2;

	int cnt = 0;

	int thisBoard[15][15];
	//用新棋盘模拟
	memcpy(thisBoard, board, sizeof(thisBoard));

	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			if (thisBoard[i][j]) ++cnt;
		}
	}

	int pos = 0, x, y;

	//一次性模拟到终局
	while (cnt <= 225) {
		//???
		if (cnt == 3) {
			pos = uidis(engine);
			//pos = xorshf96() % 225;
			if (pos > 180) {
				pos = 225;
			}
		}

		if (pos < 225) {
			for (;;) {
				//pos = xorshf96() % 225;
				//随机取数
				pos = uidis(engine);
				x = pos / 15;
				y = pos % 15;
				if (!thisBoard[x][y]) break;
			}

			int jg = judge(thisBoard, own, x, y);

			++cnt;
			thisBoard[x][y] = own;

			if (jg) {
				n->success = jg;
				if (jg == 1) ++(n->QB);
				else if (jg == 2) ++(n->QW);

				thisBoard[x][y] = 3;//这步没必要
				return n;
			}
			//交换下次下棋的颜色
			own ^= opp;
			opp ^= own;
			own ^= opp;
		}
		pos = 0;
	}
	n->success = -1;
	return n;
}


void printBoard() {
	//棋盘状态
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (board[i][j] == 0)
				std::cout << '0';
			else if (board[i][j] == 1)
				std::cout << '*';
			else std::cout << '#';
		}
		std::cout << '\n';
	}
	//cin.get();
}
void printNode(Node * r) {
	if (!r) return;
	cout << "color " << r->color << " x " << r->x << " y " << r->y << " QB/QW/N " << r->QB << " " << r->QW << " " << r->N << " isTerminated " << r->isTerminated << " depth " << r->depth << endl;
	for (Node *u = r->first; u; u = u->nxt) {
		//printNode(u);
		cout << "color " << u->color << " x " << u->x << " y " << u->y << " QB/QW/N " << u->QB << " " << u->QW << " " << u->N << " isTerminated " << u->isTerminated << " depth " << u->depth << endl;
	}
}
int main() {

#ifdef DEBUG
	freopen("out.txt", "w", stdout);
#endif
	memset(board, 0, sizeof(board));
	double duration = 0;	//运行时间
	clock_t timeStart = clock();

	string str;

#ifdef DEBUG
	str = string("{\"requests\":[{\"x\":-1,\"y\":-1},{\"x\":7,\"y\":7}],\"responses\":[{\"x\":0,\"y\":0}]}");
#else
	getline(cin, str);
#endif // DEBUG
	RecoverBoard(str);
	Node * leaf = nullptr;
	int color = 2;

	int turn = 1;
	root = new Node(color, turn, x, y, nullptr);

	for (;;) {
		duration = (double)(clock() - timeStart) / CLOCKS_PER_SEC;
		if (duration >= RuningTime) {
			int maxVal = -1;
			Node * v = nullptr;
			for (Node *u = root->first; u; u = u->nxt) {
				if (maxVal < u->N) {
					maxVal = u->N;
					v = u;
				}
			}

			if (v) {
#ifdef DEBUG
				cout << v->x << " " << v->y << " " << endl;
				board[v->x][v->y] = v->color + 2;
				printBoard();
#endif

				Json::Value ret;
				Json::Value act;
				act["x"] = v->x;
				act["y"] = v->y;

				ret["response"] = act;
				Json::FastWriter writer;
				std::cout << writer.write(ret) << endl;

#ifdef DEBUG
				std::cout << root->N << endl;
				for (v = leaf; v; v = v->parent) {
					std::cout << v->QB << " " << v->QW << " " << v->N << " " << v->color << endl;
				}
				printNode(root);
#endif
			}
			else std::cout << "NULL PTR" << endl;



			return 0;

		}
		else {
			//MCTS
			leaf = select(root);
		}
	}

	return 0;
}