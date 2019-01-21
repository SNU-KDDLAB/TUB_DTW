#include <iostream>
#include <fstream>
#include <vector>
#include "WarpPath.h"

using namespace std;

IndexPair::IndexPair(int i, int j) {
	this->u = i;
	this->v = j;
}

IndexPair::IndexPair(const IndexPair& p) {
	this->u = p.u;
	this->v = p.v;
}

IndexPair& IndexPair::operator =(const IndexPair& p) {
	u = p.u;
	v = p.v;

	return *this;
}

void IndexPair::print(std::ofstream& ofs) {
	ofs << '(' << this->u << ',' << this->v << ')';
}
	
WarpPath::WarpPath(vector<double> &s, vector<double> &t, bool* rs, 
	bool* rt, char** P, int m, int n): s(&s), t(&t), rs(rs), rt(rt)
	{

	this->seq.clear();
	
	for(int i = m-1, j = n-1;;){
		IndexPair p = IndexPair(i,j);
		vector<IndexPair>::iterator it = this->seq.begin();
		this->seq.insert(it, p);
		if(P[i][j] == 's') break;
		if(P[i][j] == 'd') {
			i--;
			j--;
		}
		else if(P[i][j] == 't') {
			i--;	
		}
		else {
			j--;
		}
	}

	this->size = this->seq.size();

}
/*
WarpPath::WarpPath(const WarpPath& w, int i, int j, char d) {
	this->seq.clear();

	for(int k = i; k<=j; k++) {
		IndexPair p = w.seq[k];
		seq.push_back(p);

//		(*(seq.begin())).print();
	}

	size = seq.size();
	s = w.s;
	t = w.t;
	rs = w.rs;
	rt = w.rt;
	dir = d;
}
*/

WarpPath::~WarpPath(){
	s = NULL;
	t = NULL;
	rs = NULL;
	rt = NULL;
}


WarpPath& WarpPath::operator =(const WarpPath& w) {
	this->seq = w.seq;
	this->size = w.size;
	this->s = w.s;
	this->t = w.t;
	this->rs = w.rs;
	this->rt = w.rt;

	return *this;
}

void WarpPath::print(ofstream& ofs, int i = 0, int j = -1) {
	
	int start = i;
	int end = j;

	if(j < 0) end = size;
	if(start < 0) start = 0;

	for(int k = start; k <= end; k++) {
		int u = seq[k].u;
		int v = seq[k].v;
		
		this->seq[k].print(ofs);
		ofs << '\t' << this->s->at(u) << "," << this->rs[u];
		ofs << '\t' << this->t->at(v) << "," << this->rt[v] << endl;
	}
}

IndexPair& WarpPath::operator[] (int x) {
	if(x < 0) return this->seq[0];
	if(x >= this->size) return this->seq[size-1];
	return this->seq[x];
}

char WarpPath::direction(int x) {
	if(x <= 0) return 'd';
	if(x >= size) {
		return 'e';
	}

	int udiff = seq[x].u - seq[x-1].u;
	int vdiff = seq[x].v - seq[x-1].v;

	if( (udiff * vdiff) == 1) return 'd';	
	else if (udiff == 1) return 't';
	else return 'l';
}


