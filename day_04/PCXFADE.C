// I N C L U D E S ////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "graph3.h" // this is all we need to include so that the program
                    // knows all the #defines, structures, prototypes etc.
#include "graph4.h"
#include "better4.h"

void main(int argc, char **argv)
{
pcx_picture background_pcx;  // this pcx structure holds background imagery

if(argc != 2) {
    printf("Usage: %s [pcxfile]\n", argv[0]);
    return;
}

//graphic mode
Set_Video_Mode(VGA256);

//load and display the image
PCX_Init((pcx_picture_ptr)&background_pcx);
PCX_Load(argv[1], (pcx_picture_ptr)&background_pcx,1);
PCX_Show_Buffer((pcx_picture_ptr)&background_pcx);

while(!kbhit()) { } //wait for keypress

Better_Fade();

//text mode
Set_Video_Mode(TEXT_MODE);
}
