#include "Ex6Funcs.h"
#include "LEDS.h"
#include "GPIO_Outputs.h"

void Display_Binary_On_LEDs(uint8_t number){
	// Checks if number is too big
	if(number < 8){
		// Checks LSB 
		if(number % 2 == 1){
			LEDS_On(LED3_port,LED3_pin);
		}
		number /= 2;
		// Checks bit1
		if(number % 2 == 1){
			LEDS_On(LED2_port,LED2_pin);
		}
		number /= 2;
		// Checks MSB
		if(number % 2 == 1){
			LEDS_On(LED1_port,LED1_pin);
		}
	}
}

// void Read_Switch(SW_values_t * SW_input_p)
	// {
	// 	uint8_t temp8;
	// 	if(SW_input_p->SW_state==not_pressed)
	// 	{
	// 		temp8=Read_Pin(SW_input_p->SW_port,SW_input_p->SW_mask);
	// 		if(temp8==0)
	// 		{
	// 			SW_input_p->debounce_time=0;
	// 			SW_input_p->SW_state=debounce_p;
	// 		}
	// 	}
	// 	else if(SW_input_p->SW_state==debounce_p)
	// 	{
	// 		SW_input_p->debounce_time+=interval;
	// 		if(SW_input_p->debounce_time>=50)
	// 		{
	// 			temp8=Read_Pin(SW_input_p->SW_port,SW_input_p->SW_mask);
	// 			if(temp8==0)
	// 			{
	// 				SW_input_p->SW_state=pressed;
	// 				// Task for a pressed switch could go here
	// 				// or in the application calling this function
	// 				// for code that is reusable.
	// 			}
	// 			else
	// 			{
	// 				SW_input_p->SW_state=not_pressed;
	// 			}
	// 		}
	// 	}
	// 	