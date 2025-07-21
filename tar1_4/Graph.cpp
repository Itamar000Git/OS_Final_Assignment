#include <iostream>
#include "Graph.hpp"
#include <stack>
#include <string>
#include <getopt.h>
#include <stdexcept>
#include <random>
#include <iomanip> 

static struct option long_options[] = {
    {"vertecs",    required_argument, 0, 'v'},
    {"edges",    required_argument, 0, 'e'},  
    {"seed",    required_argument, 0, 's'},   
    {0, 0, 0, 0}
};


Graph::Graph() : vertices(0), EdgesNum(0) {}


void  Graph::parseFlags(int argc, char* argv[]) {
    int e_num=-1,v_num=-1,seed=-1;
    int opt;
 
    while ((opt = getopt_long(argc, argv, "v:e:s:", long_options, nullptr)) != -1) {
        try{
            switch (opt) {
                    case 'v':
                        v_num = std::stoi(optarg);
                        if (v_num < 0) throw std::invalid_argument("vertices must be positive");
                        break;
                    case 'e':
                        e_num = std::stoi(optarg);
                        if (e_num < 0) throw std::invalid_argument("edges must be positive");
                        break;
                    case 's':
                        seed = std::stoi(optarg);
                        if (seed <= 0) throw std::invalid_argument("seed must be positive");
                        break;
                    default:
                        std::cerr << "Usage: " << argv[0]
                                << " -v <Vertex Number> -e <Edge Number> -s <Seed>\n";
                        exit(EXIT_FAILURE);
                }

        } catch (const std::invalid_argument& e) {
            std::cerr << "Invalid argument for -" << (char)opt << ": not a number\n";
            exit(EXIT_FAILURE);
        } catch (const std::out_of_range& e) {
            std::cerr << "Invalid argument for -" << (char)opt << ": number out of range\n";
            exit(EXIT_FAILURE);
        }
    }
    if (v_num<0 || e_num<0 || seed<0) {
        std::cerr << "Error: missing or invalid arguments\n";
        std::cerr << "Usage: " << argv[0]
                  << " -v <Vertect Number> -e <Edges Number> -s <Seed>\n";
        exit(EXIT_FAILURE);
    }
    int max_edges= (v_num*(v_num-1))/2 ;

    if(e_num>max_edges){
        std::cerr << "Error: Too many edges\n";
        exit(EXIT_FAILURE);
    }
    this->vertices = v_num;
    this->EdgesNum = e_num;
    this->adjMat.resize(v_num, std::vector<int>(v_num, inf));

    CreateRandomGraph(v_num,e_num,seed);
}


void Graph::CreateRandomGraph(size_t v_num,size_t e_num, size_t seed){
    std::vector<std::pair<int, int>> allEdges;
    for (size_t u = 0; u < v_num; ++u) {
        for (size_t v = u + 1; v < v_num; ++v) {
        allEdges.emplace_back(u, v);
        }
    }
    std::mt19937 gen(seed); 
    std::shuffle(allEdges.begin(), allEdges.end(), gen);

    for (size_t i = 0; i < e_num && i<allEdges.size(); ++i) {
        int u = allEdges[i].first;
        int v = allEdges[i].second;
       // std::cout<<"( "<<u<<" , "<<v<<" ) "<<std::endl;
        adjMat[u][v] = 1;
        adjMat[v][u] = 1;
    }
     printGraph();
}



void Graph::printGraph() {
    std::cout << "    ";
    for (int j = 0; j < vertices; ++j) {
        std::cout << std::setw(4) << j;
    }
    std::cout << "\n";

    std::cout << "    ";
    for (int j = 0; j < vertices; ++j) {
        std::cout << "----";
    }
    std::cout << "\n";

    for (int i = 0; i < vertices; ++i) {
        std::cout << std::setw(2) << i << " |";
        for (int j = 0; j < vertices; ++j) {
            if (adjMat[i][j] == inf)
                std::cout << std::setw(4) << "inf";
            else
                std::cout << std::setw(4) << adjMat[i][j];
        }
        std::cout << "\n";
    }
}


  bool Graph::EvenDeg(){
    int counter;
    for(int i=0 ; i<vertices ;i++){
        counter=0;
        for (int j=0; j<vertices ;j++){
            if(adjMat[i][j]==1){
                counter++;
            }
        }
        if(counter%2==1){
            std::cerr << "Graph is not Eulerian: vertex " << i << " has odd degree\n";
            return false;
        }
    }
    return true;
  }


void dfs(int u, const std::vector<std::vector<int>>& adj, std::vector<bool>& visited) {
    visited[u] = true;
    for (size_t v = 0; v < adj.size(); ++v) {
        if (adj[u][v] == 1 && !visited[v])
            dfs(v, adj, visited);
    }
}

bool isConnected(const std::vector<std::vector<int>>& adj) {
    int n = adj.size();
    std::vector<bool> visited(n, false);
    int start = -1;
    for (int i = 0; i < n; ++i) {
        int degree = 0;
        for (int j = 0; j < n; ++j)
            if (adj[i][j] == 1) degree++;
        if (degree > 0) {
            start = i;
            break;
        }
    }
    if (start == -1)
        return true; 

    dfs(start, adj, visited);
    for (int i = 0; i < n; ++i) {
        int degree = 0;
        for (int j = 0; j < n; ++j)
            if (adj[i][j] == 1) degree++;
        if (degree > 0 && !visited[i]){
            std::cerr << "Graph is not connected: vertex " << i << " is not reachable from vertex " << start << "\n";   
            return false;
        }
    }
    return true;
}



bool Graph::EulerianCycle(std::vector<int>& euler){
    if(!EvenDeg()|| !isConnected(adjMat)){
        return false;
    }
    if(EdgesNum==0){
        std::cerr << "Graph has no edges, The eulerian cycle is empy cycle.\n";
        return false;
    }
    std::stack<int> path;
    std::vector<std::vector<int>> copyMat=this->adjMat;
    int source=0;
    path.push(source);//start from 0 - positiv and even deg
   
    while(!path.empty()){
        int u=path.top();
        bool found=false;
        for(int i =0; i<vertices; i++ ){
            if(copyMat[u][i]==1){
                path.push(i);
                found=true;
                copyMat[u][i]=inf;
                copyMat[i][u]=inf;
                break;
            }
        }
       if (!found) {
            euler.push_back(u);
            path.pop();
        }
    }
    std::reverse(euler.begin(), euler.end());
    return true;
  }

Graph::~Graph() {
    // Destructor
}