/*
 * projectfunctions.h
 *
 *  Created on: Apr 4, 2023
 *      Author: edgelyj
 */

#ifndef INC_PROJECTFUNCTIONS_H_
#define INC_PROJECTFUNCTIONS_H_

#define SHORTDELAY  250000
#define DELAY       500000
#define LONGDELAY  1000000
#define XLONGDELAY 2500000

//A row pin will read HIGH if a button is pressed, i.e., the circuit will be closed
//When we turn off the columns, eventually, the row pin will lose power
//Once it loses power, we know which column pin the row pin depends on
//The for loop will only run twice, once for column = 1, and again for column = 2
//If the row pin still has power after the second iteration, it can only be column 3, so the function exits
//Row 1 represents the rightmost row, row 2 the middle, and row 3 the leftmost
uint8_t column_determiner(uint32_t GPIO_PIN_x)
{
	uint8_t column = 1;
	for (column = 1; column < 3; column++)
	{
		column_signal_adjuster(column);
		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_x) == 0)
		{
			break;
		}
	}
	column_signal_adjuster(0);
	return column;
}

//column_signal_adjuster turns on/off the power of the column pins for the keypad
void column_signal_adjuster(uint8_t stage){
	switch (stage)
	{
	case 0:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 1);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 1);
		break;
	case 1:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 0);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 1);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 1);
		break;
	case 2:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 0);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 1);
		break;
	case 3:
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, 1);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, 1);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, 0);
		break;
	}
}

//Simple function that takes the specified port and pin
//If the function detects that the port has no signal, it returns 1
//Works for pull-down
uint32_t delay(uint32_t time)
{
	for (uint32_t i = 0; i < time; i++){}
}

//Taking the row pin, and the column determined from column_determiner, we can identify the key
//This is because there is only 1 combo of row/column per key
//Due to the nature of the column calculated, the math to determine majority of the keys is X - column
//Example: Pin 13 is on, and column is 1
//This means it is the second row from the top, which is 4, 5, 6
//Column 1 is right most column, so this means pin 6. 7 - 1 = 6.
uint8_t key_identifier(uint32_t GPIO_PIN_x, uint8_t column)
{
	uint8_t keyPressed;
	if (GPIO_PIN_x == GPIO_PIN_10){
		switch (column)
		{
		case 3: // This is actually the *
			return 12;
		case 2: // 0
			return 0;
		case 1: // This is actually the #
			return 11;
		}
	}
	if (GPIO_PIN_x == GPIO_PIN_11) // BOTTOM ROW
	{
		keyPressed = 10 - column;
		return keyPressed;
	}
	if (GPIO_PIN_x == GPIO_PIN_13) // MIDDLE ROW
	{
		keyPressed = 7 - column;
		return keyPressed;
	}
	if (GPIO_PIN_x == GPIO_PIN_15) // LOW ROW
	{
		keyPressed = 4 - column;
		return keyPressed;
	}

}
//Makes the LEDs blink. Values greater than but less than 10 will call led_blinker again to make the LED blink
//multiple times in a row.
//Example: Suppose the input number is 8. The if(number <= 9) will fire. if(number <= 9) will call
//led_blinker(6), twice. Then, led_blinker(6) will call led_blinker(3), twice. led_blinker(3) will blinker
//the red LED, pin 14, once. So, in total, led_blinker(3) is called 4 times, causing 4 blinks.
void led_blinker(uint8_t number)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 0);
	if (number == 0){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 0);
	}
	if (number == 1){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
		delay(DELAY);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
		delay(DELAY);
	}
	if (number == 2){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		delay(DELAY);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);
		delay(DELAY);
	}
	if (number == 3){
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1);
		delay(DELAY);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);
		delay(DELAY);
	}
	if (number <= 6){
		led_blinker(number - 3);
		led_blinker(number - 3);
	}
	else if (number <= 9){
		led_blinker(number - 3);
		led_blinker(number - 3);
	}
	if (number == 10)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 1);
	}
	if (number == 12)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 0);
	}
	if (number == 13)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 0);
	}
	if (number == 14)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 1);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 0);
	}
	if (number == 15)
	{
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, 0);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, 1);
	}
	if (number == 111)
	{
		led_blinker(0);
		delay(DELAY);
		led_blinker(10);
		delay(DELAY);
		led_blinker(12);
		led_blinker(13);
		led_blinker(14);
		led_blinker(15);
	}
}

