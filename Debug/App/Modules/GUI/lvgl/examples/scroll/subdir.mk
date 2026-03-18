################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_1.c \
../App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_2.c \
../App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_3.c \
../App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_4.c \
../App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_5.c \
../App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_6.c \
../App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_7.c \
../App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_8.c \
../App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_9.c 

OBJS += \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_1.o \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_2.o \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_3.o \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_4.o \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_5.o \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_6.o \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_7.o \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_8.o \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_9.o 

C_DEPS += \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_1.d \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_2.d \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_3.d \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_4.d \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_5.d \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_6.d \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_7.d \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_8.d \
./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_9.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/examples/scroll/%.o App/Modules/GUI/lvgl/examples/scroll/%.su App/Modules/GUI/lvgl/examples/scroll/%.cyclo: ../App/Modules/GUI/lvgl/examples/scroll/%.c App/Modules/GUI/lvgl/examples/scroll/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-examples-2f-scroll

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-examples-2f-scroll:
	-$(RM) ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_1.cyclo ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_1.d ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_1.o ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_1.su ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_2.cyclo ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_2.d ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_2.o ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_2.su ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_3.cyclo ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_3.d ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_3.o ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_3.su ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_4.cyclo ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_4.d ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_4.o ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_4.su ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_5.cyclo ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_5.d ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_5.o ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_5.su ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_6.cyclo ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_6.d ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_6.o ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_6.su ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_7.cyclo ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_7.d ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_7.o ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_7.su ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_8.cyclo ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_8.d ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_8.o ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_8.su ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_9.cyclo ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_9.d ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_9.o ./App/Modules/GUI/lvgl/examples/scroll/lv_example_scroll_9.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-examples-2f-scroll

