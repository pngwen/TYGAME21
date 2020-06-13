#include <stdio.h>
#include <string.h>
#include "graph3.h"
#include "graph4.h"
#include "better4.h"


int main()
{
char fname[20];
int r,c;  //row and column index
sprite s;
pcx_picture p;

//get the file
printf("File: ");
fgets(fname, 19, stdin);
fname[strlen(fname)-1]='\0';

//get sprite width
printf("Width: ");
scanf("%d", &sprite_width);

//get sprite height
printf("Height: ");
scanf("%d", &sprite_height);

//get the row and column
printf("Row Index: ");
scanf("%d", &r);
printf("Column Index: ");
scanf("%d", &c);

// load in the players imagery
PCX_Init((pcx_picture_ptr)&p);
PCX_Load(fname, (pcx_picture_ptr)&p,0);

//load the sprite
Sprite_Init((sprite_ptr)&s,302,91,0,0,0,0);

// grab the ship from the PCX file
PCX_Grab_Bitmap((pcx_picture_ptr)&p,(sprite_ptr)&s,0,c,r);

//print the sprite info
Print_Sprite_Frame(&s, 0);

//cleanup
PCX_Delete(&p);
Sprite_Delete(&s);
}