//function seven_segment_control controls the signal to the pins for the seven segment display
//A 1 represents OFF, and a 0 means that the pin will be on
//seven_segment_control(8) will turn on all the pins, and seven_segment_control(10) will turn off the pins
//The input, number, is calculated from the key_identifier function
void seven_segment_control(uint8_t number)
{
	//All pins are on port D, from Pin 0 to Pin 7. Pin 0 controls the decimal point.
	uint32_t sevenSegmentMap[8] = {GPIO_PIN_0, GPIO_PIN_1, GPIO_PIN_2, GPIO_PIN_3,
							   GPIO_PIN_4, GPIO_PIN_5, GPIO_PIN_6, GPIO_PIN_7,};
	switch (number)
	{
		case 0:
			seven_segment_control(8);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, 1);	//Middle horizontal
			return;
		case 1:
			seven_segment_control(10);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);	//Bottom right vertical
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);	//Top right vertical
			return;
		case 2:
			seven_segment_control(8);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 1);	//Botton Right Vertical
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);	//Top Left Vertical
			return;
		case 3:
			seven_segment_control(8);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);	//Bottom Left Vertical
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_6, 1);	//Top Left Vertical
			return;
		case 4:
			seven_segment_control(8);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);	//Top Horizontal
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);	//Bottom Left Vertical
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);	//Bottom Horizontal
			return;
		case 5:
			seven_segment_control(8);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1);	//Top Right Vertical
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);	//Bottom Left Vertical
			return;
		case 6:
			seven_segment_control(8);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 1);	//Top Right Vertical
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 1);	//Top horizontal
			return;
		case 7:
			seven_segment_control(10);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, 0);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, 0);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_7, 0);	//Top horizontal
			return;
		case 8: //All on - allows for quicker simpler code
			for (uint8_t i = 1; i < 8; i++)
			{
				HAL_GPIO_WritePin(GPIOD, sevenSegmentMap[i], 0);
			}
			return;
		case 9:
			seven_segment_control(8);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, 1);
			HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, 1);
			return;
		case 10: //Reset
			for (uint8_t i = 1; i < 8; i++)
			{
				HAL_GPIO_WritePin(GPIOD, sevenSegmentMap[i], 1);
			}
			return;
	}
}

//main_two exists just to simplify the code a little bit
//It takes the input of GPIO_PIN_x, which will be either B10, B11, B13, or B15, the row pins for the keypad
uint8_t key_finder_function(GPIO_PIN_x)
{
	uint8_t columnPressed;
	uint8_t keyPressed;
	columnPressed = column_determiner(GPIO_PIN_x);
	keyPressed = key_identifier(GPIO_PIN_x, columnPressed);
	return keyPressed;
}

//Guess evaluator takes the current key pressed with the number that currently should be guessed.
//If the number is correct, the two external LEDs blink, and 1 is returned.
//If the number is greater than the value, the red LED turns on
//If the number is less than the value, the green LED turns on
uint8_t guess_evaluator(uint8_t correctNumbers[], uint8_t streak, uint8_t keyPressed)
{
	//If the key pressed is the number that needs to be guessed
	if (correctNumbers[streak] == keyPressed)
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1);
		delay(XLONGDELAY);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);
		return 1;
	}
	//If the key pressed is higher than the number that needs to be guessed - RED LED
	else if (keyPressed > correctNumbers[streak])
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1);
		delay(DELAY);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);
		return 0;
	}
	else if (keyPressed < correctNumbers[streak])
	{
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);
		delay(DELAY);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);
		return 0;
	}
}

//Final stage takes the key pressed, the bank of correct numbers, and the final guess array
//a static variable, streak, is created, in order to write the key pressed to the correct array index
//this variable is incremented by 1 each time the function is ran
//When finalGuessStreak == 4, the function will have been run 4 times, meaning that index 0-3
//of the finalGuessArray will be filled.
//A for loop them compares the values of the array correctNumbers to the array finalGuessArray
//If all the numbers match, win is 1 and is returned. If there is a mismatch, 2 is returned.
uint8_t final_stage(uint8_t keyPressed, uint8_t correctNumbers[], uint8_t finalGuessArray[])
{
	//This function should do something
	uint8_t win = 0;
	static uint8_t finalGuessStreak = 0;
	finalGuessArray[finalGuessStreak] = keyPressed;
	finalGuessStreak++;
	if (finalGuessStreak == 4)
	{
		for (uint8_t i = 0; i < 4; i++)
		{
			//If all the numbers match
			if (finalGuessArray[i] == correctNumbers[i])
			{
				win = 1;
			}
			//If a number does not match
			if (finalGuessArray[i] != correctNumbers[i])
			{
				win = 2;
				break;
			}
		}
	}
	return win;
}

//1 celebrates. 2 turns off the lights.
void victory_or_defeat(wonTheGame) // 0 means taking input still, 1 = WON, 2 = LOST
{
	if (wonTheGame == 1)
	{
		led_blinker(111);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1);
		delay(DELAY);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1);
		led_blinker(12);
		delay(DELAY);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);
		led_blinker(13);
		delay(DELAY);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);
		led_blinker(14);
		delay(DELAY);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1);
		led_blinker(15);
		delay(DELAY);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 0);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 0);
		led_blinker(14);
		delay(DELAY);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, 1);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, 1);
		led_blinker(13);
		delay(DELAY);
		led_blinker(12);
		delay(DELAY);
		HAL_NVIC_SystemReset();
	}
	if (wonTheGame == 2)
	{
		led_blinker(0);
		delay(XLONGDELAY);
		delay(XLONGDELAY);
		delay(XLONGDELAY);
		HAL_NVIC_SystemReset();
	}
}

//Generators 4 random numbers, from 1 to 9, that are different than each other.
void random_generator(uint8_t array[]) {
	for (uint8_t i = 0; i < 4; i++)
	{
		array[i] = rand() % 9 + 1;
	}
	for (uint8_t i = 0; i < 4; i++)
	{
		for (uint8_t j = i + 1; j < 4; j++)
		{
			while (array[i] == array[j]) {
				array[j] = rand() % 9 + 1;
			}
		}
	}
}
#endif /* INC_PROJECTFUNCTIONS_H_ */
