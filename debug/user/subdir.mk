################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/at32f403a_407_clock.c \
../user/at32f403a_407_int.c \
../user/can_interrupt.c \
../user/main.c \
../user/spi_interrupts.c \
../user/tmr_interrupt.c \
../user/usart_interrupts.c 

OBJS += \
./user/at32f403a_407_clock.o \
./user/at32f403a_407_int.o \
./user/can_interrupt.o \
./user/main.o \
./user/spi_interrupts.o \
./user/tmr_interrupt.o \
./user/usart_interrupts.o 

C_DEPS += \
./user/at32f403a_407_clock.d \
./user/at32f403a_407_int.d \
./user/can_interrupt.d \
./user/main.d \
./user/spi_interrupts.d \
./user/tmr_interrupt.d \
./user/usart_interrupts.d 


# Each subdirectory must supply rules for building sources it contributes
user/%.o: ../user/%.c user/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -DAT_START_F407_V1 -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DAT32F407VGT7 -DUSE_STDPERIPH_DRIVER -I"../include" -I"../include/libraries/drivers/inc" -I"../include/libraries/cmsis/cm4/core_support" -I"../include/libraries/cmsis/cm4/device_support" -std=c99 -Wmissing-prototypes -Wstrict-prototypes -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


