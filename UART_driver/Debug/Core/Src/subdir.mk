################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/adc.c \
../Core/Src/gpio.c \
../Core/Src/rtc.c \
../Core/Src/stm32l5xx_hal_msp.c \
../Core/Src/stm32l5xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32l5xx.c \
../Core/Src/ucpd.c \
../Core/Src/usart.c \
../Core/Src/usb.c 

CPP_SRCS += \
../Core/Src/CircularBuffer.cpp \
../Core/Src/LOS_D_UART.cpp \
../Core/Src/main.cpp 

C_DEPS += \
./Core/Src/adc.d \
./Core/Src/gpio.d \
./Core/Src/rtc.d \
./Core/Src/stm32l5xx_hal_msp.d \
./Core/Src/stm32l5xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32l5xx.d \
./Core/Src/ucpd.d \
./Core/Src/usart.d \
./Core/Src/usb.d 

OBJS += \
./Core/Src/CircularBuffer.o \
./Core/Src/LOS_D_UART.o \
./Core/Src/adc.o \
./Core/Src/gpio.o \
./Core/Src/main.o \
./Core/Src/rtc.o \
./Core/Src/stm32l5xx_hal_msp.o \
./Core/Src/stm32l5xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32l5xx.o \
./Core/Src/ucpd.o \
./Core/Src/usart.o \
./Core/Src/usb.o 

CPP_DEPS += \
./Core/Src/CircularBuffer.d \
./Core/Src/LOS_D_UART.d \
./Core/Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m33 -std=gnu++14 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32L552xx -c -I../Core/Inc -I../Drivers/STM32L5xx_HAL_Driver/Inc -I../Drivers/STM32L5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m33 -std=gnu11 -g3 -DDEBUG -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -DSTM32L552xx -c -I../Core/Inc -I../Drivers/STM32L5xx_HAL_Driver/Inc -I../Drivers/STM32L5xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L5xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/CircularBuffer.d ./Core/Src/CircularBuffer.o ./Core/Src/CircularBuffer.su ./Core/Src/LOS_D_UART.d ./Core/Src/LOS_D_UART.o ./Core/Src/LOS_D_UART.su ./Core/Src/adc.d ./Core/Src/adc.o ./Core/Src/adc.su ./Core/Src/gpio.d ./Core/Src/gpio.o ./Core/Src/gpio.su ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/rtc.d ./Core/Src/rtc.o ./Core/Src/rtc.su ./Core/Src/stm32l5xx_hal_msp.d ./Core/Src/stm32l5xx_hal_msp.o ./Core/Src/stm32l5xx_hal_msp.su ./Core/Src/stm32l5xx_it.d ./Core/Src/stm32l5xx_it.o ./Core/Src/stm32l5xx_it.su ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32l5xx.d ./Core/Src/system_stm32l5xx.o ./Core/Src/system_stm32l5xx.su ./Core/Src/ucpd.d ./Core/Src/ucpd.o ./Core/Src/ucpd.su ./Core/Src/usart.d ./Core/Src/usart.o ./Core/Src/usart.su ./Core/Src/usb.d ./Core/Src/usb.o ./Core/Src/usb.su

.PHONY: clean-Core-2f-Src

