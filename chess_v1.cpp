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
int x = -1;//���һ���µĵط�
int y = -1;//���һ���µĵط�

//��չ�ĵ���
const int expand_CNT = 15;

//const double C = 1.0f;
const double C = 1.0f;

//���������
default_random_engine engine(time(nullptr));
uniform_int_distribution<int> uidis(0, 224);

//����score����ѡ��
struct Point {
	int x;
	int y;
	int score;
};
bool cmpScore(const Point & a, const Point & b) {
	return a.score > b.score;
}
//�洢score
Point pointScore[256];

//���ͱ�ʶ���� 
int score[3][3][3][3][3][3];
//����Ϊ�����͵�ʶ����  //Ȩ��
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
	//������ AIʤ (����ĺڲ���ָ���֣�����ָAI)
	score[1][1][1][1][1][1] = WIN;
	score[1][1][1][1][1][0] = WIN;
	score[0][1][1][1][1][1] = WIN;
	score[1][1][1][1][1][2] = WIN;
	score[2][1][1][1][1][1] = WIN;
	//������ AI��
	score[2][2][2][2][2][2] = LOSE;
	score[2][2][2][2][2][0] = LOSE;
	score[0][2][2][2][2][2] = LOSE;
	score[2][2][2][2][2][1] = LOSE;
	score[1][2][2][2][2][2] = LOSE;
	//�ڻ���
	score[0][1][1][1][1][0] = FLEX4;
	//�׻���
	score[0][2][2][2][2][0] = flex4;
	//�ڻ���
	score[0][1][1][1][0][0] = FLEX3;
	score[0][1][1][0][1][0] = FLEX3;
	score[0][1][0][1][1][0] = FLEX3;
	score[0][0][1][1][1][0] = FLEX3;
	//�׻���
	score[0][2][2][2][0][0] = flex3;
	score[0][2][2][0][2][0] = flex3;
	score[0][2][0][2][2][0] = flex3;
	score[0][0][2][2][2][0] = flex3;
	//�ڻ��
	score[0][1][1][0][0][0] = FLEX2;
	score[0][1][0][1][0][0] = FLEX2;
	score[0][1][0][0][1][0] = FLEX2;
	score[0][0][1][1][0][0] = FLEX2;
	score[0][0][1][0][1][0] = FLEX2;
	score[0][0][0][1][1][0] = FLEX2;
	//�׻��
	score[0][2][2][0][0][0] = flex2;
	score[0][2][0][2][0][0] = flex2;
	score[0][2][0][0][2][0] = flex2;
	score[0][0][2][2][0][0] = flex2;
	score[0][0][2][0][2][0] = flex2;
	score[0][0][0][2][2][0] = flex2;
	//�ڻ�һ
	score[0][1][0][0][0][0] = FLEX1;
	score[0][0][1][0][0][0] = FLEX1;
	score[0][0][0][1][0][0] = FLEX1;
	score[0][0][0][0][1][0] = FLEX1;
	//�׻�һ
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

							//�ڳ��� 
							if (x == 4 && y == 0 || ix == 4 && iy == 0)
							{
								if (score[p1][p2][p3][p4][p5][p6] == 0)
								{
									score[p1][p2][p3][p4][p5][p6] = BLOCK4;
								}
							}

							//�׳��� 
							else if (x == 0 && y == 4 || ix == 0 && iy == 4)
							{
								if (score[p1][p2][p3][p4][p5][p6] == 0)
								{
									score[p1][p2][p3][p4][p5][p6] = block4;
								}
							}

							//������ 
							else if (x == 3 && y == 0 || ix == 3 && iy == 0)
							{
								if (score[p1][p2][p3][p4][p5][p6] == 0)
								{
									score[p1][p2][p3][p4][p5][p6] = BLOCK3;
								}
							}

							//������ 
							else if (x == 0 && y == 3 || ix == 0 && iy == 3)
							{
								if (score[p1][p2][p3][p4][p5][p6] == 0)
								{
									score[p1][p2][p3][p4][p5][p6] = block3;
								}
							}

							//���߶�
							else if (x == 2 && y == 0 || ix == 2 && iy == 0)
							{
								if (score[p1][p2][p3][p4][p5][p6] == 0)
								{
									score[p1][p2][p3][p4][p5][p6] = BLOCK2;
								}
							}

							//���߶�
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

