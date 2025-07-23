#ifndef STRATEGY_HPP
#define STRATEGY_HPP

#include "Algorithms.hpp"

class Strategy {
    Algorithms* stra = nullptr;

public:
    void setStrategy(Algorithms* alg) {
        stra = alg;
    }

    std::string execute(Graph& graph) {
        if (!stra) return "No strategy set.";
        return stra->activate(graph);
    }
};
#endif