CC          := g++

CXXFLAGS        := -O3 -Wall -std=c++11

default:
	$(CC) $(CXXFLAGS) -I ./include main.cpp -pthread -o ./release/Test -L./lib  -Wl,-rpath ./lib   -lRdlSolver
#	$(CC) $(CXXFLAGS) -I ./include main.cpp -pthread -o ./release/Test -L./lib -lSdcParser -lSolver -lMCMF -lRdlSolver
#clean:
#		rm ../../lib/libMCMF.so
#veryclean:
#		rm -f ../../main/input/*.txt
#		rm ../../lib/libMCMF.so

#g++ -O3 -Wall -std=c++11 main.cpp -I ./ -0 ../release/test -L../../lib -lSdcParser -lSolver -lMCMF -L../release -lRdlSolver
		