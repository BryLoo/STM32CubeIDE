/* USER CODE BEGIN Header */
/*******************************************************************************
* EE 329 A2 KEYPAD INTERFACE
*******************************************************************************
* @file : keypad.h
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

int Keypad_IsAnyKeyPressed(void);
int Keypad_WhichKeyIsPressed(void);
void Keypad_Config(void);
#include "main.h"

#ifndef SRC_KEYPAD_H_
#define SRC_KEYPAD_H_

#define COL_PORT GPIOC
#define ROW_PORT GPIOD
#define COL_PINS (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3)
#define ROW_PINS (GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6)
#define NUM_ROWS 4
#define NUM_COLS 4
#define SETTLE 7000 //number of iterations for settle loop
#define TRUE 1
#define FALSE 0
#define BIT0 8 //8 because binary 1000 for row gpio pins 3 to 6
#define NO_KEYPRESS -1

#endif /* SRC_KEYPAD_H_ */
