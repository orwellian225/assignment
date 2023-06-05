CXX = mpic++
RM = rm -f
CXXFLAGS=-g -std=c++11 -fopenmp
MPIFLAGS=-g -std=c++11

SRCS_LOCAL = main.cpp scan_omp.cpp scan.cpp util.cpp
OBJS_LOCAL = $(subst .cpp,.o, $(SRCS_LOCAL))

SRCS_MPI = scan_mpi.cpp util.cpp scan.cpp
OBJS_MPI = $(subst .cpp,.o, $(SRCS_MPI))

all: scan scan_mpi

scan: $(OBJS_LOCAL)
	$(CXX) $(CXXFLAGS) -o scan $(OBJS_LOCAL) 

scan_mpi: $(OBJS_MPI)
	$(CXX) $(MPIFLAGS) -o scan_mpi $(OBJS_MPI)

main.o: main.cpp
scan_mpi.o: scan_mpi.cpp
scan_omp.o: scan.cpp
scan.o: scan.cpp
util.o: util.cpp

clean:
	$(RM) $(OBJS_LOCAL) $(OBJS_MPI) scan scan_mpi

distclean: clean
	$(RM) scan