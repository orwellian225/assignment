#! /bin/sh

make distclean
make bitonic
make bitonic_omp

./bitonic 3
./bitonic 4
./bitonic 16
./bitonic 20

./bitonic_omp 8 1
./bitonic_omp 8
./bitonic_omp 4 1
./bitonic_omp 4
./bitonic_omp 16 1
./bitonic_omp 16
./bitonic_omp 20 1
./bitonic_omp 20

make distclean