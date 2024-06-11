#pragma once
#include "WorldState.hpp"
#include "MessageHandler.hpp"
#include <random>
#include <mutex>

#define MSG_TYPE_REED_REQUEST 0
#define MSG_TYPE_REED_REPLY 1
#define MSG_TYPE_REED_RELEASE 2
#define MSG_TYPE_GLASSHOUSE_REQUEST 3
#define MSG_TYPE_GLASSHOUSE_REPLY 4
#define MSG_TYPE_GLASSHOUSE_RELEASE 5

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
	int eggs;
	Bee(int id, WorldState* worldState, MessageHandler* messageHandler);
	int selectReed();
	bool requestReed();
	bool requestGlasshouse();
	bool canAccessReed();
	bool canAccessGlasshouse();
	void handleRequests();
	void releaseReed();
	void releaseGlasshouse();
	void layEgg();
};