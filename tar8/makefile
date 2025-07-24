	
C = g++
CFLAGS = -c -g -Wall --coverage -pthread
COVERAGE_DIR = coverage_files
OBJECTS1 = Graph.o MST.o MaxFlow.o PathCover.o SCC.o
OBJECTS2 = client.o
TARGET1 = server
TARGET2 = client
HEADERS = Graph.hpp Algorithms.hpp Strategy.hpp Factory.hpp MST.hpp MaxFlow.hpp PathCover.hpp SCC.hpp

VALGRIND_MEMCHECK = valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all
VALGRIND_CALLGRIND = valgrind --tool=callgrind

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJECTS1)
	$(C) -o $(TARGET1) $(OBJECTS1) -g --coverage -pthread

$(TARGET2): $(OBJECTS2)
	$(C) -o $(TARGET2) $(OBJECTS2) -g --coverage

client.o: client.cpp 
	$(C) $(CFLAGS) client.cpp -o client.o

Graph.o: Graph.cpp $(HEADERS)
	$(C) $(CFLAGS) Graph.cpp -o Graph.o

MST.o: MST.cpp $(HEADERS)
	$(C) $(CFLAGS) MST.cpp -o MST.o

MaxFlow.o: MaxFlow.cpp $(HEADERS)
	$(C) $(CFLAGS) MaxFlow.cpp -o MaxFlow.o

PathCover.o: PathCover.cpp $(HEADERS)
	$(C) $(CFLAGS) PathCover.cpp -o PathCover.o

SCC.o: SCC.cpp $(HEADERS)
	$(C) $(CFLAGS) SCC.cpp -o SCC.o

.PHONY: clean all coverage

clean:
	rm -rf $(TARGET1) $(TARGET2) *.gcda *.gcno *.gcov *.o 

coverage: $(TARGET1) $(TARGET2)
	mkdir -p $(COVERAGE_DIR)
	mv *.gcda $(COVERAGE_DIR) || true
	mv *.gcno $(COVERAGE_DIR) || true
	mv *.gcov $(COVERAGE_DIR) || true


