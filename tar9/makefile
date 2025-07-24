C = g++
CFLAGS = -c -g -Wall --coverage -pthread
COVERAGE_DIR = coverage_files
OBJECTS1 = Graph.o MST.o MaxFlow.o PathCover.o SCC.o
OBJECTS2 = client.o
TARGET1 = server
TARGET2 = client
HEADERS = Graph.hpp ../tar8/Algorithms.hpp ../tar8/Strategy.hpp ../tar8/Factory.hpp \
			../tar8/MST.hpp ../tar8/MaxFlow.hpp ../tar8/PathCover.hpp ../tar8/SCC.hpp

VALGRIND_MEMCHECK = valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all
VALGRIND_CALLGRIND = valgrind --tool=callgrind

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJECTS1)
	$(C) -o $(TARGET1) $(OBJECTS1) -g --coverage -pthread

$(TARGET2): $(OBJECTS2)
	$(C) -o $(TARGET2) $(OBJECTS2) -g --coverage

client.o: ../tar8/client.cpp
	$(C) $(CFLAGS) ../tar8/client.cpp -o client.o

Graph.o: Graph.cpp Graph.hpp
	$(C) $(CFLAGS) Graph.cpp -o Graph.o

MST.o: ../tar8/MST.cpp $(HEADERS)
	$(C) $(CFLAGS) ../tar8/MST.cpp -o MST.o

MaxFlow.o: ../tar8/MaxFlow.cpp $(HEADERS)
	$(C) $(CFLAGS) ../tar8/MaxFlow.cpp -o MaxFlow.o

PathCover.o: ../tar8/PathCover.cpp $(HEADERS)
	$(C) $(CFLAGS) ../tar8/PathCover.cpp -o PathCover.o

SCC.o: ../tar8/SCC.cpp $(HEADERS)
	$(C) $(CFLAGS) ../tar8/SCC.cpp -o SCC.o

.PHONY: clean all coverage

clean:
	rm -rf $(TARGET1) $(TARGET2) *.gcda *.gcno *.gcov *.o ../tar8/*.o

coverage: $(TARGET1) $(TARGET2)
	mkdir -p $(COVERAGE_DIR)
	mv *.gcda *.gcno *.gcov $(COVERAGE_DIR) 2>/dev/null || true
	mv ../tar8/*.gcda ../tar8/*.gcno ../tar8/*.gcov $(COVERAGE_DIR) 2>/dev/null || true
