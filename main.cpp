#include <mpi.h>
#include <thread>
#include <iostream>

using namespace std;

void firstThread(){
	cout<<"first thread\n";
}

void secondThread(){
	cout<<"second thread\n";
}


int main(int argc, char** argv){
	MPI_Init(&argc, &argv);

	int process_id;
	MPI_Comm_rank(MPI_COMM_WORLD, &process_id);

	thread first(firstThread);
	thread second(secondThread);

	first.join();
	second.join();

	MPI_Finalize();
}