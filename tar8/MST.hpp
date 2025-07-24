#ifndef MST_HPP
#define MST_HPP
#include "Algorithms.hpp"
#include "Graph.hpp"



class MST : public Algorithms{

public:
    ~MST()=default;

    /**
     * @brief Activate the MST algorithm on the given graph.
     * @param g The graph to process.
     * @return A string containing the result of the MST algorithm.
     */
    std::string activate(Graph& g) override {
        
            if (!g.isConnected(g.adjMat)) {
                return "Graph is not connected";
            }
            if(!g.nonNegativeWeights()){
                return "Graph contains negative weights, MST cannot be computed";
            }

            int w = culcMST(g);
            if (w >= 0) {
                
              //  std::cout << "MST weight: " << w << std::endl;
                return "MST weight: " + std::to_string(w);
            } else {
                return "Failed to compute MST";
            }
        }

      
    
    /**
     *  @brief Calculate the minimum spanning tree (MST) of the given graph.
     * @param g The graph to process.
     * @return The weight of the MST.
     */
    int culcMST(Graph &g);
    
};

#endif
  
