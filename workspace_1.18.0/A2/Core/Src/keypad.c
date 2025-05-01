/* USER CODE BEGIN Header */
/*******************************************************************************
* EE 329 A2 KEYPAD INTERFACE
*******************************************************************************
* @file : keypad.c
* @brief : keypad configuration and debounced detection of keypresses
* project : EE 329 S'25 Assignment 2
* authors : Brandon Wu, Bryan Lew
* version : 1
* date : 4/16/25
* compiler : STM32CubeIDE v.1.18.0
* target : NUCLEO-L4A6ZG
* clocks : 4 MHz MSI to AHB2
* @attention : (c) 2025 STMicroelectronics. All rights reserved.
*******************************************************************************
. . .
* 45678-1-2345678-2-2345678-3-2345678-4-2345678-5-2345678-6-2345678-7-234567 */
/* USER CODE END Header */

#include "keypad.h"
#include "math.h"
#include "stdio.h"

void Keypad_Config(void)  {

	  //INPUT PORT D (rows)
	  RCC->AHB2ENR   |=  (RCC_AHB2ENR_GPIODEN | RCC_AHB2ENR_GPIOCEN); //Enable clock for GPIO Port D & C
	  ROW_PORT->MODER &= ~(GPIO_MODER_MODE3 | GPIO_MODER_MODE4 | GPIO_MODER_MODE5 | GPIO_MODER_MODE6);
	  ROW_PORT->OTYPER  &= ~(GPIO_OTYPER_OT3 | GPIO_OTYPER_OT4 | GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6); //Configure pins as push-pull

	  //ROW_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD3 | GPIO_PUPDR_PUPD4 | GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6);
	  //ROW_PORT->PUPDR   |= ~(GPIO_PUPDR_PUPD3_1 | GPIO_PUPDR_PUPD4_1 | GPIO_PUPDR_PUPD5_1 | GPIO_PUPDR_PUPD6_1);

	  ROW_PORT->PUPDR = (2U << (6*2) | 2U << (5*2) | 2U << (4*2) | 2U << (3*2)); //Enable pull-down resistors for ROW pins
	  ROW_PORT->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEED3_Pos) | //Set High output speed for pins
	                      (3 << GPIO_OSPEEDR_OSPEED4_Pos) |
	     	   	    	  (3 << GPIO_OSPEEDR_OSPEED5_Pos) |
	   	     	          (3 << GPIO_OSPEEDR_OSPEED6_Pos)); //Reset aka clear the pins

	  //OUTPUT PORT C (cols)
	  COL_PORT->MODER   &= ~(GPIO_MODER_MODE0 | GPIO_MODER_MODE1 | GPIO_MODER_MODE2 | GPIO_MODER_MODE3); //Clear any preexisting mode settings for Pins
	  COL_PORT->MODER   |=  (GPIO_MODER_MODE0_0 | GPIO_MODER_MODE1_0 | GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0); //Set pins as output (mode 0)
	  COL_PORT->OTYPER  &= ~(GPIO_OTYPER_OT0 | GPIO_OTYPER_OT1 | GPIO_OTYPER_OT2 | GPIO_OTYPER_OT3); //Configure pins as push-pull

	  //COL_PORT->PUPDR   &= ~(GPIO_PUPDR_PUPD0 | GPIO_PUPDR_PUPD1 | GPIO_PUPDR_PUPD2 | GPIO_PUPDR_PUPD3);
	  COL_PORT->PUPDR = (2u << (0*2) | 2u << (1*2) | 2u << (2*2) | 2u << (3*2)); //enable pull-down resistors for COL pins

	  COL_PORT->OSPEEDR |=  ((3 << GPIO_OSPEEDR_OSPEED0_Pos) | //Set High output speed for pins
						  (3 << GPIO_OSPEEDR_OSPEED1_Pos) |
						  (3 << GPIO_OSPEEDR_OSPEED2_Pos) |
						  (3 << GPIO_OSPEEDR_OSPEED3_Pos)); //Reset aka clear the pins
	  GPIOC->BRR = (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3); // preset pins to 0
}

// -----------------------------------------------------------------------------
int Keypad_IsAnyKeyPressed(void) {
// drive all COLUMNS HI; see if any ROWS are HI
// return true if a key is pressed, false if not
// currently no debounce here - just looking for a key twitch
   COL_PORT->BSRR = COL_PINS;         	      // set all columns HI
   for ( uint16_t idx=0; idx<SETTLE; idx++ )   	// let it settle
      ;

   //Debounce Code
   if ((ROW_PORT->IDR & ROW_PINS) != 0 ) {       // got a keypress!
   	  for (int loopIdx = 0; loopIdx < 100000; loopIdx++){     // delay
   	  	  }
	  if ((ROW_PORT->IDR & ROW_PINS) != 0 ) {      //if keypress still on, return true
		  return( TRUE );
	   }
	   else {
		  return( FALSE );                          // nope.
	   }
   }
}
// -----------------------------------------------------------------------------
int Keypad_WhichKeyIsPressed(void) {
// detect and encode a pressed key at {row,col}
// assumes a previous call to Keypad_IsAnyKeyPressed() returned TRUE
// verifies the Keypad_IsAnyKeyPressed() result (no debounce here),
// determines which key is pressed and returns the encoded key ID

   int8_t iRow=0, iCol=0, iKey=0;  // keypad row & col index, key ID result
   int8_t bGotKey = 0;             // bool for keypress, 0 = no press
   printf("button pressed %d\n", 1);

   COL_PORT->BSRR = COL_PINS;                       	 // set all columns HI
   for ( iRow = 0; iRow < NUM_ROWS; iRow++ ) {      	 // check all ROWS
      if ( ROW_PORT->IDR & (BIT0 << iRow) ) {      	 // keypress in iRow!!
         COL_PORT->BRR = ( COL_PINS );            	 // set all cols LO
         for ( iCol = 0; iCol < NUM_COLS; iCol++ ) {   // 1 col at a time
            COL_PORT->BSRR = ( 0x01 << iCol );     // set this col HI
            if ( ROW_PORT->IDR & (BIT0 << iRow) ) {    // keypress in iCol!!
               bGotKey = 1;
               break;                                  // exit for iCol loop
            }
         }
         if ( bGotKey )
            break;
      }
   }
   //	encode {iRow,iCol} into LED word : row 1-3 : numeric, ‘1’-’9’
   //	                                   row 4   : ‘*’=10, ‘0’=15, ‘#’=12
   //                                    no press: send NO_KEYPRESS
   if ( bGotKey ) {
     iKey = ( iRow * NUM_COLS ) + iCol + 1;  // handle numeric keys ...

     //translate raw keypad value to actual characters on keypad
     switch (iKey) {
     	 case 4:
     		 iKey = 10;
     		 break;
     	 case 5:
     		 iKey = 4;
     		 break;
     	 case 6:
     		iKey = 5;
     		break;
     	 case 7:
     		iKey = 6;
     		break;
     	 case 8:
     		iKey = 11;
     		break;
     	 case 9:
     		iKey = 7;
     		break;
     	 case 10:
     		iKey = 8;
     		break;
     	 case 11:
     		iKey = 9;
     		break;
     	 case 12:
     		iKey = 12;
     		break;
     	 case 13:
     		iKey = 14;
     		break;
     	 case 14:
     		iKey = 0;
     		break;
     	 case 15:
     		iKey = 15;
     		break;
     	 case 16:
     		iKey = 13;
     		break;
     }

     //printf("iKey %d\n", iKey);
 	 return( iKey );                         // return encoded keypress
   }
   return( NO_KEYPRESS );                     // unable to verify keypress
}
