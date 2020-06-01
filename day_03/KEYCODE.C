// This program will just echo keyboard event codes
#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <bios.h>
#include "graph3.h"

// D E F I N E S /////////////////////////////////////////////////////////////

// begin keyboard stuff
#define KEYBOARD_INT    0x09        // the keyboard interrupt number
#define KEY_BUFFER      0x60        // the buffer port
#define KEY_CONTROL     0x61        // the controller port
#define INT_CONTROL     0x20        // the interrupt controller

int done=0;

// P R O T O T Y P E S //////////////////////////////////////////////////////
void _interrupt _far New_Key_Int(void);

// F U N C T I O N S //////////////////////////////////////////////////////////
void main()
{
void (_interrupt _far *Old_Isr)();  // holds old keyboard interrupt handler

// install our ISR
Old_Isr = _dos_getvect(KEYBOARD_INT);
_dos_setvect(KEYBOARD_INT, New_Key_Int);

//display a friendly message
printf("Press and Release Some Keys\n");

while(!done) {
    Delay(1);
}

// replace old ISR
_dos_setvect(KEYBOARD_INT, Old_Isr);
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

// now for some C to update the arrow state table
printf("%d\n", raw_key);

//terminate if we have just releaste ESC
if(raw_key == 129) {
    done = 1;
}
} // end New_Key_Int
