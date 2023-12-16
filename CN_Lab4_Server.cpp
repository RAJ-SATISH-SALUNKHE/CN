#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

struct Packet {
    int sequenceNumber;
};

int main() {
    int receiverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (receiverSocket == -1) {
        cerr << "Error creating socket" << endl;
        return -1;
    }

    sockaddr_in receiverAddr;
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(8080); // Set to the desired port.
    receiverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(receiverSocket, (const sockaddr*)&receiverAddr, sizeof(receiverAddr)) == -1) {
        cerr << "Error binding socket" << endl;
        close(receiverSocket);
        return -1;
    }

    int expectedSeqNum = 0;

    while (true) {
        Packet packet;
        socklen_t addrLen = sizeof(receiverAddr);
        recvfrom(receiverSocket, (char*)&packet, sizeof(packet), 0, (sockaddr*)&receiverAddr, &addrLen);

        // Process the received packet (frame number).
        cout << "Received Frame " << packet.sequenceNumber << endl;
        // Add a delay if needed
        // usleep(2000000); // 2 seconds delay

        // Send acknowledgment.
        sendto(receiverSocket, (const char*)&packet, sizeof(packet), 0, (const sockaddr*)&receiverAddr, sizeof(receiverAddr));
        cout << "Sent Acknowledgment for Frame " << packet.sequenceNumber << endl;

        expectedSeqNum++;
    }

    close(receiverSocket);
    return 0;
}
