// I N C L U D E S ///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#include "graph3.h" // this is all we need to include so that the program
                    // knows all the #defines, structures, prototypes etc.

// P R O T O T Y P E S ///////////////////////////////////////////////////////
void Blit2_Char(int xc,int yc,char c,int color1, int color2, int trans_flag);
void Blit2_String(int x,int y,int color1, int color2, char *string,int trans_flag);

// M A I N ///////////////////////////////////////////////////////////////////
int main()
{

// set video mode to 320x200 256 color mode
Set_Video_Mode(VGA256);

//draw a string
Blit2_String(42, 99, 0x07, 0x0c, "Hello, World", 1);

// wait for a key to be pressed
while(!kbhit()) { }

Set_Video_Mode(TEXT_MODE);
}

// F U N C T I O N S /////////////////////////////////////////////////////////
void Blit2_Char(int xc,int yc,char c,int color1, int color2, int trans_flag)
{
// this function uses the rom 8x8 character set to blit a character on the
// video screen, notice the trick used to extract bits out of each character
// byte that comprises a line
int color = color1;

int offset,x,y;
char far *work_char;
unsigned char bit_mask = 0x80;

// compute starting offset in rom character lookup table

work_char = rom_char_set + c * CHAR_HEIGHT;

// compute offset of character in video buffer

offset = (yc << 8) + (yc << 6) + xc;

for (y=0; y<CHAR_HEIGHT; y++)
    {
    //select the color
    if(y%2) {
	color = color1;
    } else {
	color = color2;
    }
    // reset bit mask
    bit_mask = 0x80;

    for (x=0; x<CHAR_WIDTH; x++)
        {
        // test for transparent pixel i.e. 0, if not transparent then draw

        if ((*work_char & bit_mask))
             video_buffer[offset+x] = color;

        else if (!trans_flag)  // takes care of transparency
            video_buffer[offset+x] = 0;

        // shift bit mask

        bit_mask = (bit_mask>>1);

        } // end for x

    // move to next line in video buffer and in rom character data area

    offset      += SCREEN_WIDTH;
    work_char++;

    } // end for y

} // end Blit_Char

//////////////////////////////////////////////////////////////////////////////

void Blit2_String(int x,int y,int color1, int color2, char *string,int trans_flag)
{
// this function blits an entire string on the screen with fixed spacing
// between each character.  it calls blit_char.

int index;

for (index=0; string[index]!=0; index++)
     {

     Blit2_Char(x+(index<<3),y,string[index],color1,color2,trans_flag);

     } /* end while */

} /* end Blit_String */
