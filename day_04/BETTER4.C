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

#include "graph3.h"
#include "graph4.h"
#include "better4.h"

void Better_Behind_Sprite(sprite_ptr sprite)
{
int old_sprite_width, old_sprite_height;

//preserve the sprite dimensions
old_sprite_width = sprite_width;
old_sprite_height = sprite_height;

//set the proper width and height
sprite_width = sprite->width;
sprite_height = sprite->height;

Behind_Sprite(sprite);

//restore the sprite dimensions
sprite_width = old_sprite_width;
sprite_height = old_sprite_height;
}


void Better_Erase_Sprite(sprite_ptr sprite)
{
int old_sprite_width, old_sprite_height;

//preserve the sprite dimensions
old_sprite_width = sprite_width;
old_sprite_height = sprite_height;

//set the proper width and height
sprite_width = sprite->width;
sprite_height = sprite->height;

Erase_Sprite(sprite);

//restore the sprite dimensions
sprite_width = old_sprite_width;
sprite_height = old_sprite_height;
}


void Better_Draw_Sprite(sprite_ptr sprite)
{
int old_sprite_width, old_sprite_height;

//preserve the sprite dimensions
old_sprite_width = sprite_width;
old_sprite_height = sprite_height;

//set the proper width and height
sprite_width = sprite->width;
sprite_height = sprite->height;

Draw_Sprite(sprite);

//restore the sprite dimensions
sprite_width = old_sprite_width;
sprite_height = old_sprite_height;
}


//functions I have written on day 4
void Better_Scale_Sprite(sprite_ptr dest, sprite_ptr src, int scale)
{
int sprite_frame;
int i,j;
int x,y;
int fx,fy;
unsigned char pixel;

// Initialize the destination sprite
Sprite_Init(dest,
            src->x,
            src->y,
	    src->anim_clock,
	    src->anim_speed,
	    src->motion_clock,
	    src->motion_speed);
dest->width = src->width * scale;
dest->height = src->height * scale;

//reallocate space for the background
_ffree(dest->background);
dest->background = (char far *)_fmalloc(dest->width * dest->height +1);

//copy the sprite frames
for(sprite_frame=0; sprite_frame < src->num_frames; sprite_frame++) {
    //allocate space for the frame
    dest->frames[sprite_frame] = (char far *)_fmalloc(dest->width * dest->height + 1);
    dest->num_frames++;

    //copy the frame, scaling the pixels as they go
    for(y=0; y<src->height; y++) {
	fy = y * scale;
        for(x=0; x<src->width; x++) {
	    pixel = src->frames[sprite_frame][y*src->width + x];
	    fx = x*scale;
	    for(i=0; i<scale; i++) {
	        for(j=0; j<scale; j++) {
		    dest->frames[sprite_frame][(fy+i) * dest->width + fx+j]=pixel;
	        }
	    }
        }
    }
}
}


void Better_Fade()
{
RGB_color color;  
int done;
int i;

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
}


void Copy_Sprite(sprite_ptr dest, sprite_ptr src)
{
//scale with factor 1 is practically a sprite copy
Better_Scale_Sprite(dest, src, 1);

//We just need to grab a few more fields
dest->x_old = src->x_old;
dest->y_old = src->y_old;
dest->curr_frame = src->curr_frame;
dest->state = src->state;
_fmemcpy(dest->background, src->background, dest->width * dest->height +1);
}


void Sprite_Fizzle_Frame(fizzler_ptr f, sprite_ptr sprite)
{
//produce a single frame of fizzle
int j;

//initialize
if(sprite) {
  //first we need to copy the sprite
  Copy_Sprite(&(f->fizzy), sprite);
  f->i=0;
  f->len = sprite->width * sprite->height + 1;
}else if(f->i >= 25) {
  //already fizzled
  return;
}

//and away we go!
for(j=f->i; j<f->len; j+=25) {
  f->fizzy.frames[f->fizzy.curr_frame][(j+rand())%f->len] = 0;
}
Better_Erase_Sprite(&(f->fizzy));
Better_Behind_Sprite(&(f->fizzy));
Better_Draw_Sprite(&(f->fizzy));

f->i += 1;
if(f->i==25) {
  Better_Erase_Sprite(&(f->fizzy));
  Sprite_Delete(&(f->fizzy));
}

}


void Sprite_Fizzle(sprite_ptr sprite)
{
int i;
fizzler f;

Sprite_Fizzle_Frame(&f, sprite);

for(i=1; i<25; i++){
  Sprite_Fizzle_Frame(&f, NULL);
  Delay(1);
}
}


void Print_Sprite_Frame(sprite_ptr sprite, int frame)
{
int i;

//print the color indexes for each non-zero part of the frame
for(i=0; i<sprite->width*sprite->height; i++) {
if(i%sprite->width== 0) { printf("\n"); }
if(sprite->frames[frame][i]) {
  printf("%02x ", sprite->frames[frame][i]);
} else {
  printf("   ");
}
}
}
