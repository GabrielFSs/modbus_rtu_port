################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp.c \
../App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_clock.c \
../App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_display_fsmc.c \
../App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_gpio.c \
../App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_rtc.c \
../App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_spi.c \
../App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_storage.c \
../App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_uart.c 

OBJS += \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp.o \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_clock.o \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_display_fsmc.o \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_gpio.o \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_rtc.o \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_spi.o \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_storage.o \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_uart.o 

C_DEPS += \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp.d \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_clock.d \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_display_fsmc.d \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_gpio.d \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_rtc.d \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_spi.d \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_storage.d \
./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_uart.d 


# Each subdirectory must supply rules for building sources it contributes
App/BSP/stm32f407vet6_blackpill_v2.3.1/%.o App/BSP/stm32f407vet6_blackpill_v2.3.1/%.su App/BSP/stm32f407vet6_blackpill_v2.3.1/%.cyclo: ../App/BSP/stm32f407vet6_blackpill_v2.3.1/%.c App/BSP/stm32f407vet6_blackpill_v2.3.1/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Modules/modbus/include -I../App/Modules/modbus/rtu -I../App/Modules/modbus/port -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-BSP-2f-stm32f407vet6_blackpill_v2-2e-3-2e-1

clean-App-2f-BSP-2f-stm32f407vet6_blackpill_v2-2e-3-2e-1:
	-$(RM) ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp.cyclo ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp.d ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp.o ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp.su ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_clock.cyclo ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_clock.d ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_clock.o ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_clock.su ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_display_fsmc.cyclo ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_display_fsmc.d ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_display_fsmc.o ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_display_fsmc.su ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_gpio.cyclo ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_gpio.d ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_gpio.o ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_gpio.su ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_rtc.cyclo ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_rtc.d ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_rtc.o ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_rtc.su ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_spi.cyclo ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_spi.d ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_spi.o ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_spi.su ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_storage.cyclo ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_storage.d ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_storage.o ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_storage.su ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_uart.cyclo ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_uart.d ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_uart.o ./App/BSP/stm32f407vet6_blackpill_v2.3.1/bsp_uart.su

.PHONY: clean-App-2f-BSP-2f-stm32f407vet6_blackpill_v2-2e-3-2e-1

