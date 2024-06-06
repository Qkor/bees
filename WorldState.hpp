#pragma once
#include <queue>
#include <vector>

using namespace std;

class WorldState{
	public:
	bool alive;
	int P, T, K, timestamp, id;
	priority_queue<int> glasshouse_queue;
	vector<priority_queue<int>> reed_queues;
	vector<int> cocoons;

	WorldState(int id, int P, int T, int K);
};