// T Y P E S //////////////////////////////////////////////////////////////////
typedef struct fizzler_typ
{
    struct sprite_typ fizzy;
    int i;
    int len;
} fizzler, *fizzler_ptr;

// My better versions of these things
void Better_Behind_Sprite(sprite_ptr sprite);

void Better_Erase_Sprite(sprite_ptr sprite);

void Better_Draw_Sprite(sprite_ptr sprite);


//functions I have written on day 4
void Better_Scale_Sprite(sprite_ptr dest, sprite_ptr src, int scale);
void Better_Fade();
void Copy_Sprite(sprite_ptr dest, sprite_ptr src);
void Sprite_Fizzle(sprite_ptr sprite);
void Sprite_Fizzle_Frame(fizzler_ptr f, sprite_ptr sprite);
void Print_Sprite_Frame(sprite_ptr sprite, int frame);
