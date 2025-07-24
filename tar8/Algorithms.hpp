#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP
#include "Graph.hpp"


class Algorithms{
    public:
    
    virtual ~Algorithms()=default;

    /**
     * @brief Activate the algorithm on the given graph.
     * @param g The graph to process.
     * @return A string containing the result of the algorithm.
     */
    virtual std::string activate(Graph& g)=0;
};

#endif