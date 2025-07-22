#ifndef MAXFLOW_HPP
#define MAXFLOW_HPP
#include "Algorithms.hpp"
#include "Graph.hpp"


class MaxFlow : public Algorithms {

public:
    ~MaxFlow()=default;
    std::string activate(Graph& g)override{
        int w= culcMaxFlow(g);
        return "Max Flow: " + std::to_string(w);
    }

    int culcMaxFlow(Graph &g);
    friend bool bfs(const std::vector<std::vector<int>>& g, int s, int t, std::vector<int>& prev);
};

#endif

