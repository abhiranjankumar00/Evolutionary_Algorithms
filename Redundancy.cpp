#include "global.h"

Redundant::Redundant(Population *_pop) {
	pop = _pop;
	obj = pop->obj;
	assert(pop != nullptr);

	disorder.resize(pop->obj.size());
	for(int i = 0; i < disorder.size(); ++i) {
		disorder[i].resize(pop->obj.size());
		for(int j = 0; j < disorder[i].size(); ++j) {
			disorder[i][j] = 0.0;
		}
	}

	assert(pop->parent.size() == popSize);
	ranks.resize(pop->parent.size());
	fill(ranks.begin(), ranks.end(), nullptr);
	for(int i = 0; i < ranks.size(); ++i) {
		ranks[i] = new vector <int> (pop->nObj, -1);
//		ranks[i].resize(pop->nObj);
	}
}

void Redundant::calculateRanks() {
	for(int i = 0; i < pop->parent.size(); ++i) {
		pop->parent[i]->idx = i;
	}

	for(int id = 0; id < pop->nObj; ++id) {
		auto f = [&id](Individual *a, Individual *b)->bool {
						return a->obj[id] < b->obj[id];
					};
		sort(pop->parent.begin(), pop->parent.end(), f);
		for(int i = 0; i < popSize; ++i) {
			(*ranks[pop->parent[i]->idx])[id] = i+1;
//			ranks[pop->parent[i]->idx]->at(id)/*[id]*/ = i+1;
		}
	}
/*
	printf("Ranks Matrix: \n" );
	for(int i = 0; i < popSize; ++i) {
		for(int j = 0; j < pop->nObj; ++j) {
			printf("%3d ", ranks[i]->at(j));
			//printf("%3d ", ranks[i][j]);
//			printf("%8.4lf ", pop->parent[i]->obj[j]);
		}
		cout << "\n";
	}
	cout << "\n" << endl;
*/
}

void Redundant::calculateDisorder() {
	calculateRanks();

	assert(disorder.size() == pop->nObj);

	BIT bit;
	int rank;

	double L = 1.0*popSize*(popSize-1)/2.0;
	//double L = 1.0*popSize*(popSize-1.0)*(2.0*popSize-1)/6.0;

	printf("\nDisorder Matrix: \n" );
	for(int i = 0; i < ranks.front()->size(); ++i) {
		assert(disorder.size() == pop->nObj);

		auto f = [&i](vector <int> *a, vector <int> *b) -> bool {
						assert(a != nullptr);
						assert(b != nullptr);
						return a->at(i) < b->at(i);
					};

		sort(ranks.begin(), ranks.end(), f);

		for(int j = 0; j < ranks.front()->size(); ++j) {
			disorder[i][j] = 0.0;
			bit.init();

			for(int k = 0; k < ranks.size(); ++k) {
				disorder[i][j] += bit.readFreq(ranks[k]->at(j), popSize+1);
				//disorder[i][j] += square(bit.readFreq(ranks[k]->at(j), popSize+1));
				bit.updateFreq(ranks[k]->at(j));
			}

			disorder[i][j] /= L;

			printf("%8.4lf ", disorder[i][j]);
		}
		cout << "\n";
	}
	cout << "\n" << endl;

}

vector <int> Redundant::reduceObjective() {
	calculateDisorder();
	DisjointSet sets;
	sets.init(pop->nObj);

	printf("Objective sets: " );
	for(auto it = (obj).begin(); it != (obj).end(); ++it) {
		printf("%d ", *it+1);
	}
	cout << "\n" << endl;

	for(int i = 0; i < disorder.size(); ++i) {
		for(int j = 0; j < disorder[i].size(); ++j) {
			if(disorder[i][j] <= eps)
				sets.mergeSet(i, j);
		}
	}

	map <int, vector <int> > m;
	for(int i = 0; i < pop->nObj; ++i) {
		int p = sets.findSet(i);
		if(m.count(p) == 0)
			m[p] = vector <int> ();
		m[p].push_back(i);
	}
	vector <double> dist(pop->nObj, 0.0);

	cout << "Redundant Objective Sets:\n";
	for(auto it = (m).begin(); it != (m).end(); ++it) {
		printf("[ " );
		for(auto jt = (it->second).begin(); jt != (it->second).end(); ++jt) {
			if(jt != (it->second).begin()) 
				printf(", " );
			printf("F(%d)", 1+obj[*jt]);

			for(auto kt = (it->second).begin(); kt != (it->second).end(); ++kt) {
				dist[*jt] += disorder[*jt][*kt];
			}
		}
		printf(" ]\n" );
	}
	cout << "\n";
/*
	cout << "Dist: ";
	for(auto it = (dist).begin(); it != (dist).end(); ++it) {
		printf("%8.4lf ", *it);
	}
	cout << "\n" << endl;
*/
	vector <int> ret;

	for(auto it = (m).begin(); it != (m).end(); ++it) {
		vector <int> &v = it->second;
		assert(v.size() > 0);
		int id = 0;
		for(int i = 1; i <= v.size()-1; ++i) {
			if(dist[v[i]] <= dist[v[id]] )
				id = i;
		}
//		ret.push_back(v[id]);
		ret.push_back(obj[v[id]]);
	}

	cout << "Reducted Objective Set: [ ";
	for(int i = 0; i < ret.size(); ++i) {
		if(i != 0)
			cout << ", ";
		printf("F(%d)", 1+ ret[i]);
	}
	cout << " ]\n" << endl;

	return ret;
}

Redundant::~Redundant() {
	for(int i = 0; i < ranks.size(); ++i) {
		delete(ranks[i]);
	}
	ranks.clear();
}
