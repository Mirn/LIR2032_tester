################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../system/src/stm32kiss/stm32kiss_adc.c \
../system/src/stm32kiss/stm32kiss_button.c \
../system/src/stm32kiss/stm32kiss_dac.c \
../system/src/stm32kiss/stm32kiss_fifo.c \
../system/src/stm32kiss/stm32kiss_gpio.c \
../system/src/stm32kiss/stm32kiss_printf.c \
../system/src/stm32kiss/stm32kiss_startup.c \
../system/src/stm32kiss/stm32kiss_ticks.c 

OBJS += \
./system/src/stm32kiss/stm32kiss_adc.o \
./system/src/stm32kiss/stm32kiss_button.o \
./system/src/stm32kiss/stm32kiss_dac.o \
./system/src/stm32kiss/stm32kiss_fifo.o \
./system/src/stm32kiss/stm32kiss_gpio.o \
./system/src/stm32kiss/stm32kiss_printf.o \
./system/src/stm32kiss/stm32kiss_startup.o \
./system/src/stm32kiss/stm32kiss_ticks.o 

C_DEPS += \
./system/src/stm32kiss/stm32kiss_adc.d \
./system/src/stm32kiss/stm32kiss_button.d \
./system/src/stm32kiss/stm32kiss_dac.d \
./system/src/stm32kiss/stm32kiss_fifo.d \
./system/src/stm32kiss/stm32kiss_gpio.d \
./system/src/stm32kiss/stm32kiss_printf.d \
./system/src/stm32kiss/stm32kiss_startup.d \
./system/src/stm32kiss/stm32kiss_ticks.d 


# Each subdirectory must supply rules for building sources it contributes
system/src/stm32kiss/%.o: ../system/src/stm32kiss/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -O2 -fmessage-length=0 -ffunction-sections -fdata-sections -ffreestanding -Wunused -Wuninitialized -Wall -Wextra -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -DSTM32F10X_LD_VL -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -I../src -I../system/include/stm32kiss -std=gnu11 -Wbad-function-cast -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


