# Overview
This lockbox project served as an introduction to embedded system programming. The project was programmed in C. This project was originally created and compiled in Keil's µVision. This means that there are proprietary folders (DebugConfig, Listings, Objects, and RTE) and files (`.scvd`, `.uvguix`, `.uvoptx`) that aren't relevant to the actual function of the program. Ignoring these proprietary directories and files, the actual files that were created by me were the `.c`, `.h`, and `.txt` files.

These are the important files within the repository:
- `LCD.h`
- `GPIO.c`
- `Keypad.c`
- `LCD.c`
- `Lockbox.c`
- `PWM.c`

The `PinLayout.txt` and `CheckList.txt` files served as reminders of how the pins were layed out/available and the things that want to be implemented respectively.

## LCD.h
Despite its name, this is the header file that connects all of the `.c` together and holds all of the global variable declarations and function prototypes. At the top of the file, the LCD pin connections are defined. Then we get the LCD function prototypes. After which, we get the global variables for the other parts of the project, like analog-to-digital conversion (ADC) and the keypad. Finally, we get the prototypes for the other `.c` files.

## Lockbox.c
The main file of the project which is where the main function is located. Inside the main function, we enable the clock for all the pins of components that require a clock. We then call all of the `init` functions within the other files. We then place the LCD cursor to its starting position. Finally, we are continuously poll for an input on the keypad, display which keys are pressed on the LCD, and if the correct password is input, we activate the servos to unlock the box. 

## GPIO.c
This file contains the GPIO pin configuration for the speaker and the LED. These components only require an output. After setting the pin configuration, we have functions for the motion sensor. The first one is the initialization of the motion sensor and after are the interrupt handlers. 

## Keypad.c
This file handles the keypad functionality. The `keyConfig` function is setting all of the pins to their required states in order to detect a key press in the keypad matrix. The last two functions are interrupt handlers, one to detect which row a key was pressed and the other for the column. Using this, we know which key was pressed.

## LCD.c
This file handles the LCD functionality. There is an initialization of the LCD pins and then functions that load the correct character to the LCD.

## PWM.c
This file enables the pulse-width modulation (PWM) functionality. The `init` function initializes the pins and then sets the period and the duty cycle of the square wave. The last two functions turn on and off the PWM output.