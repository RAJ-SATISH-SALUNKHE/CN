
///////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <math.h>

int code[7] = {0, 0, 0, 0, 0, 0, 0};
int data[4];
int hamming[7];

void encodeData() {
    hamming[2] = data[0];
    hamming[4] = data[1];
    hamming[5] = data[2];
    hamming[6] = data[3];

    hamming[0] = hamming[2] ^ hamming[4] ^ hamming[6];
    hamming[1] = hamming[2] ^ hamming[5] ^ hamming[6];
    hamming[3] = hamming[4] ^ hamming[5] ^ hamming[6];
}

int main() {
    printf("Please enter the 4 data bits (0 or 1): ");
    for (int i = 0; i < 4; i++) {
        scanf("%d", &data[i]);
    }

    encodeData();

    int clientSocket;
    struct sockaddr_in serverAddr;
    const char* serverIP = "127.0.0.1";
    char message[7];

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket < 0) {
        perror("Socket creation error");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);

    if (inet_pton(AF_INET, serverIP, &serverAddr.sin_addr) <= 0) {
        perror("Address conversion error");
        exit(1);
    }

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection error");
        exit(1);
    }

    for (int i = 0; i < 7; i++) {
        message[i] = hamming[i] + '0';
    }
    
    send(clientSocket, message, sizeof(message), 0);

    close(clientSocket);

    return 0;
}
