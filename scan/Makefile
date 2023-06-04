CXX = mpic++
RM = rm -f
CXXFLAGS=-g -std=c++11 -fopenmp

SRCS = main.cpp scan_mpi.cpp scan_omp.cpp scan.cpp util.cpp
OBJS = $(subst .cpp,.o, $(SRCS))

all: scan 

scan: $(OBJS)
	$(CXX) $(CXXFLAGS) -o scan $(OBJS) 

main.o: main.cpp
scan_mpi.o: scan_mpi.cpp
scan_omp.o: scan.cpp
scan.o: scan.cpp
util.o: util.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) scan