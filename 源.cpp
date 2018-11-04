
#include<math.h>

#include<stdlib.h>

#include<time.h>

#include<iostream>

#include<cstdio>

#include "jsoncpp/json.h" // C++����ʱĬ�ϰ����˿� 

#define N 7 //ÿ���ڵ�ķ�֧��

//����Ϊ�����͵�ʶ����  //Ȩ��

#define WIN 1 //4000

#define LOSE 2 //-4000
//����
#define FLEX4 3 //2000
//����
#define flex4 4 //-2000
//����
#define BLOCK4 5 //1000
//����
#define block4 6 //-1000 
//����
#define FLEX3 7 //1000 ?
//����
#define flex3 8 //-1000
//����
#define BLOCK3 9 //400 ?
//���� 
#define block3 10 //-600 ? 
//���
#define FLEX2 11 //400 ?
//���
#define flex2 12 //-600 ?
//�߶�
#define BLOCK2 13 //100 ?
//�߶�
#define block2 14 //-150 ?
//��һ
#define FLEX1 15 //100 ?
//��һ
#define flex1 16 //-150 ?

using namespace std;
using namespace Json;

char board[15][15]; //��ǰ���� 

void placeAt(int x, int y, int d) {
	if (x >= 0 && y >= 0)
		board[x][y] = d;
}

int put_board()
{
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 15; j++)
		{
			if (board[i][j] == 1)
			{
				cout << "x ";
			}
			else if (board[i][j] == 2)
			{
				cout << "o ";
			}
			else
			{
				cout << ". ";
			}
		}
		cout << endl;
	}
}

//��������A��B 
int copy_board(char A[][15], char B[][15])
{
	int i, j;

	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
		{
			B[i][j] = A[i][j];
		}
	}
	return 0;
}

//����ȡ���������A��B 
int inv_board(char A[][15], char B[][15])
{
	int i, j;

	int INV[3] = { 0,2,1 };

	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
		{
			B[i][j] = INV[A[i][j]];
		}
	}
	return 0;
}

//���ͱ�ʶ���� 
int score[3][3][3][3][3][3];

//�����ͱ�ʶ���鸳ֵ 
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

struct evaluation
{
	int score;
	int result;
};

//������A�ľ��ƽ�������(ȫ������)
struct evaluation evaluate(char A[][15])
{
	//������Ȩ�� 
	int weight[17] = { 0,4000,-4000,2000,-2000,1000,-1000,1000,-1000,400,-600,400,-600,100,-150,100,-150 };

	int i, j, s;

	int stat[4][17] = { 0 };

	int STAT[17];

	struct evaluation EVALUATION;

	//����ͳ��
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 10; j++)
		{
			s = score[A[i][j]][A[i][j + 1]][A[i][j + 2]][A[i][j + 3]][A[i][j + 4]][A[i][j + 5]];

			stat[1][s]++;
		}
	}

	for (j = 0; j < 15; j++)
	{
		for (i = 0; i < 10; i++)
		{
			s = score[A[i][j]][A[i + 1][j]][A[i + 2][j]][A[i + 3][j]][A[i + 4][j]][A[i + 5][j]];

			stat[2][s]++;
		}
	}

	for (i = 0; i < 10; i++)
	{
		for (j = 0; j < 10; j++)
		{
			s = score[A[i][j]][A[i + 1][j + 1]][A[i + 2][j + 2]][A[i + 3][j + 3]][A[i + 4][j + 4]][A[i + 5][j + 5]];

			stat[3][s]++;
		}
	}

	for (i = 5; i < 15; i++)
	{
		for (j = 0; j < 10; j++)
		{
			s = score[A[i][j]][A[i - 1][j + 1]][A[i - 2][j + 2]][A[i - 3][j + 3]][A[i - 4][j + 4]][A[i - 5][j + 5]];

			stat[0][s]++;
		}
	}

	s = 0;

	//���������ۼ� 
	for (i = 1; i < 17; i++)
	{
		s += (stat[1][i] + stat[2][i] + stat[3][i] + stat[0][i])*weight[i];

		STAT[i] = (stat[1][i] > 0) + (stat[2][i] > 0) + (stat[3][i] > 0) + (stat[0][i] > 0);
	}

	EVALUATION.result = 0;

	//ʤ 
	if (STAT[1] > 0)
	{
		s += 100000;
		EVALUATION.result = 1;
	}

	//�� 
	else if (STAT[2] > 0)
	{
		s -= 100000;
		EVALUATION.result = 2;
	}

	//���ֳ��ġ����� 
	else if (STAT[4] > 0 || STAT[6] > 0)
	{
		s -= 30000;
	}

	//�����޳��ġ����� 
	else if (STAT[4] == 0 && STAT[6] == 0)
	{
		int k = 0;

		//���� ���Ļ���
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (i != j)
				{
					k += stat[i][5] * stat[j][7];
				}
			}
		}

		//����
		if (STAT[3] > 0)
		{
			s += 20000;
		}

		//˫����
		else if (STAT[5] >= 2)
		{
			s += 20000;
		}

		//���Ļ���
		else if (k > 0)
		{
			s += 20000;
		}

		//�����л��� 
		else if (STAT[8] > 0 && STAT[5] == 0)
		{
			s -= 20000;
		}

		//˫���� 
		else if (STAT[7] >= 2 && STAT[8] == 0)
		{
			s += 10000;
		}
	}
	EVALUATION.score = s;

	return EVALUATION;
}

struct points
{
	int coo_x[N]; //������ 

