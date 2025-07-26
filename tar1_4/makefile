 
C = g++
CFLAGS = -c -g -Wall --coverage -pg
COVERAGE_DIR = coverage_files
OBJECTS1 = main.o Graph.o
TARGET1 = main

VALGRIND_MEMCHECK = valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all
VALGRIND_CALLGRIND = valgrind --tool=callgrind

all: $(TARGET1)

$(TARGET1): $(OBJECTS1)
	$(C) -o $(TARGET1) $(OBJECTS1) -pg -g --coverage

main.o: main.cpp Graph.hpp
	$(C) $(CFLAGS) main.cpp -o main.o

Graph.o: Graph.cpp Graph.hpp
	$(C) $(CFLAGS) Graph.cpp -o Graph.o

.PHONY: clean all coverage main

clean:
	rm -rf $(TARGET1) *.gcda *.gcno *.gcov *.o *.out.*

coverage: $(TARGET1) $(TARGET2)
	mkdir -p $(COVERAGE_DIR)
	mv *.gcda $(COVERAGE_DIR) || true
	mv *.gcno $(COVERAGE_DIR) || true
	mv *.gcov $(COVERAGE_DIR) || true

valgrind-memcheck: $(TARGET1)
	valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all ./$(TARGET1) -v 5 -e 10 -s 42

valgrind-callgrind: $(TARGET1)
	valgrind --tool=callgrind ./$(TARGET1) -v 5 -e 10 -s 42
