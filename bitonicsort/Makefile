CXX = mpic++
RM = rm -f
CXXFLAGS = -g -std=c++11 -fopenmp
MPIFLAGS = -g -std=c++11

SRCS_S = bitonic.cpp util.cpp 
SRCS_O = bitonic_omp.cpp util.cpp
SRCS_M = bitonic_mpi.cpp util.cpp

OBJS_S = $(subst .cpp,.o, $(SRCS_S))
OBJS_O = $(subst .cpp,.o, $(SRCS_O))
OBJS_M = $(subst .cpp,.o, $(SRCS_M))

# all: bitonic bitonic_omp bitonic_mpi
all: bitonic bitonic_omp

bitonic: $(OBJS_S)
	$(CXX) $(CXXFLAGS) -o bitonic $(OBJS_S)

bitonic_omp: $(OBJS_O)
	$(CXX) $(CXXFLAGS) -o bitonic_omp $(OBJS_O)

bitonic_mpi: $(OBJS_M)
	$(CXX) $(CXXFLAGS) -o bitonic_mpi $(OBJS_M)

util.o: util.cpp
bitonic.o: bitonic.cpp
bitonic_omp.o: bitonic_omp.cpp
bitonic_mpi.o: bitonic_mpi.cpp

clean:
	$(RM) $(OBJS_S) $(OBJS_O) $(OBJS_M)

distclean: clean
	$(RM) bitonic bitonic_omp bitonic_mpi