################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/modbus/rtu/mbcrc.c \
../App/Modules/modbus/rtu/mbrtu.c 

OBJS += \
./App/Modules/modbus/rtu/mbcrc.o \
./App/Modules/modbus/rtu/mbrtu.o 

C_DEPS += \
./App/Modules/modbus/rtu/mbcrc.d \
./App/Modules/modbus/rtu/mbrtu.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/modbus/rtu/%.o App/Modules/modbus/rtu/%.su App/Modules/modbus/rtu/%.cyclo: ../App/Modules/modbus/rtu/%.c App/Modules/modbus/rtu/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Modules/modbus/include -I../App/Modules/modbus/rtu -I../App/Modules/modbus/port -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-modbus-2f-rtu

clean-App-2f-Modules-2f-modbus-2f-rtu:
	-$(RM) ./App/Modules/modbus/rtu/mbcrc.cyclo ./App/Modules/modbus/rtu/mbcrc.d ./App/Modules/modbus/rtu/mbcrc.o ./App/Modules/modbus/rtu/mbcrc.su ./App/Modules/modbus/rtu/mbrtu.cyclo ./App/Modules/modbus/rtu/mbrtu.d ./App/Modules/modbus/rtu/mbrtu.o ./App/Modules/modbus/rtu/mbrtu.su

.PHONY: clean-App-2f-Modules-2f-modbus-2f-rtu

