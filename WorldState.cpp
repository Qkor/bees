#include "WorldState.hpp"

WorldState::WorldState(int P, int T, int K){
	this->T = T;
	this->K = K;
	this->P = P;
	for(int i=0;i<T;i++){
		reed_queues.push_back(priority_queue<pair<int, int>,vector<pair<int, int>>,comparePairs>());
		cocoons.push_back(0);
	}
}