#include "global.h"

void assignCrowdingDistance(vector <Individual*> &pop ) {
	for(auto it = (pop).begin(); it != (pop).end(); ++it) {
		(*it)->crowdDist = 0.0;
	}
//	for(int id = 0; id < M; ++id) {
	for(int id = 0; id < pop.front()->obj.size(); ++id) {
		auto f = [&id](Individual *a, Individual *b)->bool {
						return a->obj[id] < b->obj[id];
				};
		sort(pop.begin(), pop.end(), f);
		pop.front()->crowdDist = pop.back()->crowdDist = inf;
		double l = pop.front()->obj[id];
		double r = pop.back()->obj[id];
		for(int i = 1; i <= pop.size()-2; ++i) {
			pop[i]->crowdDist += (pop[i+1]->obj[id] - pop[i-1]->obj[id])/(r-l);
		}
	}
	for(int i = 0; i < pop.size(); ++i) {
		pop[i]->crowdDist = min(inf, pop[i]->crowdDist)/M;
	}
}

Individual* crowding(Individual *a, Individual *b) {
	int flag = dominates(a, b);
	if(flag == 1)
		return a;
	else if(flag == -1)
		return b;
	else if(a->crowdDist > b->crowdDist)
		return a;
	else
		return b;
}
