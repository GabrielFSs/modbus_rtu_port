################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Drivers/hal/hal_crc.c \
../App/Drivers/hal/hal_display.c \
../App/Drivers/hal/hal_display_port.c \
../App/Drivers/hal/hal_gpio.c \
../App/Drivers/hal/hal_rtc.c \
../App/Drivers/hal/hal_spi.c \
../App/Drivers/hal/hal_storage.c \
../App/Drivers/hal/hal_time.c \
../App/Drivers/hal/hal_timer.c \
../App/Drivers/hal/hal_uart.c 

OBJS += \
./App/Drivers/hal/hal_crc.o \
./App/Drivers/hal/hal_display.o \
./App/Drivers/hal/hal_display_port.o \
./App/Drivers/hal/hal_gpio.o \
./App/Drivers/hal/hal_rtc.o \
./App/Drivers/hal/hal_spi.o \
./App/Drivers/hal/hal_storage.o \
./App/Drivers/hal/hal_time.o \
./App/Drivers/hal/hal_timer.o \
./App/Drivers/hal/hal_uart.o 

C_DEPS += \
./App/Drivers/hal/hal_crc.d \
./App/Drivers/hal/hal_display.d \
./App/Drivers/hal/hal_display_port.d \
./App/Drivers/hal/hal_gpio.d \
./App/Drivers/hal/hal_rtc.d \
./App/Drivers/hal/hal_spi.d \
./App/Drivers/hal/hal_storage.d \
./App/Drivers/hal/hal_time.d \
./App/Drivers/hal/hal_timer.d \
./App/Drivers/hal/hal_uart.d 


# Each subdirectory must supply rules for building sources it contributes
App/Drivers/hal/%.o App/Drivers/hal/%.su App/Drivers/hal/%.cyclo: ../App/Drivers/hal/%.c App/Drivers/hal/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Modules/modbus/include -I../App/Modules/modbus/rtu -I../App/Modules/modbus/port -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Drivers-2f-hal

clean-App-2f-Drivers-2f-hal:
	-$(RM) ./App/Drivers/hal/hal_crc.cyclo ./App/Drivers/hal/hal_crc.d ./App/Drivers/hal/hal_crc.o ./App/Drivers/hal/hal_crc.su ./App/Drivers/hal/hal_display.cyclo ./App/Drivers/hal/hal_display.d ./App/Drivers/hal/hal_display.o ./App/Drivers/hal/hal_display.su ./App/Drivers/hal/hal_display_port.cyclo ./App/Drivers/hal/hal_display_port.d ./App/Drivers/hal/hal_display_port.o ./App/Drivers/hal/hal_display_port.su ./App/Drivers/hal/hal_gpio.cyclo ./App/Drivers/hal/hal_gpio.d ./App/Drivers/hal/hal_gpio.o ./App/Drivers/hal/hal_gpio.su ./App/Drivers/hal/hal_rtc.cyclo ./App/Drivers/hal/hal_rtc.d ./App/Drivers/hal/hal_rtc.o ./App/Drivers/hal/hal_rtc.su ./App/Drivers/hal/hal_spi.cyclo ./App/Drivers/hal/hal_spi.d ./App/Drivers/hal/hal_spi.o ./App/Drivers/hal/hal_spi.su ./App/Drivers/hal/hal_storage.cyclo ./App/Drivers/hal/hal_storage.d ./App/Drivers/hal/hal_storage.o ./App/Drivers/hal/hal_storage.su ./App/Drivers/hal/hal_time.cyclo ./App/Drivers/hal/hal_time.d ./App/Drivers/hal/hal_time.o ./App/Drivers/hal/hal_time.su ./App/Drivers/hal/hal_timer.cyclo ./App/Drivers/hal/hal_timer.d ./App/Drivers/hal/hal_timer.o ./App/Drivers/hal/hal_timer.su ./App/Drivers/hal/hal_uart.cyclo ./App/Drivers/hal/hal_uart.d ./App/Drivers/hal/hal_uart.o ./App/Drivers/hal/hal_uart.su

.PHONY: clean-App-2f-Drivers-2f-hal

