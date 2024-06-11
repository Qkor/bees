#pragma once
#include <set>
#include <vector>
#include <utility>

using namespace std;

class WorldState{
	public:
	int P, T, K;
	set<pair<int, int>> glasshouse_queue;
	vector<set<pair<int, int>>> reed_queues;
	vector<int> cocoons;

	WorldState(int P, int T, int K);
};