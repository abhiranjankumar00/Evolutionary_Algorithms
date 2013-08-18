#include "global.h"

// SBX Crossover
void crossover(Individual *p1, Individual *p2, Individual *c1, Individual *c2) {
	if(Rand() > cross_prob) {
		for(int i = 0; i < n; ++i) {
			c1->val[i] = p1->val[i];
			c2->val[i] = p2->val[i];
		}
		return;
	}
	for(int i = 0; i < n; ++i) {
		if(Rand() > 0.5) {
			c1->val[i] = p1->val[i];
			c2->val[i] = p2->val[i];
			continue;
		}
		//y2 >= y1
		double y1 = min(p1->val[i], p2->val[i]);
		double y2 = max(p1->val[i], p2->val[i]);

		double r = Rand();

		double beta = (y2+y1)/(y2-y1);
		double alpha = 2.0 - pow(beta, -(cross_idx+1));
		double betaq;
		if(r <= 1.0/alpha)
			betaq = pow(r*alpha, 1.0/(cross_idx+1));
		else
			betaq = pow(1.0/(2.0 - r*alpha), 1.0/(cross_idx+1));

		c1->val[i] = 0.5 * ((y1+y2) - betaq*(y2-y1));

		beta = (2.0 - y1 - y2)/(y2 - y1);
		alpha = 2 - pow(beta, -(cross_idx+1));
		if(r <= 1.0/alpha)
			betaq = pow(r*alpha, 1.0/(cross_idx+1));
		else
			betaq = pow(1.0/(2.0 - r*alpha), 1.0/(cross_idx+1));

		c2->val[i] = 0.5 * ((y1+y2) + betaq*(y2-y1));

		c1->val[i] = max(c1->val[i], 0.0); c1->val[i] = min(c1->val[i], 1.0);
		c2->val[i] = max(c2->val[i], 0.0); c2->val[i] = min(c2->val[i], 1.0);

		if(Rand() <= 0.5)
			swap(c1->val[i], c2->val[i]);
	}
}
