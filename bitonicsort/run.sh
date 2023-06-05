make distclean
make bitonic
make bitonic_omp

# ./bitonic 3
# ./bitonic 4
# ./bitonic 16
# ./bitonic 20

./bitonic_omp 8 4
./bitonic_omp 8 1
# ./bitonic_omp 4 4
# ./bitonic_omp 16 4
# ./bitonic_omp 20 4