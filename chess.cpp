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
int x;//最后一步下的地方
int y;//最后一步下的地方

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
		//memset(visited, 0, sizeof(visited));
		if (p) depth = p->depth + 1;
		else depth = 0;
		isTerminated = false;
		hasExpanded = false;
		turn = t;
		success = 0;
	}
};


Node* root;	//root 指针
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

//初始化棋盘
void RecoverBoard(string str) {
	//解析Json数据
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);
	//分析input输入和之前的输出，Recover棋盘
	int turnNum = input["responses"].size();
	for (int i = 0; i < turnNum; i++)
	{
		//对方落子2
		if (input["requests"][i]["y"].asInt() >= 0 && input["requests"][i]["x"].asInt() >= 0)
			board[input["requests"][i]["y"].asInt()][input["requests"][i]["x"].asInt()] = 2;
		//己方落子1
		if (input["responses"][i]["y"].asInt() >= 0 && input["responses"][i]["x"].asInt() >= 0)
			board[input["responses"][i]["y"].asInt()][input["responses"][i]["x"].asInt()] = 1;
	}
	if (input["requests"][turnNum]["y"].asInt() >= 0 && input["requests"][turnNum]["x"].asInt() >= 0)
		board[input["requests"][turnNum]["y"].asInt()][input["requests"][turnNum]["x"].asInt()] = 2;
}
//扩展
Node * expand(Node* n) {
	return NULL;
}
//模拟
Node * simulate(Node* n) {
	return NULL;
}
//反向传播
void packPropagation(Node* n) {

}
int main() {
	memset(board, 0, sizeof(board));
	double duration = 0;	//运行时间
	clock_t timeStart = clock();
	string str = string("{\"requests\":[{\"x\":-1,\"y\":-1},{\"x\":0,\"y\":14},{\"x\":1,\"y\":4},{\"x\":11,\"y\":1},{\"x\":4,\"y\":14},{\"x\":11,\"y\":12},{\"x\":0,\"y\":4},{\"x\":0,\"y\":8},{\"x\":7,\"y\":6},{\"x\":7,\"y\":0},{\"x\":12,\"y\":3},{\"x\":7,\"y\":2},{\"x\":5,\"y\":4},{\"x\":6,\"y\":4},{\"x\":8,\"y\":2},{\"x\":13,\"y\":10},{\"x\":1,\"y\":1},{\"x\":14,\"y\":3},{\"x\":5,\"y\":11},{\"x\":0,\"y\":2},{\"x\":5,\"y\":1},{\"x\":12,\"y\":13},{\"x\":4,\"y\":13},{\"x\":10,\"y\":2},{\"x\":0,\"y\":9},{\"x\":1,\"y\":8},{\"x\":11,\"y\":4},{\"x\":11,\"y\":11},{\"x\":14,\"y\":9},{\"x\":6,\"y\":1},{\"x\":12,\"y\":12},{\"x\":5,\"y\":0},{\"x\":0,\"y\":11},{\"x\":3,\"y\":11},{\"x\":13,\"y\":1},{\"x\":8,\"y\":0},{\"x\":3,\"y\":5},{\"x\":9,\"y\":4},{\"x\":10,\"y\":7},{\"x\":10,\"y\":5},{\"x\":13,\"y\":9},{\"x\":10,\"y\":6},{\"x\":8,\"y\":1},{\"x\":4,\"y\":12},{\"x\":6,\"y\":9},{\"x\":10,\"y\":14},{\"x\":14,\"y\":4},{\"x\":7,\"y\":1},{\"x\":13,\"y\":0},{\"x\":13,\"y\":14},{\"x\":10,\"y\":4},{\"x\":3,\"y\":4},{\"x\":4,\"y\":8},{\"x\":7,\"y\":12},{\"x\":4,\"y\":4},{\"x\":4,\"y\":6},{\"x\":7,\"y\":13},{\"x\":8,\"y\":6},{\"x\":13,\"y\":7},{\"x\":5,\"y\":12},{\"x\":2,\"y\":14},{\"x\":12,\"y\":14},{\"x\":7,\"y\":7},{\"x\":5,\"y\":9},{\"x\":11,\"y\":2},{\"x\":9,\"y\":3},{\"x\":8,\"y\":11},{\"x\":12,\"y\":8},{\"x\":9,\"y\":8},{\"x\":8,\"y\":9},{\"x\":5,\"y\":14}],\"responses\":[{\"x\":6,\"y\":2},{\"x\":11,\"y\":7},{\"x\":7,\"y\":11},{\"x\":3,\"y\":14},{\"x\":3,\"y\":3},{\"x\":11,\"y\":6},{\"x\":8,\"y\":13},{\"x\":9,\"y\":12},{\"x\":5,\"y\":8},{\"x\":9,\"y\":0},{\"x\":9,\"y\":5},{\"x\":12,\"y\":6},{\"x\":5,\"y\":5},{\"x\":2,\"y\":4},{\"x\":14,\"y\":14},{\"x\":9,\"y\":9},{\"x\":11,\"y\":3},{\"x\":10,\"y\":1},{\"x\":10,\"y\":9},{\"x\":5,\"y\":3},{\"x\":1,\"y\":3},{\"x\":12,\"y\":1},{\"x\":13,\"y\":6},{\"x\":2,\"y\":13},{\"x\":0,\"y\":1},{\"x\":2,\"y\":0},{\"x\":6,\"y\":12},{\"x\":3,\"y\":13},{\"x\":9,\"y\":13},{\"x\":9,\"y\":11},{\"x\":12,\"y\":10},{\"x\":6,\"y\":13},{\"x\":2,\"y\":7},{\"x\":14,\"y\":5},{\"x\":2,\"y\":2},{\"x\":4,\"y\":2},{\"x\":1,\"y\":5},{\"x\":14,\"y\":12},{\"x\":3,\"y\":10},{\"x\":13,\"y\":4},{\"x\":13,\"y\":13},{\"x\":1,\"y\":9},{\"x\":12,\"y\":9},{\"x\":5,\"y\":2},{\"x\":10,\"y\":8},{\"x\":7,\"y\":3},{\"x\":4,\"y\":1},{\"x\":2,\"y\":6},{\"x\":5,\"y\":13},{\"x\":6,\"y\":6},{\"x\":6,\"y\":11},{\"x\":8,\"y\":14},{\"x\":6,\"y\":7},{\"x\":0,\"y\":0},{\"x\":1,\"y\":2},{\"x\":13,\"y\":2},{\"x\":10,\"y\":11},{\"x\":8,\"y\":4},{\"x\":6,\"y\":10},{\"x\":6,\"y\":0},{\"x\":1,\"y\":7},{\"x\":2,\"y\":1},{\"x\":12,\"y\":7},{\"x\":8,\"y\"") + string(":12},{\"x\":6,\"y\":3},{\"x\":9,\"y\":14},{\"x\":3,\"y\":8},{\"x\":6,\"y\":5},{\"x\":7,\"y\":8},{\"x\":6,\"y\":8}]}");

	RecoverBoard(str);
	//棋盘状态
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
			// 时间到，找最好的点

		} else {
			//MCTS
			leaf = select(root);
		}
	}
	*/
	return 0;
}