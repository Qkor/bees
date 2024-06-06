#include <thread>
#include <iostream>
#include "WorldState.hpp"
#include "MessageHandler.hpp"

using namespace std;

// TODO define other types
#define MSG_TYPE_SELECT_REED 0

int selectReed(){
	// TODO select random free reed or return -1
	return 1;
}

void firstThread(WorldState* worldState, MessageHandler* messageHandler){
	while(worldState->alive){

		// select reed
		int selected_reed = selectReed();
		if(selected_reed < 0){
			// TODO wait for message
			continue;
		}

		// send message about selected reed to all processes
		for(int i=0;i<worldState->P;i++)
			messageHandler->sendMessage(worldState->id, worldState->timestamp, MSG_TYPE_SELECT_REED, selected_reed, i, 0);

		// get responses from other processes
		for(int i=0;i<worldState->P;i++){
			Message m = messageHandler->receiveMessage(1);
		}

		// TODO finish the algorithm
	}

}

void secondThread(WorldState* worldState, MessageHandler* messageHandler){
	while(worldState->alive){
		Message m = messageHandler->receiveMessage();
		// TODO handle messages
		switch(m.type){
			case MSG_TYPE_SELECT_REED:
				cout<<m.sender<<"\n";
		}
	}
}


int main(int argc, char** argv){
	int provided;
	MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
	if (provided < MPI_THREAD_MULTIPLE) {
		cout << "Error: thread support not provided\n";
		MPI_Finalize();
	}

	int process_id, number_of_processes;
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);
	MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);

	if(argc != 3) {
		if(!process_id)
			cout << "Usage: mpiexec -n Number_of_bees " << argv[0] << " Number_of_reeds Number_of_flowers\n";
		MPI_Finalize();
		return 1;
	}

	// number of bees
	int P = number_of_processes;
	// number of reeds
	int T = std::atoi(argv[1]);
	// number of flowers
	int K = std::atoi(argv[2]);

	MessageHandler messageHandler;

	WorldState worldState(process_id, P,T,K);
	
	thread first(firstThread, &worldState, &messageHandler);
	thread second(secondThread, &worldState, &messageHandler);

	first.join();
	second.join();

	messageHandler.~MessageHandler();
	MPI_Finalize();
}