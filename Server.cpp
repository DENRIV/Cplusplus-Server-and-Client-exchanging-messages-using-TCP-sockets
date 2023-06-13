// Server.cpp:

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int PORT = 8080;
const int BUFFER_SIZE = 1024;

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress{}, clientAddress{};
    char buffer[BUFFER_SIZE];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Bind socket to port
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(PORT);

    if (bind(serverSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to bind socket to port." << std::endl;
        return 1;
    }

    // Listen for incoming connections
    listen(serverSocket, 1);
    std::cout << "Server listening on port " << PORT << std::endl;

    // Accept connection
    socklen_t clientAddressLength = sizeof(clientAddress);
    clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddress, &clientAddressLength);
    if (clientSocket < 0) {
        std::cerr << "Failed to accept connection." << std::endl;
        return 1;
    }

    std::cout << "Client connected." << std::endl;

    // Receive and send messages
    while (true) {
        memset(buffer, 0, sizeof(buffer));
        int bytesRead = read(clientSocket, buffer, sizeof(buffer));
        if (bytesRead < 0) {
            std::cerr << "Failed to read from socket." << std::endl;
            break;
        }

        if (bytesRead == 0) {
            std::cout << "Client disconnected." << std::endl;
            break;
        }

        std::cout << "Received: " << buffer << std::endl;

        std::string message;
        std::cout << "Enter message to send: ";
        std::getline(std::cin, message);
        write(clientSocket, message.c_str(), message.length());
    }

    // Close sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}