	int coo_y[N]; //������ 

	int eva[N]; //����ֵ 

	int exi[N]; //������ 
};

//��������ѵ�N���������ӵ� 
struct points seek_points(char A[][15])
{
	struct points best_points;

	struct evaluation EVA;

	int i, j, k;

	int worth[15][15];

	int B[15][15] = { 0 };

	for (k = 0; k < N; k++)
	{
		best_points.exi[k] = 0;
	}

	EVA = evaluate(A);

	if (EVA.result > 0)
	{
		best_points.exi[0] = 1;

		best_points.eva[0] = EVA.score;

		for (i = 0; i < 15; i++)
		{
			for (j = 0; j < 15; j++)
			{
				if (A[i][j] == 1)
				{
					best_points.coo_x[0] = i;

					best_points.coo_y[0] = j;

					goto the_end;
				}
			}
		}
	}

	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
		{
			if (A[i][j] != 0)
			{
				for (k = -3; k <= 3; k++)
				{
					if (i + k >= 0 && i + k < 15)
					{
						B[i + k][j] = 1;

						if (j + k >= 0 && j + k < 15)
						{
							B[i + k][j + k] = 1;
						}
						if (j - k >= 0 && j - k < 15)
						{
							B[i + k][j - k] = 1;
						}
					}
					if (j + k >= 0 && j + k < 15)
					{
						B[i][j + k] = 1;
					}
				}
			}
		}
	}

	//��������A�ϵĿյ㣬�����ڸô����Ӻ�ľ��� 
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
		{
			worth[i][j] = -1000000;

			if (A[i][j] == 0 && B[i][j] == 1)
			{
				A[i][j] = 1;

				EVA = evaluate(A);

				worth[i][j] = EVA.score;

				A[i][j] = 0;
			}
		}
	}

	int w;

	//ɸѡ��ѵ�N���� 
	for (k = 0; k < N; k++)
	{
		w = -1000000;

		for (i = 0; i < 15; i++)
		{
			for (j = 0; j < 15; j++)
			{
				if (worth[i][j] > w)
				{
					w = worth[i][j];

					best_points.coo_x[k] = i;

					best_points.coo_y[k] = j;
				}
			}
		}
		if ((k > 0) && ((best_points.eva[0] - w) > 3000)) break;

		best_points.eva[k] = w;

		best_points.exi[k] = 1;

		worth[best_points.coo_x[k]][best_points.coo_y[k]] = -1000000;
	}

the_end:

	return best_points;
}

struct decision
{
	int coo_x;

	int coo_y;

	int eva;
};

struct decision DECISION;

int level_limit = 6;

//������MinMax�ݹ���� AlphaBeta��֦ �����׵Ĳο�ά���ٿ�
int analyse(char A[][15], int level, int alpha, int beta)
{
	if (level == level_limit)
	{
		struct points P;

		P = seek_points(A);

		alpha = P.eva[0];

		return alpha;
	}

	else if (level % 2 == 0)
	{
		struct points P;

		P = seek_points(A);

		for (int i = 0; i < N; i++)
		{
			if (P.exi[i] == 1)
			{
				char buff[15][15];

				copy_board(A, buff);

				buff[P.coo_x[i]][P.coo_y[i]] = 1;

				int a;

				a = analyse(buff, level + 1, alpha, beta);

				if (a > alpha)
				{
					alpha = a;

					if (level == 0)
					{
						DECISION.coo_x = P.coo_x[i];

						DECISION.coo_y = P.coo_y[i];

						DECISION.eva = a;
					}
				}
			}
			if (beta <= alpha) break;
		}
		return alpha;
	}

	else if (level % 2 == 1)
	{
		char BUFF[15][15];

		inv_board(A, BUFF);

		struct points P;

		P = seek_points(BUFF);

		for (int i = 0; i < N; i++)
		{
			if (P.exi[i] == 1)
			{
				char buff[15][15];

				copy_board(A, buff);

				buff[P.coo_x[i]][P.coo_y[i]] = 2;

				int a;

				a = analyse(buff, level + 1, alpha, beta);

				if (a < beta)
				{
					beta = a;
				}
			}
			if (beta <= alpha) break;
		}
		return beta;
	}
}

bool start() {
	for (int i = 0; i < 15; i++)
		for (int j = 0; j < 15; j++)
			if (board[i][j] != 0) return 0;
	return 1;
}

Value pushout(int x, int y) {
	Json::Value action;
	action["x"] = x;
	action["y"] = y;
	return action;
}

//ͼ�ν���д�������Ӱɡ�����
int main()
{

	init_score();

	string str;
	getline(cin, str);
	Reader reader;
	Value input;
	reader.parse(str, input);
	int turnID = input["responses"].size();
	for (int i = 0; i < turnID; i++) {
		placeAt(input["requests"][i]["x"].asInt(), input["requests"][i]["y"].asInt(), 2);
		placeAt(input["responses"][i]["x"].asInt(), input["responses"][i]["y"].asInt(), 1);
	}
	placeAt(input["requests"][turnID]["x"].asInt(), input["requests"][turnID]["y"].asInt(), 2);
	Value ret;
	if (start()) {
		ret["response"] = pushout(0, 0);
	}
	else {
		analyse(board, 0, -1000000, 1000000);
		board[DECISION.coo_x][DECISION.coo_y] = 1;
		ret["response"] = pushout(DECISION.coo_x, DECISION.coo_y);
	}
	FastWriter writer;
	cout << writer.write(ret) << endl;

	return 0;
}