/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dummy.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cmeng <cmeng@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 04:09:35 by cmeng             #+#    #+#             */
/*   Updated: 2024/01/23 04:15:02 by cmeng            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// g++ -std=c++98 dummy.cpp -o server

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

const int PORT_NUMBER = 1234;  // Choose a port number

int main() {
    // Step 1: Create a socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        return 1;
    }

    // Step 2: Set up server address
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT_NUMBER);

    // Step 3: Bind the socket to the address
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        perror("Error binding socket");
        close(serverSocket);
        return 1;
    }

    // Step 4: Listen for incoming connections
    if (listen(serverSocket, 10) == -1) {
        perror("Error listening for connections");
        close(serverSocket);
        return 1;
    }

    std::cout << "Server listening on port " << PORT_NUMBER << std::endl;

    // Step 5: Accept incoming connections
    sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    int clientSocket = accept(serverSocket, reinterpret_cast<sockaddr*>(&clientAddr), &clientAddrLen);
    if (clientSocket == -1) {
        perror("Error accepting connection");
        close(serverSocket);
        return 1;
    }

    char clientIP[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);

    std::cout << "Connection accepted from " << clientIP << std::endl;

    std::cout << "Connection accepted from " << inet_ntoa(clientAddr.sin_addr) << std::endl;

    // Step 6: Echo back any data received
    char buffer[1024];
    ssize_t bytesRead;

    while ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        // Echo back the data
        send(clientSocket, buffer, bytesRead, 0);
    }

    if (bytesRead == -1) {
        perror("Error receiving data");
    }

    // Step 7: Close the sockets
    close(clientSocket);
    close(serverSocket);

    return 0;
}
