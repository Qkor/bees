#pragma once
#include <mpi.h>

struct Message{
	int sender;
	int timestamp;
	int type;
	int data;
};

class MessageHandler{
	MPI_Datatype mpi_struct;
	public:
	MessageHandler();
	~MessageHandler();
	void sendMessage(int sender, int timestamp, int type, int data, int dest);
	Message receiveMessage();
};

