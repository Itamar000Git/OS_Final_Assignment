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

bool sendMatrix(int sockfd, const std::vector<std::vector<int>>& adjMat) {
    int n = adjMat.size();
    std::string type = "matrix";

 
    char type_buf[16] = {0};
    std::memcpy(type_buf, type.c_str(), std::min(type.size(), sizeof(type_buf)-1));

    if (send(sockfd, type_buf, sizeof(type_buf), 0) < 0) {
        std::cout << "Failed to send matrix type\n";
        return false;
    }
   
    if (send(sockfd, &n, sizeof(n), 0) < 0) {
        std::cout << "Failed to send matrix size\n";
        return false;
    }

    for (int i = 0; i < n; ++i) {
        if (send(sockfd, adjMat[i].data(), n * sizeof(int), 0) < 0) {
            std::cout << "Failed to send matrix row " << i << "\n";
            return false;
        }
    }

    return true;
}

bool sendRandom(int sockfd, int vertices, int edges){
    int n = vertices;
    size_t e = static_cast<size_t>(edges);
    std::string type = "random";
    std::cout << "Sending random graph with " << n << " vertices and " << e << " edges." << std::endl;

    if (send(sockfd, type.c_str(), 16, 0) < 0) {
        std::cout << "Failed to send random graph type\n";
        return false;
    }
    if (send(sockfd, &n, sizeof(n), 0) < 0) {
        std::cout << "Failed to send vertices size\n";
        return false;
    }
    if (send(sockfd, &e, sizeof(e), 0) < 0) {
        std::cout << "Failed to send edges size\n";
        return false;
    }
    return true;
}


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
    int edgesNum;
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
        std::cout << "2. Choose a random graph" << std::endl;
        std::cout << "3. Exit" << std::endl;
        int choice;
        std::cin >> choice;
        if (choice == 3) {
            std::cout << "Exiting client..." << std::endl;
            break;
        }
        if (choice != 1 && choice != 2) {
            std::cout << "Invalid choice. Please try again." << std::endl;
            std::cin.clear(); // Clear error state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Discard invalid input
            continue; // Skip sending matrix if choice is invalid
        }


        std::cout << "Enter number of vertices: "<< std::endl;
        std::cin >> vertices;
        if (vertices <= 0) {
            std::cerr << "Error: Number of vertices must be a positive integer." << std::endl;
            close(sockfd);
            return 1;
        }

        if(choice == 1){
            adjMat.clear();
            adjMat.resize(vertices, std::vector<int>(vertices,inf ));
            std::cout << "Enter adjacency matrix :" << std::endl;
            // Read adjacency matrix from standard input
            for (int i = 0; i < vertices; ++i) {
                for (int j = 0; j < vertices; ++j) {
                    std::cin >> adjMat[i][j];
                    if (adjMat[i][j] < 0) {
                        std::cerr << "Error: Adjacency matrix must contain non negativ number." << std::endl;
                        close(sockfd);
                        return 1;

                }

                }
            }
            // Send the adjacency matrix to the server
            if (!sendMatrix(sockfd, adjMat)) {
                std::cout << "Failed to send adjacency matrix to server." << std::endl;
                close(sockfd);
                return 1;
            }
            std::cout << "Adjacency matrix sent to server." << std::endl;
        }
        else if(choice ==2){
            std::cout << "Enter number of edges: "<< std::endl;
            std::cin >> edgesNum;
            if (edgesNum <= 0) {
                std::cerr << "Error: Number of edges must be a positive integer." << std::endl;
                close(sockfd);
                return 1;
            }
            if (!sendRandom(sockfd, vertices, edgesNum)) {
                std::cerr << "Failed to send random graph to server." << std::endl;
                close(sockfd);
                return 1;
            }
        }


      ////////////////////////maybe need 4 times loop
        int msg_len = 0;
        ssize_t bytes_received = recv(sockfd, &msg_len, sizeof(msg_len), MSG_WAITALL);
        if (bytes_received != sizeof(msg_len)) {
            std::cerr << "Error receiving message length from server." << std::endl;
            close(sockfd);
            return 1;
        }

        std::vector<char> buffer(msg_len + 1); 
        bytes_received = recv(sockfd, buffer.data(), msg_len, MSG_WAITALL);
        if (bytes_received != msg_len) {
            std::cerr << "Error receiving full message from server." << std::endl;
            close(sockfd);
            return 1;
        }

        // Null-terminate and print
        buffer[msg_len] = '\0';
        std::cout << "Server response: " << buffer.data() << std::endl;
///////////////////////////////////////////////////////////////////////////////
    }
    // Close the socket
    close(sockfd);
    std::cout << "Connection closed." << std::endl;
    return 0;

}