#include <iostream>
#include "Graph.hpp"
#include <stack>
#include <string>
#include <getopt.h>
#include <stdexcept>
#include <random>
#include <iomanip> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <atomic>
#include "Algorithms.hpp"
#include "MST.hpp"
#include "Strategy.hpp"
#include "Factory.hpp"
#include <random>
#include <chrono>   

Graph::Graph() : vertices(0), EdgesNum(0) {}

// Task structure for Leader-Follower pattern
struct Task {
    int client_socket;
    std::vector<std::vector<int>> matrix;
};

// Global variables for Leader-Follower
std::queue<Task> taskQueue; // Queue to hold tasks
std::mutex queueMutex; // Mutex for thread-safe access to the queue
std::condition_variable queueCV; // Condition variable to notify workers
std::atomic<bool> serverRunning{true}; // Flag to control server running state

// Worker function that runs 4 algorithms and sends results back
void workerFunction() {
    while (serverRunning) {
        std::unique_lock<std::mutex> lock(queueMutex);
        queueCV.wait(lock, []{ return !taskQueue.empty() || !serverRunning; });
        
        if (!serverRunning && taskQueue.empty()) break;
        
        Task task = taskQueue.front();
        taskQueue.pop();
        lock.unlock();
        
        // Process the task
        Graph g;
        g.parseFromMatrix(task.matrix);
        
        // Run all 4 algorithms
        std::string results = "";
        
        // MST
        Strategy mst_stra;
        Algorithms* mst_alg = AlgorithmFactory::createAlgorithm("MST");
        mst_stra.setStrategy(mst_alg);
        results += "MST: " + mst_stra.execute(g) + "\n";
        delete mst_alg;
        
        // MaxFlow
        Strategy maxFlow_stra;
        Algorithms* maxFlow_alg = AlgorithmFactory::createAlgorithm("MaxFlow");
        maxFlow_stra.setStrategy(maxFlow_alg);
        results += "MaxFlow: " + maxFlow_stra.execute(g) + "\n";
        delete maxFlow_alg;
        
        // PathCover
        Strategy pathCover_stra;
        Algorithms* pathCover_alg = AlgorithmFactory::createAlgorithm("PathCover");
        pathCover_stra.setStrategy(pathCover_alg);
        results += "PathCover: " + pathCover_stra.execute(g) + "\n";
        delete pathCover_alg;
        
        // SCC
        Strategy scc_stra;
        Algorithms* scc_alg = AlgorithmFactory::createAlgorithm("SCC");
        scc_stra.setStrategy(scc_alg);
        results += "SCC: " + scc_stra.execute(g) + "\n";
        delete scc_alg;
        
        // Send results back to client
        int result_len = results.size();
        if (send(task.client_socket, &result_len, sizeof(result_len), 0) >= 0) {
            send(task.client_socket, results.c_str(), result_len, 0);
        }
        
       // close(task.client_socket);
        std::cout << "Task completed and sent to client" << std::endl;
    }
}

