#include "MaxFlow.hpp"
#include "Graph.hpp"
#include <vector>
#include <algorithm>
#include <iostream>

#include <queue>
#include <climits>

bool bfs(const std::vector<std::vector<int>>& g, int s, int t, std::vector<int>& prev) {
    int n = g.size();
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    q.push(s);
    visited[s] = true;
    prev[s] = -1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v = 0; v < n; ++v) {
            if (!visited[v] && g[u][v] > 0) {
                q.push(v);
                prev[v] = u;
                visited[v] = true;
                if (v == t) return true;
            }
        }
    }
    return false;
}

int MaxFlow::culcMaxFlow(Graph &g) {
    int n = g.getVertices();
    int s = 0; 
    int t = n - 1; 
    std::vector<std::vector<int>> g_copy = g.adjMat;
    std::vector<int> prev(n);
    int max_flow = 0;
    while (bfs(g_copy, s, t, prev)) {
        int path_flow = INT_MAX;
        for (int v = t; v != s; v = prev[v]) {
            int u = prev[v];
            path_flow = std::min(path_flow, g_copy[u][v]);
        }
        for (int v = t; v != s; v = prev[v]) {
            int u = prev[v];
            g_copy[u][v] -= path_flow;
            g_copy[v][u] += path_flow;
        }
        max_flow += path_flow;
    }
    return max_flow;
}