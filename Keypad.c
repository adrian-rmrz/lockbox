#include <stdio.h>
#include "stm32f4xx.h" 
#include "LCD.h"

/*******************************
 * Keypad Pin Configuration
 *******************************
 */
 //Global Variables
 int open = 0; // Open Status; if safe is open, 1, otherwise 0
 int check = 0; // Variable for if password attempt should be check
 int clear = 0; // Variable to see if array should be cleared
 int password[] = {4,5,6,7}; // Password; check if match
 int keyPos = 0; // Position in key array
 int pw_attempt[] = {0}; // Holds Password attempt to be displayed

 // Functions
 void keyConfig(void) {
	// Keypad Pin Configuration
	// GPIO Port E, Pins 9-11 are for Columns, default values are already in input mode
	GPIOE -> MODER |= (1u << 24); // Pin 12, Output Mode
	GPIOE -> MODER |= (1u << 26); // Pin 13, Output Mode
	GPIOE -> MODER |= (1u << 28); // Pin 14, Output Mode
	GPIOE -> MODER |= (1u << 30); // Pin 15, Output Mode
	 
	// Set inputs to pull down
	GPIOE -> PUPDR |= (1u << 19); // Pin 9, Pull Down
	GPIOE -> PUPDR |= (1u << 21); // Pin 10, Pull Down
	GPIOE -> PUPDR |= (1u << 23); // Pin 11, Pull Down
	
	GPIOE -> ODR |= (1u << 12); // Pin 12, Send High
	GPIOE -> ODR |= (1u << 13); // Pin 13, Send High
	GPIOE -> ODR |= (1u << 14); // Pin 14, Send High
	GPIOE -> ODR |= (1u << 15); // Pin 15, Send High
	 
	// Pin 9 Interrupt Configuration
	SYSCFG -> EXTICR[2] |= (1u << 6); // Port E, Pin 9 Interrupt
	EXTI -> IMR |= (1<<9); //Interrupt Mask Enable for Pin 9
	EXTI -> RTSR |= (1<<9); //Rising trigger selection 
	
	NVIC_SetPriority(EXTI9_5_IRQn, 0);
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	NVIC_EnableIRQ(EXTI9_5_IRQn);
	
	// Pin 10 Interrupt Configuration
	SYSCFG -> EXTICR[2] |= (1u << 10); // Port E, Pin 10 Interrupt
	EXTI -> IMR |= (1<<10); //Interrupt Mask Enable for Pin 10
	EXTI -> RTSR |= (1<<10); //Rising trigger selection 
	
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	
	// Pin 11 Interrupt Configuration
	SYSCFG -> EXTICR[2] |= (1u << 14); // Port E, Pin 11 Interrupt
	EXTI -> IMR |= (1<<11); //Interrupt Mask Enable for Pin 11
	EXTI -> RTSR |= (1<<11); //Rising trigger selection 
	
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	NVIC_EnableIRQ(EXTI15_10_IRQn);

 }
	 
//===================================================
// Keypad Interrupt Handlers
//===================================================
void EXTI9_5_IRQHandler(void) {
	delay(300);
	GPIOE -> ODR = (1u << 12); // Pin 12, Keep High, Send Rest Low
	
	if ((GPIOE -> IDR & (1 << 9)) == 0) {
			GPIOE -> ODR = (1u << 13); // Pin 13 (Row 2) Send High, Rest Low
		
			if ((GPIOE -> IDR & (1 << 9)) == 0) {
					GPIOE -> ODR = (1u << 14); // Pin 14 (Row 3) Send High, Rest Low
				
					if ((GPIOE -> IDR & (1 << 9)) == 0) {
						// Automatically row 4
						open = 1;
					} else {
						// Add 7
						pw_attempt[keyPos] = 7;
					}
			} else {
				// Add 4
				pw_attempt[keyPos] = 4;
			}
	} else {
		// Add 1
		pw_attempt[keyPos] = 1;
	}
	
	++keyPos;
	
	GPIOE -> ODR |= (15u << 12); // Pin 12-15, Send High
	delay(1000);
	EXTI -> PR |= (1<<9); // Clear Pending
	NVIC_ClearPendingIRQ(EXTI9_5_IRQn); 
	
}

void EXTI15_10_IRQHandler(void) {
	delay(300); // Debounce
	GPIOE -> ODR = (1u << 12); // Pin 12 (Row 1) Keep High, Send Rest Low
	
	if ((GPIOE -> IDR & (3 << 10)) == 0) {
			GPIOE -> ODR = (1u << 13); // Pin 13 (Row 2) Send High, Rest Low
		
			if ((GPIOE -> IDR & (3 << 10)) == 0) {
					GPIOE -> ODR = (1u << 14); // Pin 14 (Row 3) Send High, Rest Low
				
					if ((GPIOE -> IDR & (3 << 10)) == 0) {
						// Automatically row 4
						// Check Column
						if ((EXTI -> PR & (1 << 10)) != 0) {
							// Add 0
							pw_attempt[keyPos] = 0;
						} else {
							// Check Password
							open = 0;
						}
					} else {
						if ((EXTI -> PR & (1 << 10)) != 0) {
							// Add 8
							pw_attempt[keyPos] = 8;
						} else {
							// Add 9
							pw_attempt[keyPos] = 9;
						}
					}
			} else {
				if ((EXTI -> PR & (1 << 10)) != 0) {
					// Add 5
					pw_attempt[keyPos] = 5;
				} else {
					// Add 6
					pw_attempt[keyPos] = 6;
				}
			}
	} else {
		if ((EXTI -> PR & (1 << 10)) != 0) {
			// Add 2
			pw_attempt[keyPos] = 2;
		} else {
			// Add 3
			pw_attempt[keyPos] = 3;
		}
	}
	
	++keyPos;
		
	GPIOE -> ODR |= (15u << 12); // Pin 12-15, Send High
	delay(1000); // Duplicates
	EXTI -> PR |= (3<<10); // Clear Pending
	NVIC_ClearPendingIRQ(EXTI15_10_IRQn); //Clear Pending
}


