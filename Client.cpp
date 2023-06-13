
// Client.cpp:

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
    int clientSocket;
    struct sockaddr_in serverAddress{};
    char buffer[BUFFER_SIZE];

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    // Connect to server
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr)) <= 0) {
        std::cerr << "Failed to setup server address." << std::endl;
        return 1;
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
        std::cerr << "Failed to connect to server." << std::endl;
        return 1;
    }

    std::cout << "Connected to server." << std::endl;

    // Send and receive messages
    while (true) {
        std::string message;
        std::cout << "Enter message to send: ";
        std::getline(std::cin, message);

        if (message == "quit")
            break;

        write(clientSocket, message.c_str(), message.length());

        memset(buffer, 0, sizeof(buffer));
        int bytesRead = read(clientSocket, buffer, sizeof(buffer));
        if (bytesRead < 0) {
            std::cerr << "Failed to read from socket." << std::endl;
            break;
        }

        std::cout << "Received: " << buffer << std::endl;
    }

    // Close socket
    close(clientSocket);

    return 0;
}
