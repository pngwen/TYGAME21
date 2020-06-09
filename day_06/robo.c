
// I N C L U D E S ///////////////////////////////////////////////////////////

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

#include "graph3.h"  // include our graphics stuff
#include "graph4.h"
#include "graph6.h"

#define CELL_COLUMNS   10  // size of cell based matrix
#define CELL_ROWS      6

#define CELL_WIDTH     32  // width of a cell in pixels
#define CELL_HEIGHT    32  // height of a cell in pixels

#define NUM_SCREENS    6   // number of screens in game

#define ROBO_MOVE      8   // speed that the player moves at

#define NUM_STARS  75

// G L O B A L S  ////////////////////////////////////////////////////////////
typedef struct star
{
    int x;
    int y;
    unsigned int plane;
    unsigned char back;
} star;

pcx_picture imagery_pcx,   // used to load in the imagery for robopunk
            intro_pcx;     // the intro screen

sprite back_cells,  // background cells sprite
       robopunk;    // robopunk

// use an array of 2-D matrices to hold the screens

char **universe[NUM_SCREENS] = {NULL,NULL,NULL,NULL,NULL,NULL};

// here is screen 1, note: it's 10x6 cells where each cell is represented
// by an ASCII character (makes it easier to draw each screen by hand).
// later the ASCII characters will be translated to bitmap id's so that
// the screen image can be drawn

char *screen_1[CELL_ROWS] = {"           ",
                             "##*###*####",
                             "###########",
                             "<==========",
                             "######:####",
                             "####<=;=>##"};


char *screen_2[CELL_ROWS] = {"      ###  ",
                             "      #:#  ",
                             "#######:###",
                             "=======;===",
                             "#<==>######",
                             "###########"};


char *screen_3[CELL_ROWS] = {"      ##<=>",
                             "  #*##<==>#",
                             "####*######",
                             "===========",
                             "###########",
                             "###########"};



char *screen_4[CELL_ROWS] = {"###        ",
                             "#<=>##     ",
                             "####<==>###",
                             "===========",
                             "###########",
                             "#<==>######"};



char *screen_5[CELL_ROWS] = {"   #<=>#   ",
                             " #:#***#:##",
                             "##:#####:##",
                             "==;=====;==",
                             "###########",
                             "###########"};


char *screen_6[CELL_ROWS] = {"           ",
                             "##         ",
                             "#*#*##     ",
                             "========>  ",
                             "#########  ",
                             "#########  "};

unsigned char star_color[] = {8, 7, 15};
int star_dx[] = {2, 4, 6};
int star_dy[] = {0, 0, 0};
star stars[NUM_STARS];

// P R O T O T Y P E S ////////////////////////////////////////////////////////
void init_stars();
void update_stars();

// F U N C T I O N S /////////////////////////////////////////////////////////

void Draw_Screen(char **screen)
{
// this function draws a screen by using the data in the universe array
// each element in the universe array is a 2-D matrix of cells, these
// cells are ASCII characters that represent the requested bitmap that
// shoulf be placed in the cell location

char *curr_row;

int index_x, index_y, cell_number;

// translation table for screen database used to convert the ASCII
// characters into id numbers

static char back_cell_lookup[] =

   { 0,0, 0, 4,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,7,1,2,3,0,
// SP  !  " #  $ % & ' ( ) * + , - . / 0 1 2 3 4 5 6 7 8 9 : ; < = > ?

    0,0,0,0,0,0,0,0,0,0, 0,0,0,0, 0,0,0,0,0,0,0,0 ,0,0,0,0,0,0 ,0, 0,0,0};
//  @ A B C D E F G H I  J K L M  N O P Q R S T U  V W X Y Z [  \  ] ^ _

// clear out the double buffer

Fill_Double_Buffer(0);

// now draw the screen row by row

for (index_y = 0; index_y<CELL_ROWS; index_y++)
    {

    // get the current row for speed

    curr_row = screen[index_y];

    // do the row

    for (index_x = 0; index_x<CELL_COLUMNS; index_x++)
        {
        // extract cell out of data structure and blit it onto screen

        cell_number = back_cell_lookup[curr_row[index_x]-32];

        // compute the screen x and y

        back_cells.x = index_x * sprite_width;
        back_cells.y = index_y * sprite_height;

        // figure out which bitmap to draw

        back_cells.curr_frame = cell_number;

        // draw the bitmap

        Draw_Sprite_DB((sprite_ptr)&back_cells);

        } // end for index_x

    } // end for index_y

} // end Draw_Screen

//////////////////////////////////////////////////////////////////////////////

