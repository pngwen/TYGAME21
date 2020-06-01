
// I N C L U D E S ///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "graph3.h" // this is all we need to include so that the program
                    // knows all the #defines, structures, prototypes etc.

// M A I N ///////////////////////////////////////////////////////////////////

void main(void)
{
unsigned long int count;

//do fast pixel load
printf("Fast Mode");
getch();

// set video mode to 320x200 256 color mode
Set_Video_Mode(VGA256);

for(count=0; count < 1000000; count++) {
    Plot_Pixel_Fast(159,99,0x0C);
}
Set_Video_Mode(TEXT_MODE);

//do normal pixel load
printf("normal Mode");
getch();

// set video mode to 320x200 256 color mode
Set_Video_Mode(VGA256);

for(count=0; count < 1000000; count++) {
    Plot_Pixel(159,99,0x0C);
}
Set_Video_Mode(TEXT_MODE);

} // end main



