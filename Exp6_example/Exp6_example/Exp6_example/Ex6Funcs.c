#include "Ex6Funcs.h"
#include "LEDS.h"
#include "GPIO_Outputs.h"
#include <avr/interrupt.h>

static uint8_t sw_count_g = 0;

void Display_Binary_On_LEDs(uint8_t number){
	number %= 8;
	// Initialize LEDs to off
	LEDS_Off(LED1_port,LED1_pin);
	LEDS_Off(LED2_port,LED2_pin);
	LEDS_Off(LED3_port,LED3_pin);
	
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

uint8_t sEOS_Init(uint8_t interval_ms)
{
	time_g = 0;
	INTERVAL = interval_ms;
	
	//sw1 init
 	sw1_status_g.sw_state = not_pressed;
	sw1_status_g.port_addr = PB;
	sw1_status_g.pin_mask = SW1;
	
	//sw2 init
	sw2_status_g.sw_state = not_pressed;
	sw2_status_g.port_addr = PA;
	sw2_status_g.pin_mask = SW2;
	
	//sw3 init
	sw3_status_g.sw_state = not_pressed;
	sw3_status_g.port_addr = PA;
	sw3_status_g.pin_mask = SW3;
	
	uint8_t OCR_value,error_flag;
	OCR_value =(uint8_t)((((interval_ms*F_CPU)+(1023999UL))/(1024000UL))-1);
	if(OCR_value<=255)
	{
		error_flag=0; // no errors;
		OCR2A=OCR_value;
		TIMSK2=0x02; // Enables OCRA match interrupt
		TCCR2A=0x02; // Auto-Reload counter on match with OCRA
		TCCR2B=0x07; // Starts the counter with a prescale of 1024
	}
	else
	{
		error_flag=0x80; // interval too big
	}
	return error_flag;
}

ISR(TIMER2_COMPA_vect)
{
	// Increment time variable for scheduler
	time_g+=INTERVAL;
	
	Read_Switch(&sw1_status_g);
	Read_Switch(&sw2_status_g);
	Read_Switch(&sw3_status_g);
}


void Read_Switch(sw_status_t * SW_input_p){
	if(SW_input_p->sw_state==not_pressed){
		uint8_t temp8=Read_Pin(SW_input_p->port_addr,SW_input_p->pin_mask);
		if(temp8==0)
		{
			SW_input_p->debounce_time=0;
			SW_input_p->sw_state=debounce_p;
		}
		
	}
	
	if(SW_input_p->sw_state==debounce_p){
		SW_input_p->debounce_time+=INTERVAL;
		if(SW_input_p->debounce_time>=50)
		{
			uint8_t temp8=Read_Pin(SW_input_p->port_addr,SW_input_p->pin_mask);
			if(temp8==0)
			{
				SW_input_p->sw_state=pressed;
			}
			else
			{
				SW_input_p->sw_state=not_pressed;
			}
		}
	}
	
	if(SW_input_p->sw_state==pressed){
		// For SW1
		if(((SW_input_p->port_addr) == PB) && ((SW_input_p->pin_mask) == SW1)){
			sw_count_g++;
		}
		// For SW2
		else if(((SW_input_p->port_addr) == PA) && ((SW_input_p->pin_mask) == SW2)){
			sw_count_g--;
		}
		// Not implementing SW3
		
		Display_Binary_On_LEDs(sw_count_g);
		
		SW_input_p->debounce_time=0;
		SW_input_p->sw_state = debounce_r;
	}
	
	if(SW_input_p->sw_state==debounce_r){
		SW_input_p->debounce_time+=INTERVAL;
		if(SW_input_p->debounce_time>=50)
		{
			SW_input_p->sw_state = not_pressed;
		}
	}
}
	