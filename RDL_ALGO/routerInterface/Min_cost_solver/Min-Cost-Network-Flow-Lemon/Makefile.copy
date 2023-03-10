CC          := g++

CXXFLAGS        := -O3 -Wall -std=c++11

MCMF: MinC.cpp 
		$(CC) $(CXXFLAGS) -pthread -o $@ $? 

clean:
		rm MCMF
veryclean:
		rm -f ../../main/input/*.txt
		rm MCMF
		