#include <stdio.h>
#include "stm32f4xx.h" 
#include "LCD.h"

/*******************************
 * tim6_delay(void)
 * Inputs: NONE
 * Outputs: NONE
 * Based on PSC=0 and ARR=21000; 
 * we get delay of approximately 1.33ms
 *******************************
 */
void tim6_delay(void){
// enable APB1 bus clock
RCC->APB1ENR|=RCC_APB1ENR_TIM6EN;
//TIM6 prescaler set at default to 0 for now
TIM6->PSC=0; // prescalar
TIM6->ARR = 21000;  //auto reload register 
TIM6->CNT=0;   //clear counter register
TIM6->CR1|=TIM_CR1_CEN;
//WHEN COUNTER IS DONE THE TIM6_SR REG UIF FLAG IS SET
while(TIM6->SR==0);
TIM6->SR=0; //CLEAR uIF FLAG
}
/*******************************
 * delay(int ms)
 * Inputs: delay in milliseconds
 * Outputs: NONE
 * An approximate delay because  
 * call of tim6_delay() creates about 1.33ms
 *******************************
 */
void delay(int ms){
int i;
for (i=ms; i>0; i--)
{
tim6_delay();
}
}
/*******************************
 * LCD_port_init()
 * Inputs: NONE
 * Outputs: NONE
 * Port Initialization
 * Refer to the #define statements at top to
 * see what ports are used to connect
 * the STMicro Board with the HD44780 LCD driver
 * Set appropriate pins as digital input/outputs
 *******************************
 */
void LCD_port_init(){
//STEP 1: Enable GPIOD in RCC AHB1ENR register
	RCC -> AHB1ENR |= 0x00000008;
//STEP 2: Set MODER of GPIOD Pins 7, 6, 3, 2, 1 & 0 as outputs
	GPIOD -> MODER |= (1u<<14); //Pin 7
	GPIOD -> MODER &=~ (1u<<15);
	
	GPIOD -> MODER |= (1u<<12); //Pin 6
	GPIOD -> MODER &=~ (1u<<13);
		
	GPIOD -> MODER |= (1u<<6); //Pin 3
	GPIOD -> MODER &=~ (1u<<7);
	
	GPIOD -> MODER |= (1u<<4); //Pin 2
	GPIOD -> MODER &=~ (1u<<5);
	
	GPIOD -> MODER |= (1u<<2); //Pin 1
	GPIOD -> MODER &=~ (1u<<3);
	
	GPIOD -> MODER |= (1u<<0); //Pin 0
	GPIOD -> MODER &=~ (1u<<1);
//STEP 3: Set OTYPER of GPIOD Pins 7, 6, 3, 2, 1 & 0 as push-pull
	GPIOD -> OTYPER &= (0<<RS);
	GPIOD -> OTYPER &= (0<<EN);
	GPIOD -> OTYPER &= (0<<DB7);
	GPIOD -> OTYPER &= (0<<DB6);
	GPIOD -> OTYPER &= (0<<DB5);
	GPIOD -> OTYPER &= (0<<DB4);

//Done with LCD port Initialization
}
/*******************************
 * LCD_init()
 * Inputs: NONE
 * Outputs: NONE
 * LCD Initialization
 * Read the manual carefully
 * We are doing initialization by instruction
 * Don't rush it.
 *******************************
 */
void LCD_init(){
// STEP 1: Wait for 100ms for power-on-reset to take effect
	delay(100);
// STEP 2: Set RS pin LOW to send instructions

// Send instructions using following format:
// Set EN=HIGH; Send 4-bit instruction; Set EN=low; delay 20ms;
// STEP 3a-3d: Set 4-bit mode (takes a total of 4 steps)

	instruction_load8(0x3, 0x3);
	instruction_load8(0x3, 0x2);

// STEP 4: Set 2 line display -- treats 16 char as 2 lines
	instruction_load8(0x2, 0x8);
// STEP 5: Set DISPLAY to OFF
	instruction_load8(0x0, 0x8);
// STEP 6: CLEAR DISPLAY
	instruction_load8(0x0, 0x1);
// STEP 7: SET ENTRY MODE - Auto increment; no scrolling
	instruction_load8(0x0, 0x6);
// STEP 8: Set Display to ON with Cursor and Blink.
	instruction_load8(0x0, 0xF);
}
/*******************************
 * place_lcd_cursor()
 * Inputs: unsigned character
 * Outputs: NONE
 * sets Cursor position to
 * Line 1, character 1 (hex address 0x80)
 * or Line 2, character 1 (hex addres 0xC0)
 *
 *******************************
 */
