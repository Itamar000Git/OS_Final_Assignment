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

    /**
     * @brief Prints the graph in a readable format.
     * @details The graph is printed as an adjacency matrix with vertices labeled.
     */
    void printGraph();

    /**
     * @brief Checks if all vertices have even degree.
     * @return True if all vertices have even degree, false otherwise.
     * @throws std::runtime_error if the graph is not Eulerian.
     */
    bool EvenDeg();

    /**
     * @brief Gets the number of vertices in the graph.
     * @return The number of vertices.
     */
    int getVertices() const { return vertices; }

    void dfs(int u , const std::vector<std::vector<int>>& adj , std::vector<bool>& visited);
    bool isConnected(const std::vector<std::vector<int>>& adj);
    
    /**
     * @brief main function that runs the server and handles client requests.
     * @param port_tcp The TCP port to listen on.
     * @param g The graph to process.
     * @throws std::runtime_error if the socket creation, binding, or listening fails.
     */
    friend void run_server(int port_tcp,Graph& g);

    /**
     * @brief Parses an adjacency matrix from a vector of vectors.
     * @param matrix The adjacency matrix to parse.
     */
    void parseFromMatrix(const std::vector<std::vector<int>>& matrix);

    /**
     * @brief Checks if all edge weights are non-negative.
     * @return True if all edge weights are non-negative, false otherwise.
     */
    bool nonNegativeWeights() const {
        for (const auto& row : adjMat) {
            for (int weight : row) {
                if (weight < 0) return false;
            }
        }
        return true;
    }

    /**
     * @brief Checks if there is a path between two vertices.
     * @param src The source vertex.
     * @param dest The destination vertex.
     * @return True if a path exists, false otherwise.
     */
    bool hasPath(int src, int dest) const;

    /**
     * @brief Checks if the graph has self-loops.
     * @return True if self-loops exist, false otherwise.
     */
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