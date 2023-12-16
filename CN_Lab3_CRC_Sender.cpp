#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;


// string data = "1001101";
// string polynomial = "1011";

string data, polynomial;


string XOR(string red)
{
    for(int i = 0; i < polynomial.length(); i++)
    {
        char dataBit = red[i];
        char polyBit = polynomial[i];

        if(dataBit == polyBit)
        {
            red[i] = '0';
        }
        else
        {
            red[i] = '1';
        }
    }

    return red;
}

string CRC() 
{
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

string modify()
{
    for(int i = 0; i < polynomial.length()-1; i++)
    {
        ::data += '0';
    }


    string remainder = CRC();
    ::data.replace(::data.length() - 3, 3, remainder);

    return ::data;
}



int main()
{
    cout << "Enter the 7-bit data to be sent : ";
    cin >> ::data;

    cout << "Enter the 4-bit generating polynomial : ";
    cin >> polynomial; 

    string modData = modify();

    cout << "The modified data is : " << modData << endl;

    


    //======================================SOCKET======================================

    int clientSocket;
    struct sockaddr_in serverAddr;
    char serverIP[] = "127.0.0.1";
    // char message[] = modData.c_str();
    const char* message = modData.c_str();

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

    send(clientSocket, message, strlen(message), 0);

    close(clientSocket);

    return 0;





    return 1;
}