/*
 * LEDS.h
 *
 * Created: 6/30/2020 11:42:34 AM
 *  Author: youngerr
 */ 


#ifndef LEDS_H_
#define LEDS_H_

#include "board_struct.h"

#define LED0_port  PC
#define LED0_pin   (1<<7)
// Assume OLED board connected to EXT4
#define LED1_port  PB
#define LED1_pin   (1<<3)
#define LED2_port  PE
#define LED2_pin   (1<<4)
#define LED3_port  PA
#define LED3_pin   (1<<7)


void LEDS_Init(volatile GPIO_t * port_addr, uint8_t pin_mask);
void LEDS_Off(volatile GPIO_t * port_addr, uint8_t pin_mask);
void LEDS_On(volatile GPIO_t * port_addr, uint8_t pin_mask);



#endif /* LEDS_H_ */