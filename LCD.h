#include "stm32f4xx.h" 

/*******************************
 * LCD pins connections to PORTD
 *******************************
 */
#define RS 7
#define EN 6 
#define DB7 3
#define DB6 2
#define DB5 1
#define DB4 0
/*******************************
 * FUNCTION PROTOTYPES
 *******************************
 */
// Delay Functions
void tim6_delay(void);
void delay(int ms);
// LCD related functions
void LCD_port_init(void);
void LCD_init(void);
void LCD_write(unsigned char data);
void place_lcd_cursor(unsigned char lineno);
void instruction_load8(uint8_t UB, uint8_t LB);
void instruction_load4(uint8_t byte);
void display_char(char x);
void display_string(char x[]);
void capitalize(char x);
void display_capital(char x[], int len);
void display_int(int number);
void display_double(double number);
void display_fivefloat(float number);

// FINAL PROJECT

// Global Variables
// ADC
extern double LightSensor_DATA;
extern double TEMP_DATA;

// Keypad
extern int clear; // Check if password should be cleared
extern int check; // Check password attempt
extern int open; // Open Status; if safe is open, 1, otherwise 0
extern int password[]; // Password; check if match
extern int prompt; // Which state we are in, different message for each state
extern char message[]; // Enter Password Message
extern char wrong[]; // Wrong Password
extern int keyPos;
extern char keys[]; // Holds keys pressed
extern int pw_attempt[]; // Holds Password attempt to be displayed
extern int iter;
	
// Prototypes
// GPIO
void sledConfig(void);
void EXTI0_IRQHandler(void); // EXTI0 Interrupt Handler
void EXTI1_IRQHandler(void); // EXTI1 Interrupt Handler
void motionSensor_init(void); // Motion Sensor initialization

// ADC
void ADC_LightSensor_Init(void); // ADC Light Sensor Initialization
void ADC_Temp_Init(void); // ADC Temp Sensor Initialization
void ADC_IRQHandler(void); // ADC Interrupt Handler
void lightSensor_check(void);
void tempSensor_check(void);
void alarmsOn(void);

// Keypad
void keyConfig(void); // Keypad Pin Configuration
void EXTI9_5_IRQHandler(void); // EXTI 5-9 Interrupt Handler
void EXTI15_10_IRQHandler(void); // EXTI3 Interrupt Handler

// PWM
void PWM_init(void); // PWM Initialization Function
void closeLatch(void); // Close servo
void openLatch(void); // Open servo
