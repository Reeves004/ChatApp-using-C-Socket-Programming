#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    SOCKET sock = INVALID_SOCKET;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // Initializing Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // Creating a socket for the client
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation error\n";
        WSACleanup();
        return 1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9090);  // Ensure this matches the server's port
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    // Connecting to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Connection Failed\n";
        closesocket(sock);
        WSACleanup();
        return 1;
    }

    std::cout << "Connection successful! Connected to server.\n";  // Connection successful message

    // Starting continuous communication
    while (true) {
        // Allowing user to input a message to send to the server
        std::string clientMessage;
        std::cout << "Enter your message to send to the server (or type 'quit' to exit): ";
        std::getline(std::cin, clientMessage);  // User types a message to send

        // Sending the message to the server
        send(sock, clientMessage.c_str(), clientMessage.length(), 0);
        std::cout << "Message sent to the server\n";

        // Checking if the user wants to quit
        if (clientMessage == "quit") {
            break; // Exit the loop and disconnect from the server
        }

        // Receiving the server's response
        recv(sock, buffer, 1024, 0);
        std::cout << "Server response: " << buffer << std::endl;

        // Clearing the buffer for the next message
        memset(buffer, 0, sizeof(buffer));
    }

    // Cleaning up
    closesocket(sock);
    WSACleanup();
    return 0;
}