void place_lcd_cursor(unsigned char lineno){
	if (lineno == 1) {
		instruction_load8(0x8, 0x0);
	} else {
		instruction_load8(0xC, 0x0);
	}
}
/*******************************
 * LCD_write()
 * Inputs: unsigned character data (8-bit)
 * Outputs: NONE
 * writes the character to LCD.
 *
 *******************************
 */
void LCD_write(unsigned char data){
	
}

void instruction_load8(uint8_t UB, uint8_t LB) {
	
	//UB - Upper Bits of Instruction
	//LB - Lower Bits of Instruction
	
	GPIOD -> ODR &= ~(1u<<RS);
	
	GPIOD -> ODR |= (1u<<EN); 
	
	GPIOD -> ODR &= ~(0xFu<<DB4);
	
	GPIOD -> ODR |= (UB<<DB4); //Send Upper Bits of Instruction
	
	GPIOD -> ODR &= ~(1u<<EN);
	
	delay(20);
	
	GPIOD -> ODR |= (1u<<EN); 
	
	GPIOD -> ODR &= ~(0xFu<<DB4);
	
	GPIOD -> ODR |= (LB<<DB4); // Send Lower Bits of Instruction
	
	GPIOD -> ODR &= ~(1u<<EN);
	
	delay(20);
}

void instruction_load4(uint8_t byte) {
	
	GPIOD -> ODR &= ~(1u<<RS);
	
	GPIOD -> ODR |= (1u<<EN); 
	
	GPIOD -> ODR &= ~(0xFu<<DB4);
	
	GPIOD -> ODR |= (byte<<DB4); //Send Upper Bits of Instruction
	
	GPIOD -> ODR &= ~(1u<<EN);
	
	delay(20);
}

void display_char(char x) {
	//Sending Upper Bits
	
	GPIOD -> ODR |= (1u<<RS); //Instruction mode
	
	GPIOD -> ODR |= (1u<<EN); 
	
	GPIOD -> ODR &= ~(0xFu<<DB4); //Clear
	
	GPIOD -> ODR |= (((x>>4) & 0xFu)<<DB4); 
	
	GPIOD -> ODR &= ~(1u<<EN);
	
	delay(20);
	
	GPIOD -> ODR |= (1u<<EN); //Lower Bits
	
	GPIOD -> ODR &= ~(0xFu<<DB4); //Clear
	
	GPIOD -> ODR |= ((x & 0xFu)<<DB4);
	
	GPIOD -> ODR &= ~(1u<<EN);
	
	delay(20);
}

void capitalize(char x) {
	GPIOD -> ODR |= (1u<<RS); //Instruction mode
	
	GPIOD -> ODR |= (1u<<EN); 
	
	GPIOD -> ODR &= ~(0xFu<<DB4); //Clear
	
	GPIOD -> ODR |= (((x>>4) & 0xFu)<<DB4);

	GPIOD -> ODR &= ~(1u << 1); //Capital
	
	GPIOD -> ODR &= ~(1u<<EN);
	
	delay(20);
	
	GPIOD -> ODR |= (1u<<EN); //Lower Bits
	
	GPIOD -> ODR &= ~(0xFu<<DB4); //Clear
	
	GPIOD -> ODR |= ((x & 0xFu)<<DB4);
	
	GPIOD -> ODR &= ~(1u<<EN);
	
	delay(20);
}

void display_string(char x[]) {
		int i;
		
		for (i = 0; i < 16 && x[i] != '\0'; i++) {
			display_char(x[i]);
		}
		
	if (x[i] != '\0') {
		place_lcd_cursor(2);
		
		for (; i < 32 && x[i] != '\0'; i++) {
			display_char(x[i + 16]);
		}
	}
}

void display_capital(char x[], int len) {
	for (int i = 0; i < len; i++) {
		capitalize(x[i]);
	}
}

void display_int(int number) {
	char arr[40];
	
	sprintf(arr, "%i", number);
	
	display_string(arr);
}

void display_double(double number) {
	char arr[40];
	
	sprintf(arr, "%.1f", number);
	
	display_string(arr);
}

void display_fivefloat(float number) {
	char arr[40];
	
	sprintf(arr, "%.5f", number);
	
	display_string(arr);
}

