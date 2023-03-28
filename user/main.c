#include "at32f403a_407_board.h"
#include "at32f403a_407_clock.h"
#include "interrupts.h"


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


void iterating_through_address_ports(void)
{
	gpio_bits_set(GPIOD, GPIO_PINS_4); //Передача по 1-ому порту
	delay_ms(100);

	gpio_bits_reset(GPIOD, GPIO_PINS_4); //Передача по 2-ому порту
	gpio_bits_set(GPIOD, GPIO_PINS_5);
	delay_ms(100);

	gpio_bits_set(GPIOD, GPIO_PINS_4); //Передача по 3-ому порту
	delay_ms(100);

	gpio_bits_reset(GPIOD, GPIO_PINS_4); //Передача по 4-ому порту
	gpio_bits_reset(GPIOD, GPIO_PINS_5);
	gpio_bits_set(GPIOD, GPIO_PINS_6);
	delay_ms(100);

	gpio_bits_set(GPIOD, GPIO_PINS_4); //Передача по 5-ому порту
	delay_ms(100);

	gpio_bits_reset(GPIOD, GPIO_PINS_4); //Передача по 6-ому порту
	gpio_bits_set(GPIOD, GPIO_PINS_5);
	delay_ms(100);

	gpio_bits_set(GPIOD, GPIO_PINS_4); //Передача по 7-ому порту
	delay_ms(100);

	gpio_bits_reset(GPIOD, GPIO_PINS_4);
	gpio_bits_reset(GPIOD, GPIO_PINS_5);
	gpio_bits_reset(GPIOD, GPIO_PINS_6);
}


void iterating_over_MUX(void)
{
	gpio_bits_set(GPIOD, GPIO_PINS_7); //Включаю первый мультиплексор 8x1, передача по 0-ому порту
	delay_ms(100);

	iterating_through_address_ports(); //Перебираются адресные входы

	gpio_bits_reset(GPIOD, GPIO_PINS_7);
	gpio_bits_set(GPIOD, GPIO_PINS_8);
	delay_ms(100);

	iterating_through_address_ports();

	gpio_bits_reset(GPIOD, GPIO_PINS_8);
	gpio_bits_set(GPIOD, GPIO_PINS_9);
	delay_ms(100);

	iterating_through_address_ports();

	gpio_bits_reset(GPIOD, GPIO_PINS_9);
	gpio_bits_set(GPIOD, GPIO_PINS_10);
	delay_ms(100);

	iterating_through_address_ports();

	gpio_bits_reset(GPIOD, GPIO_PINS_10);
	gpio_bits_set(GPIOD, GPIO_PINS_11);
	delay_ms(100);

	iterating_through_address_ports();

	gpio_bits_reset(GPIOD, GPIO_PINS_11);
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
