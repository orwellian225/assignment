make distclean
make sssp
make sssp_omp

# ./sssp ./graphs/graph_0.txt
# ./sssp ./graphs/graph_1.txt
# ./sssp ./graphs/graph_2.txt
# ./sssp ./graphs/graph_3.txt
# ./sssp ./graphs/graph_4.txt
# ./sssp ./graphs/graph_5.txt
# ./sssp ./graphs/graph_6.txt
# ./sssp ./graphs/graph_7.txt
# ./sssp ./graphs/graph_8.txt
# ./sssp ./graphs/graph_9.txt
./sssp ./graphs/graph_10.txt

# ./sssp_omp ./graphs/graph_0.txt
# ./sssp_omp ./graphs/graph_1.txt
# ./sssp_omp ./graphs/graph_2.txt
# ./sssp_omp ./graphs/graph_3.txt
# ./sssp_omp ./graphs/graph_4.txt
# ./sssp_omp ./graphs/graph_5.txt
# ./sssp_omp ./graphs/graph_6.txt
# ./sssp_omp ./graphs/graph_7.txt
# ./sssp_omp ./graphs/graph_8.txt
# ./sssp_omp ./graphs/graph_9.txt
./sssp_omp ./graphs/graph_10.txt 0 8
./sssp_omp ./graphs/graph_10.txt 0 2