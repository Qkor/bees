#include <thread>
#include <iostream>
#include <unistd.h> 
#include "WorldState.hpp"
#include "MessageHandler.hpp"
#include "Bee.hpp"

using namespace std;

void firstThread(Bee* bee){
	while(bee->alive){
		do{
			bee->selectReed();
		}while(bee->selected_reed == -1);
		// cout<<"Bee "<<bee->id<<" selected reed "<<bee->selected_reed<<"\n";
		
		bee->requestReed();
		cout<<"Bee "<<bee->id<<" requested reed "<<bee->selected_reed<<"\n";
		while(!bee->canAccessReed()) sleep(0.1); // wait until reed is free
		cout<<"Bee "<<bee->id<<" accessed reed "<<bee->selected_reed<<"\n";
		
		while(bee->alive){
			bee->requestGlasshouse();
			while(!bee->canAccessGlasshouse()) sleep(0.1); // wait until glasshouse is free
			cout<<"Bee "<<bee->id<<" accessed glasshouse\n";
			sleep(1);
			cout<<"Bee "<<bee->id<<" left glasshouse\n";
			bee->releaseGlasshouse();

			bee->layEgg();
			cout<<"Bee "<<bee->id<<" layed "<<bee->eggs<<" egg\n";
		}

		cout<<"Bee "<<bee->id<<" left reed "<<bee->selected_reed<<"\n";
		bee->releaseReed();
	}
	cout<<"Bee "<<bee->id<<" died\n";
}

void secondThread(Bee* bee){
	while(1){
		bee->handleRequests();
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
	WorldState worldState(P,T,K);
	Bee bee(process_id, &worldState, &messageHandler);
	
	thread first(firstThread, &bee);
	thread second(secondThread, &bee);

	first.join();
	second.join();

	messageHandler.~MessageHandler();
	MPI_Finalize();
}