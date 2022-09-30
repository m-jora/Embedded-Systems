#ifndef _PORT_H
#define _PORT_H

#define LED0_port (PC)
#define LED0_pin (1<<7)
#define LED1_port (PB)
#define LED1_pin (1<<3)
#define LED2_port (PE)
#define LED2_pin (1<<4)
#define LED3_port (PA)
#define LED3_pin (1<<7)

#define PC ((volatile GPIO_t *)(&PINC))
#define PB ((volatile GPIO_t *)(&PINB))
#define PE ((volatile GPIO_t *)(&PINE))
#define PA ((volatile GPIO_t *)(&PINA))

#endif
