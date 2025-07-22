#ifndef MST_HPP
#define MST_HPP
#include "Algorithms.hpp"
#include "Graph.hpp"



class MST : public Algorithms{

public:
    ~MST()=default;

    std::string activate(Graph& g) override {
            if (!g.isConnected(g.adjMat)) {
                return "Graph is not connected";
            }
            if(!g.nonNegativeWeights()){
                return "Graph contains negative weights, MST cannot be computed";
            }

            int w = culcMST(g);
            if (w >= 0) {
                std::cout << "MST weight: " << w << std::endl;
                return "MST weight: " + std::to_string(w);
            } else {
                return "Failed to compute MST";
            }
        }

      
    

    int culcMST(Graph &g);
    
};

#endif
  
