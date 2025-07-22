#ifndef MAXFLOW_HPP
#define MAXFLOW_HPP
#include "Algorithms.hpp"
#include "Graph.hpp"


class MaxFlow : public Algorithms {

public:
    ~MaxFlow()=default;
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

    int culcMaxFlow(Graph &g);
    friend bool bfs(const std::vector<std::vector<int>>& g, int s, int t, std::vector<int>& prev);
};

#endif

