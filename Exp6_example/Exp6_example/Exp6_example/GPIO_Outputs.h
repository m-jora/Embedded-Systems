/*
 * GPIO_Outputs.h
 *
 * Created: 6/30/2020 11:42:34 AM
 *  Author: youngerr
 */ 


#ifndef GPIO_OUTPUTS_H_
#define GPIO_OUTPUTS_H_

void GPIO_Output_Init(volatile GPIO_t * port_addr, uint8_t pin_mask);
void Input_Init(volatile GPIO_t * port_addr, uint8_t pin_mask);
void Pull_Up_Enable(volatile GPIO_t * port_addr, uint8_t pin_mask);
void GPIO_Output_Set(volatile GPIO_t * port_addr, uint8_t pin_mask);
void GPIO_Output_Clear(volatile GPIO_t * port_addr, uint8_t pin_mask);
uint8_t Read_Pin(volatile GPIO_t * port_addr, uint8_t pin_mask);

#endif /* GPIO_OUTPUTS_H_ */
