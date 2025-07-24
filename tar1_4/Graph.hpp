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
    
    /**
     * @brief Prints the graph in a readable format.
     * @details The graph is printed as an adjacency matrix with vertices labeled.
     */
    void printGraph();
    /**
     * @brief Finds an Eulerian cycle in the graph.
     * @param euler Vector to store the Eulerian cycle.
     * @return true if an Eulerian cycle exists, false otherwise.
     * @details An Eulerian cycle is a cycle that visits every edge exactly once.
     */
    bool EulerianCycle(std::vector<int>& euler);

    /**
     * @brief Checks if the graph has even degree for all vertices.
     * @return true if all vertices have even degree, false otherwise.
     * @throws std::runtime_error if the graph is not Eulerian.
     * @details An Eulerian graph is a graph where all vertices have even degree.
     */
    bool EvenDeg();

    /**
     * @brief Creates a random graph with the specified number of vertices and edges.
     * @param v_num Number of vertices in the graph.
     * @param e_num Number of edges in the graph.
     * @param seed Seed for random number generation.
     */
    void CreateRandomGraph(size_t v_num,size_t e_num, size_t seed);


    /**
     * @brief Checks if the graph is Eulerian and finds an Eulerian cycle.
     * @param euler Vector to store the Eulerian cycle.
     * @return true if an Eulerian cycle exists, false otherwise.
     * @details An Eulerian cycle is a cycle that visits every edge exactly once.
     */
    friend void dfs(int u , const std::vector<std::vector<int>>& adj , std::vector<bool>& visited);

    /**
     * @brief Checks if the graph is connected.
     * @param adj Adjacency matrix of the graph.
     * @return true if the graph is connected, false otherwise.
     * @details A connected graph is a graph where there is a path between every pair of vertices
     */
    friend bool isConnected(const std::vector<std::vector<int>>& adj);

    /**
     * @brief Parses command line flags for graph parameters.
     * @param argc Number of command line arguments.
     * @param argv Array of command line arguments.
     * @throws std::invalid_argument if any argument is not a valid number.
     * @throws std::out_of_range if any argument is out of the valid range.
     */
    void parseFlags(int argc, char* argv[]) ;
};
#endif // GRAPH_HPP