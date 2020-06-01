
// I N C L U D E S ///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "graph3.h" // this is all we need to include so that the program
                    // knows all the #defines, structures, prototypes etc.

// M A I N ///////////////////////////////////////////////////////////////////

void main(void)
{
RGB_color orig;  //The original red color
RGB_color cur;   //The current red color
int done=0; // exit flag
int fade=-1;
int x,y;

// set video mode to 320x200 256 color mode
Set_Video_Mode(VGA256);

//extract the color
Get_Palette_Register(0x0c, &orig);
cur = orig;

//plot the pixel
Plot_Pixel_Fast(158,98,0x0c);
Plot_Pixel_Fast(159,98,0x0c);
Plot_Pixel_Fast(160,98,0x0c);
Plot_Pixel_Fast(158,99,0x0c);
Plot_Pixel_Fast(159,99,0x0c);
Plot_Pixel_Fast(160,99,0x0c);
Plot_Pixel_Fast(158,100,0x0c);
Plot_Pixel_Fast(159,100,0x0c);
Plot_Pixel_Fast(160,100,0x0c);

// wait for user to hit a key

while(!done) {
  //fade the color
  if(fade < 0 && (!cur.red || !cur.green || !cur.blue)) {
    fade = 1;
  } else if(cur.red == orig.red || cur.green == orig.green || cur.blue == orig.blue) {
    fade = -1;
  }
  cur.red += fade;
  cur.blue += fade;
  cur.green += fade;
  Set_Palette_Register(0x0c, &cur);

  // Check for keyboard
  if(kbhit()) {
    done = 1;
  }
  Delay(1);
}

// reset back set video mode to 320x200 256 color mode

Set_Video_Mode(TEXT_MODE);

} // end main



