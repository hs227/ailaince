all: threadpool03

CXXFLAGS += -g -Wall -std=c++0x -pthread

threadpool03: main.cpp threadpool.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ 

.PHONY: clean

clean:
	rm -f *.exe
