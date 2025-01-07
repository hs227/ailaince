all: threadpool11

CXXFLAGS += -g -Wall -std=c++11 -pthread

threadpool11: main.cpp threadpool.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ 

.PHONY: clean

clean:
	rm -f *.exe
