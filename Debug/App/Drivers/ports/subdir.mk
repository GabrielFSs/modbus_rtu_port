################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Drivers/ports/hal_crc_stm32f4.c \
../App/Drivers/ports/hal_display_fsmc_stm32f4.c \
../App/Drivers/ports/hal_gpio_stm32f4.c \
../App/Drivers/ports/hal_spi_stm32f4.c \
../App/Drivers/ports/hal_storage_stm32f4.c \
../App/Drivers/ports/hal_time_stm32f4.c \
../App/Drivers/ports/hal_timer_stm32f4.c \
../App/Drivers/ports/hal_uart_stm32f4.c 

OBJS += \
./App/Drivers/ports/hal_crc_stm32f4.o \
./App/Drivers/ports/hal_display_fsmc_stm32f4.o \
./App/Drivers/ports/hal_gpio_stm32f4.o \
./App/Drivers/ports/hal_spi_stm32f4.o \
./App/Drivers/ports/hal_storage_stm32f4.o \
./App/Drivers/ports/hal_time_stm32f4.o \
./App/Drivers/ports/hal_timer_stm32f4.o \
./App/Drivers/ports/hal_uart_stm32f4.o 

C_DEPS += \
./App/Drivers/ports/hal_crc_stm32f4.d \
./App/Drivers/ports/hal_display_fsmc_stm32f4.d \
./App/Drivers/ports/hal_gpio_stm32f4.d \
./App/Drivers/ports/hal_spi_stm32f4.d \
./App/Drivers/ports/hal_storage_stm32f4.d \
./App/Drivers/ports/hal_time_stm32f4.d \
./App/Drivers/ports/hal_timer_stm32f4.d \
./App/Drivers/ports/hal_uart_stm32f4.d 


# Each subdirectory must supply rules for building sources it contributes
App/Drivers/ports/%.o App/Drivers/ports/%.su App/Drivers/ports/%.cyclo: ../App/Drivers/ports/%.c App/Drivers/ports/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Modules/modbus/include -I../App/Modules/modbus/rtu -I../App/Modules/modbus/port -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Drivers-2f-ports

clean-App-2f-Drivers-2f-ports:
	-$(RM) ./App/Drivers/ports/hal_crc_stm32f4.cyclo ./App/Drivers/ports/hal_crc_stm32f4.d ./App/Drivers/ports/hal_crc_stm32f4.o ./App/Drivers/ports/hal_crc_stm32f4.su ./App/Drivers/ports/hal_display_fsmc_stm32f4.cyclo ./App/Drivers/ports/hal_display_fsmc_stm32f4.d ./App/Drivers/ports/hal_display_fsmc_stm32f4.o ./App/Drivers/ports/hal_display_fsmc_stm32f4.su ./App/Drivers/ports/hal_gpio_stm32f4.cyclo ./App/Drivers/ports/hal_gpio_stm32f4.d ./App/Drivers/ports/hal_gpio_stm32f4.o ./App/Drivers/ports/hal_gpio_stm32f4.su ./App/Drivers/ports/hal_spi_stm32f4.cyclo ./App/Drivers/ports/hal_spi_stm32f4.d ./App/Drivers/ports/hal_spi_stm32f4.o ./App/Drivers/ports/hal_spi_stm32f4.su ./App/Drivers/ports/hal_storage_stm32f4.cyclo ./App/Drivers/ports/hal_storage_stm32f4.d ./App/Drivers/ports/hal_storage_stm32f4.o ./App/Drivers/ports/hal_storage_stm32f4.su ./App/Drivers/ports/hal_time_stm32f4.cyclo ./App/Drivers/ports/hal_time_stm32f4.d ./App/Drivers/ports/hal_time_stm32f4.o ./App/Drivers/ports/hal_time_stm32f4.su ./App/Drivers/ports/hal_timer_stm32f4.cyclo ./App/Drivers/ports/hal_timer_stm32f4.d ./App/Drivers/ports/hal_timer_stm32f4.o ./App/Drivers/ports/hal_timer_stm32f4.su ./App/Drivers/ports/hal_uart_stm32f4.cyclo ./App/Drivers/ports/hal_uart_stm32f4.d ./App/Drivers/ports/hal_uart_stm32f4.o ./App/Drivers/ports/hal_uart_stm32f4.su

.PHONY: clean-App-2f-Drivers-2f-ports

