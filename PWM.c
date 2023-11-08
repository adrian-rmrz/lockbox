#include <stdio.h>
#include "stm32f4xx.h" 
#include "LCD.h"

/*******************************
 * PWM
 *******************************
 */
void PWM_init(void) {	
	GPIOB -> MODER &=~(1u<<18); // Alternate Function mode for Port B, Pin 9
	GPIOB -> MODER |= (1u<<19);
	
	GPIOC -> MODER &=~(1u<<18); // Alternate Function mode for Port C, Pin 9
	GPIOC -> MODER |= (1u<<19);
	
	GPIOB -> AFR[1] |= (2u << 4); // Alternate function 2 to Port B, Pin 9
	
	GPIOC -> AFR[1] |= (2u << 4); // Alternate function 2 to Port C, Pin 8
	
		// Configure Output Pin
	TIM4 -> CCMR2 &= ~(3u << 8); //Output Mode Channel 4
	TIM4 -> CCER &= ~(1u << 13); // Select Polarity (Active High)
	
	TIM3 -> CCMR2 &= ~(3u << 8); //Output Mode Channel 3
	TIM3 -> CCER &= ~(1u << 13); // Select Polarity (Active High)
	
		// Select PWM Mode
	TIM4 -> CCMR2 |= (3u << 13); // Timer 4 Channel 4 PWM Mode 1
	TIM4 -> CCMR2 &= ~(1u << 12);
	
	TIM3 -> CCMR2 |= (3u << 13); // Timer 3 Channel 3 PWM Mode 1
	TIM3 -> CCMR2 &= ~(1u << 12);
	
		// Program Period and Duty Cycle
	TIM4 -> PSC = 7;
	TIM4 -> ARR = 40000;
	TIM4 -> CCR4 = 4000;
	
	TIM3 -> PSC = 7;
	TIM3 -> ARR = 40000;
	TIM3 -> CCR4 = 4000;
	
		// Set Preload Bit
	TIM4 -> CCMR2 |= (1u << 11);
	TIM4 -> CR1 |= (1u << 7);
	
	TIM3 -> CCMR2 |= (1u << 11);
	TIM3 -> CR1 |= (1u << 7);
	
		// Select Counting Mode
	TIM4 -> CR1 &= ~(7u << 4);

	TIM3 -> CR1 &= ~(7u << 4);
	
		// Enable Capture Compare and Counter
	TIM4 -> CCER |= (1u << 12);
	TIM4 -> CR1 |= (1u << 0);
	
	TIM3 -> CCER |= (1u << 12);
	TIM3 -> CR1 |= (1u << 0);
}


void closeLatch(void) {
	TIM4 -> CCR4 = 4600;
}

void openLatch(void) {
	TIM4 -> CCR4 = 4000;
}