void Rotate_Lights(void)
{

// this function useds color rotation to move the walkway lights
// three color registers are used.
// note: this function has static variables which track timing parameters
// and also if the funtion has been entered yet.

static int clock=0,entered_yet=0;// used for timing, note: they are static!

RGB_color color,
          color_1,
          color_2,
          color_3;

// this function blinks the running lights on the walkway

if (!entered_yet)
   {

   // reset the palette registers 96,97,98 to red,
   // black, black

   color.red   = 255;
   color.green = 0;
   color.blue  = 0;

   Set_Palette_Register(96,(RGB_color_ptr)&color);

   color.red = color.green = color.blue = 0;

   Set_Palette_Register(97,(RGB_color_ptr)&color);
   Set_Palette_Register(98,(RGB_color_ptr)&color);

   // system has initialized, so flag it

   entered_yet=1;

   } // end if first time into function

// try and rotate the light colors i.e. color rotation

   if (++clock==3)  // is it time to rotate
      {
      // get the colors

      Get_Palette_Register(96,(RGB_color_ptr)&color_1);
      Get_Palette_Register(97,(RGB_color_ptr)&color_2);
      Get_Palette_Register(98,(RGB_color_ptr)&color_3);

      // set the colors

      Set_Palette_Register(97,(RGB_color_ptr)&color_1);
      Set_Palette_Register(98,(RGB_color_ptr)&color_2);
      Set_Palette_Register(96,(RGB_color_ptr)&color_3);

      // reset the clock

      clock=0;

      } // end if time to rotate

} // end Rotate_Lights

///////////////////////////////////////////////////////////////////////////////


// M A I N ////////////////////////////////////////////////////////////////////

void main(void)
{

int index,
    curr_screen=0,
    done=0;
int dy=0;

// S E C T I O N   1  /////////////////////////////////////////////////////////

// set video mode to 320x200 256 color mode
Set_Video_Mode(VGA256);

// create a double buffer

if (!Create_Double_Buffer(SCREEN_HEIGHT))
   {
   printf("\nNot enough memory to create double buffer.");

   } // end if

// S E C T I O N   2  /////////////////////////////////////////////////////////

// load intro screen and display for a few secs.

PCX_Init((pcx_picture_ptr)&intro_pcx);

PCX_Load("roboint.pcx", (pcx_picture_ptr)&intro_pcx,1);

PCX_Show_Buffer((pcx_picture_ptr)&intro_pcx);

// let user see it

Delay(50);

PCX_Delete((pcx_picture_ptr)&intro_pcx);

// S E C T I O N   3  /////////////////////////////////////////////////////////

// load in background and animation cells

PCX_Init((pcx_picture_ptr)&imagery_pcx);

PCX_Load("robopunk.pcx", (pcx_picture_ptr)&imagery_pcx,1);

// initialize sprite size

sprite_width  = 32;
sprite_height = 32;

// create a sprite for robopunk

Sprite_Init((sprite_ptr)&robopunk,0,0,0,0,0,0);

// create a sprite to hold the background cells

Sprite_Init((sprite_ptr)&back_cells,0,0,0,0,0,0);

// extract animation cells for robopunk

PCX_Grab_Bitmap((pcx_picture_ptr)&imagery_pcx,(sprite_ptr)&robopunk,0,3,0);
PCX_Grab_Bitmap((pcx_picture_ptr)&imagery_pcx,(sprite_ptr)&robopunk,1,5,0);
PCX_Grab_Bitmap((pcx_picture_ptr)&imagery_pcx,(sprite_ptr)&robopunk,2,4,0);
PCX_Grab_Bitmap((pcx_picture_ptr)&imagery_pcx,(sprite_ptr)&robopunk,3,5,0);
PCX_Grab_Bitmap((pcx_picture_ptr)&imagery_pcx,(sprite_ptr)&robopunk,4,6,0);
PCX_Grab_Bitmap((pcx_picture_ptr)&imagery_pcx,(sprite_ptr)&robopunk,5,1,0);
PCX_Grab_Bitmap((pcx_picture_ptr)&imagery_pcx,(sprite_ptr)&robopunk,6,2,0);
PCX_Grab_Bitmap((pcx_picture_ptr)&imagery_pcx,(sprite_ptr)&robopunk,7,1,0);
PCX_Grab_Bitmap((pcx_picture_ptr)&imagery_pcx,(sprite_ptr)&robopunk,8,0,0);


// extract background cells

for (index=0; index<8; index++)
    {

    PCX_Grab_Bitmap((pcx_picture_ptr)&imagery_pcx,(sprite_ptr)&back_cells,index,index,1);

    } // end for index

// done with pcx file so obliterate it

PCX_Delete((pcx_picture_ptr)&imagery_pcx);

// S E C T I O N   4  /////////////////////////////////////////////////////////

// set up universe data structure

universe[0] = (char **)screen_1;
universe[1] = (char **)screen_2;
universe[2] = (char **)screen_3;
universe[3] = (char **)screen_4;
universe[4] = (char **)screen_5;
universe[5] = (char **)screen_6;

Draw_Screen((char **)universe[curr_screen]);
Show_Double_Buffer(double_buffer);

init_stars();
// place robopunk

robopunk.x = 160;
robopunk.y = 74;

robopunk.curr_frame = 0;

// scan background under robopunk

Behind_Sprite_DB((sprite_ptr)&robopunk);

// S E C T I O N   5  /////////////////////////////////////////////////////////

// main event loop

while(!done)
     {

// S E C T I O N   6  /////////////////////////////////////////////////////////

     // erase robopunk

     Erase_Sprite_DB((sprite_ptr)&robopunk);

     update_stars();

     //move vertically (if needed)
     if(dy) {
     	robopunk.y += dy;
	dy+=1;
	if(robopunk.y >= 199-robopunk.height) {
	    done=1;
	    continue;
	}
     }

     // test if user has hit key

     if (kbhit())
        {
        // get the key

// S E C T I O N   7  /////////////////////////////////////////////////////////

        switch(getch())
              {

              case 'a': // move the player left
                      {
                      // advance the animation frame and move player

                      // test if player is moving right, if so
                      // show player turning before moving

                      if (robopunk.curr_frame > 0 && robopunk.curr_frame < 5)
                         {
                         robopunk.curr_frame = 0;
                         } // end if player going right
                      else
                      if (robopunk.curr_frame == 0 )
                         robopunk.curr_frame = 5;
                      else
                         {
                         // player is already in leftward motion so continue

                         if (++robopunk.curr_frame > 8)
                            robopunk.curr_frame = 5;

                         // move player to left

                         robopunk.x-=ROBO_MOVE;

                         // test if edge was hit

                         if (robopunk.x < 8)
                            {
                            // test if there is another screen to the left

                            if (curr_screen==0)
                               {
                               robopunk.x += ROBO_MOVE;
                               } // end if already at end of universe
                            else
                               {
                               // warp robopunk to other edge of screen
                               // and change screens

                               robopunk.x = SCREEN_WIDTH - 40;

                               // scroll to next screen to the left

                               curr_screen--;

                               Draw_Screen((char **)universe[curr_screen]);

                               } // end else move a screen to the left

                            } // end if hit left edge of screen

                         } // end else

                      } break;


              case 's': // move the player right
                      {

                      // advance the animation frame and move player

                      // test if player is moving left, if so
                      // show player turning before moving

                      if (robopunk.curr_frame > 4)
                         {
                         robopunk.curr_frame = 0;
                         } // end if player going right
                      else
                      if (robopunk.curr_frame == 0 )
                         robopunk.curr_frame =1;
                      else
                         {
                         // player is already in rightward motion so continue

                         if (++robopunk.curr_frame > 4)
                            robopunk.curr_frame = 1;

                         // move player to right

                         robopunk.x+=ROBO_MOVE;

                         // test if edge was hit

                         if (robopunk.x > SCREEN_WIDTH-40)
                            {
                            // test if there is another screen to the left

                            if (curr_screen==5)
                               {
                               robopunk.x -= ROBO_MOVE;
			       dy=4;
                               } // end if already at end of universe
                            else
                               {
                               // warp robopunk to other edge of screen
                               // and change screens

                               robopunk.x = 8;

                               // scroll to next screen to the right

                               curr_screen++;

                               Draw_Screen((char **)universe[curr_screen]);

                               } // end else move a screen to the right

                            } // end if hit right edge of screen

                         } // end else

                      } break;

              case 'q': // exit the demo
                      {
                      done=1;

                      } break;

              default:break;

              } // end switch


        } // end if keyboard hit

// S E C T I O N   8  /////////////////////////////////////////////////////////

     // scan background under robopunk

     Behind_Sprite_DB((sprite_ptr)&robopunk);

     // draw him

     Draw_Sprite_DB((sprite_ptr)&robopunk);

     // do any background animation

// S E C T I O N   9  /////////////////////////////////////////////////////////

     // move the walkway lights

     Rotate_Lights();

     // show the double buffer

     Show_Double_Buffer(double_buffer);

     // wait a bit

     Delay(1);

     } // end while

// S E C T I O N   10 /////////////////////////////////////////////////////////

// use one of screen fx as exit

Disolve_Color(0x0c);
Fade_Lights();

// reset the video mode back to text

Set_Video_Mode(TEXT_MODE);

// free the double buffer

Delete_Double_Buffer();

} // end main



