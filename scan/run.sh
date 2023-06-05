#! /bin/sh

make clean
make
make scan_mpi

# ./scan 1
# ./scan 2
# ./scan 4
# ./scan 8

mpirun -np 4 ./scan_mpi 3