#include "interrupts.h"

#define USART_INT_FLAGS_ALL 	(USART_TDBE_FLAG | USART_TDC_FLAG)
#define USART_COUNT 8
#define BYTES_TO_TRANSFER 10

usart_type* _usarts[] = {
	USART1,
	USART2,
	USART3,
	UART4,
	UART5,
	USART6,
	UART7,
	UART8
};

IRQn_Type _usart_irq[] = {
	USART1_IRQn,
	USART2_IRQn,
	USART3_IRQn,
	UART4_IRQn,
	UART5_IRQn,
	USART6_IRQn,
	UART7_IRQn,
	UART8_IRQn,
};

crm_periph_clock_type _usart_crm[] = {
	CRM_USART1_PERIPH_CLOCK,
	CRM_USART2_PERIPH_CLOCK,
	CRM_USART3_PERIPH_CLOCK,
	CRM_UART4_PERIPH_CLOCK,
	CRM_UART5_PERIPH_CLOCK,
	CRM_USART6_PERIPH_CLOCK,
	CRM_UART7_PERIPH_CLOCK,
	CRM_UART8_PERIPH_CLOCK
};

gpio_type* _usart_tx_gpio[] = {
	GPIOA,
	GPIOA,
	GPIOB,
	GPIOC,
	GPIOC,
	GPIOC,
	GPIOE,
	GPIOE
};

uint32_t _usart_tx_pin[] = {
	GPIO_PINS_9,
	GPIO_PINS_2,
	GPIO_PINS_10,
	GPIO_PINS_10,
	GPIO_PINS_12,
	GPIO_PINS_6,
	GPIO_PINS_8,
	GPIO_PINS_1
};

uint32_t _usart_flags[USART_COUNT];
confirm_state _usarts_transmit_started[USART_COUNT];
uint8_t _usart_bytes[USART_COUNT];

void USART_init(uint8_t index, uint32_t baudrate);
void USART_cleanup(uint8_t index);
void USART_transmit(uint8_t index, uint32_t bytecount, uint16_t data);
void USART_TestInterrupt(uint8_t index);
void USART_IRQHandler(uint8_t index);
void USART_FlagHandler(usart_type* usart_x, uint32_t* flags, uint32_t flag);
void USART_FlagsClear(void);

confirm_state USART_TestInterrupts(void)
{
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOC_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOE_PERIPH_CLOCK, TRUE);

  for (uint8_t i = 0; i < USART_COUNT; ++i)
  {
	USART_TestInterrupt(i);
	if (_usart_flags[i] != USART_INT_FLAGS_ALL)
	{
	  return FALSE;
	}
  }
  return TRUE;
}

void USART_TestInterrupt(uint8_t index)
{
  _usart_flags[index] = 0;
  _usart_bytes[index] = BYTES_TO_TRANSFER;
  _usarts_transmit_started[index] = FALSE;
  USART_init(index, 300);
  while (_usart_flags[index] != USART_INT_FLAGS_ALL);
//  while (usart_flag_get(_usarts[index], USART_TDBE_FLAG) == RESET
//	  || usart_flag_get(_usarts[index], USART_TDC_FLAG) == RESET);
  USART_cleanup(index);
}

/**
  * @brief  uart setup code
  * @param  baudrate: uart baudrate
  * @retval none
  */
