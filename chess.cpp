#include <iostream>
#include <ctime>
#include <time.h>
#include<random>
#include<algorithm>
#include "jsoncpp/json.h"

//#define DEBUG

using namespace std;
int board[15][15];
int sec = 1000;
const double RuningTime = 0.85f;
float confident = 1.96;
int equivalence = 1000;
int x = -1;//���һ���µĵط�
int y = -1;//���һ���µĵط�

//const double C = 1.0f;
const double C = 1.96f;

//���������
default_random_engine engine(time(nullptr));
uniform_int_distribution<int> uidis(0, 224);


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
		//Node * newChild=new Node(opp,)
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
			
			if (x != -1 && y != -1) {
				board[x][y] = bestChild->color;
				leaf = select(bestChild);
				board[x][y] = 0;
			}
			else {
				//switchRole();
				leaf = select(bestChild);
				//switchRole();
			}
			
			//++(r->Q);
			//if (bestChild->success == 1) ++(r->N);
		}

	}
	else if (n->newChild) {
		bestChild = n->newChild;
		n->newChild = bestChild->nxt;
		bestChild->nxt = n->first;
		n->first = bestChild;

		leaf = simulate(bestChild);

	}


	if (leaf->success == 1) ++(n->QB);
	else if (leaf->success == 2)++(n->QW);

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
//���ؾ����ڵĵ�
Node * euldVis(Node * n) {
	int c = 1;
	if (n->color == 1) c = 2;

	Node *u = nullptr;
	Node *v = nullptr;

	//���ֿ�����expand�ڣ���
	/*if (n->turn == 3) {
		u = new Node(n->color, n->turn + 1, -1, -1, n);
		u->depth = n->depth + 1;
		v = u;
	}*/

	//���ڵ���ڣ����ڵ㲻�ǵ�һ������
	/*int x2 = -2, y2 = -2;
	if (n->parent && n->parent->x != -1) {
		x2 = n->parent->x;
		y2 = n->parent->y;
	}*/

	int dis1 = -1, dis2 = -1;

	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			if (board[i][j]) continue;
			if (true) {
				u = new Node(c, n->color, i, j, n);
				//u->depth = n->depth + 1;
				u->nxt = v;
				v = u;
				if (judge(board, c, i, j) == c) {
					v->success = v->isTerminated = c;
					v->nxt = nullptr;
					v->hasExpanded = true;
					return v;
				}
			}
		}
	}
	
	return u;
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
	int turnNum = input["responses"].size();
	for (int i = 0; i < turnNum; i++)
	{
		//�Է�����2
		if (input["requests"][i]["y"].asInt() >= 0 && input["requests"][i]["x"].asInt() >= 0)
			board[input["requests"][i]["y"].asInt()][input["requests"][i]["x"].asInt()] = 2;
		//��������1
		if (input["responses"][i]["y"].asInt() >= 0 && input["responses"][i]["x"].asInt() >= 0)
			board[input["responses"][i]["y"].asInt()][input["responses"][i]["x"].asInt()] = 1;
	}
	if (input["requests"][turnNum]["y"].asInt() >= 0 && input["requests"][turnNum]["x"].asInt() >= 0)
		board[input["requests"][turnNum]["y"].asInt()][input["requests"][turnNum]["x"].asInt()] = 2;
}
//��չ ������չ��ʱ�򷵻ص��Ƿ�Ϊ�վֽڵ�
Node * expand(Node* n) {
	int c = 1;
	if (n->color == 1) c = 2;

	for (int i = 0; i < 15; ++i) {
		for (int j = 0; j < 15; ++j) {
			if (board[i][j] == 0) {
				//���Ӯ�ˣ���ֱ���վ�
				if (judge(board, c, i, j) == c) {
					
#ifdef DEBUG
					board[i][j] = 2 + c;
					printBoard();
					board[i][j] = 0;
#endif

					Node *v = new Node(c, n->turn + 1, i, j, n);
					v->depth = n->depth + 1;
					v->nxt = nullptr;
					v->success = v->isTerminated = c;
					v->hasExpanded = true;
					n->newChild = v;
					return n;
				}
			}
		}
	}

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

//���򴫲�����
/*
void packPropagation(Node* n) {

}
*/

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
	//string str = string("{\"requests\":[{\"x\":-1,\"y\":-1},{\"x\":0,\"y\":14},{\"x\":1,\"y\":4},{\"x\":11,\"y\":1},{\"x\":4,\"y\":14},{\"x\":11,\"y\":12},{\"x\":0,\"y\":4},{\"x\":0,\"y\":8},{\"x\":7,\"y\":6},{\"x\":7,\"y\":0},{\"x\":12,\"y\":3},{\"x\":7,\"y\":2},{\"x\":5,\"y\":4},{\"x\":6,\"y\":4},{\"x\":8,\"y\":2},{\"x\":13,\"y\":10},{\"x\":1,\"y\":1},{\"x\":14,\"y\":3},{\"x\":5,\"y\":11},{\"x\":0,\"y\":2},{\"x\":5,\"y\":1},{\"x\":12,\"y\":13},{\"x\":4,\"y\":13},{\"x\":10,\"y\":2},{\"x\":0,\"y\":9},{\"x\":1,\"y\":8},{\"x\":11,\"y\":4},{\"x\":11,\"y\":11},{\"x\":14,\"y\":9},{\"x\":6,\"y\":1},{\"x\":12,\"y\":12},{\"x\":5,\"y\":0},{\"x\":0,\"y\":11},{\"x\":3,\"y\":11},{\"x\":13,\"y\":1},{\"x\":8,\"y\":0},{\"x\":3,\"y\":5},{\"x\":9,\"y\":4},{\"x\":10,\"y\":7},{\"x\":10,\"y\":5},{\"x\":13,\"y\":9},{\"x\":10,\"y\":6},{\"x\":8,\"y\":1},{\"x\":4,\"y\":12},{\"x\":6,\"y\":9},{\"x\":10,\"y\":14},{\"x\":14,\"y\":4},{\"x\":7,\"y\":1},{\"x\":13,\"y\":0},{\"x\":13,\"y\":14},{\"x\":10,\"y\":4},{\"x\":3,\"y\":4},{\"x\":4,\"y\":8},{\"x\":7,\"y\":12},{\"x\":4,\"y\":4},{\"x\":4,\"y\":6},{\"x\":7,\"y\":13},{\"x\":8,\"y\":6},{\"x\":13,\"y\":7},{\"x\":5,\"y\":12},{\"x\":2,\"y\":14},{\"x\":12,\"y\":14},{\"x\":7,\"y\":7},{\"x\":5,\"y\":9},{\"x\":11,\"y\":2},{\"x\":9,\"y\":3},{\"x\":8,\"y\":11},{\"x\":12,\"y\":8},{\"x\":9,\"y\":8},{\"x\":8,\"y\":9},{\"x\":5,\"y\":14}],\"responses\":[{\"x\":6,\"y\":2},{\"x\":11,\"y\":7},{\"x\":7,\"y\":11},{\"x\":3,\"y\":14},{\"x\":3,\"y\":3},{\"x\":11,\"y\":6},{\"x\":8,\"y\":13},{\"x\":9,\"y\":12},{\"x\":5,\"y\":8},{\"x\":9,\"y\":0},{\"x\":9,\"y\":5},{\"x\":12,\"y\":6},{\"x\":5,\"y\":5},{\"x\":2,\"y\":4},{\"x\":14,\"y\":14},{\"x\":9,\"y\":9},{\"x\":11,\"y\":3},{\"x\":10,\"y\":1},{\"x\":10,\"y\":9},{\"x\":5,\"y\":3},{\"x\":1,\"y\":3},{\"x\":12,\"y\":1},{\"x\":13,\"y\":6},{\"x\":2,\"y\":13},{\"x\":0,\"y\":1},{\"x\":2,\"y\":0},{\"x\":6,\"y\":12},{\"x\":3,\"y\":13},{\"x\":9,\"y\":13},{\"x\":9,\"y\":11},{\"x\":12,\"y\":10},{\"x\":6,\"y\":13},{\"x\":2,\"y\":7},{\"x\":14,\"y\":5},{\"x\":2,\"y\":2},{\"x\":4,\"y\":2},{\"x\":1,\"y\":5},{\"x\":14,\"y\":12},{\"x\":3,\"y\":10},{\"x\":13,\"y\":4},{\"x\":13,\"y\":13},{\"x\":1,\"y\":9},{\"x\":12,\"y\":9},{\"x\":5,\"y\":2},{\"x\":10,\"y\":8},{\"x\":7,\"y\":3},{\"x\":4,\"y\":1},{\"x\":2,\"y\":6},{\"x\":5,\"y\":13},{\"x\":6,\"y\":6},{\"x\":6,\"y\":11},{\"x\":8,\"y\":14},{\"x\":6,\"y\":7},{\"x\":0,\"y\":0},{\"x\":1,\"y\":2},{\"x\":13,\"y\":2},{\"x\":10,\"y\":11},{\"x\":8,\"y\":4},{\"x\":6,\"y\":10},{\"x\":6,\"y\":0},{\"x\":1,\"y\":7},{\"x\":2,\"y\":1},{\"x\":12,\"y\":7},{\"x\":8,\"y\"") + string(":12},{\"x\":6,\"y\":3},{\"x\":9,\"y\":14},{\"x\":3,\"y\":8},{\"x\":6,\"y\":5},{\"x\":7,\"y\":8},{\"x\":6,\"y\":8}]}");
	//string str = string("{\"requests\":[{\"x\":-1,\"y\":-1}],\"responses\":[]}");
	string str;
	getline(cin, str);
	RecoverBoard(str);
	Node * leaf = nullptr;

	int color = 2;

	int turn = 1;
	root = new Node(color, turn, x, y, nullptr);

	for(;;){
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