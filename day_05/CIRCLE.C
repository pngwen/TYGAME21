//Draw a circle
#include <io.h>
#include <stdlib.h>
#include "graph3.h"
#include "graph4.h"
#include "graph5.h"


// P R O T O T Y P E S ////////////////////////////////////////////////////////
void draw_circle(double x, double y, double r, unsigned char c);

int main()
{
//set up trig tables
Create_Tables();

//go graphical
Set_Video_Mode(VGA256);

//wait for keypress
while(!kbhit())
{
    draw_circle(rand()%320, rand()%200, rand()%50, rand()%256);
}

//back to text mode
Set_Video_Mode(TEXT_MODE);
}


//draw a circle at (x,y) with radius r
void draw_circle(double x, double y, double r, unsigned char c)
{
int angle;
double px, py;
for(angle = 0; angle < 360; angle++)
{
    px = x + r*cos_look[angle];
    py = y + r*sin_look[angle];
    if(px >= 0 && px < 320 && py >= 0 && py < 200) {
      Plot_Pixel_Fast(px, py, c);
    }
}
}
