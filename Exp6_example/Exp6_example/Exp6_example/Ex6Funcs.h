#ifndef EX6FUNCS_H_
#define EX6FUNCS_H_

#include "board_struct.h"

typedef enum {not_pressed, debounce_p, pressed, held,
debounce_r} sw_state_t;

typedef struct
{
	volatile uint8_t * port_addr;
	uint8_t pin_mask;
	sw_state_t sw_state;
	uint8_t debounce_time;
} sw_status_t;

uint8_t INTERVAL;
uint32_t time_g;
sw_status_t sw1_status_g, sw2_status_g, sw3_status_g;


void Display_Binary_On_LEDs(uint8_t number);

uint8_t sEOS_Init(uint8_t interval_ms);
	
ISR(TIMER2_COMPA_vect);

void Read_Switch(sw_status_t * SW_input_p);

#endif
