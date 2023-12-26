#include<iostream>
#include"algorithm.h"
#include<fstream>
#include<string>
#include<sstream>
#include<random>
#include<time.h>
using namespace std;
int Tabu::count_f() {
	int conf = 0;
	for (int i = 0; i < vexNum; i++) {
		for (int j = i + 1; j < vexNum; j++) {
			if (edges[i][j] == 1) {
				if (Sol[i] == Sol[j]) {
					conf++;
				}
			}
		}
	}
	return conf;
}
void Tabu::initColor(int*p) {
	conflict = 0;
	for (int i = 0; i < vexNum; i++) {

		Sol[i] = p[i];
		//cout << i + 1 << ":" << p[index][i] << endl;
	}
	for (int i = 0; i < vexNum; i++) {
		for (int j = i + 1; j < vexNum; j++) {
			if (edges[i][j] == 1) {
				AdjTable[i][Sol[j]]++;
				AdjTable[j][Sol[i]]++;
				if (Sol[i] == Sol[j]) {
					conflict++;
				}
			}
		}
	}
	Best_f = conflict;
}
void Tabu::init() {
	Sol = new int[vexNum];
	BestSol = new int[vexNum];
	TabuTenure = new int* [vexNum];
	AdjTable = new int* [vexNum];
	for (int i = 0; i < vexNum; i++) {
		TabuTenure[i] = new int[k];
		AdjTable[i] = new int[k];
		for (int j = 0; j < k; j++) {
			TabuTenure[i][j] = 0;
			AdjTable[i][j] = 0;
		}
	}
	nbVex = new int* [vexNum];
	nbVexNum = new int[vexNum];
	for (int i = 0; i < vexNum; i++) {
		nbVex[i] = new int[vexNum - 1];
		nbVexNum[i] = 0;
		for (int j = 0; j < vexNum - 1; j++) {
			nbVex[i][j] = 0;
		}
	}
	for (int i = 0; i < vexNum; i++) {
		for (int j = 0; j < vexNum; j++) {
			if (edges[i][j] == 1) {
				int tmp = nbVexNum[i];
				nbVexNum[i]++;
				nbVex[i][tmp] = j;
			}
		}
	}
	Sols = new int* [vexNum * (k - 1)];
	for (int i = 0; i < vexNum * (k - 1); i++) {
		Sols[i] = new int[3];
	}
}
void Tabu::reInit(int* sol) {
	for (int i = 0; i < vexNum; i++) {
		for (int j = 0; j < k; j++) {
			TabuTenure[i][j] = 0;
			AdjTable[i][j] = 0;
		}
	}
	initColor(sol);
	iter = 0;
	
}
void Group::init_p(int index) {
	tabu[index].edges = edges;
	tabu[index].k = k;
	tabu[index].vexNum = vexNum;
	tabu[index].init();
	p[index] = new int[vexNum];
	for (int i = 0; i < vexNum; i++) {
		p[index][i] = rand() % k;
	}
	tabu[index].initColor(p[index]);
	
	f[index] = tabu[index].count_f();
}
void Group::init() {
	
	string str;
	fstream f;
	f.open("D:\\forCode\\npbenchmark.data-data\\GCP\\Instance\\DSJC1000.5.txt", ios::in);
	getline(f, str);
	istringstream ss(str);
	ss >> vexNum >> edgeNum >> k;
	c[0] = new int[vexNum];
	c[1] = new int[vexNum];
	edges = new int* [vexNum];
	for (int i = 0; i < vexNum; i++) {
		edges[i] = new int[vexNum];
		for (int j = 0; j < vexNum; j++) {
			edges[i][j] = 0;
		}
	}
	while (getline(f, str)) {
		istringstream ss(str);
		int v1, v2;
		ss >> v1 >> v2;
		edges[v1][v2] = 1;
		edges[v2][v1] = 1;
	}
	for (int i = 0; i < 5; i++) {
		init_p(i);
	}
	//conflict = Best_f = countConflict();
}
void Group::GPX(int* p1, int* p2, int* c) {
	set<int>::iterator it;
	set<int>* p[2];
	p[0] = new set<int>[k];
	p[1] = new set<int>[k];
	int* s[2];
	s[0] = new int[vexNum];
	s[1] = new int[vexNum];
	memcpy(s[0], p1, vexNum * sizeof(int));
	memcpy(s[1], p2, vexNum * sizeof(int));
	for (int i = 0; i < vexNum; i++) {
		p[0][p1[i]].insert(i);
		p[1][p2[i]].insert(i);
	}
	int equ_k[2000];
	int max_size;
	int max_equ_count;
	int n1, n2;
	int tmp;
	for (int l = 0; l < k; l++)
	{
		n1 = l % 2;
		n2 = (l + 1) % 2;
		max_size = -1;
		for (int i = 0; i < k; i++)
		{
			tmp = p[n1][i].size();
			if (tmp >= max_size)
			{
				if (tmp > max_size)
				{
					max_size = tmp;
					max_equ_count = 0;
				}
				equ_k[max_equ_count] = i;
				max_equ_count++;
			}
		}
		int del_color, del_v, del_color_2;
		del_color = equ_k[rand() % max_equ_count];//从最大颜色中随机选了一个颜色
		it = p[n1][del_color].begin();
		while (it != p[n1][del_color].end())
		{
			del_v = *it;
			del_color_2 = s[n2][del_v];
			p[n2][del_color_2].erase(del_v);
			c[del_v] = l;
			//cout << c[del_v] << endl;

			it++;
		}
		p[n1][del_color].clear();
	}
	for (int i = 0; i < k; i++)  //未取出来的结点随机分配颜色
	{
		it = p[0][i].begin();
		while (it != p[0][i].end())
		{
			c[*it] = rand() % k;
			it++;
		}
	}
}
Tabu::Move* Tabu::FindMove() {
	int delt = 1000;
	Move* res = new Move;
	int flag = -1;  //-1代表找不到需要移动的颜色，0表示需要移动禁忌点颜色，1表示移动非禁忌点颜色
	for (int i = 0; i < vexNum; i++) {
		if (AdjTable[i][Sol[i]] > 0) {
			for (int j = 0; j < k; j++) {
				if (j != Sol[i]) {
					int temp = AdjTable[i][j] - AdjTable[i][Sol[i]];
					if (temp <= delt && ((iter > TabuTenure[i][j]) || (temp + conflict < Best_f))) {
						if (temp < delt) {
							solCount = 0;
							delt = temp;
						}
						Sols[solCount][0] = i;
						Sols[solCount][1] = Sol[i];
						Sols[solCount][2] = j;
						solCount++;
					}
				}
			}
		}
	}
	int t = rand() % solCount;
	res->u = Sols[t][0];
	res->vi = Sols[t][1];
	res->vj = Sols[t][2];
	res->delt = delt;
	return res;
}
void Tabu::MakeMove(Move* move) {
	
	if (move->delt == 1000) {
		return;
	}
	int u = move->u, vi = move->vi, vj = move->vj, dt = move->delt;
	if (Best_f > conflict + dt) {
		Best_f = conflict + dt;
	}
	Sol[u] = vj;
	conflict = conflict + dt;
	TabuTenure[u][vi] = iter + conflict + 0.6 * (rand() % 10);
	int* h_nbVex = nbVex[u];
	int nbNum = nbVexNum[u];
	int tmp;
	for (int i = 0; i < nbNum; i++) {
		tmp = h_nbVex[i];
		AdjTable[tmp][vi]--;
		AdjTable[tmp][vj]++;
	}
}
void Tabu::TabuSearch() {
	Move* move = new Move;
	while (conflict > 0 && iter < MAX_TABU_ITER) {
		move = FindMove();
		MakeMove(move);
		iter++;
	}
}
void Group::copy_popu(int index1, int index2) {
	memcpy(p[index1], p[index2], vexNum * sizeof(int));
	f[index1] = f[index2];
}
bool Group::check() {
	set<int>::iterator it;
	set<int>* s[2];
	s[0] = new set<int>[k];
	s[1] = new set<int>[k];
	for (int i = 0; i < vexNum; i++) {
		s[0][p[0][i]].insert(i);
		s[1][p[1][i]].insert(i);
	}
	for (int i = 0; i < k; i++) {
		int size = s[0][i].size();
		if (!size) {
			continue;
		}
		it = s[0][i].begin();
		for (int j = 0; j < k; j++) {
			if (s[1][j].count(*it)) {
				if (s[1][j].size() == size) {
					it++;
					while (it != s[0][i].end()) {
						if (!s[1][j].count(*it)) {
							return false;
						}
						it++;
					}
					s[1][j].clear();
					break;
				}
				else {
					return false;
				}
			}
		}
		return true;
	}
}
void Group::compute() {
	clock_t start, end;
	srand(seed);
	init();
	int generation = 0;
	int best_f = 0;
	start = clock();
	do {
		GPX(p[0], p[1], c[0]);
		GPX(p[1], p[0], c[1]);
		tabu[0].reInit(c[0]);
		
		tabu[0].TabuSearch();
		int conf1 = tabu[0].count_f();
		memcpy(p[0], tabu[0].Sol, vexNum * sizeof(int));
		f[0] = conf1;
		tabu[1].reInit(c[1]);
		
		tabu[1].TabuSearch();
		int conf2 = tabu[1].count_f();
		memcpy(p[1], tabu[1].Sol, vexNum * sizeof(int));
		f[1] = conf2;
		int min_f = 10000;
		for (int i = 0; i < 3; i++) {
			if (min_f > f[i]) {
				min_f = f[i];
				best_f = i;
			}
		}
		copy_popu(E1, best_f);
		if (f[BEST] > f[E1]) {
			copy_popu(BEST, E1);
		}
		if (generation % ITER_CYCLE == 0)
		{
			copy_popu(P1, E2);
			copy_popu(E2, E1);
			init_p(E1);
		}
		generation++;
		end = clock();
		if ((end - start) / 1000 > timeLimit) {
			break;
		}
	} while (generation < MAX_CROSS && f[BEST] != 0);
	end = clock();
	cout << "generation= " << generation << "\t time=" << (end - start) <<"\t conflict:" << f[BEST] << endl;
	fstream fout;
	fout.open("D:\\forCode\\out.txt", ios::out);
	for (int i = 0; i < vexNum; i++) {
		fout << p[BEST][i] << endl;
	}
}