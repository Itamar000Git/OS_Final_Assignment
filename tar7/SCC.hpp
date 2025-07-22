#ifndef SCC_HPP
#define SCC_HPP

#include "Algorithms.hpp"
#include "Graph.hpp"
#include <string>
#include <vector>

class SCC : public Algorithms {
public:
    std::string activate(Graph& g)override;
    std::vector<int> findSCC(Graph& g);
};

#endif