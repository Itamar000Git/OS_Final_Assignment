#ifndef MST_HPP
#define MST_HPP
#include "Algorithms.hpp"
#include "Graph.hpp"



class MST : public Algorithms{

public:
    ~MST()=default;
    std::string activate(Graph& g)override{
        int w= culcMST(g);
        return "MST weight: " + std::to_string(w);
    }

    int culcMST(Graph &g);
    
};

#endif
  
