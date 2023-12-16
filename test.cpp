#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

string data = ""; // Declare data as a global variable
string polynomial;

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
    string red = data;

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
    for(int i = 0; i < polynomial.length() - 1; i++)
    {
        data += '0'; // Work with the global 'data' variable
    }

    string remainder = CRC();
    data.replace(data.length() - 3, 3, remainder); // Work with the global 'data' variable

    return data; // Return the global 'data' variable
}

int main()
{
    cout << "Enter the 7-bit data to be sent : ";
    cin >> data; // Update the global 'data' variable

    cout << "Enter the 4-bit generating polynomial : ";
    cin >> polynomial; 

    string modData = modify();

    cout << "The modified data is : " << modData << endl;

    // Your socket code here

    return 0;
}
