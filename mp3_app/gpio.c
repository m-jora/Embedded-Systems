
void GPIO_Output_Init(volatile GPIO_t* base_addr,uint8_t pin_mask)
{
	(base_addr -> GPIO_DDR) |= pin_mask;
}

