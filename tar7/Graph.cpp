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
#include "Algorithms.hpp"
#include "MST.hpp"
#include "Strategy.hpp"
#include "Factory.hpp"

Graph::Graph() : vertices(0), EdgesNum(0) {}

void run_server(int port_tcp, Graph& g) {

    int server_fd, new_socket, max_sd, activity, sd;
    int client_socket[FD_SETSIZE] = {0};
    struct sockaddr_in address;
    fd_set readfds;

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

        for (int i = 0; i < FD_SETSIZE; i++) {////////////////////////////////////////////////////
            sd = client_socket[i];
            if (sd > 0 && FD_ISSET(sd, &readfds)) {

                
                int alg_len = 0;
                size_t received = recv(sd, &alg_len, sizeof(alg_len), MSG_WAITALL);
                if (received != sizeof(alg_len)) {
                    std::cerr << "Failed to receive algorithm length\n";
                    close(sd);
                    client_socket[i] = 0;
                    continue;
                }
                //Read the algorithm type
                std::vector<char> alg_buf(alg_len);
                received = recv(sd, alg_buf.data(), alg_len, MSG_WAITALL);

                if (received != alg_len) {
                    std::cerr << "Failed to receive algorithm type\n";
                    close(sd);
                    client_socket[i] = 0;
                    continue;
                }
                std::string alg(alg_buf.begin(), alg_buf.end());
                //Read the matrix size (number of vertices)
                int n = 0;
                received = recv(sd, &n, sizeof(n), MSG_WAITALL);// Read the size of the matrix
                if (received != sizeof(n)) {
                    std::cerr << "Failed to receive matrix size\n";
                    close(sd);
                    client_socket[i] = 0;
                    continue;
                }
                // Read adjacency matrix
                std::vector<std::vector<int>> newMatrix(n, std::vector<int>(n));
                bool success = true;
                for (int row = 0; row < n; ++row) {
                    received = recv(sd, newMatrix[row].data(), n * sizeof(int), MSG_WAITALL);
                    if (received != n * sizeof(int)) {
                        std::cerr << "Failed to receive row " << row << "\n";
                        success = false;
                        break;
                    }
                }
                if (success) {
                    std::cout << "Received adjacency matrix of size " << n << "x" << n << "\n";
                    g.parseFromMatrix(newMatrix);
                    std::cout << "Graph updated.\n";
                    g.printGraph();

                    Strategy stra; //create a Strategy object
                    // Create the algorithm object using the factory
                    Algorithms* algObj = AlgorithmFactory::createAlgorithm(alg);

                    stra.setStrategy(algObj); // Set the strategy with the created algorithm
                    std::string result = stra.execute(g);// Execute the strategy on the graph

                    if (send(sd, result.c_str(), result.size(), 0) < 0) { // Send the result back to the client
                        std::cerr << "Failed to send result to client\n";
                        close(sd);
                        client_socket[i] = 0;
                        continue;
                    }
                    std::cout << "Result sent to client." << std::endl;
                  
    
                }
                else{
                    close(sd);
                    client_socket[i] = 0;
                }

                
            }
        }
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


// void dfs(int u, const std::vector<std::vector<int>>& adj, std::vector<bool>& visited) {
//     visited[u] = true;
//     for (size_t v = 0; v < adj.size(); ++v) {
//         if (adj[u][v] == 1 && !visited[v])
//             dfs(v, adj, visited);
//     }
// }

// bool isConnected(const std::vector<std::vector<int>>& adj) {
//     int n = adj.size();
//     std::vector<bool> visited(n, false);
//     int start = -1;
//     for (int i = 0; i < n; ++i) {
//         int degree = 0;
//         for (int j = 0; j < n; ++j)
//             if (adj[i][j] == 1) degree++;
//         if (degree > 0) {
//             start = i;
//             break;
//         }
//     }
//     if (start == -1)
//         return true; 

//     dfs(start, adj, visited);
//     for (int i = 0; i < n; ++i) {
//         int degree = 0;
//         for (int j = 0; j < n; ++j)
//             if (adj[i][j] == 1) degree++;
//         if (degree > 0 && !visited[i]){
//             std::cerr << "Graph is not connected: vertex " << i << " is not reachable from vertex " << start << "\n";   
//             return false;
//         }
//     }
//     return true;
// }



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
