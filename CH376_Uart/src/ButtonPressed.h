#ifndef ButtonPressed
#define ButtonPressed
#include "ch376.h"

#define NumberOfButtons 2

char KeyPressed(int buttonNumber, unsigned char pinofButton, unsigned char portBit, int confidenceLevel);

char Pressed[NumberOfButtons];
int Pressed_Confidence_Level[NumberOfButtons]; 
int Released_Confidence_Level[NumberOfButtons]; 
	
char KeyPressed(int buttonNumber, unsigned char pinofButton, unsigned char portBit, int confidenceLevel)
{
    
    if (bit_is_clear(pinofButton, portBit))
    {
         Pressed_Confidence_Level[buttonNumber] ++; 
         Released_Confidence_Level[buttonNumber] = 0; 
         if (Pressed_Confidence_Level[buttonNumber] >confidenceLevel) 
         {
	         if (Pressed[buttonNumber] == 0)
	         {
		          Pressed[buttonNumber] = 1;
		          return 1;
	         }
	         Pressed_Confidence_Level[buttonNumber] = 0;
         }
    }
    else
    {
         Released_Confidence_Level[buttonNumber] ++; 
         Pressed_Confidence_Level[buttonNumber] = 0; 
         if (Released_Confidence_Level[buttonNumber] >confidenceLevel)
         {
	          Pressed [buttonNumber]= 0;
	          Released_Confidence_Level[buttonNumber] = 0;
         } 
    }
	return 0;
}
#endif
