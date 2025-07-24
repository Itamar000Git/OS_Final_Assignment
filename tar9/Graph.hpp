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
   
  

public:
    int vertices;
    int EdgesNum;
    std::vector<std::vector<int>> adjMat; // adjacency matrix representation
    Graph();
    ~Graph()=default;
    void printGraph();
    //bool EulerianCycle(std::vector<int>& euler);
    bool EvenDeg();
    int getVertices() const { return vertices; }
    //void CreateRandomGraph(size_t v_num,size_t e_num, size_t seed);

    void dfs(int u , const std::vector<std::vector<int>>& adj , std::vector<bool>& visited);
    bool isConnected(const std::vector<std::vector<int>>& adj);
    
    //void parseFlags(int argc, char* argv[]) ;
    friend void run_server(int port_tcp,Graph& g);
    void parseFromMatrix(const std::vector<std::vector<int>>& matrix);
    //friend void CheckForEulerianCycle(Graph& g, int socket_fd);
    bool nonNegativeWeights() const {
        for (const auto& row : adjMat) {
            for (int weight : row) {
                if (weight < 0) return false;
            }
        }
        return true;
    }
    bool hasPath(int src, int dest) const;

    bool hasSelfLoops() const {
        for (int i = 0; i < vertices; ++i) {
            if (adjMat[i][i] > 0) {
                return true; // Self-loop found
            }
        }
        return false; // No self-loops
    }
};
#endif // GRAPH_HPP