void CreateRandomGraph(size_t v_num, size_t e_num, Graph& g, int max_weight = 20) {
    std::cout << "Creating random graph with " << v_num << " vertices and " << e_num << " edges." << std::endl;
    g.vertices = v_num;
    g.EdgesNum = 0;
    g.adjMat.assign(v_num, std::vector<int>(v_num, 0)); 

    std::vector<std::pair<int, int>> allEdges;
    for (size_t u = 0; u < v_num; ++u) {
        for (size_t v = u + 1; v < v_num; ++v) {
            allEdges.emplace_back(u, v);
        }
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(allEdges.begin(), allEdges.end(), gen);
    std::uniform_int_distribution<int> weight_dist(1, max_weight); 
    for (size_t i = 0; i < e_num && i < allEdges.size(); ++i) {
        int u = allEdges[i].first;
        int v = allEdges[i].second;
        int w = weight_dist(gen);
        g.adjMat[u][v] = w;
        g.adjMat[v][u] = w;
        g.EdgesNum++;
  
    }
      g.printGraph(); 
}

void run_server(int port_tcp, Graph& g) {

    int server_fd, new_socket, max_sd, activity, sd;
    int client_socket[FD_SETSIZE] = {0};
    struct sockaddr_in address;
    fd_set readfds;

    // Create worker threads (followers)
    const int NUM_WORKERS = 4; // Number of worker threads
    std::vector<std::thread> workers; // Create worker threads
    for (int i = 0; i < NUM_WORKERS; ++i) {
        workers.emplace_back(workerFunction);
    }

    // Create a TCP socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        throw std::runtime_error("socket failed");
    }
    int opt = 1;
    // Set socket options to allow reuse of the address
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("setsockopt failed");
    }

    // Set up the address structure for TCP
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port_tcp);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {// Bind the socket to the address
        throw std::runtime_error("bind failed");
    }
    if (listen(server_fd, 10) < 0) {// Listen for incoming connections
        throw std::runtime_error("listen failed");
    }
    std::cout << "Server listening over tcp on port " << port_tcp << std::endl;


    while (true) {// Main loop for handling connections and commands
        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds); // Add TCP server socket to the set
        FD_SET(0, &readfds); // Add stdin to the set

       
        max_sd = server_fd;
        if (max_sd < 0) max_sd = 0;

        for (int i = 0; i < FD_SETSIZE; i++) { // Add all client sockets to the set
            sd = client_socket[i];
            if (sd > 0)
                FD_SET(sd, &readfds);
            if (sd > max_sd)
                max_sd = sd;
        }
        
        activity = select(max_sd + 1, &readfds, nullptr, nullptr, nullptr);// Wait for activity on the sockets
        if (activity < 0 && errno != EINTR) {
            std::cout << "select error" << std::endl;
            break;
        }

        // Handle new TCP connections
        if (FD_ISSET(server_fd, &readfds)) {
            socklen_t addrlen = sizeof(address);
            if ((new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen)) < 0) {
                std::cout << "accept error" << std::endl;
                continue;
            }
            for (int i = 0; i < FD_SETSIZE; i++) {
                if (client_socket[i] == 0) {
                    client_socket[i] = new_socket; // Store the new socket in the client_socket array
                    break;
                }
            }
            std::cout << "New connection, socket fd: " << new_socket << std::endl;
        }
           // Handle input from stdin
        if (FD_ISSET(0, &readfds)) {
            std::string input;
            std::getline(std::cin, input);
            if (input == "exit") {
                std::cout << "Exiting server..." << std::endl;
                break;
            }
            else {
                std::cout << "Unknown command: " << input << std::endl;
            }
            
        }
        for (int i = 0; i < FD_SETSIZE; i++) {
            sd = client_socket[i];
            if (sd > 0 && FD_ISSET(sd, &readfds)) {
                char type_buffer[16] = {0};
                ssize_t received = recv(sd, type_buffer, sizeof(type_buffer), MSG_WAITALL);
                if (received <= 0) {
                    std::cerr << "Failed to receive type\n";
                    close(sd);
                    client_socket[i] = 0;
                    continue;
                }
                std::string type(type_buffer);

                int n = 0;
                received = recv(sd, &n, sizeof(n), MSG_WAITALL);
                if (received != sizeof(n) || n <= 0) {
                    std::cerr << "Failed to receive valid matrix size\n";
                    close(sd);
                    client_socket[i] = 0;
                    continue;
                }

                std::cout << "Received matrix size: " << n << "\n";
                bool success = true;
                std::vector<std::vector<int>> newMatrix;

                if (type == "matrix") {
                    newMatrix.resize(n, std::vector<int>(n));
                    for (int row = 0; row < n; ++row) {
                        received = recv(sd, newMatrix[row].data(), n * sizeof(int), MSG_WAITALL);
                        if (received != n * sizeof(int)) {
                            std::cerr << "Failed to receive row " << row << "\n";
                            success = false;
                            break;
                        }
                    }
                } else if (type == "random") {
                    size_t e_num = 0;
                    received = recv(sd, &e_num, sizeof(e_num), MSG_WAITALL);
                    if (received != sizeof(e_num)) {
                        std::cerr << "Failed to receive number of edges\n";
                        close(sd);
                        client_socket[i] = 0;
                        continue;
                    }

                    std::cout << "Received random graph request with vertices: " << n << " edges: " << e_num << "\n";
                    Graph tempGraph;
                    CreateRandomGraph(n, e_num, tempGraph);
                    newMatrix = tempGraph.adjMat;
                } else {
                    std::cerr << "Unknown graph type received: " << type << "\n";
                    close(sd);
                    client_socket[i] = 0;
                    continue;
                }

                if (success) {
                    std::cout << "Received adjacency matrix of size " << n << "x" << n << "\n";
                    {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        taskQueue.push({sd, newMatrix});
                    }
                    queueCV.notify_one();
                
                    std::cout << "Task added to queue for processing" << std::endl;
                } else {
                    close(sd);
                    client_socket[i] = 0;
                }
            }
        }
    }

    // Cleanup - signal workers to stop and wait for them
    serverRunning = false;
    queueCV.notify_all();
    for (auto& worker : workers) {
        worker.join();
    }

    for (int i = 0; i < FD_SETSIZE; i++) {
        if (client_socket[i] > 0) {
            close(client_socket[i]);
        }
    }

    std::cout << "Server closed" << std::endl;

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


