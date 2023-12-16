#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[1024];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation error");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);  
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection error");
        exit(1);
    }

    FILE *fileToSend = fopen("file_to_send.txt", "rb");
    if (fileToSend == NULL) {
        perror("File opening error");
        exit(1);
    }

    while (1) {
        size_t bytesRead = fread(buffer, 1, sizeof(buffer), fileToSend);
        if (bytesRead <= 0) {
            break;
        }
        send(clientSocket, buffer, bytesRead, 0);
    }

    fclose(fileToSend);
    close(clientSocket);

    return 0;
}