void USART_init(uint8_t index, uint32_t baudrate)
{
  /* enable the usart clock */
  crm_periph_clock_enable(_usart_crm[index], TRUE);

  /* configure the usart tx pin */
  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = _usart_tx_pin[index];
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(_usart_tx_gpio[index], &gpio_init_struct);

  /* config usart nvic interrupt */
  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(_usart_irq[index], 0, 0);

  /* configure usart param */
  usart_init(_usarts[index], baudrate, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(_usarts[index], TRUE);

  /* enable usart interrupt */
  usart_interrupt_enable(_usarts[index], USART_TDC_INT, TRUE);
  usart_interrupt_enable(_usarts[index], USART_TDBE_INT, TRUE);

  usart_enable(_usarts[index], TRUE);
}

void USART_cleanup(uint8_t index)
{
//  /* disable usart interrupt */
//  usart_interrupt_enable(_usarts[index], USART_TDC_INT, FALSE);
//  usart_interrupt_enable(_usarts[index], USART_TDBE_INT, FALSE);
//  usart_interrupt_enable(_usarts[index], USART_IDLE_INT, FALSE);
  usart_enable(_usarts[index], FALSE);
  usart_transmitter_enable(_usarts[index], FALSE);
  crm_periph_clock_enable(_usart_crm[index], FALSE);
}

/**
  * @brief  uart transmit code
  * @param  bytecount: number of bytes to transmit
  * @param  data: byte value to transmit
  * @retval none
  */
void USART_transmit(uint8_t index, uint32_t bytecount, uint16_t data)
{
  for (uint32_t i = 0; i < bytecount; ++i)
  {
    while(usart_flag_get(_usarts[index], USART_TDBE_FLAG) == RESET);
    usart_data_transmit(_usarts[index], data);
  }
}

/**
  * @brief  this function clears all flags set by interrupts.
  * @param  none
  * @retval none
  */
void USART_FlagsClear(void)
{
  for (int i = 0; i < USART_COUNT; ++i)
  {
	_usart_flags[i] = 0;
  }
}

/**
  * @brief  this function handles usart interrupt handler.
  * @param  usart_x: select the usart or the uart peripheral
  * @param  flags: flags to be set
  * @retval none
  */
void USART_IRQHandler(uint8_t index)
{
  usart_type* usart_x = _usarts[index];
  uint32_t* flags = &_usart_flags[index];

  /* ignore irq in init() */
//  if (_usart_bytes[index] == BYTES_TO_TRANSFER)
  if (_usarts_transmit_started[index] == FALSE)
  {
	flag_status tdc = usart_flag_get(usart_x, USART_TDC_FLAG);
	_usarts_transmit_started[index] = TRUE;
	usart_data_transmit(usart_x, 42);
	tdc = usart_flag_get(usart_x, USART_TDC_FLAG);
	return;
  }

//  USART_FlagHandler(usart_x, flags, USART_TDC_FLAG);		// transmit data complete flag
//  USART_FlagHandler(usart_x, flags, USART_TDBE_FLAG);		// transmit data buffer empty flag
  if (usart_flag_get(usart_x, USART_TDBE_FLAG) != RESET)
  {
	*flags |= USART_TDBE_FLAG;
	usart_interrupt_enable(usart_x, USART_TDBE_INT, FALSE);
  }
  if (usart_flag_get(usart_x, USART_TDC_FLAG) != RESET)
  {
	*flags |= USART_TDC_FLAG;
	usart_interrupt_enable(usart_x, USART_TDC_INT, FALSE);
  }

//  USART_FlagHandler(usart_x, flags, USART_BFF_FLAG);		// break frame flag
//  USART_FlagHandler(usart_x, flags, USART_IDLEF_FLAG);		// idle flag
//  USART_FlagHandler(usart_x, flags, USART_CTSCF_FLAG); 	// cts change flag (not available for UART4,UART5,USART6,UART7 and UART8)
//  USART_FlagHandler(usart_x, flags, USART_RDBF_FLAG);		// receive data buffer full flag
//  USART_FlagHandler(usart_x, flags, USART_ROERR_FLAG);		// receiver overflow error flag
//  USART_FlagHandler(usart_x, flags, USART_NERR_FLAG);		// noise error flag
//  USART_FlagHandler(usart_x, flags, USART_FERR_FLAG);		// framing error flag
//  USART_FlagHandler(usart_x, flags, USART_PERR_FLAG);		// parity error flag
}

/**
  * @brief  this function handles usart flag check.
  * @param  usart_x: select the usart or the uart peripheral
  * @param  flags: flags to be set
  * @param  flag: flag to check
  * @retval none
  */
void USART_FlagHandler(usart_type* usart_x, uint32_t* flags, uint32_t flag)
{
  if (usart_flag_get(usart_x, flag) != RESET)
  {
	*flags |= flag;
	usart_flag_clear(usart_x, flag);
  }
}

void USART1_IRQHandler(void)
{
  USART_IRQHandler(0);
}


void USART2_IRQHandler(void)
{
  USART_IRQHandler(1);
}

void USART3_IRQHandler(void)
{
  USART_IRQHandler(2);
}

void UART4_IRQHandler(void)
{
  USART_IRQHandler(3);
}

void UART5_IRQHandler(void)
{
  USART_IRQHandler(4);
}

void USART6_IRQHandler(void)
{
  USART_IRQHandler(5);
}

void UART7_IRQHandler(void)
{
  USART_IRQHandler(6);
}

void UART8_IRQHandler(void)
{
  USART_IRQHandler(7);
}
