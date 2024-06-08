#pragma once
#include "WorldState.hpp"
#include "MessageHandler.hpp"
#include <random>
#include <mutex>

#define MSG_TYPE_REED_REQUEST 0
#define MSG_TYPE_REED_REPLY 1
#define MSG_TYPE_REED_RELEASE 2

#define MSG_TAG_REQUEST 0
#define MSG_TAG_REPLY 1

class Bee{
	public:
	std::mutex mtx;
	WorldState* worldState;
	MessageHandler* messageHandler;
	int timestamp;
	int id;
	bool alive;
	int selected_reed;
	bool reed_acquired;
	int eggs;
	Bee(int id, WorldState* worldState, MessageHandler* messageHandler);
	int selectReed();
	bool requestReed();
	bool canAccessReed();
	void handleRequests();
	void releaseReed();
};