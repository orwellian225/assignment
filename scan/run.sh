#!/bin/sh

make distclean
make

# ./scan 1
# ./scan 2
# ./scan 4
# ./scan 8
./scan 24

# ./scan_omp 1 1
# ./scan_omp 1
# ./scan_omp 2 1
# ./scan_omp 2
# ./scan_omp 4 1
# ./scan_omp 4
# ./scan_omp 8 1
# ./scan_omp 8
./scan_omp 24 1
./scan_omp 24

mpirun -np 1 ./scan_mpi 24
mpirun -np 4 ./scan_mpi 24

make distclean