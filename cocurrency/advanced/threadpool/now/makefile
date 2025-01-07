all: threadpool

CXXFLAGS += -g -Wall -std=c++20 -pthread

threadpool: main.cpp 
	$(CXX) $(CXXFLAGS) $^ -o $@ 

.PHONY: clean

clean:
	rm -f *.exe
