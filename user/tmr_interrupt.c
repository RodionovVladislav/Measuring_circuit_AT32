#include "interrupts.h"

#define TMR_INT_FLAGS_CH 	TMR_C1_FLAG | TMR_C1_FLAG | TMR_C1_FLAG | TMR_C1_FLAG
#define TMR_INT_FLAGS_ALL 	TMR_OVF_FLAG | TMR_TRIGGER_FLAG | TMR_INT_FLAGS_CH
#define TMR_COUNT 			14
#define TMR_PR 				0xffffu
#define TMR_DT 				0x00ffu

uint8_t _tmrIndex = 0;

tmr_type* _tmrs[] = {
	TMR1,
	TMR2,
	TMR3,
	TMR4,
	TMR5,
	TMR6,
	TMR7,
	TMR8,

	TMR9,
	TMR10,
	TMR11,
	TMR12,
	TMR13,
	TMR14
};

IRQn_Type _tmr_irq[] = {
//	TMR1_CH_IRQn,
	TMR1_OVF_TMR10_IRQn,
	TMR2_GLOBAL_IRQn,
	TMR3_GLOBAL_IRQn,
	TMR4_GLOBAL_IRQn,
	TMR5_GLOBAL_IRQn,
	TMR6_GLOBAL_IRQn,			// only ovf
	TMR7_GLOBAL_IRQn,			// only ovf
//	TMR8_CH_IRQn,
	TMR8_OVF_TMR13_IRQn,

	TMR1_BRK_TMR9_IRQn,			//
	TMR1_OVF_TMR10_IRQn,
	TMR1_TRG_HALL_TMR11_IRQn,
	TMR8_BRK_TMR12_IRQn,
	TMR8_OVF_TMR13_IRQn,
	TMR8_TRG_HALL_TMR14_IRQn
};

crm_periph_clock_type _tmr_crm[] = {
	CRM_TMR1_PERIPH_CLOCK,
	CRM_TMR2_PERIPH_CLOCK,
	CRM_TMR3_PERIPH_CLOCK,
	CRM_TMR4_PERIPH_CLOCK,
	CRM_TMR5_PERIPH_CLOCK,
	CRM_TMR6_PERIPH_CLOCK,
	CRM_TMR7_PERIPH_CLOCK,
	CRM_TMR8_PERIPH_CLOCK,

	CRM_TMR9_PERIPH_CLOCK,
	CRM_TMR10_PERIPH_CLOCK,
	CRM_TMR11_PERIPH_CLOCK,
	CRM_TMR12_PERIPH_CLOCK,
	CRM_TMR13_PERIPH_CLOCK,
	CRM_TMR14_PERIPH_CLOCK
};

uint32_t _tmr_ch_flags[] = { TMR_C1_FLAG, TMR_C2_FLAG, TMR_C3_FLAG, TMR_C4_FLAG };
uint32_t _tmr_ch_ints[] = { TMR_C1_INT, TMR_C2_INT, TMR_C3_INT, TMR_C4_INT };

uint32_t _tmr_flags[TMR_COUNT];
confirm_state _tmr_transmit_started[TMR_COUNT];

void TMR_init(uint8_t index);
void TMR_cleanup(uint8_t index);
void TMR_IRQHandler(uint8_t index);

confirm_state TMR_TestInterrupts(void)
{
  for (uint8_t i = 0; i < TMR_COUNT; ++i)
  {
	_tmrIndex = i;
	_tmr_flags[i] = 0;
	TMR_init(i);
	while (_tmr_flags[i] != TMR_OVF_FLAG);
	TMR_cleanup(i);
  }
  return TRUE;
}

void TMR_IRQHandler(uint8_t index)
{
  if (tmr_flag_get(_tmrs[index], TMR_OVF_FLAG) == SET)
  {
	_tmr_flags[index] |= TMR_OVF_FLAG;
	tmr_flag_clear(_tmrs[index], TMR_OVF_FLAG);
	tmr_interrupt_enable(_tmrs[index], TMR_OVF_INT, FALSE);
  }
  if (tmr_flag_get(_tmrs[index], TMR_C4_FLAG) == SET)
  {
	_tmr_flags[index] |= TMR_C4_FLAG;
	tmr_flag_clear(_tmrs[index], TMR_C4_FLAG);
	tmr_interrupt_enable(_tmrs[index], TMR_C4_INT, FALSE);
  }
}

/**
  * @brief  timer setup code
  * @param  none
  * @retval none
  */
void TMR_init(uint8_t index)
{
  crm_periph_clock_enable(_tmr_crm[index], TRUE);
  uint16_t presc = (uint16_t)(system_core_clock / 24000000) - 1;
  tmr_base_init(_tmrs[index], TMR_PR, presc);
  tmr_cnt_dir_set(_tmrs[index], TMR_COUNT_UP);
  tmr_clock_source_div_set(_tmrs[index], TMR_CLOCK_DIV1);

  tmr_period_buffer_enable(_tmrs[index], TRUE);

  tmr_flag_clear(_tmrs[index], TMR_OVF_FLAG);

  nvic_priority_group_config(NVIC_PRIORITY_GROUP_4);
  nvic_irq_enable(_tmr_irq[index], 0, 0);
  tmr_interrupt_enable(_tmrs[index], TMR_OVF_INT, TRUE);

  if (index > 0 && index < 5)
  {
	tmr_output_config_type tmr_oc_init_structure;

    tmr_output_default_para_init(&tmr_oc_init_structure);
    tmr_oc_init_structure.oc_mode = TMR_OUTPUT_CONTROL_PWM_MODE_A;
    tmr_oc_init_structure.oc_idle_state = FALSE;
    tmr_oc_init_structure.oc_polarity = TMR_OUTPUT_ACTIVE_HIGH;
    tmr_oc_init_structure.oc_output_state = TRUE;

    tmr_output_channel_config(_tmrs[index], TMR_SELECT_CHANNEL_4, &tmr_oc_init_structure);
    tmr_channel_value_set(_tmrs[index], TMR_SELECT_CHANNEL_4, TMR_DT);
    tmr_output_channel_buffer_enable(_tmrs[index], TMR_SELECT_CHANNEL_4, TRUE);

	tmr_flag_clear(_tmrs[index], TMR_C4_FLAG);
    tmr_interrupt_enable(_tmrs[index], TMR_C4_INT, TRUE);
  }

  tmr_counter_enable(_tmrs[index], TRUE);
}

void TMR_cleanup(uint8_t index)
{
  tmr_counter_enable(_tmrs[index], FALSE);
  tmr_period_buffer_enable(_tmrs[index], FALSE);
  crm_periph_clock_enable(_tmr_crm[index], FALSE);
}

void TMR1_CH_IRQHandler(void)
{
}

void TMR1_OVF_TMR10_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}

void TMR2_GLOBAL_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}

void TMR3_GLOBAL_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}

void TMR4_GLOBAL_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}

void TMR5_GLOBAL_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}

void TMR6_GLOBAL_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}

void TMR7_GLOBAL_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}

void TMR8_OVF_TMR13_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}

void TMR1_BRK_TMR9_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}

void TMR1_TRG_HALL_TMR11_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}

void TMR8_BRK_TMR12_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}

void TMR8_TRG_HALL_TMR14_IRQHandler(void)
{
  TMR_IRQHandler(_tmrIndex);
}
