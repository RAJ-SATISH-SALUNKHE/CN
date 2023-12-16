#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>

using namespace std;

string data, polynomial;

string XOR(string red) {
    for (int i = 0; i < polynomial.length(); i++) {
        char dataBit = red[i];
        char polyBit = polynomial[i];

        if (dataBit == polyBit) {
            red[i] = '0';
        } else {
            red[i] = '1';
        }
    }

    return red;
}

string CRC() {
    string red = ::data;

    if (red.length() < polynomial.length()) return red;

    while (red.length() >= polynomial.length()) {
        while (red.length() > 0 && red[0] == '0') {
            red.erase(0, 1);
        }

        if (red.length() >= polynomial.length()) {
            red = XOR(red);
        }
    }

    return red;
}

bool checkCRC() {
    string remainder = CRC();

    for (char bit : remainder) {
        if (bit == '1') {
            return false;  
        }
    }

    return true;  
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[1024];

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        perror("Socket creation error");
        exit(1);
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);  // Change to your desired port

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind error");
        exit(1);
    }

    if (listen(serverSocket, 5) < 0) {
        perror("Listen error");
        exit(1);
    }

    cout << "Server listening on port 12345..." << endl;

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen);
    if (clientSocket < 0) {
        perror("Accept error");
        exit(1);
    }

    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived <= 0) {
        cout << "No data received from the client." << endl;
        close(clientSocket);
        close(serverSocket);
        return 0;
    }

    buffer[bytesReceived] = '\0';
    ::data = buffer;

    cout << "Received data: " << ::data << endl;

    cout << "Enter the 4-bit generating polynomial: ";
    cin >> polynomial;

    bool isDataValid = checkCRC();

    if (isDataValid) {
        cout << "Received data is error-free. Original data: " << ::data << endl;
    } else {
        cout << "Received data contains errors." << endl;
    }

    close(clientSocket);
    close(serverSocket);

    return 0;
}
