// I N C L U D E S ////////////////////////////////////////////////////////////
#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include <fcntl.h>
#include <memory.h>
#include <malloc.h>
#include <math.h>
#include <string.h>

#include <graph.h>   // microsoft's stuff if we need it

#include "graph3.h"  // the module from day 3
#include "graph4.h"  // the module from day 4
#include "better4.h" // my better functions



// F U N C T I O N S //////////////////////////////////////////////////////////
void main()
{
sprite tank;
pcx_picture objects_pcx;
int index;
int dir=1;
sprite scaled_tank[5];

// load the .PCX file with the tank cells

// load in the players imagery
PCX_Init((pcx_picture_ptr)&objects_pcx);
PCX_Load("tanks.pcx", (pcx_picture_ptr)&objects_pcx,0);


// initialize sprite size and data structure
sprite_width  = 16;
sprite_height = 16;

// place tank (player) in center of screen
Sprite_Init((sprite_ptr)&tank,152,91,0,0,0,0);

// grab all 16 images from the tanks pcx picture
for (index=0; index<16; index++)
    {

    PCX_Grab_Bitmap((pcx_picture_ptr)&objects_pcx,(sprite_ptr)&tank,index,index,0);

    } // end for index

//get rid of the PCX
PCX_Delete((pcx_picture_ptr)&objects_pcx);

//scale the tank at each level
for(index=0; index<5; index++) {
    Better_Scale_Sprite(scaled_tank+index, &tank, index+1);
}

// set video mode to 320x200 256 color mode
Set_Video_Mode(VGA256);

Better_Behind_Sprite(scaled_tank);
index=0;

//wait for keyboard
while(!kbhit())
{
    Better_Erase_Sprite(scaled_tank+index);

    //adjust scale
    index += dir;
    if(index==0 || index == 4) {
	dir=-dir;
    }

    Better_Behind_Sprite(scaled_tank+index);
    Better_Draw_Sprite(scaled_tank+index);

    //make the next sprite
    Delay(1);
}

Sprite_Fizzle(scaled_tank + index);

// go back to text mode
Set_Video_Mode(TEXT_MODE);
}
