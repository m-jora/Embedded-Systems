#ifndef EX6FUNCS_H_
#define EX6FUNCS_H_

#include "board_struct.h"

void Display_Binary_On_LEDs(uint8_t number);

// void Read_Switch(SW_values_t * SW_input_p);

typedef enum {not_pressed, debounce_p, pressed, held,
debounce_r} switch_state_t;

typedef struct{
	uint8_t SW_mask;
	uint8_t volatile *SW_port;
	uint8_t debounce_time;
	switch_state_t SW_state; 
} SW_values_t;

#endif
