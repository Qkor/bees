#include <mpi.h>
#include <thread>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class WorldState{
	public:
	int T, K;
	priority_queue<int> glasshouse_queue;
	vector<priority_queue<int>> reed_queues;
	vector<int> cocoons;

	WorldState(int T, int K){
		this->T = T;
		this->K = K;
		for(int i=0;i<T;i++){
			reed_queues.push_back(priority_queue<int>());
			cocoons.push_back(0);
		}
	}
};

void firstThread(WorldState* world_state){
	cout<<"first thread\n";
}

void secondThread(WorldState* world_state){
	cout<<"second thread\n";
}


int main(int argc, char** argv){
	MPI_Init(&argc, &argv);

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

	WorldState world_state(T,K);

	thread first(firstThread, &world_state);
	thread second(secondThread, &world_state);

	first.join();
	second.join();

	MPI_Finalize();
}