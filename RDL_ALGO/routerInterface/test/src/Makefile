CC          := g++

CXXFLAGS        := -O3 -Wall -std=c++11

default:
	$(CC) $(CXXFLAGS) -I ./ -fPIC -shared RdlSolver.cpp DbWrapper.cpp -pthread  -Wl,-rpath ../lib -L../lib -lSdcParser -lSolver -lMCMF -o ../lib/libRdlSolver.so
#		$(CC) $(CXXFLAGS) -I ./ main.cpp -pthread -o ../release/TestWithLib -L../lib -lSdcParser -lSolver -lMCMF -lRdlSolver
#clean:
#		rm ../../lib/libMCMF.so
#veryclean:
#		rm -f ../../main/input/*.txt
#		rm ../../lib/libMCMF.so

#g++ -O3 -Wall -std=c++11 main.cpp -I ./ -0 ../release/test -L../../lib -lSdcParser -lSolver -lMCMF -L../release -lRdlSolver
		
