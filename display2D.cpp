#include <iostream>
#include <cstdlib>
#include <cmath>
#include <string>
#include <cstdio>
#include <cassert>
using namespace std;

int main() {
	printf("Enter choice:\n"
			"0. First Generation\n"
			"1. Final Generation\n");
	int option;
	scanf("%d", &option);
	assert(0 <= option && option <= 1);

	FILE *gp = popen("gnuplot -persist", "w");

	if(option == 0)
		fprintf(gp, "set title 'First Generation'\n unset key\n plot 'first.out' w points pointtype 6 pointsize 1\n");
	else
		fprintf(gp, "set title 'Final Generation'\n unset key\n plot 'final.out' w points pointtype 6 pointsize 1\n");

	return 0;
}