void init_stars()
{
int i;
int offset;

offset = (stars[i].y << 8) + (stars[i].y << 6) + stars[i].x;
for(i=0; i<NUM_STARS; i++) {
    stars[i].x = rand() % 320;
    stars[i].y = rand() % 200;
    stars[i].plane = rand() % 3;
    stars[i].back = double_buffer[offset];
}
}


void update_stars()
{
int i;
int offset;

for(i=0; i<NUM_STARS; i++) {
    //erase the star (if it was drawn)
    if(!stars[i].back)
      Plot_Pixel_Fast_DB(stars[i].x, stars[i].y, 0);

    //move the star
    stars[i].x += star_dx[stars[i].plane];
    if(stars[i].x < 0) {
	stars[i].x += 320;
    } else if(stars[i].x >=320) {
	stars[i].x -= 320;
    }
    stars[i].y += star_dy[stars[i].plane];
    if(stars[i].y < 0) {
	stars[i].y += 200;
    } else if(stars[i].y >=200) {
	stars[i].y -= 200;
    }

    //draw the star
    offset = (stars[i].y << 8) + (stars[i].y << 6) + stars[i].x;
    stars[i].back = double_buffer[offset];
    if(!stars[i].back)
      Plot_Pixel_Fast_DB(stars[i].x, stars[i].y, star_color[stars[i].plane]);
}
}


