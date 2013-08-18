#include "global.h"

bool dominates(Individual *a, Individual *b) {
	assert(a != nullptr);
	assert(b != nullptr);
	assert(a->obj.size() == b->obj.size());
	int flag1 = 0, flag2 = 0;

	for(int i = 0; i < a->obj.size(); ++i) {
		if(abs(a->obj[i] - b->obj[i]) <= 1e-50)
			continue;
		else if(a->obj[i] < b->obj[i])
			flag1 = 1;
		else 
			flag2 = 1;
	}
	if(flag1 == 1 && flag2 == 0)
		return 1;
	else if(flag1 == 0 && flag2 == 1)
		return -1;
	else 
		return 0;
}
