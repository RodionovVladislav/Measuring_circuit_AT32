#ifndef INCLUDE_INTERRUPTS_H_
#define INCLUDE_INTERRUPTS_H_

#include "at32f403a_407_board.h"

confirm_state CAN_TestInterrupts(void);
confirm_state SPI_TestInterrupts(void);
confirm_state TMR_TestInterrupts(void);
confirm_state USART_TestInterrupts(void);

#endif /* INCLUDE_INTERRUPTS_H_ */
