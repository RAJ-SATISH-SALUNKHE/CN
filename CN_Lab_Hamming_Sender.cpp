#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <math.h>

int code[] = {0,0,0,0,0,0,0};
int red[] = {0,0,0,0,0,0,0};


int main()
{
    printf("Please enter the 4 data bits: ");
    for(int i=0; i<3; i++)
    {
        int a = pow(2,i)-1;
        red[a] = 1;
    }

    printf("\n");
    for(int i=0; i<7; i++)
    {
        if(!red[i])scanf("%d", &code[i]);
    }


    code[0] = code[2]^code[4]^code[6];
    code[1] = code[2]^code[5]^code[6];
    code[3] = code[4]^code[5]^code[6];


    printf("The resultant array is : \n");
    for(int i=0; i<7; i++)
    {
        printf("%d", code[i]);
    }

    return 0;
}