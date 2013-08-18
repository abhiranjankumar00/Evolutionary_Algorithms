#include "global.h"

void setSeed() {
	time_t tm;
	time(&tm);
	srand(tm);
}

double Rand() {
	const int static MOD = 1e6;
	return (rand() % (MOD+1))*1.0 / MOD;
}
