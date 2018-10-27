#include <iostream>
#include <ctime>
#include <time.h>
using namespace std;
int board[15][15];
int sec = 1000;
const double RuningTime = 0.85f;
float confident = 1.96;
int equivalence = 1000;
int x;//���һ���µĵط�
int y;//���һ���µĵط�
Node* root;	//root ָ��
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
//��ʼ������
void initBoard(int (*board)[15]){

}

Node * select(Node* n) {
	//
}
//��չ
Node * expand(Node n) {

}
//ģ��
Node * simulate(Node n) {

}
//���򴫲�
void packPropagation(Node n) {

}
int main() {
	memset(board, 0, sizeof(board));
	double duration = 0;	//����ʱ��
	clock_t timeStart = clock();
	initBoard(board);
	
	int color = 2;
	int turn = 1;
	root = new Node(color, turn, x, y, nullptr);
	
	for(;;){
		duration = (double)(clock() - timeStart) / CLOCKS_PER_SEC;
		if (duration >= RuningTime) {
			// ʱ�䵽������õĵ�

		} else {
			//MCTS
			select(root);
		}
	}
	
	return 0;
}