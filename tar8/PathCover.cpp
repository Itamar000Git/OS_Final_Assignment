#include "PathCover.hpp"
#include "Graph.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

int PathCover::findPathCover(Graph &g, std::vector<std::vector<int>>& allPaths) {
    int n = g.getVertices();
    std::vector<bool> visited(n, false);
    int pathCover = 0;

    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            std::vector<int> path;
            std::vector<int> stack = {i};
            while (!stack.empty()) {
                int u = stack.back();
                stack.pop_back();
                if (!visited[u]) {
                    visited[u] = true;
                    path.push_back(u);
                    for (int v = 0; v < n; ++v) {
                        if (g.adjMat[u][v] != 0 && g.adjMat[u][v] != inf && !visited[v]) {
                            stack.push_back(v);
                        }
                    }
                }
            }
            if (!path.empty()) {
                allPaths.push_back(path);
                pathCover++;
            }
        }
    }
    return pathCover;
}