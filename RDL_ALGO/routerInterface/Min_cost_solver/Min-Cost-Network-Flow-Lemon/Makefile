#CC          := ./gcc-7.5.0/gcc-build-7.5.0/bin/g++
CC := g++
CXXFLAGS        := -g -Wall -std=c++11

default:
		$(CC) $(CXXFLAGS)  -DDEBUG -I ./ -fPIC -shared MCMF.h MCMF.cpp -pthread -o ../../test/lib/libMCMF.so

clean:
		rm ../../lib/libMCMF.so
veryclean:
		rm -f ../../main/input/*.txt
		rm ../../lib/libMCMF.so
		
