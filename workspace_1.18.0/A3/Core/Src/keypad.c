/*
 * keypad.c
 *
 *  Created on: Apr 14, 2025
 *      Author: Jesus
 */
#include "stm32l4xx.h"
#include "keypad.h"

#define COL_PORT COLUMNPORT
#define COL_PINS COLUMN_PINS
#define ROW_PORT ROWPORT
#define NUM_COLS NUM_COLUMNS



void Keypad_Config(void)
{

	//enable GPIO clock for PORT D
	RCC->AHB2ENR |= (RCC_AHB2ENR_GPIODEN);

	//configure port D pins as outputs
	//Each GPIO pin is 2 bits
	// MODER = controls mode of GPIO pin to 00(input) or 01(output) or 10(alternate FN) or 11(analog)
	// << shift left
	KeyPadPort->MODER &= ~(							// clearing all bits to 00 or all inputs
							GPIO_MODER_MODE0 |
							GPIO_MODER_MODE1 |
							GPIO_MODER_MODE2 |
							GPIO_MODER_MODE3 |
							GPIO_MODER_MODE4 |
							GPIO_MODER_MODE5 |
							GPIO_MODER_MODE6);

	COLUMNPORT->MODER |= (							// setting  as outputs (01)
							GPIO_MODER_MODE4_0 |
							GPIO_MODER_MODE5_0 |
							GPIO_MODER_MODE6_0);

	COLUMNPORT	->OTYPER &= ~(
								GPIO_OTYPER_OT4 |
								GPIO_OTYPER_OT5 |
								GPIO_OTYPER_OT6);

	KeyPadPort->PUPDR &= ~(							// set to neither push pull or pull down x
						GPIO_PUPDR_PUPD1 |
						GPIO_PUPDR_PUPD2 |
						GPIO_PUPDR_PUPD3 |
						GPIO_PUPDR_PUPD4 |
						GPIO_PUPDR_PUPD5 |
						GPIO_PUPDR_PUPD6 );

	ROWPORT->PUPDR |= (							// setting pull down for row pins
						GPIO_PUPDR_PUPD0_1|
						GPIO_PUPDR_PUPD1_1 |
						GPIO_PUPDR_PUPD2_1 |
						GPIO_PUPDR_PUPD3_1);

	COLUMNPORT->OSPEEDR |= (
						(3 << GPIO_OSPEEDR_OSPEED4_Pos) |
						(3 << GPIO_OSPEEDR_OSPEED5_Pos)	|
						(3 << GPIO_OSPEEDR_OSPEED6_Pos));

	COLUMNPORT->BRR = COLUMN_PINS;
}

int Keypad_IsAnyKeyPressed(void) {
// drive all COLUMNS HI; see if any ROWS are HI
// return true if a key is pressed, false if not
// currently no debounce here - just looking for a key twitch
	COLUMNPORT->BSRR = COLUMN_PINS;         	      // set all columns HI
   for ( uint16_t idx=0; idx<SETTLE; idx++ )   	// let it settle
      ;
   if ((ROWPORT->IDR & ROW_PINS) != 0 )   {      // got a keypress!
	   for ( uint16_t idx=0; idx<SETTLE; idx++ )
		   ;
   if ((ROWPORT->IDR & ROW_PINS) != 0 ) {
      return( TRUE );
   }
   }
   else
   {
      return( FALSE );
   }// nope.
}

int Keypad_WhichKeyIsPressed(void) {
// detect and encode a pressed key at {row,col}
// assumes a previous call to Keypad_IsAnyKeyPressed() returned TRUE
// verifies the Keypad_IsAnyKeyPressed() result (no debounce here),
// determines which key is pressed and returns the encoded key ID

   int8_t iRow= 0, iCol= 0, iKey= 0;  // keypad row & col index, key ID result

   int8_t bGotKey = 0;             // bool for keypress, 0 = no press



   COL_PORT->BSRR = COL_PINS;                       	 // set all columns HI

   for (iRow = 0; iRow < 4; iRow++ ) {      	 // check all ROWS

      if ( ROW_PORT->IDR & (1 << iRow) ) {      	 // keypress in iRow!!

         COL_PORT->BRR = ( COL_PINS );            	 // set all cols LO

         for (iCol = 0; iCol < 3; iCol++ ) {   // 1 col at a time

            COL_PORT->BSRR = ( 1 << (4+iCol) );     // set this col HI

            if ( ROW_PORT->IDR & (1 << iRow) ) {    // keypress in iCol!!

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
	   const char keymap[4][3] = {
			   {'1', '2', '3'},
			   {'4', '5', '6'},
			   {'7', '8', '9'},
			   {'*', '0', '#'}
	   };
	   return keymap[iRow][iCol];
// 	if ( iKey == KEY_ZERO )                 //    works for ‘*’, ‘#’ too
//         iKey = CODE_ZERO;
//
// 	if ( iKey == KEY_STAR )
// 	     iKey = CODE_STAR;
//
// 	if ( iKey == KEY_HASH )
// 	     iKey = CODE_HASH;
//
// 	return( iKey );                         // return encoded keypress
}
   return( NO_KEYPRESS );                     // unable to verify keypress
}


