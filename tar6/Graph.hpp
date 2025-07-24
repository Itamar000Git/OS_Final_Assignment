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
     * @brief Checks if the graph has an Eulerian cycle.
     * @param euler A vector to store the Eulerian cycle if it exists.
     * @return True if an Eulerian cycle exists, false otherwise.
     */
    bool EulerianCycle(std::vector<int>& euler);

    /**
     * @brief Checks if all vertices have even degree.
     * @return True if all vertices have even degree, false otherwise.
     */
    bool EvenDeg();

    /**
     * @brief Creates a random graph with a specified number of vertices and edges.
     * @param v_num Number of vertices.
     * @param e_num Number of edges.
     */
    void CreateRandomGraph(size_t v_num,size_t e_num, size_t seed);

    /**
     * @brief dfs function for depth-first search.
     */
    friend void dfs(int u , const std::vector<std::vector<int>>& adj , std::vector<bool>& visited);
    /**
     * @brief Checks if the graph is connected.
     * @param adj The adjacency matrix of the graph.
     * @return True if the graph is connected, false otherwise.
     */
    friend bool isConnected(const std::vector<std::vector<int>>& adj);

    /**
     * @brief Parses a flags from command line arguments.
     * @param argc The number of command line arguments.
     */
    void parseFlags(int argc, char* argv[]) ;

    /**
     * @brief main function to run the server.
     * @param port_tcp The TCP port to listen on.
     * @param g The graph to operate on.
     * @throws std::runtime_error if socket creation, binding, or listening fails.
     * @details This function sets up a TCP server that listens for incoming connections and handles commands
     * from clients. It uses select for multiplexing I/O operations and can handle multiple clients simultaneously.
     * It also allows for commands to be entered via stdin to control the server.
     */
    friend void run_server(int port_tcp,Graph& g);

    /**
     * @brief Parses an adjacency matrix from a vector of vectors.
     * @param matrix The adjacency matrix to parse.
     */
    void parseFromMatrix(const std::vector<std::vector<int>>& matrix);

    /**
     * @brief Checks if there is a path between two vertices in the graph.
     * @param src The source vertex.
     * @param dest The destination vertex.
     * @return True if a path exists, false otherwise.
     */
    friend void CheckForEulerianCycle(Graph& g, int socket_fd);
};
#endif // GRAPH_HPP