// This is a little utility program to display the VGA Pallette
// Author: Robert Lowe <pngwen@acm.org>
// Created: June 1, 2020
#include <conio.h>
#include "graph3.h"

//Draw an 8x16 color swatch
void draw_swatch(int x, int y, unsigned char color);

void main()
{
    const char *row_label[] = { "0x00", "0x10", "0x20", "0x30", "0x40",
	                        "0x50", "0x60", "0x70", "0x80", "0x90",
			        "0xA0", "0xB0", "0xC0", "0xD0", "0xE0",
				"0xF0" };
    const char *col_label[] = { "0", "1", "2", "3", "4", "5", "6", "7",
	                        "8", "9", "A", "B", "C", "D", "E", "F" };
    int r,c;  //row and column index
    int x,y;  //x,y coordinate

    // set video mode to 320x200 256 color mode
    Set_Video_Mode(VGA256);

    /*
     * The layout we wish to achieve is this:
     *     0 1 2 3 4 5 6 7 8 9 A B C D E F
     * 0x00
     * 0x10
     * 0x20
     * 0x30
     * 0x40
     * 0x50
     * 0x70
     * 0x80   Colors Go Here 
     * 0x90       each color space is 16w x 8h
     * 0xA0
     * 0xB0
     * 0xC0
     * 0xD0
     * 0xE0
     * 0xF0
     * Rows: 17 * 8 = 136 pixels
     * Cols: 36 * 8 = 288 pixels
     * Starting row = 100 - (136/2) - 1 = 31
     * Starig Col = 160 - (288/2) - 1 = 15
     */

    //Draw the palette label
    Blit_String(100,8,0x07,"DEFAULT VGA PALETTE",1);

    //Draw the col headers
    x=15+32+4;
    y=31;
    for(c=0; c<16; c++, x+=16) {
	Blit_String(x, y, 0x07, col_label[c], 1);
    }
    y+=8;

    //Draw the rows
    for(r=0; r<16; r++, y+=8) {
	x=15;
	Blit_String(x,y,0x07, row_label[r], 1);
	x+=32;
	for(c=0; c<16; c++, x+=16){
	    draw_swatch(x,y, (r<<4) + c);
	}
    }

    //print exit instructions
    Blit_String(0,192,0x07, "press any key to exit", 1);

    //wait for a key press
    while(!kbhit()) {}


    // reset back set video mode to 320x200 256 color mode
    Set_Video_Mode(TEXT_MODE);
}


void draw_swatch(int x, int y, unsigned char color)
{
    int right=x+16;
    int bottom=y+8;
    int py;

    for(x; x<right; x++) {
	for(py=y; py<bottom; py++) {
	    Plot_Pixel_Fast(x, py, color);
	}
    }
}
