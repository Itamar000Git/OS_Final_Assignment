#include "MST.hpp"
#include "Graph.hpp"
#include <vector>
#include <algorithm>
#include <iostream>



struct Edge {
    int u, v, w;
    bool operator<(const Edge& other) const {
        return w < other.w;
    }
};

class UnionFind {
    std::vector<int> parent;
public:
    UnionFind(int n) : parent(n) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int x, int y) {
        parent[find(x)] = find(y);
    }
};

int MST::culcMST(Graph &graph) {
    Graph g=graph;
    int n = g.getVertices();
    for(int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (g.adjMat[i][j] == 0 && i != j) {
                g.adjMat[i][j] = inf; // Use inf to represent no edge
            }
        }
    }
    std::vector<Edge> edges;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) { 
            if (g.adjMat[i][j] != inf) {
                edges.push_back({i, j, g.adjMat[i][j]});
            }
        }
    }
    std::sort(edges.begin(), edges.end());
    UnionFind uf(n);
    int mst_weight = 0, edges_used = 0;
    for (const auto& e : edges) {
        if (uf.find(e.u) != uf.find(e.v)) {
            uf.unite(e.u, e.v);
            mst_weight += e.w;
            edges_used++;
        }
    }
    if (edges_used != n - 1) {
       // std::cerr << "Graph is not connected, MST does not exist\n";
        return -1;
    }
    return mst_weight;
}