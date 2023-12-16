#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>

using namespace std;

struct Packet {
    int sequenceNumber;
};

int main() {
    int senderSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (senderSocket == -1) {
        cerr << "Error creating socket" << endl;
        return -1;
    }

    sockaddr_in receiverAddr;
    receiverAddr.sin_family = AF_INET;
    receiverAddr.sin_port = htons(8080); // Change to the receiver's port.
    receiverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to receiver's IP.

    int totalFrames;
    int windowSize;
    cout << "Enter the Total number of frames: ";
    cin >> totalFrames;
    cout << "Enter the Window Size: ";
    cin >> windowSize;

    int base = 0;
    int nextSeqNum = 0;

    while (base < totalFrames) {
        while (nextSeqNum < base + windowSize && nextSeqNum < totalFrames) {
            Packet packet;
            packet.sequenceNumber = nextSeqNum;
            sendto(senderSocket, (const char*)&packet, sizeof(packet), 0, (const sockaddr*)&receiverAddr, sizeof(receiverAddr));
            cout << "Sent Frame " << nextSeqNum << endl;
            nextSeqNum++;
            // Sleep for a specific time (e.g., 2 seconds) between sending frames
            usleep(2000000);
        }

        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(senderSocket, &readSet);

        timeval timeout;
        timeout.tv_sec = 5; // Set your timeout value here (in seconds).
        timeout.tv_usec = 0;

        int ready = select(senderSocket + 1, &readSet, NULL, NULL, &timeout);

        if (ready == -1) {
            cerr << "Error in select" << endl;
            close(senderSocket);
            return -1;
        } else if (ready == 0) {
            // Timeout occurred, retransmit frames in the window.
            cout << "Timeout! Retransmitting window..." << endl;
            nextSeqNum = base;
        } else {
            // Acknowledgment received.
            Packet ack;
            recvfrom(senderSocket, (char*)&ack, sizeof(ack), 0, NULL, NULL);
            cout << "Received Acknowledgment for Frame " << ack.sequenceNumber << endl;
            base = ack.sequenceNumber + 1;
        }
    }

    close(senderSocket);
    return 0;
}
