#include <stdio.h>
#include "stm32f4xx.h" 
#include "LCD.h"

//===================================================
// ADC
//===================================================
// Global Variables
double TEMP_DATA;
double LightSensor_DATA;

//Functions
void ADC_LightSensor_Init(void){
    // initialize clock for Port A and ADC
    RCC -> AHB1ENR |= (1);
    RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN;    

    // initialize Port A pin 4 as ADC pin
     GPIOA -> MODER |= ((1<<9)|(1<<8));
    
    // Set resolution to 10 bit in CR1 register (default is 12 bit)
    ADC1 -> CR1 |= (1<<24);
    
    // Set Sampling time to 84 (100) cycles in SMPR2 register (default is 000 3 cycles)
    ADC1 -> SMPR2 |= (1<<5);
    
    // Set Continuous mode ADC (CR2 register)
    ADC1 -> CR2 |= (1<<1);

    // Specify channel number 1 of the 1st conversion in SQR3 register
    ADC1 -> SQR3 |= (1<<2);
    
    //Set end of conversion interrupt in CR1 register
    //ADC1 -> CR1 |= (1<<5);
    
    // Turn on ADC conversion
    ADC1 -> CR2 |= (1);
    
    // Start Conversion
    ADC1 -> CR2 |= (1<<30);
    
    // LET NVIC handle ADC Interrupts
    __enable_irq();
    NVIC_ClearPendingIRQ(ADC_IRQn);
    NVIC_SetPriority(ADC_IRQn,0);
    NVIC_EnableIRQ(ADC_IRQn);
		ADC_IRQHandler();
}

void ADC_Temp_Init(void){
    // initialize clock for Port A and ADC
    RCC -> AHB1ENR |= (1);
    RCC -> APB2ENR |= RCC_APB2ENR_ADC2EN;    

    // initialize Port A pin 2 as ADC pin
    GPIOA -> MODER |= ((1<<4) | (1<<5));
    
    // Set resolution to 10 bit in CR1 register (default is 12 bit)
    ADC2 -> CR1 |= (1<<24);
    
    // Set Sampling time to 84 (100) cycles in SMPR2 register (default is 000 3 cycles)
    ADC2 -> SMPR2 |= (1<<5);
    
    // Set Continuous mode ADC (CR2 register)
    ADC2 -> CR2 |= (1<<1);

    // Specify channel number 1 of the 1st conversion in SQR3 register
    ADC2 -> SQR3 |= (1<<1);
    
    //Set end of conversion interrupt in CR1 register
    //ADC2 -> CR1 |= (1<<5);
    
    // Turn on ADC conversion
    ADC2 -> CR2 |= (1);
    
    // Start Conversion
    ADC2 -> CR2 |= (1<<30);
    
    // LET NVIC handle ADC Interrupts
    __enable_irq();
    NVIC_ClearPendingIRQ(ADC_IRQn);
    NVIC_SetPriority(ADC_IRQn,0);
    NVIC_EnableIRQ(ADC_IRQn);
}

void ADC_IRQHandler(void){
    NVIC_ClearPendingIRQ(ADC_IRQn);
     
    // read the converted value from Register DR into ADC_DATA variable.
    // reading the DR clears the interrupt flag.
    if(ADC1 -> SR&(1<<1)){
        LightSensor_DATA = (ADC1 -> DR)*3.0/1023;    //multiples the reference voltage to raw data, divides proportion
				lightSensor_check();
				ADC1 -> CR1 &= ~(1U<<5);
				ADC2 -> CR1 |= (1<<5);
		}
		
		if(ADC2 -> SR&(1<<1)){
				tempSensor_check();
				TEMP_DATA = (((ADC2 -> DR) - 75.0)/10.0);
				ADC2 -> CR1 &= ~(1U<<5);

					/*
					VOUT = TC*TA+V0° // solve for TA
					TA = (Vout - V0)//TC, where Vout is value read from dr
					V0 = 500
					TC = 10*/
		}
}

void lightSensor_check(void) {
	if (LightSensor_DATA > 1) { //Light bright turn off LED
	GPIOA -> ODR &= ~(1u << 3); // Port A, Pin 3 (LED) Off
	GPIOA -> ODR &= ~(1u << 8); // Port A, Pin 8 (LED) OFF
	}
	else if (LightSensor_DATA < 1){
	GPIOA -> ODR |= (1u << 3); // Port A, Pin 3 (LED) ON
	GPIOA -> ODR |= (1u << 8); // Port A, Pin 8 (LED) ON
	}
	
}

void tempSensor_check(void) {
	if (TEMP_DATA > 30) {
		alarmsOn();
	}
	
}
void alarmsOn(void) {		
	for (int i = 0; i < 30; i++){
		GPIOA -> ODR ^= (1u << 9); // Port A, Pin 9 (Speaker) Toggled Speaker
			
		GPIOA -> ODR ^= (1u << 5); // Port A, Pin 5 (LED) toggled ON

		GPIOA -> ODR ^= (1u << 6); // Port A, Pin 6 (LED) toggled ON
		
		GPIOA -> ODR ^= (1u << 7); // Port A, Pin 7 (LED) toggled ON
		delay(1000);
	}
	
}	
