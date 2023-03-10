CC          := g++

CXXFLAGS        := -O3 -Wall -std=c++11

#../bin/main: main.cpp net.cpp net.h
default:
		$(CC) $(CXXFLAGS) -fPIC -shared SdcParser.h SdcParser.cpp net.cpp net.h -pthread -o ../../test/lib/libSdcParser.so
clean:
		rm -f ../../lib/libSdcParser.so
veryclean:
		rm -f ../../lib/libSdcParser.so
		rm -f ../output/*.out