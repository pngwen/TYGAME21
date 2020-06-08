//Draw a circle
#include <io.h>
#include <stdlib.h>
#include "graph3.h"
#include "graph4.h"
#include "graph5.h"


void Scale2_Polygon(polygon_ptr poly, float sx, float sy);


int main()
{
polygon ship;

// build up a little spaceship polygon
ship.vertices[0].x = 3;
ship.vertices[0].y = -19;

ship.vertices[1].x = 12;
ship.vertices[1].y = -1;

ship.vertices[2].x = 17;
ship.vertices[2].y = 2;

ship.vertices[3].x = 17;
ship.vertices[3].y = 9;

ship.vertices[4].x = 8;
ship.vertices[4].y = 14;

ship.vertices[5].x = 5;
ship.vertices[5].y = 8;

ship.vertices[6].x = -5;
ship.vertices[6].y = 8;

ship.vertices[7].x = -8;
ship.vertices[7].y = 14;

ship.vertices[8].x = -17;
ship.vertices[8].y = 9;

ship.vertices[9].x = -17;
ship.vertices[9].y = 2;

ship.vertices[10].x = -12;
ship.vertices[10].y = -1;

ship.vertices[11].x = -3;
ship.vertices[11].y = -19;

ship.vertices[12].x = -3;
ship.vertices[12].y = -8;

ship.vertices[13].x = 3;
ship.vertices[13].y = -8;

// set position of shaceship

ship.lxo = 160;
ship.lyo = 100;

ship.num_vertices = 14;
ship.b_color      = 1;
ship.closed       = 1;

//set up trig tables
Create_Tables();

//go graphical
Set_Video_Mode(VGA256);

//scale and draw
Scale2_Polygon(&ship, 1.5, 5);
Draw_Polygon_Clip((polygon_ptr)&ship);

//wait for keypress
while(!kbhit());

//back to text mode
Set_Video_Mode(TEXT_MODE);
}


void Scale2_Polygon(polygon_ptr poly, float sx, float sy)
{

int index;

// scale each vertex of the polygon

for (index=0; index<poly->num_vertices; index++)
    {
    // multiply by the scaling factor

    poly->vertices[index].x*=sx;
    poly->vertices[index].y*=sy;

    } // end for

} // end Scale_Polygon
