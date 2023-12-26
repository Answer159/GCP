#include<set>
#pragma once
#define MAX_TABU_ITER 10000
#define K 48  // ��ɫ��
#define ITER_CYCLE 10 //the number of tgenerations into one cycle
#define P1 0
#define P2 1
#define E1 2
#define E2 3
#define BEST 4
#define MAX_CROSS 30000

class Tabu {
public:
	int* Sol;  //��ǰȾɫ����
	int* BestSol;   //��õ�Ⱦɫ����
	int Best_f;  //��õĳ�ͻ��
	int conflict;
	int** TabuTenure;   //���ɱ�
	int** AdjTable;  //��¼��ͻ
	int** Sols;
	int solCount = 0;
	int** nbVex;  //ÿ���������Ľ��
	int* nbVexNum;  //ÿ�����������ĸ���
	int k;
	int vexNum, edgeNum, timeLimit = 6000, iter = 0;
	int** edges;
	struct Move {
		int u;
		int vi;
		int vj;
		int delt;
	};
	void reInit(int* sol);
	void init();
	Move* FindMove();
	void MakeMove(Move* move);
	int count_f();
	void TabuSearch();
	void initColor(int* p);
};

class Group {
public:
	int* p[5];
	int* c[2];
	int f[5];
	Tabu tabu[5];
	int k;
	
	int** edges;
	int vexNum, edgeNum, seed = 7164, timeLimit = 6000, iter = 0;
	char* sourcePath;
	char* outPath;
	struct Move {
		int u;
		int vi;
		int vj;
		int delt;
	};
	void init();
	void init_p(int index);
	void GPX(int* p1, int* p2, int* c);
	void compute();
	void copy_popu(int index1, int index2);
	bool check();
};
