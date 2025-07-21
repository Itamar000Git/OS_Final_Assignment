 
C = g++
CFLAGS = -c -g -Wall --coverage
COVERAGE_DIR = coverage_files
OBJECTS1 = Graph.o
OBJECTS2 = client.o
TARGET1 = server
TARGET2 = client


VALGRIND_MEMCHECK = valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all
VALGRIND_CALLGRIND = valgrind --tool=callgrind

all: $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJECTS1)
	$(C) -o $(TARGET1) $(OBJECTS1) -g --coverage

$(TARGET2): $(OBJECTS2)
	$(C) -o $(TARGET2) $(OBJECTS2) -g --coverage

client.o: client.cpp 
	$(C) $(CFLAGS) client.cpp -o client.o

Graph.o: Graph.cpp Graph.hpp
	$(C) $(CFLAGS) Graph.cpp -o Graph.o

.PHONY: clean all coverage

clean:
	rm -rf $(TARGET1) $(TARGET2) *.gcda *.gcno *.gcov *.o 

coverage: $(TARGET1) $(TARGET2)
	mkdir -p $(COVERAGE_DIR)
	mv *.gcda $(COVERAGE_DIR) || true
	mv *.gcno $(COVERAGE_DIR) || true
	mv *.gcov $(COVERAGE_DIR) || true


