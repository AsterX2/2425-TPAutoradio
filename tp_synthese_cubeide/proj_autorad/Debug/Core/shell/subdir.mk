################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/shell/drv_uart1.c \
../Core/shell/shell.c 

OBJS += \
./Core/shell/drv_uart1.o \
./Core/shell/shell.o 

C_DEPS += \
./Core/shell/drv_uart1.d \
./Core/shell/shell.d 


# Each subdirectory must supply rules for building sources it contributes
Core/shell/%.o Core/shell/%.su Core/shell/%.cyclo: ../Core/shell/%.c Core/shell/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/vincent/Documents/ese_3a/autoradio/2425-TPAutoradio/tp_synthese_cubeide/proj_autorad/Core/shell" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-shell

clean-Core-2f-shell:
	-$(RM) ./Core/shell/drv_uart1.cyclo ./Core/shell/drv_uart1.d ./Core/shell/drv_uart1.o ./Core/shell/drv_uart1.su ./Core/shell/shell.cyclo ./Core/shell/shell.d ./Core/shell/shell.o ./Core/shell/shell.su

.PHONY: clean-Core-2f-shell

