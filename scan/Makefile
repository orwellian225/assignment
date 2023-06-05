CXX = mpic++
RM = rm -f
CXXFLAGS = -g -std=c++11 -fopenmp
MPIFLAGS = -g -std=c++11

SRCS_S = scan.cpp util.cpp 
SRCS_O = scan_omp.cpp util.cpp
SRCS_M = scan_mpi.cpp util.cpp

OBJS_S = $(subst .cpp,.o, $(SRCS_S))
OBJS_O = $(subst .cpp,.o, $(SRCS_O))
OBJS_M = $(subst .cpp,.o, $(SRCS_M))

all: scan

scan: $(OBJS_S)
	$(CXX) $(CXXFLAGS) -o sssp $(OBJS_S)

scan_omp: $(OBJS_O)
	$(CXX) $(CXXFLAGS) -o sssp_omp $(OBJS_O)

scan_mpi: $(OBJS_M)
	$(CXX) $(CXXFLAGS) -o sssp_mpi $(OBJS_M)

scan: $(OBJS_LOCAL)
	$(CXX) $(CXXFLAGS) -o scan $(OBJS_LOCAL) 

scan_mpi: $(OBJS_MPI)
	$(CXX) $(MPIFLAGS) -o scan_mpi $(OBJS_MPI)

scan_mpi.o: scan_mpi.cpp
scan_omp.o: scan.cpp
scan.o: scan.cpp
util.o: util.cpp

clean:
	$(RM) $(OBJS_S) $(OBJS_O) $(OBJS_M)

distclean: clean
	$(RM) scan scan_omp scan_mpi 