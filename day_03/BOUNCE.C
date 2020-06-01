//Bounce a dot

// I N C L U D E S ///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "graph3.h" // this is all we need to include so that the program
                    // knows all the #defines, structures, prototypes etc.

// M A I N ///////////////////////////////////////////////////////////////////

void main(void)
{
int done=0; // exit flag
int x=159;
int y=99;
int dx=5;
int dy=3;
unsigned char bg=0x00;

// set video mode to 320x200 256 color mode
Set_Video_Mode(VGA256);


// wait for user to hit a key
while(!done) {
  //erase
  Plot_Pixel_Fast(x, y, bg);

  //move
  x+=dx;
  if(x < 0) {
      x = 0;
      dx = -dx;
  } else if(x > 319) {
      x = 319;
      dx = -dx;
  }
  y+=dy;
  if(y < 0) {
      y = 0;
      dy = -dy;
  } else if(y > 199) {
      y = 199;
      dy = -dy;
  }

  //draw
  Plot_Pixel_Fast(x, y, 0x0c);

  // Check for keyboard
  if(kbhit()) {
    done = 1;
  }
  Delay(1);
}

// reset back set video mode to 320x200 256 color mode

Set_Video_Mode(TEXT_MODE);

} // end main
