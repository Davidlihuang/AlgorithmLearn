CC          := g++

# CXXFLAGS        := -O3 -Wall -std=c++11
CXXFLAGS        := -g -Wall -std=c++11


../bin/main: main.cpp parse.h parse.cpp tile.h tile.cpp node.h node.cpp edge.h edge.cpp io.h io.cpp bp.h bp.cpp maze.h maze.cpp rdlblock.h rdlblock.cpp
		$(CC) $(CXXFLAGS) -pthread -o $@ $? 
clean:
		rm -f ../bin/main

veryclean:
		rm -f ../bin/main
		rm -f ../output/*.txt
		rm -f ../../output/*.json
		rm -f ../../output/*.csv
		rm -f ../../output/*.png
		rm -f ../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/*.csv
		rm -f ../../python/*.txt
