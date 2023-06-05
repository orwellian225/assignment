CXX = mpic++
RM = rm -f
CXXFLAGS = -g -std=c++11 -fopenmp
MPIFLAGS = -g -std=c++11

SRCS_S = sssp.cpp util.cpp 
SRCS_O = sssp_omp.cpp util.cpp
SRCS_M = sssp_mpi.cpp util.cpp

OBJS_S = $(subst .cpp,.o, $(SRCS_S))
OBJS_O = $(subst .cpp,.o, $(SRCS_O))
OBJS_M = $(subst .cpp,.o, $(SRCS_M))

all: sssp #sssp_omp sssp_mpi

sssp: $(OBJS_S)
	$(CXX) $(CXXFLAGS) -o sssp $(OBJS_S)

sssp_omp: $(OBJS_O)
	$(CXX) $(CXXFLAGS) -o sssp_omp $(OBJS_O)

sssp_mpi: $(OBJS_M)
	$(CXX) $(CXXFLAGS) -o sssp_mpi $(OBJS_M)

util.o: util.cpp
sssp.o: sssp.cpp
sssp_omp.o: sssp_omp.cpp
sssp_mpi.o: sssp_mpi.cpp

clean:
	$(RM) $(OBJS_S) $(OBJS_O) $(OBJS_M)

distclean: clean
	$(RM) sssp sssp_omp sssp_mpi 