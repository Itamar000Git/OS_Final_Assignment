#include "Graph.hpp"
#include <string>
#include <getopt.h>






int main(int argc, char* argv[]) {
    Graph g;
    g.parseFlags(argc,argv);
    std::vector<int> euler;
    if (g.EulerianCycle(euler)) {
        std::cout << "Eulerian Cycle exists:\n";
        for (int v : euler) {
            std::cout << v << " -> ";
        }
        std::cout << "\n";
    } else {
        std::cout << "No Eulerian Cycle exists.\n";
    }
    return 0;
}
