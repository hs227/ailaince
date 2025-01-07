all: threadpool98

CXXFLAGS += -g -Wall -std=c++98 -pthread

threadpool98: main.cpp threadpool.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ 

.PHONY: clean

clean:
	rm -f *.exe
