#include "WorldState.hpp"

WorldState::WorldState(int P, int T, int K){
	this->T = T;
	this->K = K;
	this->P = P;
	for(int i=0;i<T;i++){
		reed_queues.push_back(set<pair<int, int>>());
		cocoons.push_back(0);
	}
}