//�ж���Ӯ��Ҫ�õ�������
int fx[4] = { 0,-1,-1,-1 };
int fy[4] = { -1,0,-1,1 };
int tx[4] = { 0,1,1,1 };
int ty[4] = { 1,0,1,-1 };

//��ɫ:1 �Լ�,��ɫ:2 ������
//�ڵ����ݽṹ
struct Node {
	int N, QB, QW;	//ģ�����,��Ӯ����,��Ӯ����
	Node *first, *nxt;
	Node * newChild;
	Node * parent;
	int color;	//��ɫ�����һ�����µ���ɫ
	int x;	//�����
	int y;	//�����
	//unsigned int visited[15][15];
	int depth;	//̽�����
	int isTerminated;	//�Ƿ�Ϊ�վ�
	int turn;	//��ǰӦ���������ɫ
	bool hasExpanded;	//�Ƿ�����չ
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


Node* root;	//root ָ��
//��չ
Node* expand(Node*);
//ģ��
Node* simulate(Node*);

//���򴫲�
void packPropagation(Node*);

//UCT��ʽ
double evaluate(Node *r);
//��ӡboard
void printBoard();
//�ж���Ӯ�����Ӯ�򷵻���ɫ�����δӮ�򷵻�0
int judge(int(*)[15], int, int, int);

Node * select(Node* n) {
	int own = n->color;
	int opp = 1;
	if (own == 1) opp = 2;
	++(n->N);
	//�����վ�ֱ�ӷ���
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
	//���򴫲�������Ӯ
	if (leaf->success == 1) {
		++(n->QB);
	}
	else if (leaf->success == 2) {
		++(n->QW);
	}
	return leaf;
}

//UCT ��ʽ
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
//�������淵����õ�ǰN���㡣
Node * scoreBoard(Node * r) {
	return NULL;
}
void judgeScore(int(*board)[15], int color, int x, int y);
const int SIZE = 15;
//����������ĵ�(��Ҫͨ�������������м�֦����)
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
				//���ж��վ�
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

