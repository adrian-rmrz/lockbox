#include <stdio.h>
#include "stm32f4xx.h" 
#include "LCD.h"

/*******************************
 * Speaker and LED Pin Configuration
 *******************************
 */
 void sledConfig(void) {
	GPIOA -> MODER |= (1u << 4); // Port A, Pin 2 (Speaker) Output Mode
	GPIOA -> MODER &= ~(1u << 5);
	
	GPIOA -> MODER |= (1u << 6); // Port A, Pin 3 (LED) Output Mode
	GPIOA -> MODER &= ~(1u << 7);
	
	GPIOA -> MODER |= (1u << 8); // Port A, Pin 4 (LED) Output Mode
	GPIOA -> MODER &= ~(1u << 9);
	
	GPIOA -> MODER |= (1u << 10); // Port A, Pin 5 (LED) Output Mode
	GPIOA -> MODER &= ~(1u << 11);
	
	GPIOA -> MODER |= (1u << 12); // Port A, Pin 6 (LED) Output Mode
	GPIOA -> MODER &= ~(1u << 13);
	
	GPIOA -> MODER |= (1u << 14); // Port A, Pin 7 (LED) Output Mode
	GPIOA -> MODER &= ~(1u << 15);
 }

//===================================================
// Motion Sensor
//===================================================

void motionSensor_init(void){
    //set PA0 to input
    GPIOA -> MODER &= ~(1u<<0);
    GPIOA -> MODER &= ~(1u<<1);
    
     //Connect the portA pin0 to external interrupt line0
    SYSCFG->EXTICR[0] &= ~(0x000Fu);//EXTICR1_EXTI0_PA; //and because we want to clear all other
    EXTI->IMR |= (1<<0);//Interrupt Mask
    EXTI->RTSR|= (1<<0);//Rising trigger selection
      

		__enable_irq();
    NVIC_SetPriority(EXTI0_IRQn,0);
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI0_IRQn);
}
 
void EXTI0_IRQHandler(void) {
	//Check if LCD is on
	GPIOD -> ODR ^= (1u << 12);
	alarmsOn();
	EXTI -> PR |= (1<<0); // Clear Pending
	NVIC_ClearPendingIRQ(EXTI0_IRQn); 
}

void EXTI1_IRQHandler(void) {
	//Check if access granted
	
	EXTI -> PR |= (1<<1); // Clear Pending
	NVIC_ClearPendingIRQ(EXTI1_IRQn); 
}
 
