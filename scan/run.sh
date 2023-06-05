#! /bin/sh

make distclean
make

./scan 1
./scan 2
./scan 4
./scan 8
./scan 16

./scan_omp 1 1
./scan_omp 1
./scan_omp 2 1
./scan_omp 2
./scan_omp 4 1
./scan_omp 4
./scan_omp 8 1
./scan_omp 8
./scan_omp 16 1
./scan_omp 16

# mpirun -np 4 ./scan_mpi 3

make distclean