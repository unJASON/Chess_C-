#include <iostream>
#include <ctime>
#include <time.h>
using namespace std;
int board[15][15];
int sec = 1000;
const double RuningTime = 0.85f;
float confident = 1.96;
int equivalence = 1000;
int x;//最后一步下的地方
int y;//最后一步下的地方
Node* root;	//root 指针
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
	

	Node(int c,int t,int x,int y,Node * p = nullptr) {
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
//初始化棋盘
void initBoard(int (*board)[15]){

}

Node * select(Node* n) {
	//
}
//扩展
Node * expand(Node n) {

}
//模拟
Node * simulate(Node n) {

}
//反向传播
void packPropagation(Node n) {

}
int main() {
	memset(board, 0, sizeof(board));
	double duration = 0;	//运行时间
	clock_t timeStart = clock();
	initBoard(board);
	
	int color = 2;
	int turn = 1;
	root = new Node(color, turn, x, y, nullptr);
	
	for(;;){
		duration = (double)(clock() - timeStart) / CLOCKS_PER_SEC;
		if (duration >= RuningTime) {
			// 时间到，找最好的点

		} else {
			//MCTS
			select(root);
		}
	}
	
	return 0;
}