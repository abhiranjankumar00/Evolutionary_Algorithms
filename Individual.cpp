#include "global.h"

Individual:: Individual() {
	val.resize(n);
	theta.resize(M-1);
//	obj.resize(M);
}

void Individual::print(FILE *fp) {
	for(int i = 0; i < obj.size(); ++i) {
		if(i != 0)
			fprintf(fp, "\t");
		fprintf(fp, "%e", obj[i]);
	}
	fprintf(fp, "\n");
}

void Individual::print(const vector <int> &idx, FILE* fp) {
	for(int i = 0; i < idx.size(); ++i) {
		if(i != 0)
			fprintf(fp, "\t");
		fprintf(fp, "%e", obj[idx[i]]);
	}
	fprintf(fp,"\n");
}

void Individual:: init() {
	g = crowdDist = 0.0;
	for(int i = 0; i < n; ++i) {
		val[i] = Rand();
	}
}

void Individual::calculateG() {
	g = 0.0;
	for(int i = M-1; i < n; ++i) {
		g += ((val[i] - 0.5)*(val[i] - 0.5));
	}
}

void Individual::scale() {
	calculateG();
	for(int i = 0; i < M-1; ++i) {
		double x = val[i];
		if(i < I-1)
			theta[i] = x*PI/2.0;
		else
			theta[i] = PI*(1+2*g*x)/(4.0*(1+g));
	}
}

double Individual::f(int idx) {
	assert(idx >= 0 && idx < M);
	double ret = 1+100*g;
	for(int i = 0; i < M-1-idx; ++i) {
		ret *= cos(theta[i]);
	}
	if(idx != 0)
		ret *= sin(theta[M-1-idx]);
	return ret;
}

void Individual::evaluateObjective(const vector <int> &ob) {
	scale();
	obj.resize(ob.size());
	for(int i = 0; i < ob.size(); ++i) {
		obj[i] = f(ob[i]);
	}
/*
	for(int i = 0; i < M; ++i) {
		obj[i] = f(i);
	}
*/
}

Individual::~Individual() {
	val.clear();
	obj.clear();
	theta.clear();
}

// Polynomial mutation
void Individual::mutate() {
	for(int i = 0; i < n; ++i) {
		if(Rand() > mut_prob)
			continue;
		double &y = val[i];
		double r = Rand();
		double mut_pow = 1.0/(mut_idx + 1.0);
		double delta;

		if(r <= 0.5) {
			double x = 1-y;
			double val = 2*r + (1.0 - 2*r)*pow(x, mut_idx+1.0);
			delta = pow(val, mut_pow) - 1;
		}
		else {
			double x = y;
			double val = 2*(1-r) + 2*(r-0.5)*pow(x, mut_idx+1.0);
			delta = 1 - pow(val, mut_pow);
		}
		y += delta;

		y = max(y, 0.0);
		y = min(y, 1.0);
	}
}
