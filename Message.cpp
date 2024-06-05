#include "Message.hpp"
#include <cstddef>

void createMPIType(MPI_Datatype &mpi_struct) {
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