#include "Bee.hpp"

Bee::Bee(int id, WorldState* worldState, MessageHandler* messageHandler){
	this->id = id;
	this->worldState = worldState;
	this->messageHandler = messageHandler;
	alive = true;
	eggs = 0;
	timestamp = 0;
	selected_reed = -1;
	reed_acquired = false;
}

int Bee::selectReed(){
	std::vector<int> available_reeds;
	for(int i=0;i<worldState->T;i++){
		if(worldState->reed_queues[i].empty())
			available_reeds.push_back(i);
	}
	if (available_reeds.empty()){
		selected_reed = -1;
		return -1;
	}
	std::random_device rd; 
	std::mt19937 gen(rd()); 
	std::uniform_int_distribution<> distr(0, available_reeds.size()-1);  
	int reed_num = distr(gen);
	selected_reed = reed_num;
	return reed_num;
}

bool Bee::requestReed(){
	mtx.lock();
	timestamp++;
	int request_timestamp = timestamp;
	std::pair<int,int> request;
	request.first = request_timestamp;
	request.second = id;
	worldState->reed_queues[selected_reed].push(request);
	mtx.unlock();

	for(int i=0;i<worldState->P;i++){
		if(i==id) continue;
		messageHandler->sendMessage(id,request_timestamp,MSG_TYPE_REED_REQUEST,selected_reed,i,MSG_TAG_REQUEST);
	}

	bool canAccess = true;
	for(int i=0;i<worldState->P;i++){
		if(i==id) continue;
		Message m = messageHandler->receiveMessage(MSG_TAG_REPLY);
		if(m.timestamp <= request_timestamp) canAccess = false;
	}
	return canAccess;
}

bool Bee::requestGlasshouse(){
	mtx.lock();
	timestamp++;
	int request_timestamp = timestamp;
	std::pair<int,int> request;
	request.first = request_timestamp;
	request.second = id;
	worldState->glasshouse_queue.push(request);
	mtx.unlock();

	for(int i=0;i<worldState->P;i++){
		if(i==id) continue;
		messageHandler->sendMessage(id,request_timestamp,MSG_TYPE_GLASSHOUSE_REQUEST,0,i,MSG_TAG_REQUEST);
	}

	bool canAccess = true;
	for(int i=0;i<worldState->P;i++){
		if(i==id) continue;
		Message m = messageHandler->receiveMessage(MSG_TAG_REPLY);
		if(m.timestamp <= request_timestamp) canAccess = false;
	}
	return canAccess;
}

void Bee::handleRequests(){
	Message m = messageHandler->receiveMessage(MSG_TAG_REQUEST);
	mtx.lock();
	timestamp = max(timestamp, m.timestamp) + 1;
	std::pair<int,int> request;
	request.first = m.timestamp;
	request.second = m.sender;
	switch(m.type){
		case MSG_TYPE_REED_REQUEST:
			messageHandler->sendMessage(id,timestamp,MSG_TYPE_REED_REPLY,m.data,m.sender,MSG_TAG_REPLY);
			worldState->reed_queues[m.data].push(request);
			break;
		case MSG_TYPE_REED_RELEASE:
			worldState->reed_queues[m.data].pop();
			break;
		case MSG_TYPE_GLASSHOUSE_REQUEST:
			messageHandler->sendMessage(id,timestamp,MSG_TYPE_GLASSHOUSE_REPLY,0,m.sender,MSG_TAG_REPLY);
			worldState->glasshouse_queue.push(request);
			break;
		case MSG_TYPE_GLASSHOUSE_RELEASE:
			worldState->glasshouse_queue.pop();
			break;
	}
	mtx.unlock();
}

void Bee::releaseReed(){
	mtx.lock();
	timestamp++;
	int request_timestamp = timestamp;
	worldState->reed_queues[selected_reed].pop();
	mtx.unlock();

	for(int i=0;i<worldState->P;i++){
		if(i==id) continue;
		messageHandler->sendMessage(id,request_timestamp,MSG_TYPE_REED_RELEASE,selected_reed,i,MSG_TAG_REQUEST);
	}

	selected_reed = -1;
	reed_acquired = false;
}

void Bee::releaseGlasshouse(){
	mtx.lock();
	timestamp++;
	int request_timestamp = timestamp;
	worldState->glasshouse_queue.pop();
	mtx.unlock();

	for(int i=0;i<worldState->P;i++){
		if(i==id) continue;
		messageHandler->sendMessage(id,request_timestamp,MSG_TYPE_GLASSHOUSE_RELEASE,selected_reed,i,MSG_TAG_REQUEST);
	}
}

bool Bee::canAccessReed(){
	mtx.lock();
	bool canAccess = !(worldState->reed_queues[selected_reed].empty()) && worldState->reed_queues[selected_reed].top().second == id;
	mtx.unlock();
	return canAccess;
}

bool Bee::canAccessGlasshouse(){
	mtx.lock();
	bool canAccess = !(worldState->glasshouse_queue.empty()) && worldState->glasshouse_queue.top().second == id;
	mtx.unlock();
	return canAccess;
}

void Bee::layEgg(){
	eggs++;
	if(eggs >= 5) alive = false;
}