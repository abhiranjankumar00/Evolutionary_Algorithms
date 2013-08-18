CPP=g++ -O2 -g -std=c++0x -Wno-unused-result -D_GLIBCXX_DEBUG

a.out: rand.o Individual.o Population.o Crossover.o CrowdDistance.o Dominance.o NSGA2.o Redundancy.o BIT.o DisjointSet.o
	$(CPP) -o a.out -lm rand.o Individual.o Population.o Crossover.o CrowdDistance.o Dominance.o NSGA2.o Redundancy.o BIT.o DisjointSet.o

rand.o: rand.cpp global.h
	$(CPP) -c rand.cpp

Individual.o: Individual.cpp global.h
	$(CPP) -c Individual.cpp

Population.o: Population.cpp Individual.o global.h
	$(CPP) -c Population.cpp

Crossover.o: Crossover.cpp Individual.o global.h
	$(CPP) -c Crossover.cpp

CrowdDistance.o: CrowdDistance.cpp Individual.o global.h
	$(CPP) -c CrowdDistance.cpp

Dominance.o: Dominance.cpp Individual.o global.h
	$(CPP) -c Dominance.cpp

NSGA2.o: NSGA2.cpp global.h
	$(CPP) -c NSGA2.cpp

Redundancy.o: Redundancy.cpp Population.o global.h BIT.o DisjointSet.o
	$(CPP) -c Redundancy.cpp

BIT.o: BIT.cpp global.h
	$(CPP) -c BIT.cpp

DisjointSet.o: DisjointSet.cpp global.h
	$(CPP) -c DisjointSet.cpp

# a.out: global.h NSGA2.cpp rand.cpp Individual.cpp Population.cpp Crossover.cpp CrowdDistance.cpp Dominance.cpp Redundancy.cpp
#	$(CPP) NSGA2.cpp rand.cpp Individual.cpp Population.cpp Crossover.cpp CrowdDistance.cpp Dominance.cpp Redundancy.cpp
