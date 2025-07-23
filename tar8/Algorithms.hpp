#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP
#include "Graph.hpp"


class Algorithms{
    public:
    virtual ~Algorithms()=default;
    virtual std::string activate(Graph& g)=0;
};

#endif