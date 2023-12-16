

///////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int hamming[8] = {0, 0, 0, 0, 0, 0, 0, 0};
int correctedHamming[8];

void decodeHamming() {
    for (int i = 1; i <= 7; i++) {
        correctedHamming[i] = hamming[i];
    }

    int parity1 = hamming[1] ^ hamming[3] ^ hamming[5] ^ hamming[7];
    int parity2 = hamming[2] ^ hamming[3] ^ hamming[6] ^ hamming[7];
    int parity4 = hamming[4] ^ hamming[5] ^ hamming[6] ^ hamming[7];

    int errorPosition = parity1 + 2 * parity2 + 4 * parity4;

    if (errorPosition > 0) {
        printf("Error detected at bit position %d\n", errorPosition);
        correctedHamming[errorPosition] ^= 1;
    } else {
        printf("No error in received data, received data is: ");
        for (int i = 1; i <= 7; i++) {
            printf("%d", correctedHamming[i]);
        }
        printf("\n");
    }
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[8];

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation error");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);

    if (bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr))< 0){
        perror("Bind error");
        exit(1);
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Listen error");
        exit(1);
    }

    printf("Server listening on port 12345...\n");

    clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &addrLen);
    if (clientSocket < 0) {
        perror("Accept error");
        exit(1);
    }

    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead > 0) {
        for (int i = 0; i < 7; i++) {
            hamming[i + 1] = buffer[i] - '0';
        }
        decodeHamming();
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}
