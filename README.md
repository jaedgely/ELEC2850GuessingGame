# ELEC2850GuessingGame
Simple guessing game using the STM32F407G-DISC1 Microcontroller.

Relies on a custom header file (projectfunctions.h, included), as well as time.h, and the STM32 HAL Library.

Interrupt happens when user pushes down the blue button. The LEDs will turn on/off in a clockwise manner. Once the button is released, the LED that it was released on will begin blinking. The user then needs to guess a number. If the number is too high, a green external LED is turned on briefly. If the number is too low, a red external LED is turned on briefly. If the number is correct, both external LEDs will blink. Correctly guessed LEDs will be turned on. Once the user guesses all four numbers correctly, the need re-enter the numbers in the order that they guessed them to win.

E.g., The game starts, and four numbers, 5, 2, 3, 9 are generated. The user selects the blue LED. They must guess 5. Then, they select the orange LED. They must guess 2. 
Once all the LEDs are on, they must input 5-2-3-9. If they do so, they win, and the external LEDs as well as the onboard LEDs will blink in rapid succession before resetting.

If they input 2-5-3-9, they lost, so all the LEDs will turn off for a few seconds before resetting.

This project got very messy. There is most definitely a way to condense some of the if statements or while loops, most likely by creating one or two new functions, but I'd rather not touch it. Maybe one day I'll get around to it.

An alternative to the global array and global variable could be to create a pointer inside the interrupt. However, the global array is more of a straight forward approach.
