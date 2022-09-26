#define set_outputs_macro(port, mask) ((port)|= (mask))
#define clear_outputs_macro(port, mask) ((port)&= (~(mask))

GPIO_Output_Init(volatile GPIO_t *addr, uint8_t pin_mask);
GPIO_Output_Set(volatile GPIO_t *addr, uint8_t pin_mask);
GPIO_Output_Clear(volatile GPIO_t *addr, uint8_t pin_mask);


