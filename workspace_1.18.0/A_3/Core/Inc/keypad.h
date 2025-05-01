/*
 * keypad.h
 *
 *  Created on: Apr 14, 2025
 *      Author: Jesus
 */
//#include "stm32l4xx_hal_gpio.h"
#include "stm32l4a6xx.h" //gives access to register names since we cannot use HAL commands

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_


void Keypad_Config(void);
int Keypad_IsAnyKeyPressed(void);
int Keypad_WhichKeyIsPressed(void);

#define NUM_ROWS 4
#define NUM_COLUMNS 3

#define ROWPORT GPIOD
#define COLUMNPORT GPIOD
#define KeyPadPort GPIOD //?

#define ROW_PINS ((1<<0) | (1<<1) | (1<< 2) | (1<<3)) // 0 1 2 & 3 are rows
#define COLUMN_PINS ((1 << 4) | (1 << 5) | (1 << 6)) // 4 5 6 as columns

#define SETTLE 1000
#define FALSE 0
#define TRUE !FALSE // ?
#define BIT0 1

#define KEY_ZERO 11
#define KEY_STAR 10
#define KEY_HASH 12

#define CODE_ZERO 0
#define CODE_STAR 15
#define CODE_HASH 14

#define NO_KEYPRESS -15




#endif /* INC_KEYPAD_H_ */
