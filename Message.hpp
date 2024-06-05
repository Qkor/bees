#pragma once
#include <mpi.h>

struct Message{
	int sender;
	int timestamp;
	int type;
	int data;
};

void createMPIType(MPI_Datatype &mpi_struct);