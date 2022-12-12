/*
 * Read_Inputs.h
 *
 * Created: 11/17/2020 11:17:45 AM
 *  Author: youngerr
 */ 


#ifndef READ_INPUTS_H_
#define READ_INPUTS_H_

#include "board_struct.h"

void GPIO_Input_Init(volatile GPIO_t *port_addr, uint8_t pin_mask);

void GPIO_Pull_Up_Enable(volatile GPIO_t *port_addr, uint8_t pin_mask);

void GPIO_Pull_Up_Disable(volatile GPIO_t *port_addr, uint8_t pin_mask);

uint8_t GPIO_Read_Inputs(volatile GPIO_t *port_addr);

uint8_t GPIO_Read_Pin(volatile GPIO_t *port_addr, uint8_t pin_mask);

#endif /* READ_INPUTS_H_ */