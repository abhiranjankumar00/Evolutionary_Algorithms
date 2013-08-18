#include "global.h"

void BIT::init() {
	fill(tree.begin(), tree.end(), 0);
}

void BIT::updateFreq(int idx) {
	while(idx <= popSize+5) {
		tree[idx]++;
		idx += (idx & (-idx));
	}
}

int BIT::readFreq(int idx) {
	int ret = 0;
	while(idx > 0) {
		ret += tree[idx];
		idx -= (idx & (-idx));
	}
	return ret;
}

int BIT::readFreq(int l, int r) {
	if(l > r)
		return 0;
	int ret = readFreq(r);
	if(l > 0)
		ret -= readFreq(l-1);
	return ret;
}

BIT::BIT() {
	tree.resize(popSize+10);
	init();
}
