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
	gpio_bits_set(GPIOD, GPIO_PINS_4); //�������� �� 1-��� �����
	delay_ms(100);

	gpio_bits_reset(GPIOD, GPIO_PINS_4); //�������� �� 2-��� �����
	gpio_bits_set(GPIOD, GPIO_PINS_5);
	delay_ms(100);

	gpio_bits_set(GPIOD, GPIO_PINS_4); //�������� �� 3-��� �����
	delay_ms(100);

	gpio_bits_reset(GPIOD, GPIO_PINS_4); //�������� �� 4-��� �����
	gpio_bits_reset(GPIOD, GPIO_PINS_5);
	gpio_bits_set(GPIOD, GPIO_PINS_6);
	delay_ms(100);

	gpio_bits_set(GPIOD, GPIO_PINS_4); //�������� �� 5-��� �����
	delay_ms(100);

	gpio_bits_reset(GPIOD, GPIO_PINS_4); //�������� �� 6-��� �����
	gpio_bits_set(GPIOD, GPIO_PINS_5);
	delay_ms(100);

	gpio_bits_set(GPIOD, GPIO_PINS_4); //�������� �� 7-��� �����
	delay_ms(100);

	gpio_bits_reset(GPIOD, GPIO_PINS_4);
	gpio_bits_reset(GPIOD, GPIO_PINS_5);
	gpio_bits_reset(GPIOD, GPIO_PINS_6);
}


void iterating_over_MUX(void)
{
	gpio_bits_set(GPIOD, GPIO_PINS_7); //������� ������ ������������� 8x1, �������� �� 0-��� �����
	delay_ms(100);

	iterating_through_address_ports(); //������������ �������� �����

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
	gpio_bits_set(GPIOD, GPIO_PINS_3); //������� ������������� 1x8
	gpio_bits_set(GPIOD, GPIO_PINS_0); //����������� 1-�� �����

	iterating_over_MUX();

	gpio_bits_reset(GPIOD, GPIO_PINS_0); //���������� 1-�� � ����������� 2-�� �����
	gpio_bits_set(GPIOD, GPIO_PINS_1);

	iterating_over_MUX();

	gpio_bits_set(GPIOD, GPIO_PINS_0); // 3-� �����

	iterating_over_MUX();

	gpio_bits_reset(GPIOD, GPIO_PINS_0); // 4-� �����
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