	//����������
	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			if (board[i][j]) continue;
			if (true) {
				//���
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
	//���Ͽ��ܻ����ĵ�
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

//color �ҵ���ɫ
const int INITWEIGHT = 1000;
void judgeScore(int(*board)[15], int color, int x, int y) {
	//printBoard();
	int opp = 1;
	if (color == 1) opp = 2;
	int blankNum;	//�����հײ���̫��
	int nx;
	int ny;
	int total_oppScore = 0;
	int total_selfScore = 0;
	int oppScore = INITWEIGHT;
	int selfScore = INITWEIGHT;
	int weight;	//��¼���˸���
	//��¼4���Խǵķ�����score�����
	for (int i = 0; i < 4; ++i) {
		//�����Լ��ķ���
		nx = x;
		ny = y;
		blankNum = 0;
		weight = INITWEIGHT;
		for (int j = 0; j < 4; ++j) {
			nx += fx[i];
			ny += fy[i];
			//�����߽�ͣ�²�����
			if (nx < 0 || nx>14 || ny < 0 || ny>14) {
				blankNum = 0;
				selfScore = selfScore / 10;
				break;
			}
			if (board[nx][ny] == color) {//�����Լ�����ɫ
				//������ͬ��ɫ�����
				blankNum = 0;
				selfScore = selfScore + weight * 10;
				weight = weight * 10;
			}
			else if (board[nx][ny] == opp) {//�������ֵ���ɫ
			   //ͣ�²�����
				blankNum = 0;
				selfScore = selfScore / 10;
				break;
			}
			else {	//û������
				if (blankNum > 1) {
					break;
				}
				else {
					//�����հ���ò�Ҫ̫��
					blankNum++;
					weight = weight / 10;
					continue;
				}

			}
		}
		//����һ���������¼���
		nx = x;
		ny = y;
		blankNum = 0;
		for (int j = 0; j < 4; ++j) {
			nx += tx[i];
			ny += ty[i];
			//�����߽�ͣ�²�����
			if (nx < 0 || nx>14 || ny < 0 || ny>14) {
				blankNum = 0;
				selfScore = selfScore / 10;
				break;
			}
			if (board[nx][ny] == color) {
				//������ͬ��ɫ�����
				blankNum = 0;
				selfScore = selfScore + weight * 10;
				weight = weight * 10;
			}
			else if (board[nx][ny] == opp) {//�������ֵ���ɫ
			   //ͣ�²�����
				blankNum = 0;
				selfScore = selfScore / 10;
				break;
			}
			else {
				if (blankNum > 1) {
					break;
				}
				else {
					//�����հ���ò�Ҫ̫��
					blankNum++;
					weight = weight / 10;
					continue;
				}
			}
		}

		//������ֵķ���
		nx = x;
		ny = y;
		blankNum = 0;
		weight = INITWEIGHT;
		for (int j = 0; j < 4; ++j) {
			nx += fx[i];
			ny += fy[i];
			//�����߽�ͣ�²�����
			if (nx < 0 || nx>14 || ny < 0 || ny>14) {
				blankNum = 0;
				oppScore = oppScore / 10;
				break;
			}
			if (board[nx][ny] == opp) {//�������ֵ���ɫ
				//������ͬ��ɫ�����
				blankNum = 0;
				oppScore = oppScore + weight * 10;
				weight = weight * 10;
			}
			else if (board[nx][ny] == color) {//�����Լ�����ɫ
				//ͣ�²�����
				blankNum = 0;
				oppScore = oppScore / 10;
				break;
			}
			else {	//û������
				if (blankNum > 1) {
					break;
				}
				else {
					//�����հ���ò�Ҫ̫��
					blankNum++;
					weight = weight / 10;
					continue;
				}

			}
		}
		nx = x;
		ny = y;
		//����һ���������¼���
		blankNum = 0;
		for (int j = 0; j < 4; ++j) {
			nx += tx[i];
			ny += ty[i];
			//�����߽�ͣ�²�����
			if (nx < 0 || nx>14 || ny < 0 || ny>14) {
				blankNum = 0;
				oppScore = oppScore / 10;
				break;
			}
			if (board[nx][ny] == opp) {
				//������ͬ��ɫ�����
				blankNum = 0;
				oppScore = oppScore + weight * 10;
				weight = weight * 10;
			}
			else if (board[nx][ny] == color) {//�����Լ�����ɫ��������
				blankNum = 0;
				oppScore = oppScore / 10;
				break;
			}
			else {
				if (blankNum > 1) {//�հ�̫��������
					break;
				}
				else {
					//�����հ���ò�Ҫ̫��
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

//�ж���Ӯ�����Ӯ�򷵻���ɫ�����δӮ�򷵻�0
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
//��ʼ������
void RecoverBoard(string str) {
	//����Json����
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);
	//����input�����֮ǰ�������Recover����
	int turnNum_rsp = input["responses"].size();
	int turnNum_req = input["requests"].size();
	//�Է�����
	int zero = 0;
	if (input["requests"][zero]["x"] != -1)
	{
		//�������Ļغ��Ƿ�ѡ���˻���,�����غϲ��ܴ�resopnses�б�
		if (turnNum_rsp >= 2 && input["responses"][1]["x"] == -1)
		{
			//����ǰ�µ���
			for (int i = 0; i < 2; i++)
			{
				//��������1
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 1;
				//�Է�����2
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 2;
			}
			//���ֺ��µ���
			for (int i = 2; i < turnNum_rsp; i++)
			{
				//�Է�����2
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 2;
				//��������1
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 1;
			}
			if (input["requests"][turnNum_rsp]["x"].asInt() >= 0 && input["requests"][turnNum_rsp]["y"].asInt() >= 0)
				board[input["requests"][turnNum_rsp]["x"].asInt()][input["requests"][turnNum_rsp]["y"].asInt()] = 2;
			//��������ߵ����һ���嵽ȫ�ֱ���x,y
			x = input["requests"][turnNum_rsp]["x"].asInt();
			y = input["requests"][turnNum_rsp]["y"].asInt();
		}
		//δ������غϻ�δ����
		else
		{
			for (int i = 0; i < turnNum_rsp; i++)
			{
				//�Է�����2
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 2;
				//��������1
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 1;
			}
			if (input["requests"][turnNum_rsp]["x"].asInt() >= 0 && input["requests"][turnNum_rsp]["y"].asInt() >= 0)
				board[input["requests"][turnNum_rsp]["x"].asInt()][input["requests"][turnNum_rsp]["y"].asInt()] = 2;
			//��������ߵ����һ���嵽ȫ�ֱ���x,y
			x = input["requests"][turnNum_rsp]["x"].asInt();
			y = input["requests"][turnNum_rsp]["y"].asInt();
		}
	}
	//��������
	else
	{
		//�Է�ѡ���˻���
		if (turnNum_req >= 3 && input["requests"][2]["x"] == -1)
		{
			//����ǰ�µ���
			for (int i = 0; i < 2; i++)
			{
				//��������1
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 1;
				//�Է�����2
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 2;
			}
			//���ֺ��µ���
			for (int i = 2; i < turnNum_rsp; i++)
			{
				//�Է�����2
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 2;
				//��������1
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 1;
			}
			if (input["requests"][turnNum_rsp]["x"].asInt() >= 0 && input["requests"][turnNum_rsp]["y"].asInt() >= 0)
				board[input["requests"][turnNum_rsp]["x"].asInt()][input["requests"][turnNum_rsp]["y"].asInt()] = 2;
			//��������ߵ����һ���嵽ȫ�ֱ���x,y
			//����غϽ��յ��ĶԷ����һ����-1,-1����任
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
		//δ�ﵽ����غϻ�δ����
		else
		{
			for (int i = 0; i < turnNum_rsp; i++)
			{
				//�Է�����2
				if (input["requests"][i]["x"].asInt() >= 0 && input["requests"][i]["y"].asInt() >= 0)
					board[input["requests"][i]["x"].asInt()][input["requests"][i]["y"].asInt()] = 2;
				//��������1
				if (input["responses"][i]["x"].asInt() >= 0 && input["responses"][i]["y"].asInt() >= 0)
					board[input["responses"][i]["x"].asInt()][input["responses"][i]["y"].asInt()] = 1;
			}
			if (input["requests"][turnNum_rsp]["x"].asInt() >= 0 && input["requests"][turnNum_rsp]["y"].asInt() >= 0)
				board[input["requests"][turnNum_rsp]["x"].asInt()][input["requests"][turnNum_rsp]["y"].asInt()] = 2;
			//��������ߵ����һ���嵽ȫ�ֱ���x,y
			x = input["requests"][turnNum_rsp]["x"].asInt();
			y = input["requests"][turnNum_rsp]["y"].asInt();
		}
	}
}
//��չ ������չ��ʱ�򷵻ص��Ƿ�Ϊ�վֽڵ�
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

//ģ��
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
	//��������ģ��
	memcpy(thisBoard, board, sizeof(thisBoard));

	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			if (thisBoard[i][j]) ++cnt;
		}
	}

	int pos = 0, x, y;

	//һ����ģ�⵽�վ�
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
				//���ȡ��
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

				thisBoard[x][y] = 3;//�ⲽû��Ҫ
				return n;
			}
			//�����´��������ɫ
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
	//����״̬
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
	double duration = 0;	//����ʱ��
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