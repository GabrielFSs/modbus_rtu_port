################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/modbus/functions/mbfunccoils.c \
../App/Modules/modbus/functions/mbfuncdiag.c \
../App/Modules/modbus/functions/mbfuncdisc.c \
../App/Modules/modbus/functions/mbfuncfile.c \
../App/Modules/modbus/functions/mbfuncholding.c \
../App/Modules/modbus/functions/mbfuncinput.c \
../App/Modules/modbus/functions/mbfuncother.c \
../App/Modules/modbus/functions/mbutils.c 

OBJS += \
./App/Modules/modbus/functions/mbfunccoils.o \
./App/Modules/modbus/functions/mbfuncdiag.o \
./App/Modules/modbus/functions/mbfuncdisc.o \
./App/Modules/modbus/functions/mbfuncfile.o \
./App/Modules/modbus/functions/mbfuncholding.o \
./App/Modules/modbus/functions/mbfuncinput.o \
./App/Modules/modbus/functions/mbfuncother.o \
./App/Modules/modbus/functions/mbutils.o 

C_DEPS += \
./App/Modules/modbus/functions/mbfunccoils.d \
./App/Modules/modbus/functions/mbfuncdiag.d \
./App/Modules/modbus/functions/mbfuncdisc.d \
./App/Modules/modbus/functions/mbfuncfile.d \
./App/Modules/modbus/functions/mbfuncholding.d \
./App/Modules/modbus/functions/mbfuncinput.d \
./App/Modules/modbus/functions/mbfuncother.d \
./App/Modules/modbus/functions/mbutils.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/modbus/functions/%.o App/Modules/modbus/functions/%.su App/Modules/modbus/functions/%.cyclo: ../App/Modules/modbus/functions/%.c App/Modules/modbus/functions/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Modules/modbus/include -I../App/Modules/modbus/rtu -I../App/Modules/modbus/port -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-modbus-2f-functions

clean-App-2f-Modules-2f-modbus-2f-functions:
	-$(RM) ./App/Modules/modbus/functions/mbfunccoils.cyclo ./App/Modules/modbus/functions/mbfunccoils.d ./App/Modules/modbus/functions/mbfunccoils.o ./App/Modules/modbus/functions/mbfunccoils.su ./App/Modules/modbus/functions/mbfuncdiag.cyclo ./App/Modules/modbus/functions/mbfuncdiag.d ./App/Modules/modbus/functions/mbfuncdiag.o ./App/Modules/modbus/functions/mbfuncdiag.su ./App/Modules/modbus/functions/mbfuncdisc.cyclo ./App/Modules/modbus/functions/mbfuncdisc.d ./App/Modules/modbus/functions/mbfuncdisc.o ./App/Modules/modbus/functions/mbfuncdisc.su ./App/Modules/modbus/functions/mbfuncfile.cyclo ./App/Modules/modbus/functions/mbfuncfile.d ./App/Modules/modbus/functions/mbfuncfile.o ./App/Modules/modbus/functions/mbfuncfile.su ./App/Modules/modbus/functions/mbfuncholding.cyclo ./App/Modules/modbus/functions/mbfuncholding.d ./App/Modules/modbus/functions/mbfuncholding.o ./App/Modules/modbus/functions/mbfuncholding.su ./App/Modules/modbus/functions/mbfuncinput.cyclo ./App/Modules/modbus/functions/mbfuncinput.d ./App/Modules/modbus/functions/mbfuncinput.o ./App/Modules/modbus/functions/mbfuncinput.su ./App/Modules/modbus/functions/mbfuncother.cyclo ./App/Modules/modbus/functions/mbfuncother.d ./App/Modules/modbus/functions/mbfuncother.o ./App/Modules/modbus/functions/mbfuncother.su ./App/Modules/modbus/functions/mbutils.cyclo ./App/Modules/modbus/functions/mbutils.d ./App/Modules/modbus/functions/mbutils.o ./App/Modules/modbus/functions/mbutils.su

.PHONY: clean-App-2f-Modules-2f-modbus-2f-functions

