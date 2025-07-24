#ifndef PATHCOVER_HPP
#define PATHCOVER_HPP
#include "Algorithms.hpp"
#include "Graph.hpp"
#include <sstream>


class PathCover : public Algorithms {

public:
    std::vector<std::vector<int>> allPaths; // Store all paths found
    ~PathCover()=default;

    /**
     * @brief Activate the Path Cover algorithm on the given graph.
     * @param g The graph to process.
     * @return A string containing the result of the Path Cover algorithm.
     */
    std::string activate(Graph& g)override {

        if (g.hasSelfLoops()) {
            return "Graph contains self-loops, Path Cover cannot be computed";
        }
        std::vector<std::vector<int>> paths;
        int count = findPathCover(g, paths);
        std::ostringstream oss;
        oss << "Path Cover (" << count << " paths):\n";
        for (const auto& path : paths) {
            for (size_t i = 0; i < path.size(); ++i) {
                oss << path[i];
                if (i < path.size() - 1)
                    oss << " -> ";
            }
            oss << "\n";
        }
        //std::cout <<"The path cover is:\n" <<oss.str()<< std::endl;
        return oss.str();
    }
    /**
     * @brief Find the path cover in the given graph.
     * @param g The graph to process.
     * @param allPaths A vector to store all paths found.
     * @return The number of paths found in the path cover.
     */
    int findPathCover(Graph &g, std::vector<std::vector<int>>& allPaths);
};

#endif
