################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Middlewares/fatfs/source/diskio.c \
../App/Middlewares/fatfs/source/ff.c \
../App/Middlewares/fatfs/source/ffsystem.c \
../App/Middlewares/fatfs/source/ffunicode.c 

OBJS += \
./App/Middlewares/fatfs/source/diskio.o \
./App/Middlewares/fatfs/source/ff.o \
./App/Middlewares/fatfs/source/ffsystem.o \
./App/Middlewares/fatfs/source/ffunicode.o 

C_DEPS += \
./App/Middlewares/fatfs/source/diskio.d \
./App/Middlewares/fatfs/source/ff.d \
./App/Middlewares/fatfs/source/ffsystem.d \
./App/Middlewares/fatfs/source/ffunicode.d 


# Each subdirectory must supply rules for building sources it contributes
App/Middlewares/fatfs/source/%.o App/Middlewares/fatfs/source/%.su App/Middlewares/fatfs/source/%.cyclo: ../App/Middlewares/fatfs/source/%.c App/Middlewares/fatfs/source/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Modules/modbus/include -I../App/Modules/modbus/rtu -I../App/Modules/modbus/port -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Middlewares-2f-fatfs-2f-source

clean-App-2f-Middlewares-2f-fatfs-2f-source:
	-$(RM) ./App/Middlewares/fatfs/source/diskio.cyclo ./App/Middlewares/fatfs/source/diskio.d ./App/Middlewares/fatfs/source/diskio.o ./App/Middlewares/fatfs/source/diskio.su ./App/Middlewares/fatfs/source/ff.cyclo ./App/Middlewares/fatfs/source/ff.d ./App/Middlewares/fatfs/source/ff.o ./App/Middlewares/fatfs/source/ff.su ./App/Middlewares/fatfs/source/ffsystem.cyclo ./App/Middlewares/fatfs/source/ffsystem.d ./App/Middlewares/fatfs/source/ffsystem.o ./App/Middlewares/fatfs/source/ffsystem.su ./App/Middlewares/fatfs/source/ffunicode.cyclo ./App/Middlewares/fatfs/source/ffunicode.d ./App/Middlewares/fatfs/source/ffunicode.o ./App/Middlewares/fatfs/source/ffunicode.su

.PHONY: clean-App-2f-Middlewares-2f-fatfs-2f-source

