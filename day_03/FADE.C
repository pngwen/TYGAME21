// I N C L U D E S ///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "graph3.h" // this is all we need to include so that the program
                    // knows all the #defines, structures, prototypes etc.

// M A I N ///////////////////////////////////////////////////////////////////

void main(void)
{
RGB_color color;  
int done;
int x,y;
int i;

// set video mode to 320x200 256 color mode
Set_Video_Mode(VGA256);

//set each pixel to a random value
for(x=0; x<320; x++) {
    for(y=0; y<200; y++) {
	Plot_Pixel_Fast(x, y, (unsigned char)(rand() & 0xff));
    }
}

// fade to black
do {
    //assume this is the last time
    done = 1;

    //decrement every register
    for(i=0; i<=255; i++) {
        Get_Palette_Register(i, &color);
	if(color.red) {
	    done = 0;
	    color.red--;
	}
	if(color.green) {
	    done = 0;
	    color.green--;
	}
	if(color.blue) {
	    done = 0;
	    color.blue--;
	}
	Set_Palette_Register(i, &color);
    }
    Delay(2); //sloooowly
} while(!done);

Set_Video_Mode(TEXT_MODE);
}
