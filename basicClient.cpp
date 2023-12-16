#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char serverIP[] = "127.0.0.1";  // Change to the server's IP
    char message[] = "Hello, server!";
    
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation error");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);  // Change to the server's port
    if (inet_pton(AF_INET, serverIP, &serverAddr.sin_addr) <= 0) {
        perror("Address conversion error");
        exit(1);
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection error");
        exit(1);
    }

    send(clientSocket, message, strlen(message), 0);

    close(clientSocket);

    return 0;
}
