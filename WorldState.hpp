#pragma once
#include <queue>
#include <vector>

using namespace std;

class WorldState{
	public:
	int T, K;
	priority_queue<int> glasshouse_queue;
	vector<priority_queue<int>> reed_queues;
	vector<int> cocoons;

	WorldState(int T, int K);
};