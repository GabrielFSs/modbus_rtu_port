################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_1.c \
../App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_2.c \
../App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_3.c \
../App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_4.c \
../App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_5.c \
../App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_6.c \
../App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_7.c 

OBJS += \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_1.o \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_2.o \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_3.o \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_4.o \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_5.o \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_6.o \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_7.o 

C_DEPS += \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_1.d \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_2.d \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_3.d \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_4.d \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_5.d \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_6.d \
./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_7.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/examples/widgets/label/%.o App/Modules/GUI/lvgl/examples/widgets/label/%.su App/Modules/GUI/lvgl/examples/widgets/label/%.cyclo: ../App/Modules/GUI/lvgl/examples/widgets/label/%.c App/Modules/GUI/lvgl/examples/widgets/label/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-examples-2f-widgets-2f-label

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-examples-2f-widgets-2f-label:
	-$(RM) ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_1.cyclo ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_1.d ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_1.o ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_1.su ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_2.cyclo ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_2.d ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_2.o ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_2.su ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_3.cyclo ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_3.d ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_3.o ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_3.su ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_4.cyclo ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_4.d ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_4.o ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_4.su ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_5.cyclo ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_5.d ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_5.o ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_5.su ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_6.cyclo ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_6.d ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_6.o ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_6.su ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_7.cyclo ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_7.d ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_7.o ./App/Modules/GUI/lvgl/examples/widgets/label/lv_example_label_7.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-examples-2f-widgets-2f-label

