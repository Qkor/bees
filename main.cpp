#include <thread>
#include <iostream>
#include <unistd.h> 
#include "WorldState.hpp"
#include "MessageHandler.hpp"

using namespace std;

// TODO define other types
#define MSG_TYPE_SELECT_REED 0
#define MSG_TYPE_ACK 1
#define MSG_TYPE_DENY 2

int selectReed(){
	// TODO select random free reed or return -1
	return 1;
}

void firstThread(WorldState* worldState, MessageHandler* messageHandler){
	while(worldState->alive){

		// select reed
		worldState->selected_reed = selectReed();
		if(worldState->selected_reed < 0){
			// TODO wait for message
			continue;
		}

		// send message about selected reed to all processes
		for(int i=0;i<worldState->P;i++)
			messageHandler->sendMessage(worldState->id, worldState->timestamp, MSG_TYPE_SELECT_REED, worldState->selected_reed, i, 0);


		// TODO implement accessing critical section and updating timestamps (it doesnt work correctly now)
		// get responses from other processes
		bool canAccessReed = true;
		for(int i=0;i<worldState->P;i++){
			Message m = messageHandler->receiveMessage(1);
			if(m.type == MSG_TYPE_DENY)
				canAccessReed = false;
		}

		// accessing reed
		if(canAccessReed){
			worldState->reed_acquired = true;
			cout<<"Bee "<<worldState->id<<" accessed reed "<<worldState->selected_reed<<"\n";
			// while(worldState->eggs<5){
			// 	// TODO implement accessing glasshouse queue
			// }
			sleep(1);
			cout<<"Bee "<<worldState->id<<" left reed "<<worldState->selected_reed<<"\n";
		}

		// leaving reed
		worldState->reed_acquired = false;
		worldState->selected_reed = -1;
		sleep(1);

	}
}

void secondThread(WorldState* worldState, MessageHandler* messageHandler){
	while(worldState->alive){
		Message m = messageHandler->receiveMessage();
		// TODO handle messages
		switch(m.type){
			case MSG_TYPE_SELECT_REED:
				if((worldState->selected_reed == m.data) && (worldState->reed_acquired || (worldState->timestamp < m.timestamp) || (worldState->timestamp == m.timestamp && worldState->id < m.sender)))
					messageHandler->sendMessage(worldState->id, worldState->timestamp, MSG_TYPE_DENY, 0, m.sender, 1);
				else
					messageHandler->sendMessage(worldState->id, worldState->timestamp, MSG_TYPE_ACK, 0, m.sender, 1);
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