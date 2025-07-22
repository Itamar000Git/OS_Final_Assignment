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
    // int alg_len = input.size();
    // if (send(sockfd, &alg_len, sizeof(alg_len), 0) < 0) {
    //     std::cerr << "Failed to send algorithm type length\n";
    //     return false;
    // }
    // if (send(sockfd, input.c_str(), input.size(), 0) < 0) {
    //     std::cerr << "Failed to send algorithm type\n";
    //     return false;
    // }

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
        if (choice != 1 ) {
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


        // std::cout<<"Enter your Algorithem choice:"<<std::endl;
        // std::cout << "1. MST" << std::endl;
        // std::cout << "2. Max Flow" << std::endl;
        // std::cout << "3. Path Cover" << std::endl;
        // std::cout << "4. SCC" << std::endl;
        // std::cout << "5. Exit" << std::endl;
        // int client_choice;
        // std::string input;
        // std::cin >> client_choice;
        // switch (client_choice) {
        //     case 1:
        //         std::cout << "MST algorithm selected." << std::endl;
        //         input = "MST";
        //         break;
        //     case 2:
        //         std::cout << "Max Flow algorithm selected." << std::endl;
        //         input = "MaxFlow";
        //         break;
        //     case 3:
        //         std::cout << "Path Cover algorithm selected." << std::endl;
        //         input = "PathCover";
        //         break;
        //     case 4:
        //         std::cout << "SCC algorithm selected." << std::endl;
        //         input = "SCC";
        //         break;
        //     case 5:
        //         std::cout << "Exiting client..." << std::endl;
        //         close(sockfd);
        //         return 0;
        //     default:
        //         std::cerr << "Invalid choice. Please try again." << std::endl;
        //         continue; // Skip sending matrix if choice is invalid
        // }
        // Send the adjacency matrix to the server
        if (!sendMatrix(sockfd, adjMat)) {
            std::cerr << "Failed to send adjacency matrix to server." << std::endl;
            close(sockfd);
            return 1;
        }
        std::cout << "Adjacency matrix sent to server." << std::endl;
        //std::cout << "Asked for " << input << "." << std::endl;

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