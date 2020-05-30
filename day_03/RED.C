
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
int index=0;
int x,y;
unsigned char colors[]={ 0x04, 0x0c, 0x27, 0x28,
                         0x27, 0x0c };

// set video mode to 320x200 256 color mode

Set_Video_Mode(VGA256);

// plot 10000 dots


// wait for user to hit a key

while(!done) {
  for(x=158; x<=160; x++) {
    for(y=98; y<=100; y++) {
      Plot_Pixel_Fast(x,y,colors[index]);
    }
  }
  index = (index + 1) % sizeof(colors);

  // Check for keyboard
  if(kbhit()) {
    done = 1;
  }
  Delay(10);
}

// reset back set video mode to 320x200 256 color mode

Set_Video_Mode(TEXT_MODE);

} // end main



