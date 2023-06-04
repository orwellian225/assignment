CXX = mpic++
RM = rm -f
CXXFLAGS=-g -std=c++11 -fopenmp

SRCS = main.cpp sssp_mpi.cpp sssp_omp.cpp sssp.cpp util.cpp
OBJS = $(subst .cpp,.o, $(SRCS))

all: sssp

sssp: $(OBJS)
	$(CXX) $(CXXFLAGS) -o sssp $(OBJS) 

build/main.o: main.cpp
build/sssp_mpi.o: sssp_mpi.cpp
build/sssp_omp.o: sssp_omp.cpp
build/sssp.o: sssp.cpp
build/util.o: util.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) sssp