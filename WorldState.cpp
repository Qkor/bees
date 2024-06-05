#include "WorldState.hpp"

WorldState::WorldState(int T, int K){
	this->T = T;
	this->K = K;
	for(int i=0;i<T;i++){
		reed_queues.push_back(priority_queue<int>());
		cocoons.push_back(0);
	}
}