void Graph::dfs(int u, const std::vector<std::vector<int>>& adj, std::vector<bool>& visited) {
    visited[u] = true;
    for (size_t v = 0; v < adj.size(); ++v) {
        if (adj[u][v] == 1 && !visited[v])
            dfs(v, adj, visited);
    }
}

bool Graph::isConnected(const std::vector<std::vector<int>>& adj) {
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
  bool Graph::hasPath(int src, int dest) const{
    if (src < 0 || src >= vertices || dest < 0 || dest >= vertices) {
        std::cerr << "Invalid source or destination vertex.\n";
        return false;
    }
    std::vector<bool> visited(vertices, false);
    std::stack<int> stack;
    stack.push(src);
    
    while (!stack.empty()) {
        int current = stack.top();
        stack.pop();
        
        if (current == dest) {
            return true; // Path found
        }
        
        if (!visited[current]) {
            visited[current] = true;
            for (int i = 0; i < vertices; ++i) {
                if (adjMat[current][i] > 0 && !visited[i]) { // Check for positive weights
                    stack.push(i);
                }
            }
        }
    }
    return false; // No path found
  }



void Graph::parseFromMatrix(const std::vector<std::vector<int>>& matrix) {
    this->vertices = matrix.size();
    this->adjMat = matrix;
    for(int i = 0; i < vertices; ++i) {
        for (int j = 0; j < vertices; ++j) {
            if (matrix[i][j] == 1) {
                EdgesNum++;
            }
        }
    }
}


int main(int argc, char* argv[]) {
    Graph g;
    std::vector<int> euler;
  
    if (argc < 3) { // Check if the required arguments are provided
          std::cerr << "Usage: " << argv[0]
                  << " -T <tcp_port> \n";
        return 0;
    }
    int port_tcp;
    int opt;
    bool has_tcp = false;
     while ((opt = getopt(argc, argv, "T:")) != -1) {
        switch (opt) {
             
            case 'T':
                if(std::atoi(optarg) <= 0) {
                    std::cerr << "Error: any flag must be a positive integer." << std::endl;
                    exit(EXIT_FAILURE);
                }
                port_tcp = std::atoi(optarg);
                if (port_tcp <= 0 || port_tcp > 65535) { // Check if port is valid
                    std::cerr << "Error: Port must be a positive integer between 1 and 65535." << std::endl;
                    exit(EXIT_FAILURE);
                }
                has_tcp = true;
                break;
            
            default:
                std::cerr << "Usage: " << argv[0]
                          << " -T <tcp_port>\n";
                exit(EXIT_FAILURE);
        }
    }
    if (!has_tcp ) {
        std::cerr << "Error: Missing TCP port." << std::endl;
        exit(EXIT_FAILURE);
    }

     run_server(port_tcp, g); // Start the server with the provided TCP port

    return 0;
}
