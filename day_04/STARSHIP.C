// I N C L U D E S ////////////////////////////////////////////////////////////
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
#include <graph.h>   // microsoft's stuff if we need it

#include "graph3.h"  // the module from day 3
#include "graph4.h"  // the module from day 4
#include "better4.h" // my better functions

// D E F I N E S //////////////////////////////////////////////////////////////
#define RED1 0x04
#define RED2 0x0C
#define RED_GLOW 0x20

#define NUM_STARS  75


typedef struct star
{
    int x;
    int y;
    unsigned int plane;
} star;

// G L O B A L S //////////////////////////////////////////////////////////////
unsigned char star_color[] = {8, 7, 15};
int star_dx[] = {2, 4, 6};
int star_dy[] = {0, 0, 0};

star stars[NUM_STARS];

// P R O T O T Y P E S ////////////////////////////////////////////////////////
void init_stars();
void update_stars();

// F U N C T I O N S //////////////////////////////////////////////////////////
int main()
{
pcx_picture ship_pcx;
sprite ship;
RGB_color red1, red2;
RGB_color glow;
unsigned int frame_counter=0;
int dir=-1;
int fade_count=0;
int trajectory [4][3] = { {0,0,0},
                         {1,2,4},
			 {2,4,6},
			 {-1, -2, -4} };
int tx=2, ty=0;
int tcount=0;

// set video mode to 320x200 256 color mode
Set_Video_Mode(VGA256);

// load in the players imagery
PCX_Init((pcx_picture_ptr)&ship_pcx);
PCX_Load("starship.pcx", (pcx_picture_ptr)&ship_pcx,1);

// initialize sprite size and data structure
sprite_width  = 16;
sprite_height = 16;

// place the ship on the far right side of the screen
Sprite_Init((sprite_ptr)&ship,151,91,0,0,0,0);

// grab the ship from the PCX file
PCX_Grab_Bitmap((pcx_picture_ptr)&ship_pcx,(sprite_ptr)&ship,0,0,0);

//get rid of the PCX
PCX_Delete((pcx_picture_ptr)&ship_pcx);

//grab our reds
Get_Palette_Register(RED1, &red1);
Get_Palette_Register(RED2, &red2);
Get_Palette_Register(RED_GLOW, &glow);

init_stars();

while(!kbhit())
{
update_stars();
Draw_Sprite(&ship);

//handle strobing red1 and red2
if(frame_counter & 0x03) {
    Set_Palette_Register(RED1, &red1);
    Set_Palette_Register(RED2, &red2);
} else {
    Set_Palette_Register(RED1, &red2);
    Set_Palette_Register(RED2, &red1);
}

//do the glowing thing
if(glow.red) glow.red += dir;
if(glow.green) glow.green += dir;
if(glow.blue) glow.blue += dir;
Set_Palette_Register(RED_GLOW, &glow);
fade_count++;
if(fade_count == 25)
{
    fade_count = 0;
    dir = -dir;
}

//change star trajectory
tcount++;
if(tcount == 100)
{
    tx = rand()%4;
    ty = rand()%4;
    star_dx[0] = trajectory[tx][0];
    star_dx[1] = trajectory[tx][1];
    star_dx[2] = trajectory[tx][2];
    star_dy[0] = trajectory[ty][0];
    star_dy[1] = trajectory[ty][1];
    star_dy[2] = trajectory[ty][2];
    tcount = rand()%100;
}

frame_counter++;
Delay(1);
}

// go back to text mode
Set_Video_Mode(TEXT_MODE);
}


void init_stars()
{
int i;

for(i=0; i<NUM_STARS; i++) {
    stars[i].x = rand() % 320;
    stars[i].y = rand() % 200;
    stars[i].plane = rand() % 3;
}
}


void update_stars()
{
int i;
for(i=0; i<NUM_STARS; i++) {
    //erase the star
    Plot_Pixel_Fast(stars[i].x, stars[i].y, 0);

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
    Plot_Pixel_Fast(stars[i].x, stars[i].y, star_color[stars[i].plane]);
}
}
