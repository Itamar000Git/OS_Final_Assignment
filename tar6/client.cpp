#include <iostream>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <netdb.h>
#include <unistd.h>
#include <vector>
#include <limits>

int inf= std::numeric_limits<int>::max();

/**
 * @brief Sends an adjacency matrix to the server.
 * @param sockfd The socket file descriptor.
 * @param adjMat The adjacency matrix to send.
 * @return True if the matrix was sent successfully, false otherwise.
 */
bool sendMatrix(int sockfd, const std::vector<std::vector<int>>& adjMat) {
    int n = adjMat.size();

    if (send(sockfd, &n, sizeof(n), 0) < 0) {
        std::cerr << "Failed to send matrix size\n";
        return false;
    }

    for (int i = 0; i < n; ++i) {
        if (send(sockfd, adjMat[i].data(), n * sizeof(int), 0) < 0) {
            std::cerr << "Failed to send matrix row " << i << "\n";
            return false;
        }
    }

    return true;
}

/**
 * @brief Checks if the graph has an Eulerian cycle and sends the result to the client.
 * @param g The graph to check.
 * @param socket_fd The socket file descriptor to send the result to.
 */
int main(int argc, char* argv[]) {
    if (argc < 3) { // Check if there are enough arguments
        std::cerr << "Usage: " << argv[0] << "-h <hostname> -p <port> " << std::endl;
        return 1;
    }
    const char* hostname;
    int port;
    int opt;
    bool has_host = false, has_port = false;
    
     while ((opt = getopt(argc, argv, "h:p:")) != -1) {
        switch (opt) {
             
            case 'h':
                hostname = optarg;
                has_host = true;
                break;
            case 'p':
                
                port = std::atoi(optarg);
                has_port = true;
                if (port <= 0 || port > 65535) { // Check if port is valid
                    std::cerr << "Error: Port must be a positive integer between 1 and 65535." << std::endl;
                    exit(EXIT_FAILURE);
                }
                break;
        
            default:
                std::cerr << "Usage: " << argv[0]
                          << " -h <hostname> -p <port>\n";
                exit(EXIT_FAILURE);
        }
    }
    if (!has_host || !has_port) {
        std::cerr << "Error: Missing hostname or port." << std::endl;
        exit(EXIT_FAILURE);
    }

    int vertices;
    std::vector<std::vector<int>> adjMat;
    int sockfd = -1;
 // Resolve hostname
    struct hostent* server = gethostbyname(hostname);
    if (!server) {
        std::cerr << "No such host: " << hostname << std::endl;
        return 1;
    }

    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error opening socket" << std::endl;
        return 1;
    }
    // Set up server address structure
    struct sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    std::memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    serv_addr.sin_port = htons(port);

    // Connect to server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        close(sockfd);
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    while(true){
        std::cout<<"Enter your choice:"<<std::endl;
        std::cout << "1. Send adjacency matrix to server" << std::endl;
        std::cout << "2. Exit" << std::endl;
        int choice;
        std::cin >> choice;
        if (choice == 2) {
            std::cout << "Exiting client..." << std::endl;
            break;
        }


        std::cout << "Enter number of vertices: "<< std::endl;
        std::cin >> vertices;
        if (vertices <= 0) {
            std::cerr << "Error: Number of vertices must be a positive integer." << std::endl;
            close(sockfd);
            return 1;
        }
        adjMat.clear();
        adjMat.resize(vertices, std::vector<int>(vertices,inf ));
        std::cout << "Enter adjacency matrix (0 for no edge, 1 for edge):" << std::endl;
        // Read adjacency matrix from standard input
        for (int i = 0; i < vertices; ++i) {
            for (int j = 0; j < vertices; ++j) {
                std::cin >> adjMat[i][j];
                if (adjMat[i][j] != 0 && adjMat[i][j] != 1) {
                    std::cerr << "Error: Adjacency matrix must contain only 0 or 1." << std::endl;
                    close(sockfd);
                    return 1;

            }

            }
        }


        // Send the adjacency matrix to the server
        if (!sendMatrix(sockfd, adjMat)) {
            std::cerr << "Failed to send adjacency matrix to server." << std::endl;
            close(sockfd);
            return 1;
        }
        std::cout << "Adjacency matrix sent to server." << std::endl;

        // Wait for server response
        char buffer[1024];
        ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Null-terminate the received data
            std::cout << "Server response: " << buffer << std::endl;
        }
        else if (bytes_received < 0) {
            std::cerr << "Error receiving response from server." << std::endl;
            close(sockfd);
            return 1;
        } else {
            std::cout << "Server closed the connection." << std::endl;
        }
    }
    // Close the socket
    close(sockfd);
    std::cout << "Connection closed." << std::endl;
    return 0;

}