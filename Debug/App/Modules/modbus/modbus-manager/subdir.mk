################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/modbus/modbus-manager/modbus_manager.c 

OBJS += \
./App/Modules/modbus/modbus-manager/modbus_manager.o 

C_DEPS += \
./App/Modules/modbus/modbus-manager/modbus_manager.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/modbus/modbus-manager/%.o App/Modules/modbus/modbus-manager/%.su App/Modules/modbus/modbus-manager/%.cyclo: ../App/Modules/modbus/modbus-manager/%.c App/Modules/modbus/modbus-manager/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus/modbus-slave/ -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus-slave/modbus-master -I../App/Modules/modbus/modbus-manager/ -I../App/Modules/modbus/modbus-manager/port/ -I../App/Modules/modbus/modbus-master/ -I../App/Modules/modbus/modbus-master/port -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-modbus-2f-modbus-2d-manager

clean-App-2f-Modules-2f-modbus-2f-modbus-2d-manager:
	-$(RM) ./App/Modules/modbus/modbus-manager/modbus_manager.cyclo ./App/Modules/modbus/modbus-manager/modbus_manager.d ./App/Modules/modbus/modbus-manager/modbus_manager.o ./App/Modules/modbus/modbus-manager/modbus_manager.su

.PHONY: clean-App-2f-Modules-2f-modbus-2f-modbus-2d-manager

