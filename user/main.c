#include "at32f403a_407_board.h"
#include "at32f403a_407_clock.h"
#include "interrupts.h"
#include "stdbool.h"

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
		gpio_bits_set(GPIOD, pin);
	}
	else
	{
		gpio_bits_reset(GPIOD, pin);
	}
}


void iterating_over_MUX(void)
{
	for (int i = 0; i < 5; ++i)
	{
		gpio_bits_set(GPIOD, mux_enable[i]);
		for (int j = 0; j < 8; ++j)
		{
			set_gpio(mux_address[0], j & 1);
			set_gpio(mux_address[1], j & 2);
			set_gpio(mux_address[2], j & 4);
			delay_ms(100);
		}
		gpio_bits_reset(GPIOD, mux_enable[i]);
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


void dma_configuration(void)
{
	dma_init_type dma_init_struct;
	crm_periph_clock_enable(CRM_DMA1_PERIPH_CLOCK, TRUE);
	dma_reset(DMA1_CHANNEL1);
	dma_default_para_init(&dma_init_struct);
	dma_init_struct.buffer_size = 1;
	dma_init_struct.direction = DMA_DIR_PERIPHERAL_TO_MEMORY;
	dma_init_struct.loop_mode_enable = TRUE;
	dma_init_struct.

}


// ADCCLK must be less than 28 MHz
void adc_configuration(void)
{
	adc_base_config_type adc_base_struct;
	crm_periph_clock_enable(CRM_ADC1_PERIPH_CLOCK, TRUE);
	crm_adc_clock_div_set(CRM_ADC_DIV_6);
	adc_combine_mode_select(ADC_INDEPENDENT_MODE);

	adc_base_default_para_init(&adc_base_struct);
	adc_base_struct.data_align = ADC_RIGHT_ALIGNMENT;
	adc_base_struct.sequence_mode = FALSE;
	adc_base_struct.repeat_mode = FALSE;
	adc_base_struct.ordinary_channel_length = 1;
	adc_base_config(ADC1, &adc_base_struct);
	adc_ordinary_channel_set(ADC1, ADC_CHANNEL_1, 1, ADC_SAMPLETIME_239_5);
	adc_ordinary_conversion_trigger_set(ADC1, ADC12_);

}


int main(void)
{
	MUX_manage();
	while(1)
	{

	}
}
