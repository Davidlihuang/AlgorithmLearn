CC          := g++

CXXFLAGS        := -O3 -Wall -std=c++11

../bin/main: main.cpp net.cpp net.h
		$(CC) $(CXXFLAGS) -pthread -o $@ $? 
clean:
		rm -f ../bin/main
veryclean:
		rm -f ../bin/main
		rm -f ../output/*.out