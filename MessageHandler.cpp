#include "MessageHandler.hpp"
#include <cstddef>

MessageHandler::MessageHandler() {
	int block_lengths[4] = {1,1,1,1};
	MPI_Datatype types[4] = {MPI_INT,MPI_INT,MPI_INT,MPI_INT};
	MPI_Aint offsets[4];
	offsets[0] = offsetof(Message, sender);
	offsets[1] = offsetof(Message, timestamp);
	offsets[2] = offsetof(Message, type);
	offsets[3] = offsetof(Message, data);
	MPI_Type_create_struct(4, block_lengths, offsets, types, &mpi_struct);
	MPI_Type_commit(&mpi_struct);
}

void MessageHandler::sendMessage(int sender, int timestamp, int type, int data, int dest, int tag){
	Message m;
	m.sender = sender;
	m.timestamp = timestamp;
	m.type = type;
	m.data = data;
	MPI_Send(&m, 1, mpi_struct, dest, tag, MPI_COMM_WORLD);
}

Message MessageHandler::receiveMessage(int tag){
	Message m;
	MPI_Recv(&m, 1, mpi_struct, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	return m;
}

MessageHandler::~MessageHandler(){
	MPI_Type_free(&mpi_struct);
}