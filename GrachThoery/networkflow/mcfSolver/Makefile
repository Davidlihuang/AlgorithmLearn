CC=g++

CFLAGS=-g -Wall
LIBS=-lm -lstdc++

CSOURCES=$(wildcard *.cpp)
OBJECTS=$(patsubst %.cpp,%.o,${CSOURCES})

TARGETS=mcf_solver
INPUT=gen_graph
all: ${TARGETS}

mcf_solver: ${OBJECTS}
	$(CC) $(CFLAGS) main.cpp grid.cpp mcf.cpp pqueue.cpp $(LIBS) -o $@ 
gengraph:${gen_graph}
	$(CC) $(CFLAGS) genGraph.cpp grid.cpp mcf.cpp pqueue.cpp $(LIBS) -o $@ 
.cpp.o:
	$(CC) $(CFLAGS) -c $<
