#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>
#include <fstream>
#include "WarpPath.h"
#define inf 1e19

using namespace std;

double AWarp_UB(vector<double> &s, vector<double> &t, bool*, bool*, char** w= NULL);
double UBCases(double a, double b, char c, bool ra, bool rb);
double LBCases(double a, double b, char c, bool ra, bool rb);

double MIN(double a1, double a2, double a3 = inf, double a12 = 1e19) {
	double temp = a12;
	
	if (a1 < temp) temp = a1;
	if (a2 < temp) temp = a2;
	if (a3 < temp) temp = a3;

	return temp;
}


double square(double s, double t, bool rs, bool rt) {
	if(rs && !rt) return t*t;
	else if(!rs && rt) return s*s;
	else if(rs && rt) return 0.0;
	else return ((s-t)*(s-t));
}


double dist_A(WarpPath &w, int i, int j) {
	double dist = 0;

	for(int k = i; k <= j; k++) 
		dist += UBCases(w.s->at(w[k].u), w.t->at(w[k].v), w.direction(k), w.rs[w[k].u], w.rt[w[k].v]);
	
	return dist;
}

int max(int i, int j)
{
	if(i > j) return i;
	else return j;
}

double TUB_Basis(WarpPath &w, int i, int j, char d, double *mins, int &budget) {
	/*
	input : <w[i], w[i+1],...,w[j]>
	output : SUM(dist(w[k])) for i < k < j
	*/

	if(i > j) return 0;
	
	double dist = 0;

	if(d == 'l') budget = w.s->at(w[i].u);
	else if(d == 't') budget = w.t->at(w[i].v);
	else budget = 0;
	
	mins[3] = inf;

	if(i+1 >= j)
	{
		mins[3] = 0;
		if(d == 'l')
		{
			if(w.rt[w[i].v]) mins[2] = 0;
			else mins[2] = abs(w.t->at(w[i].v));

			if(w.rt[w[j].v]) mins[1] = 0;
			else mins[1] = abs(w.t->at(w[j].v));

			mins[0] = MIN(mins[1], mins[2]);
		}
		else
		{
			if(w.rs[w[i].u]) mins[2] = 0;
			else mins[2] = abs(w.s->at(w[i].u));
			
			if(w.rs[w[j].u]) mins[1] = 0;
			else mins[1] = abs(w.s->at(w[j].u));

			mins[0] = MIN(mins[1], mins[2]);
		}
		return dist;
	}

	for(int k = i+1; k < j; k++) 
	{
		int u = w[k].u;
		int v = w[k].v;
		
		if(d == 'l')
		{
			if(w.rt[v])
			{
				budget -= w.t->at(v);
				mins[3] = 0;
			}
			else
			{
				budget--;
				mins[3] = MIN(abs(w.t->at(v)), mins[3]);
			}
		}

		else if(d == 't')
		{	
			if(w.rs[u])
			{
				budget -= w.s->at(u);
				mins[3] = 0;

			}
			else
			{
				budget--;
				mins[3] = MIN(abs(w.s->at(u)), mins[3]);
			}
	
		}
		
		dist += square(w.s->at(u), w.t->at(v), w.rs[u], w.rt[v]);
				
	}
	

	if(d == 'l') 
	{
		if(w.rt[w[i].v] || w.rt[w[j].v]) mins[0] = 0;
		else mins[0] = MIN(abs(mins[3]), abs(w.t->at(w[i].v)), abs(w.t->at(w[j].v)));

		if(w.rt[w[j].v]) mins[1] = 0;
		else mins[1] = MIN(abs(mins[3]), abs(w.t->at(w[j].v)));

		if(w.rt[w[i].v]) mins[2] = 0;
		else mins[2] = MIN(abs(mins[3]), abs(w.t->at(w[i].v)));
	}
	else 
	{	
		if(w.rs[w[i].u] || w.rs[w[j].u]) mins[0] = 0;
		else mins[0] = MIN(mins[3], abs(w.s->at(w[i].u)), abs(w.s->at(w[j].u)));

		if(w.rs[w[j].u]) mins[1] = 0;
		else mins[1] = MIN(mins[3], abs(w.s->at(w[j].u)));

		if(w.rs[w[i].u]) mins[2] = 0;
		else mins[2] = MIN(mins[3], abs(w.s->at(w[i].u)));
	}

	if(budget < 0)	budget = 0;
	
	return dist;
}

