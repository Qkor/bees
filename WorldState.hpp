#pragma once
#include <queue>
#include <vector>
#include <utility>

using namespace std;

struct comparePairs {
    bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) {
        return (a.first > b.first) || ((a.first == b.first) && (a.second > b.second));
    }
};

class WorldState{
	public:
	int P, T, K;
	priority_queue<pair<int, int>, vector<pair<int, int>>, comparePairs> glasshouse_queue;
	vector<priority_queue<pair<int, int>, vector<pair<int, int>>, comparePairs>> reed_queues;
	vector<int> cocoons;

	WorldState(int P, int T, int K);
};