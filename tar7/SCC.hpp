#ifndef SCC_HPP
#define SCC_HPP

#include "Algorithms.hpp"
#include "Graph.hpp"
#include <vector>
#include <string>

class SCC : public Algorithms {
public:
    ~SCC() = default;

    std::string activate(Graph& g) override;

    std::vector<std::vector<int>> findSCC(Graph& g);
};

#endif // SCC_HPP
