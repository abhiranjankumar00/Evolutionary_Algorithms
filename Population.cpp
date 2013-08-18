#include "global.h"

Population::Population(const vector <int> &_obj) {
	parent.resize(popSize);
	child.resize(popSize);
	mixed.resize(2*popSize);
	fill(parent.begin(), parent.end(), nullptr);
	fill(child.begin(), child.end(), nullptr);
	fill(mixed.begin(), mixed.end(), nullptr);
	idx1.resize(popSize);
	idx2.resize(popSize);

	nObj = _obj.size();
	assert(_obj.size() <= M && _obj.size() > 0);
	obj.resize(_obj.size());
	for(int i = 0; i < _obj.size(); ++i) {
		obj[i] = _obj[i];
		assert(obj[i] >= 0 && obj[i] < M);
	}
}

void Population::initialise() {
	for(int i = 0; i < popSize; ++i) {
		parent[i] = new Individual();
		parent[i]->init();
		parent[i]->evaluateObjective(obj); 
	}
	assignCrowdingDistance(parent);
}

void Population::runGeneration() {
	createChild();
	assert(child.size() == popSize);
	for(int i = 0; i < popSize; ++i) {
		assert(child[i] != nullptr);
		child[i]->mutate();
		child[i]->evaluateObjective(obj);
	}

	for(int i = 0; i < popSize; ++i) {
		mixed[i] = parent[i];
	}
	for(int i = popSize; i <= 2*popSize-1; ++i) {
		mixed[i] = child[i-popSize];
	}

	vector <vector<Individual*> > fronts;
	generateFronts(fronts);

	parent.clear();

	int id;
	for(id = 0; id < fronts.size() && parent.size() + fronts[id].size() <= popSize; id++) {
		for(int j = 0; j < fronts[id].size(); ++j) {
			parent.push_back(fronts[id][j]);
		}
	}
	assignCrowdingDistance(fronts[id]);
	auto f = [](Individual *a, Individual *b) -> bool {
				return a->crowdDist > b->crowdDist;
					};
	sort(fronts[id].begin(), fronts[id].end(), f);

	for(; id < fronts.size(); id++) {
		for(int j = 0; j < fronts[id].size(); j++) {
			if(parent.size() < popSize)
				parent.push_back(fronts[id][j]);
			else
				delete(fronts[id][j]);
		}
	}
	fronts.clear();
	for(auto it = (child).begin(); it != (child).end(); ++it) {
		*it = nullptr;
	}
	for(auto it = (mixed).begin(); it != (mixed).end(); ++it) {
		*it = nullptr;
	}
}
// Use tournament selection
void Population::createChild() {
	for(int i = 0; i < popSize; ++i) {
		idx1[i] = idx2[i] = i;
//		delete(child[i]);
//		child[i] = nullptr;
		child[i] = new Individual();
	}

	for(int i = 0; i < popSize; ++i) {
		int tmp = i + (rand()%(popSize-i));
		swap(idx1[i], idx1[tmp]);
		tmp = i + (rand()%(popSize-i));
		swap(idx2[i], idx2[tmp]);
	}

	Individual *p1, *p2;
	for(int i = 0; i < popSize; i+=4) {
		p1 = crowding(parent[idx1[i]], parent[idx1[i+1]]);
		p2 = crowding(parent[idx1[i+2]], parent[idx1[i+3]]);
		crossover(p1, p2, child[i], child[i+1]);
		assert(child[i] != nullptr);
		assert(child[i+1] != nullptr);

		p1 = crowding(parent[idx2[i]], parent[idx2[i+1]]);
		p2 = crowding(parent[idx2[i+2]], parent[idx2[i+3]]);
		crossover(p1, p2, child[i+2], child[i+3]);
		assert(child[i+2] != nullptr);
		assert(child[i+3] != nullptr);
	}
}

void Population::generateFronts(vector <vector<Individual*>> &fronts) {
	fronts.clear();
	list <Individual *> *lst = nullptr;
	list <Individual *> *tmp = nullptr;

	lst = new list <Individual*>();
	for(auto it = (mixed).begin(); it != (mixed).end(); ++it) {
		lst->push_back(*it);
	}

	while(lst->empty() == false) {
		tmp = new list <Individual*>();
		auto it = lst->begin();

		while(it != lst->end()) {
			bool isDominated = false;
			auto jt = it; ++jt;

			while(jt != lst->end()) {
				int flag = dominates(*it, *jt);
				if(flag == 0) {
					jt++;
				}
				else if(flag == 1) {
					tmp->push_back(*jt);
					jt = lst->erase(jt);
				}
				else { // flag == -1
					isDominated = true;
					break;
				}
			}

			if(isDominated == true) {
				tmp->push_back(*it);
				it = lst->erase(it);
			}
			else 
				++it;
		}

		fronts.push_back(vector <Individual*>());
		for(auto it = (*lst).begin(); it != (*lst).end(); ++it) {
			fronts.back().push_back(*it);
		}
		lst->clear();
		delete(lst);
		lst = tmp;
		tmp = nullptr;
	}
}

void Population::runNSGA2() {
	double g = 0.0;
/*
	for(int i = 0; i < popSize; ++i) {
		g += parent[i]->g;
	}
	g/=popSize;
*/
	printf("Generation ");
	for(int i = 0; i < nGen; ++i) {
		runGeneration();
		printf("#%d ", i+1);
		fflush(stdout);
/*
		g = 0.0;
		for(int i = 0; i < popSize; ++i) {
			g += parent[i]->g;
		}
		g/=popSize;
		cout << "g = " << g << "\n";
		fflush(stdout);
*/
	}
	cout << endl;
}

Population::~Population() {
	for(auto it = (parent).begin(); it != (parent).end(); ++it) {
		delete(*it);
	}
}

void Population::print(FILE *fp) {
	for(int i = 0; i < popSize; ++i) {
		parent[i]->print(fp);
	}
	fprintf(fp, "\n");
	fflush(fp);
}

void Population::print(const vector <int>& idx, FILE *fp) {
	for(int i = 0; i < popSize; ++i) {
		parent[i]->print(idx, fp);
	}
	fprintf(fp, "\n");
	fflush(fp);
}
