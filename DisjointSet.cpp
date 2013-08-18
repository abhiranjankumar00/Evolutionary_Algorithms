#include "global.h"

DisjointSet::DisjointSet() {
	par.resize(M);
}

void DisjointSet::init(int sz) {
	assert(sz > 0 && sz <= M);
	par.resize(sz);
	for(int i = 0; i < par.size(); ++i) {
		par[i] = i;
	}
}

int DisjointSet::findSet(int idx) {
	return par[idx] = (idx == par[idx]?idx: findSet(par[idx]));
}

void DisjointSet::mergeSet(int u, int v) {
	int pu = findSet(u);
	int pv = findSet(v);
	if(pu < pv)
		par[pv] = pu;
	else
		par[pu] = pv;
}
