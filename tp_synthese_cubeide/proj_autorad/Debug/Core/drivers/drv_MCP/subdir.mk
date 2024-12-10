################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/drivers/drv_MCP/drv_MCP.c 

OBJS += \
./Core/drivers/drv_MCP/drv_MCP.o 

C_DEPS += \
./Core/drivers/drv_MCP/drv_MCP.d 


# Each subdirectory must supply rules for building sources it contributes
Core/drivers/drv_MCP/%.o Core/drivers/drv_MCP/%.su Core/drivers/drv_MCP/%.cyclo: ../Core/drivers/drv_MCP/%.c Core/drivers/drv_MCP/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"/home/vincent/Documents/ese_3a/autoradio/2425-TPAutoradio/tp_synthese_cubeide/proj_autorad/Core/shell" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-drivers-2f-drv_MCP

clean-Core-2f-drivers-2f-drv_MCP:
	-$(RM) ./Core/drivers/drv_MCP/drv_MCP.cyclo ./Core/drivers/drv_MCP/drv_MCP.d ./Core/drivers/drv_MCP/drv_MCP.o ./Core/drivers/drv_MCP/drv_MCP.su

.PHONY: clean-Core-2f-drivers-2f-drv_MCP

