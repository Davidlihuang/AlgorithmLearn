CC          := g++

CXXFLAGS        := -g -Wall -std=c++17

.DEFAULT_GOAL	:= default

default:
		@echo "build Astar"
		$(CC) $(CXXFLAGS) -I ./src ./src/main.cc ./src/astar.cc ./src/graph.cc  -pthread -o ./bin/astart
		@echo "build bidAstar"
		$(CC) $(CXXFLAGS) -I ./src ./src/bidMain.cc ./src/bidirectionAstar.cc ./src/graph.cc  -pthread -o ./bin/bidAstart


		
