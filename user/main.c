#include "at32f403a_407_board.h"
#include "at32f403a_407_clock.h"
#include "interrupts.h"
#include "math.h"

uint16_t mux_enable[5] = {GPIO_PINS_7, GPIO_PINS_8, GPIO_PINS_9, GPIO_PINS_10, GPIO_PINS_11};
uint16_t mux_address[3] = {GPIO_PINS_4, GPIO_PINS_5, GPIO_PINS_6};

void port_init_output(void)
{
	crm_periph_clock_enable(CRM_GPIOD_PERIPH_CLOCK, TRUE);

	gpio_init_type gpio_init_struct;
	gpio_default_para_init(&gpio_init_struct);
	gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
	gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
	gpio_init_struct.gpio_mode = GPIO_MODE_OUTPUT;
	gpio_init_struct.gpio_pins = GPIO_PINS_ALL;
	gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
	gpio_init(GPIOD, &gpio_init_struct);
	gpio_pins_huge_driven_config(GPIOD, GPIO_PINS_ALL, TRUE);
}


void set_gpio(uint16_t pin, bool state)
{
	if (state)
	{
		gpio_bit_set(GPIOD, pin);
	}
	else
	{
		gpio_bit_reset(GPIOD, pin);
	}
}


void iterating_over_MUX(void)
{
	for (int i = 0; i < 5; ++i)
	{
		gpio_bit_set(GPIOD, mux_enable[i]);
		for (int j = 0; j < 8; ++j)
		{
			set_gpio(mux_address[0], j & 1);
			set_gpio(mux_address[1], j & 2);
			set_gpio(mux_address[2], j & 4);
		}
		gpio_bit_reset(GPIOD, mux_enable[i]);
	}
}


void MUX_manage(void)
{
	port_init_output();

	gpio_bits_set(GPIOD, GPIO_PINS_3); //Включаю мультиплексор 1x8
	gpio_bits_set(GPIOD, GPIO_PINS_0); //Подключение 1-ой платы

	iterating_over_MUX();

	gpio_bits_reset(GPIOD, GPIO_PINS_0); //Выключение 1-ой и подключение 2-ой платы
	gpio_bits_set(GPIOD, GPIO_PINS_1);

	iterating_over_MUX();

	gpio_bits_set(GPIOD, GPIO_PINS_0); // 3-я плата

	iterating_over_MUX();

	gpio_bits_reset(GPIOD, GPIO_PINS_0); // 4-я плата
	gpio_bits_reset(GPIOD, GPIO_PINS_1);
	gpio_bits_set(GPIOD, GPIO_PINS_2);

	iterating_over_MUX();

	gpio_bits_reset(GPIOD, GPIO_PINS_2);
}


void adc_init_structure(void)
{
	adc_base_config_type adc_base_struct;
	adc_base_default_para_init(&adc_base_struct);


}


int main(void)
{
	MUX_manage();
	while(1)
	{

	}
}
