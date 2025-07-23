
#ifndef FACTORY_HPP
#define FACTORY_HPP

#include "Algorithms.hpp"
#include "MST.hpp"
#include "SCC.hpp"
#include "MaxFlow.hpp"
#include "PathCover.hpp"

class AlgorithmFactory {
public:
    static Algorithms* createAlgorithm(const std::string& name) {
        if (name == "MST")
            return new MST();
        else if (name == "SCC")
            return new SCC();
        else if (name == "MaxFlow")
            return new MaxFlow();
        else if (name == "PathCover")
            return new PathCover();
        else
            return nullptr;
    }
};

#endif 