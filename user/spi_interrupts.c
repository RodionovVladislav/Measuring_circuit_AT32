#include "interrupts.h"

#define SPI_INT_FLAGS_ALL 	SPI_I2S_TDBE_FLAG
#define SPI_COUNT 4
#define BYTES_TO_TRANSFER 10

spi_type* _spis[] = {
	SPI1,
	SPI2,
	SPI3,
	SPI4
};

IRQn_Type _spi_irq[] = {
	SPI1_IRQn,
	SPI2_I2S2EXT_IRQn,
	SPI3_I2S3EXT_IRQn,
	SPI4_IRQn
};

crm_periph_clock_type _spi_crm[] = {
	CRM_SPI1_PERIPH_CLOCK,
	CRM_SPI2_PERIPH_CLOCK,
	CRM_SPI3_PERIPH_CLOCK,
	CRM_SPI4_PERIPH_CLOCK
};

gpio_type* _spi_mosi_gpio[] = {
	GPIOA,
	GPIOB,
	GPIOB,
	GPIOE
};

uint32_t _spi_mosi_pin[] = {
	GPIO_PINS_7,
	GPIO_PINS_15,
	GPIO_PINS_5,
	GPIO_PINS_6
};

uint32_t _spi_flags[SPI_COUNT];
confirm_state _spi_transmit_started[SPI_COUNT];
uint8_t _spi_bytes[SPI_COUNT];

void SPI_init(uint8_t index);
void SPI_cleanup(uint8_t index);
void SPI_IRQHandler(uint8_t index);

confirm_state SPI_TestInterrupts(void)
{
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);

  for (uint8_t i = 0; i < SPI_COUNT; ++i)
  {
	_spi_flags[i] = 0;
	_spi_transmit_started[i] = FALSE;
	SPI_init(i);
	while (_spi_flags[i] != SPI_INT_FLAGS_ALL);
	SPI_cleanup(i);
  }

  return TRUE;
}

void SPI_IRQHandler(uint8_t index)
{
  if (_spi_transmit_started[index] == FALSE)
  {
	_spi_transmit_started[index] = TRUE;
	spi_i2s_data_transmit(_spis[index], 42);
	return;
  }

  if (spi_i2s_flag_get(_spis[index], SPI_I2S_TDBE_FLAG) == SET)
  {
	_spi_flags[index] |= SPI_I2S_TDBE_FLAG;
	spi_i2s_interrupt_enable(_spis[index], SPI_I2S_TDBE_INT, FALSE);
  }
}

/**
  * @brief  spi setup code
  * @param  none
  * @retval none
  */
void SPI_init(uint8_t index)
{
  crm_periph_clock_enable(_spi_crm[index], TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);

  /* configure the spi mosi pin */
  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = _spi_mosi_pin[index];
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(_spi_mosi_gpio[index], &gpio_init_struct);

  spi_init_type spi_init_struct;
  spi_default_para_init(&spi_init_struct);
  spi_init_struct.transmission_mode = SPI_TRANSMIT_FULL_DUPLEX;
  spi_init_struct.master_slave_mode = SPI_MODE_MASTER;
  // spi "speed"
  spi_init_struct.mclk_freq_division = SPI_MCLK_DIV_1024;
  spi_init_struct.first_bit_transmission = SPI_FIRST_BIT_LSB;
  spi_init_struct.frame_bit_num = SPI_FRAME_8BIT;
  spi_init_struct.clock_polarity = SPI_CLOCK_POLARITY_LOW;
  spi_init_struct.clock_phase = SPI_CLOCK_PHASE_2EDGE;
  spi_init_struct.cs_mode_selection = SPI_CS_SOFTWARE_MODE;
  spi_init(_spis[index], &spi_init_struct);

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(_spi_irq[index], 0, 0);
  spi_i2s_interrupt_enable(_spis[index], SPI_I2S_TDBE_INT, TRUE);

//  spi_crc_polynomial_set(_spis[index], 7);
//  spi_crc_enable(_spis[index], TRUE);
  spi_enable(_spis[index], TRUE);
}

void SPI_cleanup(uint8_t index)
{
  spi_enable(_spis[index], FALSE);
  crm_periph_clock_enable(_spi_crm[index], TRUE);
}

void SPI1_IRQHandler(void)
{
  SPI_IRQHandler(0);
}

void SPI2_I2S2EXT_IRQHandler(void)
{
  SPI_IRQHandler(1);
}

void SPI3_I2S3EXT_IRQHandler(void)
{
  SPI_IRQHandler(2);
}

void SPI4_IRQHandler(void)
{
  SPI_IRQHandler(3);
}
