################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/modbus/mb.c \
../App/Modules/modbus/modbus_map.c \
../App/Modules/modbus/modbus_slave_cb.c 

OBJS += \
./App/Modules/modbus/mb.o \
./App/Modules/modbus/modbus_map.o \
./App/Modules/modbus/modbus_slave_cb.o 

C_DEPS += \
./App/Modules/modbus/mb.d \
./App/Modules/modbus/modbus_map.d \
./App/Modules/modbus/modbus_slave_cb.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/modbus/%.o App/Modules/modbus/%.su App/Modules/modbus/%.cyclo: ../App/Modules/modbus/%.c App/Modules/modbus/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Modules/modbus/include -I../App/Modules/modbus/rtu -I../App/Modules/modbus/port -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-modbus

clean-App-2f-Modules-2f-modbus:
	-$(RM) ./App/Modules/modbus/mb.cyclo ./App/Modules/modbus/mb.d ./App/Modules/modbus/mb.o ./App/Modules/modbus/mb.su ./App/Modules/modbus/modbus_map.cyclo ./App/Modules/modbus/modbus_map.d ./App/Modules/modbus/modbus_map.o ./App/Modules/modbus/modbus_map.su ./App/Modules/modbus/modbus_slave_cb.cyclo ./App/Modules/modbus/modbus_slave_cb.d ./App/Modules/modbus/modbus_slave_cb.o ./App/Modules/modbus/modbus_slave_cb.su

.PHONY: clean-App-2f-Modules-2f-modbus

