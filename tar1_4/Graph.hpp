#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <unordered_map>
#include <list>
#include <utility>
#include <algorithm>
#include <iostream>
#include <limits>
#include <string>
#include <getopt.h>

int const inf = std::numeric_limits<int>::max();



class Graph {
    private:
    int vertices;
    std::vector<std::vector<int>> adjMat; // adjacency matrix representation
    int EdgesNum;

public:
    Graph();
    ~Graph();
    void printGraph();
    bool EulerianCycle(std::vector<int>& euler);
    bool EvenDeg();

    void CreateRandomGraph(size_t v_num,size_t e_num, size_t seed);

    friend void dfs(int u , const std::vector<std::vector<int>>& adj , std::vector<bool>& visited);
    friend bool isConnected(const std::vector<std::vector<int>>& adj);
    void parseFlags(int argc, char* argv[]) ;
};
#endif // GRAPH_HPP