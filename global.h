#include <vector>
#include <iostream>
#include <list>
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <map>
using namespace std;

extern const int M;
extern const int K;
extern const int I;
extern const int n;
extern const int popSize;
extern const int nGen;

extern const double inf;
extern const double PI;
extern const double eps;

extern const double mut_prob;
extern const int mut_idx;
extern const double cross_prob;
extern const int cross_idx;

double Rand();
void setSeed();
double square(double);

class Individual {
private:
	vector <double> theta;

	void scale();
	void calculateG();
	double f(int);	// DTLZ5 objectives; index = [0..M-1]
public:
	vector <double> val;
	vector <double> obj;
	double crowdDist;
	double g;
	int idx;

	Individual();
	~Individual();
	void init();
	void evaluateObjective(const vector<int> &);
	void mutate();
	void print(FILE *);
	void print(const vector <int> &, FILE*);
};

class Population {
private:
	vector <Individual*> child;
	vector <Individual*> mixed;
	vector <int> idx1;
	vector <int> idx2;

	void runGeneration();
	void createChild();
	void generateFronts(vector <vector<Individual*> > &);
public:
	vector <Individual*> parent;
	vector <int> obj;
	int nObj;

	Population(const vector <int>& );
	~Population();
	void initialise();
	void runNSGA2();
	void print(FILE *);
	void print(const vector <int> &, FILE *);
};

void crossover(Individual *p1, Individual *p2, Individual *c1, Individual *c2) ;

void assignCrowdingDistance(vector <Individual*>&);
bool dominates(Individual *a, Individual *b);
Individual* crowding(Individual *a, Individual *b) ;

class Redundant {
public:
	Population *pop;
	vector <vector<double> > disorder;
	vector <vector<int>* > ranks;
	vector <int> obj;

	Redundant(Population *);
	~Redundant();

	void calculateRanks();
	void calculateDisorder();
	vector <int> reduceObjective() ;
};

class BIT {
public:
	vector <int> tree;

	BIT();
	void init();
	void updateFreq(int);
	int readFreq(int);
	int readFreq(int, int);
};

class DisjointSet {
public:
	vector <int> par;

	DisjointSet();
	void init(int);
	int findSet(int);
	void mergeSet(int, int);
};
