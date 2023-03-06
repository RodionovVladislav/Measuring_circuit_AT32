#include "interrupts.h"

#define CAN_INT_FLAGS_ALL 	CAN_TM0TCF_FLAG
#define CAN_COUNT 2

can_type* _cans[] = {
	CAN1,
	CAN2
};

IRQn_Type _can_irq[] = {
	USBFS_H_CAN1_TX_IRQn,
	CAN2_TX_IRQn
};

crm_periph_clock_type _can_crm[] = {
	CRM_CAN1_PERIPH_CLOCK,
	CRM_CAN2_PERIPH_CLOCK
};

gpio_type* _can_tx_gpio[] = {
	GPIOA,
	GPIOB,
};

uint32_t _can_tx_pin[] = {
	GPIO_PINS_12,
	GPIO_PINS_13,
};

uint32_t _can_flags[CAN_COUNT];
confirm_state _can_transmit_started[CAN_COUNT];

void CAN_init(uint8_t index);
void CAN_cleanup(uint8_t index);
void CAN_transmit(uint8_t index, uint8_t data);
void CAN_TX_IRQHandler(uint8_t index);

confirm_state CAN_TestInterrupts(void)
{
  crm_periph_clock_enable(CRM_IOMUX_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOA_PERIPH_CLOCK, TRUE);
  crm_periph_clock_enable(CRM_GPIOB_PERIPH_CLOCK, TRUE);

  for (uint8_t i = 0; i < CAN_COUNT; ++i)
  {
	_can_flags[i] = 0;
	_can_transmit_started[i] = FALSE;
	CAN_init(i);
	CAN_transmit(i, 42);
	if (_can_flags[i] != CAN_INT_FLAGS_ALL)
	  return FALSE;
	CAN_cleanup(i);
  }

  return TRUE;
}

void CAN_TX_IRQHandler(uint8_t index)
{
  if (can_flag_get(_cans[index], CAN_TM0TCF_FLAG) == SET)
  {
	_can_flags[index] |= CAN_TM0TCF_FLAG;
	can_interrupt_enable(_cans[index], CAN_TCIEN_INT, FALSE);
  }
}

void CAN_init(uint8_t index)
{
  crm_periph_clock_enable(_can_crm[index], TRUE);

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_STRONGER;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = _can_tx_pin[index];
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(_can_tx_gpio[index], &gpio_init_struct);

  can_base_type can_base_struct;
  can_baudrate_type can_baudrate_struct;
  can_filter_init_type can_filter_init_struct;

  /* can base init */
  can_default_para_init(&can_base_struct);
  can_base_struct.mode_selection = CAN_MODE_LOOPBACK;
  can_base_struct.ttc_enable = FALSE;
  can_base_struct.aebo_enable = TRUE;
  can_base_struct.aed_enable = TRUE;
  can_base_struct.prsf_enable = FALSE;
  can_base_struct.mdrsel_selection = CAN_DISCARDING_FIRST_RECEIVED;
  can_base_struct.mmssr_selection = CAN_SENDING_BY_ID;
  can_base_init(_cans[index], &can_base_struct);

  /* can baudrate, set baudrate = pclk/(baudrate_div *(1 + bts1_size + bts2_size)) */
  can_baudrate_struct.baudrate_div = 10;
  can_baudrate_struct.rsaw_size = CAN_RSAW_3TQ;
  can_baudrate_struct.bts1_size = CAN_BTS1_8TQ;
  can_baudrate_struct.bts2_size = CAN_BTS2_3TQ;
  can_baudrate_set(_cans[index], &can_baudrate_struct);

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(_can_irq[index], 0, 0);
  can_interrupt_enable(_cans[index], CAN_TCIEN_INT, TRUE);
}

void CAN_cleanup(uint8_t index)
{
  crm_periph_clock_enable(CRM_CAN1_PERIPH_CLOCK, FALSE);
}

void CAN_transmit(uint8_t index, uint8_t data)
{
  uint8_t transmit_mailbox;
  can_tx_message_type tx_message_struct;
  tx_message_struct.standard_id = 0x400;
  tx_message_struct.extended_id = 0;
  tx_message_struct.id_type = CAN_ID_STANDARD;
  tx_message_struct.frame_type = CAN_TFT_DATA;
  tx_message_struct.dlc = 8;
  for (uint8_t i = 0; i < 8; ++i)
  {
    tx_message_struct.data[i] = data;
  }
  transmit_mailbox = can_message_transmit(_cans[index], &tx_message_struct);
  while(can_transmit_status_get(_cans[index], (can_tx_mailbox_num_type)transmit_mailbox) != CAN_TX_STATUS_SUCCESSFUL);
}

void USBFS_H_CAN1_TX_IRQHandler(void)
{
  CAN_TX_IRQHandler(0);
}

void CAN2_TX_IRQHandler(void)
{
  CAN_TX_IRQHandler(1);
}
