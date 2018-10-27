#include <iostream>
#include <ctime>
#include <time.h>
#include "jsoncpp/json.h"
using namespace std;
int board[15][15];
int sec = 1000;
const double RuningTime = 0.85f;
float confident = 1.96;
int equivalence = 1000;
int x;//���һ���µĵط�
int y;//���һ���µĵط�

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
		//memset(visited, 0, sizeof(visited));
		if (p) depth = p->depth + 1;
		else depth = 0;
		isTerminated = false;
		hasExpanded = false;
		turn = t;
		success = 0;
	}
};


Node* root;	//root ָ��
Node* expand(Node*);

Node * select(Node* n) {
	//
	if (n->isTerminated) {
		if (n->success == 1) ++(n->QB);
		else if (n->success == 2) ++(n->QW);
		return n;
	}
	if (n->hasExpanded) {
		expand(n);
		n->hasExpanded = true;
	}
	return NULL;
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
//��չ
Node * expand(Node* n) {
	return NULL;
}
//ģ��
Node * simulate(Node* n) {
	return NULL;
}
//���򴫲�
void packPropagation(Node* n) {

}
int main() {
	memset(board, 0, sizeof(board));
	double duration = 0;	//����ʱ��
	clock_t timeStart = clock();
	string str = string("{\"requests\":[{\"x\":-1,\"y\":-1},{\"x\":0,\"y\":14},{\"x\":1,\"y\":4},{\"x\":11,\"y\":1},{\"x\":4,\"y\":14},{\"x\":11,\"y\":12},{\"x\":0,\"y\":4},{\"x\":0,\"y\":8},{\"x\":7,\"y\":6},{\"x\":7,\"y\":0},{\"x\":12,\"y\":3},{\"x\":7,\"y\":2},{\"x\":5,\"y\":4},{\"x\":6,\"y\":4},{\"x\":8,\"y\":2},{\"x\":13,\"y\":10},{\"x\":1,\"y\":1},{\"x\":14,\"y\":3},{\"x\":5,\"y\":11},{\"x\":0,\"y\":2},{\"x\":5,\"y\":1},{\"x\":12,\"y\":13},{\"x\":4,\"y\":13},{\"x\":10,\"y\":2},{\"x\":0,\"y\":9},{\"x\":1,\"y\":8},{\"x\":11,\"y\":4},{\"x\":11,\"y\":11},{\"x\":14,\"y\":9},{\"x\":6,\"y\":1},{\"x\":12,\"y\":12},{\"x\":5,\"y\":0},{\"x\":0,\"y\":11},{\"x\":3,\"y\":11},{\"x\":13,\"y\":1},{\"x\":8,\"y\":0},{\"x\":3,\"y\":5},{\"x\":9,\"y\":4},{\"x\":10,\"y\":7},{\"x\":10,\"y\":5},{\"x\":13,\"y\":9},{\"x\":10,\"y\":6},{\"x\":8,\"y\":1},{\"x\":4,\"y\":12},{\"x\":6,\"y\":9},{\"x\":10,\"y\":14},{\"x\":14,\"y\":4},{\"x\":7,\"y\":1},{\"x\":13,\"y\":0},{\"x\":13,\"y\":14},{\"x\":10,\"y\":4},{\"x\":3,\"y\":4},{\"x\":4,\"y\":8},{\"x\":7,\"y\":12},{\"x\":4,\"y\":4},{\"x\":4,\"y\":6},{\"x\":7,\"y\":13},{\"x\":8,\"y\":6},{\"x\":13,\"y\":7},{\"x\":5,\"y\":12},{\"x\":2,\"y\":14},{\"x\":12,\"y\":14},{\"x\":7,\"y\":7},{\"x\":5,\"y\":9},{\"x\":11,\"y\":2},{\"x\":9,\"y\":3},{\"x\":8,\"y\":11},{\"x\":12,\"y\":8},{\"x\":9,\"y\":8},{\"x\":8,\"y\":9},{\"x\":5,\"y\":14}],\"responses\":[{\"x\":6,\"y\":2},{\"x\":11,\"y\":7},{\"x\":7,\"y\":11},{\"x\":3,\"y\":14},{\"x\":3,\"y\":3},{\"x\":11,\"y\":6},{\"x\":8,\"y\":13},{\"x\":9,\"y\":12},{\"x\":5,\"y\":8},{\"x\":9,\"y\":0},{\"x\":9,\"y\":5},{\"x\":12,\"y\":6},{\"x\":5,\"y\":5},{\"x\":2,\"y\":4},{\"x\":14,\"y\":14},{\"x\":9,\"y\":9},{\"x\":11,\"y\":3},{\"x\":10,\"y\":1},{\"x\":10,\"y\":9},{\"x\":5,\"y\":3},{\"x\":1,\"y\":3},{\"x\":12,\"y\":1},{\"x\":13,\"y\":6},{\"x\":2,\"y\":13},{\"x\":0,\"y\":1},{\"x\":2,\"y\":0},{\"x\":6,\"y\":12},{\"x\":3,\"y\":13},{\"x\":9,\"y\":13},{\"x\":9,\"y\":11},{\"x\":12,\"y\":10},{\"x\":6,\"y\":13},{\"x\":2,\"y\":7},{\"x\":14,\"y\":5},{\"x\":2,\"y\":2},{\"x\":4,\"y\":2},{\"x\":1,\"y\":5},{\"x\":14,\"y\":12},{\"x\":3,\"y\":10},{\"x\":13,\"y\":4},{\"x\":13,\"y\":13},{\"x\":1,\"y\":9},{\"x\":12,\"y\":9},{\"x\":5,\"y\":2},{\"x\":10,\"y\":8},{\"x\":7,\"y\":3},{\"x\":4,\"y\":1},{\"x\":2,\"y\":6},{\"x\":5,\"y\":13},{\"x\":6,\"y\":6},{\"x\":6,\"y\":11},{\"x\":8,\"y\":14},{\"x\":6,\"y\":7},{\"x\":0,\"y\":0},{\"x\":1,\"y\":2},{\"x\":13,\"y\":2},{\"x\":10,\"y\":11},{\"x\":8,\"y\":4},{\"x\":6,\"y\":10},{\"x\":6,\"y\":0},{\"x\":1,\"y\":7},{\"x\":2,\"y\":1},{\"x\":12,\"y\":7},{\"x\":8,\"y\"") + string(":12},{\"x\":6,\"y\":3},{\"x\":9,\"y\":14},{\"x\":3,\"y\":8},{\"x\":6,\"y\":5},{\"x\":7,\"y\":8},{\"x\":6,\"y\":8}]}");

	RecoverBoard(str);
	//����״̬
	for (int i = 0; i < 15; i++) {
		for (int j = 0; j < 15; j++) {
			if (board[i][j] == 0)
				cout << '0';
			else if (board[i][j] == 1)
				cout << '*';
			else cout << '#';
		}
		cout << '\n';
	}
	cin.get();
	/*
	Node * leaf = nullptr;

	int color = 2;
	int turn = 1;
	root = new Node(color, turn, x, y, nullptr);

	for(;;){
		duration = (double)(clock() - timeStart) / CLOCKS_PER_SEC;
		if (duration >= RuningTime) {
			// ʱ�䵽������õĵ�

		} else {
			//MCTS
			leaf = select(root);
		}
	}
	*/
	return 0;
}