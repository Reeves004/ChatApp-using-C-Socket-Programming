#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrLen = sizeof(clientAddr);
    char buffer[1024] = {0};

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Create a socket for the server
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        WSACleanup();
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9090);  // The port the server listens on
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Accept any incoming connections

    // Bind the socket to an IP address and port
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Start listening for incoming connections
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port 9090...\n";

    // Accept a client connection
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen)) == INVALID_SOCKET) {
        std::cerr << "Accept failed\n";
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connection successful! Client connected.\n";  // Connection successful message

    // Start continuous communication
    while (true) {
        // Receive data from the client
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Received from client: " << buffer << std::endl;

        // Check if the client wants to quit
        if (std::string(buffer) == "quit") {
            std::cout << "Client disconnected.\n";
            break; // Exit the loop and close the connection
        }

        // Send a response to the client
        std::string serverMessage;
        std::cout << "Enter a message to send to the client (or type 'exit' to quit): ";
        std::getline(std::cin, serverMessage); // User types a message to send to the client

        // Check if user wants to exit the server
        if (serverMessage == "exit") {
            std::cout << "Server is shutting down...\n";
            break;  // Exit the loop and close the server
        }

        send(clientSocket, serverMessage.c_str(), serverMessage.length(), 0);

        // Confirmation that the server sent the message
        std::cout << "Message sent to client: " << serverMessage << std::endl;

        // Clear the buffer for the next message
        memset(buffer, 0, sizeof(buffer));
    }

    // Clean up
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
