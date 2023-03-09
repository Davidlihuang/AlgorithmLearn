CC          := g++

CXXFLAGS        := -O3 -Wall -std=c++11

default:
		$(CC) $(CXXFLAGS) -fPIC -shared solver.cpp solver.h parse.h parse.cpp tile.h tile.cpp node.h node.cpp edge.h edge.cpp io.h io.cpp bp.h bp.cpp maze.h maze.cpp rdlblock.h rdlblock.cpp -pthread -o ../../test/lib/libSolver.so
clean:
		rm -f ../../test/lib/libSolver.so

veryclean:
		rm -f ../../lib/libSolver.so
		rm -f ../output/*.txt
		rm -f ../../output/*.json
		rm -f ../../output/*.csv
		rm -f ../../output/*.png
		rm -f ../../Min_cost_solver/Min-Cost-Network-Flow-Lemon/Demo_Data/*.csv
		rm -f ../../python/*.txt