#ifndef WARPPATH_H
#define WARPPATH_H

#include <vector>
#include <fstream>
using namespace std;

class WarpPath;
class IndexPair{
friend class WarpPath;
friend void TUB_Cost(WarpPath&, int, int, char, double*);
friend double dist_A(WarpPath&, int, int);
//friend double dist_T(WarpPath, int, int, int);
friend double TUB_Basis(WarpPath&, int, int, char, double*, int&);

//friend ostream & operator << (ostream &, const int &);
public:
	IndexPair(int, int);
	IndexPair(const IndexPair&);
	~IndexPair(){}
	IndexPair& operator =(const IndexPair&);
	void print (ofstream&);
	
private:
	int u;
	int v;
};



class WarpPath{
friend void TUB_Cost(WarpPath&, int, int, char, double*);
friend double dist_A(WarpPath&, int, int);
//friend double dist_T(WarpPath, int, int, int);
friend double TUB_Basis(WarpPath&, int, int, char, double*, int&);

//friend ostream & operator << (ostream &, const int &);
public:
	WarpPath(vector<double>&, vector<double>&, bool*, bool*, char**, int, int);
	WarpPath(const WarpPath&, int, int, char);
	~WarpPath();
	WarpPath& operator =(const WarpPath&);
	void print(ofstream&, int, int);
	IndexPair& operator[] (int);
	char direction(int);
	int get_size() const { return size; }

private:
	vector<IndexPair> seq;
	int size;
	vector<double> *s;
	vector<double> *t;
	bool* rs;
	bool* rt;
	char dir;
};

#endif
