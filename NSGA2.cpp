#include "global.h"

const double eps = 0.40;
const double inf = 1e100;

const int I = 2;
const int M = 5;
const int K = 10;
const int n = M+K-1;	// No. of decision variables.
const int popSize = 500;
const int nGen = 500;

const double PI = 2*acos(0);

//const double mut_prob = 1/n;
const double mut_prob = 0.1;
const int mut_idx = 50;

const double cross_prob = 0.9;
const int cross_idx = 5;

double square(double x) {
	return x*x;
}

static_assert(I <= M, "Number of dimensions greater than number of objectives");
static_assert(n == M+K-1, "Number of decision variables doesn't satisfy DTLSZ5 criteria");
static_assert(popSize % 4 == 0 && popSize > 0, "Population size must ba multiple of 4");
static_assert(I != 1, "Number of dimensions must be greater than 1");

int main()
{
	setSeed();
	printf("---------------------------------------------------------\n");
	printf("Problem Specification: DTLZ(%d, %d)\n", I, M);
	printf("---------------------------------------------------------\n");

/*
	Individual id;
	id.init();
	id.evaluateObjective();
	cout << "vals: ";
	for(auto it = (id.val).begin(); it != (id.val).end(); ++it) {
		cout << *it << ", ";
	}
	cout << "\n";
	cout << "\n";
	cout << "Thetas: ";
	for(auto it = (id.theta).begin(); it != (id.theta).end(); ++it) {
		cout << *it << ", ";
	}
	cout << "\n";
	cout << "\n";
	cout << "id.g = " << id.g << "\n";

	cout << "Obj: ";
	for(auto it = (id.obj).begin(); it != (id.obj).end(); ++it) {
		cout << *it << ", ";
	}
	cout << "\n";
	cout << "-----------------------------\n\n";
*/

	Population *pop ;
	vector <int> obj;
	for(int i = 0; i < M; ++i) {
		obj.push_back(i);
	}

	int iterationCounter = 0;
	Population *first, *last;
	first = last = nullptr;

	while(true) {
		iterationCounter++;
		printf("Iteration #%d\n", iterationCounter);
		pop = new Population(obj);
		pop->initialise();
		pop->runNSGA2();

		Redundant *red = new Redundant(pop);
		vector <int> newObj = red->reduceObjective();

//		pop->print();

		if(iterationCounter == 1) {
			first = pop;
		}

		if(newObj == obj)
			break;
		else
			obj = newObj;

		if(iterationCounter != 1) {
			delete(pop);
			delete(red);
		}
	}
	last = pop;
	FILE *fst = fopen("first.out", "w");
	FILE *snd = fopen("final.out", "w");
	first->print(obj, fst);
	last->print(snd);
//	last->print(obj);

/*
	pop = new Population(obj);
	pop->initialise();
	pop->runNSGA2();

	Redundant red(pop);
	vector <int> newOb = red.reduceObjective();

	pop = new Population(newOb);
	pop->initialise();
	pop->runNSGA2();
	Redundant newRed(pop);
	newRed.reduceObjective();
*/	

/*
	cout << "Val: ";
	for(auto it = (pop.parent.back()->val).begin(); it != (pop.parent.back()->val).end(); ++it) {
		cout << *it << ", ";
	}
	cout << "\n" << "\n";

	cout << "Obj: ";
	for(auto it = (pop.parent.back()->obj).begin(); it != (pop.parent.back()->obj).end(); ++it) {
		cout << *it << ", ";
	}
	cout << "\n" << "\n";


	for(int i = 0; i < 10; ++i) {
		printf("{" );
		for(int j = 0; j < M; ++j) {
			printf("%8.4lf  ", pop.parent[i]->obj[j]);
		}
		printf("}\n" );
		fflush(stdout);
	}
	cout << "\n";

	cout << "Cnt: ";
	for(int cnt = 0; cnt < 50; ++cnt) {
		pop.runGeneration();
		printf("%d ", cnt);
		fflush(stdout);
	}
	cout << "\n";
	cout << endl;

	for(int i = 0; i < 10; ++i) {
		printf("{" );
		for(int j = 0; j < M; ++j) {
			printf("%8.4lf  ", pop.parent[i]->obj[j]);
		}
		printf("}\n" );
		fflush(stdout);
	}
	cout << "\n";
*/

	return 0;
}

