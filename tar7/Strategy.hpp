#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include "Algorithms.hpp"

class Strategy {
    Algorithms* stra = nullptr;

public:
    /**
     * @brief Sets the strategy for the algorithm.
     * @param alg The algorithm to set as the strategy.
     */
    void setStrategy(Algorithms* alg) {
        stra = alg;
    }
    /**
     * @brief Executes the algorithm on the given graph.
     * @param graph The graph to process.
     * @return A string containing the result of the algorithm.
     */
    std::string execute(Graph& graph) {
        if (!stra) return "No strategy set.";
        return stra->activate(graph);
    }
};
#endif