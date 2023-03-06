#include <at32f403a_407_board.h>
#include <at32f403a_407_clock.h>
#include <interrupts.h>

int i = 0;

void port_init_output(void)
{
	crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);
	crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

	gpio_init_type gpio_init_struct;
	gpio_default_para_init(&gpio_init_struct);
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_ALL;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOD, &gpio_init_struct);
	gpio_init(GPIOB, &gpio_init_struct);
	gpio_pins_huge_driven_config(GPIOD, GPIO_PINS_ALL, TRUE);
	gpio_pins_huge_driven_config(GPIOB, GPIO_PINS_ALL, TRUE);
}
void MUX_manage(void)
{

	while (i < 9)
	{

		i++;
	}
}

int main(void)
{
	while(1)
	{
		
	}
}