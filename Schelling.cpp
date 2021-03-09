#include "Schelling.h"
#include <iostream>
#include <math.h>
#include <algorithm>
#include <vector>
#include <set>
#include <stdlib.h>
#include <time.h>
#include <tuple>
#include <unistd.h>
#include <thread>
#include <chrono>
using namespace std;

Schelling::Schelling(int threshold, int percentX, int percentEmpty, int size) {
	srand(time(NULL));

	this->threshold = threshold;
	this->sideSize = size;
	grid.assign(sideSize,vector<Agent>(sideSize));

	const int numTotal = sideSize*sideSize;
	int numEmpty = round(numTotal * (percentEmpty/100.0));
	const int numFilled = numTotal - numEmpty;
	int numX = round(numFilled * (percentX/100.0));

	vector<Agent> tmp(numTotal);
	for (int i=0;i<numTotal;++i) {
		if (i<numX)
			tmp[i]=X;
		else if (i<numFilled)
			tmp[i]=O;
		else
			tmp[i]=EMPTY;
	}
	random_shuffle(tmp.begin(),tmp.end());
	for (int i=0;i<sideSize;++i) {
		for (int j=0;j<sideSize;++j) {
			grid[i][j]=tmp[i*sideSize+j];
		}
	}
}

void Schelling::getNeighborsPos(int y, int x, vector<pair<int,int>>& ret) {
	for (int i=0;i<8;++i) {
		int newX = x + dx[i];
		int newY = y + dy[i];
		if (newX<0 || newX>=sideSize || newY<0 || newY>=sideSize) continue;
		if (grid[newY][newX] != EMPTY)
			ret.push_back({newY,newX});
	}
}

void Schelling::getNeighbors(int y, int x, vector<Agent>& ret) {
	for (int i=0;i<8;++i) {
		int newX = x + dx[i];
		int newY = y + dy[i];
		if (newX<0 || newX>=sideSize || newY<0 || newY>=sideSize) continue;
		if (grid[newY][newX] != EMPTY)
			ret.push_back(grid[newY][newX]);
	}
}

void Schelling::move(int y, int x) {
	// choose new location
	int idx = rand() % vacant.size();
	int newY,newX; 
	tie(newY,newX)=vacant[idx];

	// move
	grid[newY][newX]=grid[y][x];
	grid[y][x]=EMPTY;
	vacant.push_back({y,x});
	vacant.erase(vacant.begin() + idx);

	// update check
	check.insert({newY,newX});
	vector<pair<int,int>> neighborsPos;
	getNeighborsPos(y,x,neighborsPos);
	for (auto pos : neighborsPos) {
		check.insert(pos);
	}
	neighborsPos.clear();
	getNeighborsPos(newY,newX,neighborsPos);
	for (auto pos : neighborsPos) {
		check.insert(pos);
	}
}

void Schelling::run() {
	for (int i=0;i<sideSize;++i) {
		for (int j=0;j<sideSize;++j) {
			if (grid[i][j] != EMPTY)  {
				check.insert({i,j});
			} else {
				vacant.push_back({i,j});
			}
		}
	}

	for (int round=0;round<1000 && check.size();++round) {
		outputGrid();
		this_thread::sleep_for(chrono::milliseconds(500));
		for (auto pos : check) {
			int y,x; tie(y,x)=pos;
			if (grid[y][x] == EMPTY)
				continue;
			vector<Agent> neighbors;
			getNeighbors(y,x,neighbors);
			int cntSame=0;
			for (auto val : neighbors) {
				if (val == grid[y][x]) {
					cntSame++;
				}
			}
			if (neighbors.size() != 0 && 
					(double)cntSame/neighbors.size() < threshold/100.0) {
				dissatisfied.push_back({y,x});
			}
		}
		check.clear();
		for (auto pos : dissatisfied) {
			int y,x; tie(y,x)=pos;
			move(y,x);
		}
		dissatisfied.clear();
	}
}

void Schelling::outputGrid() {
	string symbol = "\u2588";
	cout << "\033[2J\033[1;1H";
	for (int i=0;i<sideSize;++i) {
		for (int j=0;j<sideSize;++j) {
			if (grid[i][j] == X) {
				cout<<"\033[40;31m";
				cout<<symbol<<symbol;
			} else if (grid[i][j] == O) {
				cout<<"\033[40;34m";
				cout<<symbol<<symbol;
			} else {
				cout<<"\033[40;30m";
				cout<<symbol<<symbol;
			}
		}
		cout<<'\n';
	}
	cout<<"\033[0m";
}