void TUB_Cost(WarpPath &w, int i, int j, char d, double *vars) {
	for(int x = 0; x<4; x++)
		vars[x] = 0;

	if(i > j) return;

//	if(j >= w.size) end = w.size-1;
	
	
	if( (d == 'l' && w.rs[w[i-1].u] ) || (d == 't' && w.rt[w[i-1].v]) )
	{
		double *mins = new double[4];
		int budget;
		for(int x = 0; x < 4; x++) mins[x] = inf;
	/*
	mins[0] : min(w[k]) i <= k <= j
	mins[1] : min(w[k]) i < k <= j
	mins[2] : min(w[k]) i <= k < j
	mins[3] : min(w[k]) i < k < j
	*/

		double basis = TUB_Basis(w, i-1, j, d, mins, budget);

		double square_i = square(w.s->at(w[i-1].u), w.t->at(w[i-1].v), w.rs[w[i-1].u], w.rt[w[i-1].v]);
		double square_j = square(w.s->at(w[j].u), w.t->at(w[j].v), w.rs[w[j].u], w.rt[w[j].v]);
		
		int bi, bj;

		if(d == 'l' && w.rt[w[i-1].v]) bi = w.t->at(w[i-1].v);
		else if(d == 't' && w.rs[w[i-1].u]) bi = w.s->at(w[i-1].u);
		else bi = 1;

		if(d == 'l' && w.rt[w[j].v]) bj = w.t->at(w[j].v);
		else if(d == 't' && w.rs[w[j].u]) bj = w.s->at(w[j].u);
		else bj = 1;
		

		budget = budget-bi-bj;

		if(budget < 0) budget = 0;

		vars[3] = basis + budget*mins[3]*mins[3];
		vars[2] = basis + square_i + budget*mins[2]*mins[2];
		vars[1] = basis + square_j + budget*mins[1]*mins[1];
		vars[0] = basis + square_i + square_j + budget*mins[0]*mins[0];
		
		delete[] mins;
	}
	else
	{
		double basis = dist_A(w,i,j-1);
		for(int x = 0; x<4; x++)
		{
			vars[x] = basis;
		}

		double sc = UBCases(w.s->at(w[i-1].u), w.t->at(w[i-1].v), w.direction(i-1), w.rs[w[i-1].u], w.rt[w[i-1].v]);
		double ec = UBCases(w.s->at(w[j].u), w.t->at(w[j].v), w.direction(j), w.rs[w[j].u], w.rt[w[j].v]);

		vars[1] += ec;
		vars[0] += ec;

		if(i > 0 && i <= j)
		{
			vars[2] += sc;
			vars[0] += sc;
		}
	}

}

double TUB_Dynamic(std::vector<double> &s, std::vector<double> &t, bool* run_s, bool* run_t)
{
/*
input : two run-length encoded sequences
output : distance between two sequences
*/

	int ns = s.size();
	int nt = t.size();

//	cout << ns << endl;
//	cout << nt << endl;
	
	if(ns == 0 || nt == 0) return 0;

	char **P = new char*[ns];

	for (int i = 0; i < ns; i++) {
		P[i] = new char[nt];
	}

	AWarp_UB(s, t, run_s, run_t, P);

//	cout << "AWarp Path complete" << endl;

	P[0][0] = 's';
	//generate warping path


//	ofstream debug("WarpPathLog");
//	for(int i = 1; i< ns; i++) {
//		for(int j = 1; j<nt; j++) {
//			debug << P[i][j] << " ";	
//		}	
//		debug << endl;
//	}

	WarpPath w = WarpPath(s, t, run_s, run_t, P, ns, nt);
	// matrices not used anymore
	for (int i = 0; i < ns; i++) {
		delete[] P[i];
	}
	delete[] P;


	// split into subpaths
	// calculate distance
	char cur = w.direction(0);
	int start = 0;
	
	double d1 = 0;
	double d2 = 0;
	

//	ofstream ofs("align_TUB");
	for(int i = 0; i < w.get_size(); i++) {
		char next = w.direction(i+1);
		//subpath identified
		if(cur != next) {
			// add subpath distance
	//		ofs << cur << endl;
	//		ofs << start << ", " << i << endl;

			double *dist = new double[4];

			TUB_Cost(w,start,i,cur,dist);
			
			double temp1 = MIN(d2 + dist[0], d1 + dist[1]);
			double temp2 = MIN(d2 + dist[2], d1 + dist[3]);

	//		if(temp1 == d2 + dist[0]) w.print(ofs, start-1, i);
	//		else w.print(ofs, start, i);

			d1 = temp1;
			d2 = temp2;

			delete[] dist;
	//		ofs << "d1 : " << d1 << '\t' << "d2 : " << d2 <<  endl;
			
	//		ofs << "---------------" << endl;

			start = i+1;
			cur = next;
		}
		
	}
	
//	ofs.close();
	return d1;


}

