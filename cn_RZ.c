//RZ


#include <conio.h>
#include <dos.h>
#include <graphics.h>
#include <stdio.h>
#include <string.h>
#define BUFFER_SIZE 8

void stepUp(void);
void stepDown(void);
void stepUpHalf(void);
void drawBit(void);

int x, y, i;
int data[BUFFER_SIZE];
int flag = 1;

void drawBit()
{
    line(x, y, x + 15, y);
    x = x + 15;
}

void stepUp()
{
    line(x, y, x, getmaxy()/4);
    y = getmaxy()/4;
}

void stepDown()
{
    line(x, y, x, 3*getmaxy()/4);
    y = 3*getmaxy()/4;
}

void stepHalf()
{
    line(x, y, x, getmaxy()/2);
    y = getmaxy()/2;
}


int main()
{
	 int gdriver = DETECT;
	 int gmode;
	 int err;

	 initgraph(&gdriver, &gmode, "C:\\Turboc3\\BGI");

	 err = graphresult();
	 if (err != grOk)
	 {
		 printf("Graphics Error: %s\n",
		 grapherrormsg(err));

		 return 0;
	}

	x = getmaxx()/4;
	y = getmaxy()/4;

//    line(getmaxx()/4,0,getmaxx()/4,getmaxy());
//	line(0,getmaxy()/2,getmaxx(),getmaxy()/2);

	printf("Please enter your 8-bit data: ");


    for(i=0; i<8; i++)
    {
        scanf("%d", &data[i]);
        if(data[i]>=1)
        {
            data[i]=1;
        }
        else
        {
            data[i]=0;
        }
    }


    //fgets(data, 8, stdin);


    line(getmaxx()/4,0,getmaxx()/4,getmaxy());
	line(0,getmaxy()/2,getmaxx(),getmaxy()/2);


	if(data[0]==1)
    {
        drawBit();
        stepHalf();
        drawBit();
    }
    else
    {
        stepDown();
        drawBit();
        stepHalf();
        drawBit();
    }


    for(i=1; i<8; i++)
    {
        if(data[i]==1)
        {
            stepUp();
            drawBit();
            stepHalf();
            drawBit();
        }
        else
        {
            stepDown();
            drawBit();
            stepHalf();
            drawBit();
        }
    }



	getch();
	return 0;


}
