// This is a crude drawing program which allows the user to select
// colors and draw with the arrow keys
// Author: Robert Lowe
#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include "graph3.h" 

// G L O B A L S //////////////////////////////////////////////////////////////
unsigned char color[] = { 0x00, //black
                          0x0f, //white
			  0x28, //red
			  0x2f, //green
			  0x20, //blue
			  0x2a, //orange
			  0x2c, //yellow
			  0x23, //purple
			  0x06, //brown
			  0x19  //gray
                        };
int cur_color = 0x0f;	//current color
int cx, cy; 		//cursor position
int done = 0;		//set to true when it is time to exit

// D E F I N E S /////////////////////////////////////////////////////////////

// begin keyboard stuff
#define KEYBOARD_INT    0x09        // the keyboard interrupt number
#define KEY_BUFFER      0x60        // the buffer port
#define KEY_CONTROL     0x61        // the controller port
#define INT_CONTROL     0x20        // the interrupt controller
#define ESC_KEY		129
#define ONE_KEY		2
#define ZERO_KEY	11
#define KEY_UP		72
#define KEY_DOWN	80
#define KEY_LEFT	75
#define KEY_RIGHT	77

// P R O T O T Y P E S //////////////////////////////////////////////////////
void _interrupt _far New_Key_Int(void);
void draw_swatch(int x, int y, unsigned char color);


// F U N C T I O N S //////////////////////////////////////////////////////////
void main()
{
int x, i;
void (_interrupt _far *Old_Isr)();  // holds old keyboard interrupt handler

// install our ISR
Old_Isr = _dos_getvect(KEYBOARD_INT);
_dos_setvect(KEYBOARD_INT, New_Key_Int);

// set video mode to 320x200 256 color mode
Set_Video_Mode(VGA256);
cx = 159;
cy = 99;

//draw the color keys
i=1;
x=0;
for(i=1; i<=10; i++, x+= 32)
{
    draw_swatch(x, 192, color[i%10]);
}
Blit_String(0, 192, 0x07, "  1   2   3   4   5   6   7   8   9   0", 1);

while(!done) {
    Delay(1);
}

// replace old ISR
_dos_setvect(KEYBOARD_INT, Old_Isr);

// reset back set video mode to 320x200 256 color mode
Set_Video_Mode(TEXT_MODE);
}


void _interrupt _far New_Key_Int(void)
{
int raw_key;

// I'm in the mood for some inline!

_asm
   {
   sti                    ; re-enable interrupts
   in al, KEY_BUFFER      ; get the key that was pressed
   xor ah,ah              ; zero out upper 8 bits of AX
   mov raw_key, ax        ; store the key in global
   in al, KEY_CONTROL     ; set the control register
   or al, 82h             ; set the proper bits to reset the FF
   out KEY_CONTROL,al     ; send the new data back to the control register
   and al,7fh
   out KEY_CONTROL,al     ; complete the reset
   mov al,20h
   out INT_CONTROL,al     ; re-enable interrupts
                          ; when this baby hits 88 mph, your gonna see
                          ; some serious @#@#$%

   } // end inline assembly

//respond to the key presses
if(raw_key == ESC_KEY) {
    done = 1;
} else if(raw_key >= ONE_KEY && raw_key <= ZERO_KEY) {
    //convert to index
    raw_key -= ONE_KEY;
    raw_key++;
    if(raw_key == 10) { raw_key = 0; }

    //select the color
    cur_color = color[raw_key];
} else if( raw_key == KEY_UP) {
    cy--;
} else if( raw_key == KEY_DOWN) {
    cy++;
} else if( raw_key == KEY_LEFT) {
    cx--;
} else if( raw_key == KEY_RIGHT) {
    cx++;
}

//draw after each keypress
Plot_Pixel_Fast(cx, cy, cur_color);
} // end New_Key_Int


void draw_swatch(int x, int y, unsigned char color)
{
    int right=x+32;
    int bottom=y+8;
    int py;

    for(x; x<right; x++) {
	for(py=y; py<bottom; py++) {
	    Plot_Pixel_Fast(x, py, color);
	}
    }
}
