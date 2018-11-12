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
//����������ʼȨֵ
const int INITWEIGHT = 10000;

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
	long levelScore;	//�㼶����
	long score;		//��������

	Node(int c, int t, int x, int y , long scr, long level_Score, Node * p = nullptr) {
		parent = p;
		color = c;
		
		levelScore = level_Score;
		score = scr;

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
		//����չ�����һ���ڵ��Ӧ��ע���ʱ���������е㶼��ռ���ˣ�������Ҳû��child�ˡ���
		if (n->newChild == NULL) {
			n->isTerminated = true;
			n->success = -1;
			return n;
		}
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
		
	}else if (n->newChild) {
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

//UCT ��ʽ + ��̬��������
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
	
	//��̬��������
	double s3 = (double)(r->score)/double(r->levelScore);
	double weight = 1 / (double)((r->N) + 1);

	//ȡȨֵ��ģ�����Խ����̬��������Ӱ��ԽС
	return (s1 + s2)* (1-weight) + s3 * weight;
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
			pointScore[i*SIZE + j].score = -INITWEIGHT*10;
		}
	}
	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			if (board[i][j]) continue;
			if (true) {
				//���ж��վ�
				if (judge(board, c, i, j) == c) {
					Node *v = new Node(c, n->color, i, j,1,1, n);
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
#ifdef DEBUG
	//cout <<"4,9:"<<pointScore[4 * 15 + 9].score<<endl;
	//cout <<"10,9:"<<pointScore[10 * 15 + 9].score<<endl;
#endif
	sort(pointScore, pointScore + 15 * 15, cmpScore);
	
	//��̬��������������Ҫʹ��
	double levelScore = 0;
	for (int i = 0; i < expand_CNT; ++i) {
		if (!board[pointScore[i].x][pointScore[i].y]) {
			levelScore = levelScore + pointScore[i].score;
		}
	}
	int final_exp = expand_CNT;
	
	while (true) {
		if (pointScore[final_exp].score >= 222000) {
			levelScore = levelScore + pointScore[final_exp].score;
			final_exp++;
		}
		else {
			break;
		}
	}
	for (int i = 0; i < expand_CNT; ++i) {
		//�����������û����ô����ˣ�����Ҫ���ǵ�
		if (!board[pointScore[i].x][pointScore[i].y]) {

			u = new Node(c, n->color, pointScore[i].x, pointScore[i].y,pointScore[i].score,levelScore, n);
#ifdef DEBUG
			//cout <<"x:"<< pointScore[i].x<<"y:"<< pointScore[i].y <<"score:" << pointScore[i].score<<endl;
#endif 
			u->nxt = v;
			v = u;
		}
	}

	//���Ͽ��ܻ����ĵ�
	final_exp = expand_CNT;
	while (true){
		if (pointScore[final_exp].score >= 222000){
			u = new Node(c, n->color, pointScore[final_exp].x, pointScore[final_exp].y, pointScore[final_exp].score,levelScore, n);
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

//�����������
int* buildRandomSequence(int length) {
	int* array = new int[length];
	for (int i = 0; i < length; i++) {
		array[i] = i;
	}
	int x = 0, tmp = 0;
	for (int i = length - 1; i > 0; i--) {
		//x = random.next(0, i + 1);
		//x = xorshf96() % i;
		x = uidis(engine) % i;
		tmp = array[i];
		array[i] = array[x];
		array[x] = tmp;
	}
	return array;
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
	bool board_Flag[225];
	memset(board_Flag, false, sizeof(board_Flag));
	
	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			if (thisBoard[i][j]) {
				++cnt;
				board_Flag[i * 15 + j] = true;
			}
		}
	}
	int pos=0 , i=0, x, y;
	//һ����ģ�⵽�վ�
	int* rand_perm = buildRandomSequence(SIZE*SIZE);
	
	while (cnt <= 225) {
		for (;i<225;) {
			if ( board_Flag[ rand_perm[i] ] ){
				//�Ѿ�������ȡ��һ��
				i++;
			}else{
				pos = rand_perm[i];
 				x = pos / 15;
				y = pos % 15;
				i++;
				break;
			}
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
		pos = 0;
	}
	n->success = -1;
	return n;
}


void printBoard() {
	//����״̬
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (board[j][i] == 0)
				std::cout << "0 ";
			else if (board[j][i] == 1)
				std::cout << "* ";
			else std::cout << "# ";
		}
		std::cout << '\n';
	}
	//cin.get();
}
void printNode(Node * r) {
	if (!r) return;
	cout << "color " << r->color << " x " << r->x << " y " << r->y << " QB/QW/N " << r->QB << " " << r->QW << " " << r->N << " isTerminated " << r->isTerminated << " depth " << r->depth<<"score:"<<(double)(r->score)/(double) (r->levelScore) << endl;
	for (Node *u = r->first; u; u = u->nxt) {
		//printNode(u);
		cout << "color " << u->color << " x " << u->x << " y " << u->y << " QB/QW/N " << u->QB << " " << u->QW << " " << u->N << " isTerminated " << u->isTerminated << " depth " << u->depth << "score:" << (double)(u->score)/ (double)(u->levelScore) << endl;
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
	str = string("{\"requests\":[{\"x\":-1,\"y\":-1},{\"x\":6,\"y\":9},{\"x\":7,\"y\":9},{\"x\":9,\"y\":10},{\"x\":10,\"y\":7},{\"x\":6,\"y\":8},{\"x\":8,\"y\":7},{\"x\":5,\"y\":12},{\"x\":10,\"y\":8},{\"x\":7,\"y\":7},{\"x\":11,\"y\":11},{\"x\":9,\"y\":5},{\"x\":8,\"y\":11},{\"x\":11,\"y\":6},{\"x\":5,\"y\":10},{\"x\":7,\"y\":11},{\"x\":11,\"y\":5},{\"x\":12,\"y\":8},{\"x\":6,\"y\":14},{\"x\":12,\"y\":9},{\"x\":6,\"y\":6},{\"x\":7,\"y\":13},{\"x\":6,\"y\":4},{\"x\":4,\"y\":5},{\"x\":12,\"y\":3},{\"x\":7,\"y\":3},{\"x\":8,\"y\":5},{\"x\":10,\"y\":4},{\"x\":6,\"y\":5},{\"x\":6,\"y\":1},{\"x\":9,\"y\":2},{\"x\":10,\"y\":9},{\"x\":5,\"y\":2},{\"x\":13,\"y\":10},{\"x\":8,\"y\":0},{\"x\":11,\"y\":1},{\"x\":12,\"y\":14},{\"x\":13,\"y\":5},{\"x\":5,\"y\":8},{\"x\":2,\"y\":6},{\"x\":10,\"y\":3},{\"x\":1,\"y\":9},{\"x\":5,\"y\":7},{\"x\":7,\"y\":1},{\"x\":3,\"y\":4},{\"x\":3,\"y\":9},{\"x\":1,\"y\":3},{\"x\":4,\"y\":7},{\"x\":6,\"y\":3},{\"x\":2,\"y\":11},{\"x\":4,\"y\":10},{\"x\":0,\"y\":11},{\"x\":0,\"y\":5},{\"x\":4,\"y\":12},{\"x\":0,\"y\":8},{\"x\":4,\"y\":3},{\"x\":10,\"y\":12},{\"x\":2,\"y\":2},{\"x\":1,\"y\":8},{\"x\":14,\"y\":2},{\"x\":4,\"y\":2},{\"x\":3,\"y\":2},{\"x\":3,\"y\":13},{\"x\":3,\"y\":1},{\"x\":4,\"y\":0},{\"x\":14,\"y\":10},{\"x\":0,\"y\":0},{\"x\":1,\"y\":14},{\"x\":7,\"y\":14},{\"x\":12,\"y\":6},{\"x\":14,\"y\":8},{\"x\":14,\"y\":7},{\"x\":10,\"y\":13},{\"x\":13,\"y\":0},{\"x\":11,\"y\":13},{\"x\":9,\"y\":13},{\"x\":12,\"y\":13},{\"x\":11,\"y\":12},{\"x\":13,\"y\":14},{\"x\":12,\"y\":12},{\"x\":14,\"y\":12},{\"x\":14,\"y\":14},{\"x\":10,\"y\":14}],\"responses\":[{\"x\":7,\"y\":10},{\"x\":7,\"y\":8},{\"x\":8,\"y\":9},{\"x\":9,\"y\":8},{\"x\":8,\"y\":8},{\"x\":8,\"y\":10},{\"x\":6,\"y\":11},{\"x\":9,\"y\":9},{\"x\":10,\"y\":10},{\"x\":9,\"y\":7},{\"x\":10,\"y\":6},{\"x\":8,\"y\":6},{\"x\":9,\"y\":6},{\"x\":6,\"y\":10},{\"x\":6,\"y\":12},{\"x\":9,\"y\":11},{\"x\":11,\"y\":9},{\"x\":6,\"y\":13},{\"x\":11,\"y\":8},{\"x\":10,\"y\":5},{\"x\":11,\"y\":4},{\"x\":7,\"y\":5},{\"x\":6,\"y\":7},{\"x\":7,\"y\":4},{\"x\":7,\"y\":6},{\"x\":9,\"y\":4},{\"x\":8,\"y\":3},{\"x\":5,\"y\":6},{\"x\":12,\"y\":11},{\"x\":6,\"y\":2},{\"x\":11,\"y\":10},{\"x\":8,\"y\":2},{\"x\":12,\"y\":10},{\"x\":8,\"y\":4},{\"x\":9,\"y\":3},{\"x\":10,\"y\":2},{\"x\":11,\"y\":3},{\"x\":4,\"y\":6},{\"x\":5,\"y\":9},{\"x\":12,\"y\":5},{\"x\":5,\"y\":5},{\"x\":5,\"y\":3},{\"x\":3,\"y\":5},{\"x\":3,\"y\":6},{\"x\":7,\"y\":0},{\"x\":13,\"y\":11},{\"x\":8,\"y\":1},{\"x\":5,\"y\":4},{\"x\":1,\"y\":10},{\"x\":0,\"y\":10},{\"x\":4,\"y\":9},{\"x\":2,\"") + string("y\":7},{\"x\":5,\"y\":11},{\"x\":0,\"y\":6},{\"x\":2,\"y\":10},{\"x\":2,\"y\":5},{\"x\":4,\"y\":11},{\"x\":3,\"y\":10},{\"x\":11,\"y\":2},{\"x\":1,\"y\":5},{\"x\":4,\"y\":1},{\"x\":1,\"y\":2},{\"x\":0,\"y\":4},{\"x\":3,\"y\":0},{\"x\":2,\"y\":8},{\"x\":0,\"y\":3},{\"x\":4,\"y\":4},{\"x\":3,\"y\":11},{\"x\":8,\"y\":14},{\"x\":14,\"y\":4},{\"x\":13,\"y\":7},{\"x\":14,\"y\":9},{\"x\":13,\"y\":8},{\"x\":7,\"y\":12},{\"x\":11,\"y\":7},{\"x\":8,\"y\":13},{\"x\":13,\"y\":13},{\"x\":8,\"y\":12},{\"x\":10,\"y\":11},{\"x\":9,\"y\":12},{\"x\":13,\"y\":12},{\"x\":7,\"y\":2}]}");
#else
	getline(cin, str);
#endif // DEBUG
	RecoverBoard(str);
#ifdef DEBUG
	printBoard();
#endif
	//******************************************************
	//����ִ�ף���4�غ�����������
	//����Json����
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);
	//����input�����֮ǰ�������Recover����
	int turnNum_rsp = input["responses"].size();
	int turnNum_req = input["requests"].size();
	//�Է�����
	int zero = 0;
	if (input["requests"][zero]["x"] != -1 && turnNum_rsp==1)
	{
		Json::Value ret;
		Json::Value act;
		act["x"] = -1;
		act["y"] = -1;

		ret["response"] = act;
		Json::FastWriter writer;
		std::cout << writer.write(ret) << endl;
		return 0;
	}
	/*****************************����ִ�ڣ��Էϵ�һ�ӣ����Ľ�֮һ����������**************************************/
	else if (input["requests"][zero]["x"] == -1 && turnNum_rsp == 0)
	{
		Json::Value ret;
		Json::Value act;
		act["x"] = 0;
		act["y"] = 0;

		ret["response"] = act;
		Json::FastWriter writer;
		std::cout << writer.write(ret) << endl;
		return 0;
	}
	//*****************************************************

	Node * leaf = nullptr;
	int color = 2;
	
	int turn = 1;
	root = new Node(color, turn, x, y,-1,-1, nullptr);

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
				board[v->x][v->y] = v->color;
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