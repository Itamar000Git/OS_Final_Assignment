#ifndef MAXFLOW_HPP
#define MAXFLOW_HPP
#include "Algorithms.hpp"
#include "Graph.hpp"


class MaxFlow : public Algorithms {

public:
    ~MaxFlow()=default;

    /**
     * @brief Activate the Max Flow algorithm on the given graph.
     * @param g The graph to process.
     * @return A string containing the result of the Max Flow algorithm.
     */
    std::string activate(Graph& g)override{
        if (!g.nonNegativeWeights()) {
            return "Graph contains negative weights, Max Flow cannot be computed";
        }
        if (!g.hasPath(0, g.getVertices() - 1)) {
            return "No path from source to sink, Max Flow cannot be computed";
        }
        if(g.getVertices() == 0 || g.getVertices() == 1){
            return "Graph is empty or has only one vertex, Max Flow cannot be computed";
        }
        int w= culcMaxFlow(g);
        std::cout <<"Max Flow: " << w << std::endl;
        return "Max Flow: " + std::to_string(w);
    }

    /**
     * @brief Calculate the maximum flow in the given graph.
     * @param g The graph to process.
     * @return The maximum flow value.
     */
    int culcMaxFlow(Graph &g);

    /**
     * @brief Checks if there is a path from source to sink using BFS.
     * @param g The graph to check.
     * @param s The source vertex.
     * @param t The sink vertex.
     * @param prev A vector to store the previous vertex in the path.
     * @return True if a path exists, false otherwise.
     */
    friend bool bfs(const std::vector<std::vector<int>>& g, int s, int t, std::vector<int>& prev);
};

#endif

