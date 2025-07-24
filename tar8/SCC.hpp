#ifndef SCC_HPP
#define SCC_HPP

#include "Algorithms.hpp"
#include "Graph.hpp"
#include <vector>
#include <string>

class SCC : public Algorithms {
public:
    ~SCC() = default;

    /**
     * @brief Activate the SCC algorithm on the given graph.
     * @param g The graph to process.
     * @return A string containing the result of the SCC algorithm.
     */
    std::string activate(Graph& g) override;

    /**
     * @brief Find the strongly connected components (SCCs) in the given graph.
     * @param g The graph to process.
     * @return A vector of vectors, where each inner vector contains the vertices of an SCC.
     * @details This function uses Kosaraju's algorithm to find all SCCs in the
     */
    std::vector<std::vector<int>> findSCC(Graph& g);
};

#endif // SCC_HPP
