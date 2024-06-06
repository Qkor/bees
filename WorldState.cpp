#include "WorldState.hpp"

WorldState::WorldState(int id, int P, int T, int K){
	this->id = id;
	this->T = T;
	this->K = K;
	this->P = P;
	alive = true;
	timestamp = 0;
	selected_reed = -1;
	reed_acquired = false;
	for(int i=0;i<T;i++){
		reed_queues.push_back(priority_queue<int>());
		cocoons.push_back(0);
	}
}