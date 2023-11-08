// Code to integrate all features of security lockbox

//===================================================
// Headers
//===================================================
#include "stm32f4xx.h"
#include "LCD.h"

//===================================================
// Global Variables and Constants
//===================================================
int prompt = 0; // Which state we are in, different message for each state
char message[] = "ENTER PASSWORD:"; // Enter Password Message
char wrong[] = "WRONG PASSWORD"; // Wrong Password
int iter = 0;

//===================================================
// Main
//===================================================
int main(void) {
	// Turn on clock
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // Port A for GPIO (LEDs and Speaker), Analog Inputs (Temperature and Light), and Motion Sensors
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN; // Port B for Servos
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOEEN; // Port E for Interrupts i.e keypad
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN; // Port D for LCD
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // Port D for LCD
	RCC -> APB1ENR |= RCC_APB1ENR_TIM4EN; // Timer 4 for PWM
	RCC -> APB1ENR |= RCC_APB1ENR_TIM3EN; // Timer 3 for PWM
	RCC -> APB2ENR |= RCC_APB2ENR_ADC1EN; // ADC 1 Clock Enable
	RCC -> APB2ENR |= RCC_APB2ENR_ADC2EN; // ADC 2 Clock Enable
	RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN; // System Config

	//TESTING
	GPIOD -> MODER |= (1u<<24); // Green LED output mode
	GPIOD -> MODER &=~(1u<<25);

	//Speaker and LED Configuration
	sledConfig();

	//Keypad Configuration
	keyConfig();

	// Motion Sensor Interrupt Configuration (Port A, Pin 0 and 1)
	SYSCFG -> EXTICR[0] &= ~(0x00FFu); // Motion Sensor for Keypad (Port A, Pin 0) and Motion Sensor for Inside Box (Port A, Pin 1)

	EXTI -> IMR |= (3<<0); //Interrupt Mask Enable for Motion Sensors
	EXTI -> RTSR |= (3<<0); //Rising trigger selections

	// ADC Initialization
	//ADC_LightSensor_Init();
	//ADC_Temp_Init();

	// PWM
	PWM_init();

	// NVIC
	__enable_irq();

	// Motion Sensor
	motionSensor_init();

	NVIC_SetPriority(EXTI0_IRQn,0); // Sets Highest Priority to Motion Sensor outside keypad
	NVIC_ClearPendingIRQ(EXTI0_IRQn); // Clears Pending
	NVIC_EnableIRQ(EXTI0_IRQn); // Enables interrupts for Motion Sensor

	NVIC_SetPriority(EXTI1_IRQn,0); // Sets Highest Priority to Motion Sensor inside box
	NVIC_ClearPendingIRQ(EXTI1_IRQn); // Clears Pending
	NVIC_EnableIRQ(EXTI1_IRQn); // Enables interrupts for Motion Sensor

	// ADC Interrupts
	NVIC_SetPriority(ADC_IRQn, 1);
	NVIC_ClearPendingIRQ(ADC_IRQn);
	NVIC_EnableIRQ(ADC_IRQn);

	LCD_port_init();
	LCD_init();
	place_lcd_cursor(1);
	display_string(message);
	place_lcd_cursor(2);
	open = 0;

	while (1) {
		if (open == 0) {
			closeLatch();
		} else {
			openLatch();
		}

		place_lcd_cursor(2);
		//Display which keys were pressed
		for (int i = 0; i < keyPos; ++i) {
				display_int(pw_attempt[i]);
		}

	}
}
