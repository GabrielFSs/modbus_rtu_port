################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/modbus/modbus-sniffer/modbus_sniffer.c 

OBJS += \
./App/Modules/modbus/modbus-sniffer/modbus_sniffer.o 

C_DEPS += \
./App/Modules/modbus/modbus-sniffer/modbus_sniffer.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/modbus/modbus-sniffer/%.o App/Modules/modbus/modbus-sniffer/%.su App/Modules/modbus/modbus-sniffer/%.cyclo: ../App/Modules/modbus/modbus-sniffer/%.c App/Modules/modbus/modbus-sniffer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-modbus-2f-modbus-2d-sniffer

clean-App-2f-Modules-2f-modbus-2f-modbus-2d-sniffer:
	-$(RM) ./App/Modules/modbus/modbus-sniffer/modbus_sniffer.cyclo ./App/Modules/modbus/modbus-sniffer/modbus_sniffer.d ./App/Modules/modbus/modbus-sniffer/modbus_sniffer.o ./App/Modules/modbus/modbus-sniffer/modbus_sniffer.su

.PHONY: clean-App-2f-Modules-2f-modbus-2f-modbus-2d-sniffer

