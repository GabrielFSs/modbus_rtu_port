################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/src/debugging/test/lv_test_display.c \
../App/Modules/GUI/lvgl/src/debugging/test/lv_test_fs.c \
../App/Modules/GUI/lvgl/src/debugging/test/lv_test_helpers.c \
../App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev.c \
../App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev_gesture.c \
../App/Modules/GUI/lvgl/src/debugging/test/lv_test_screenshot_compare.c 

OBJS += \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_display.o \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_fs.o \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_helpers.o \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev.o \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev_gesture.o \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_screenshot_compare.o 

C_DEPS += \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_display.d \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_fs.d \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_helpers.d \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev.d \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev_gesture.d \
./App/Modules/GUI/lvgl/src/debugging/test/lv_test_screenshot_compare.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/src/debugging/test/%.o App/Modules/GUI/lvgl/src/debugging/test/%.su App/Modules/GUI/lvgl/src/debugging/test/%.cyclo: ../App/Modules/GUI/lvgl/src/debugging/test/%.c App/Modules/GUI/lvgl/src/debugging/test/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-debugging-2f-test

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-debugging-2f-test:
	-$(RM) ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_display.cyclo ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_display.d ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_display.o ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_display.su ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_fs.cyclo ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_fs.d ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_fs.o ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_fs.su ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_helpers.cyclo ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_helpers.d ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_helpers.o ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_helpers.su ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev.cyclo ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev.d ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev.o ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev.su ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev_gesture.cyclo ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev_gesture.d ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev_gesture.o ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_indev_gesture.su ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_screenshot_compare.cyclo ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_screenshot_compare.d ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_screenshot_compare.o ./App/Modules/GUI/lvgl/src/debugging/test/lv_test_screenshot_compare.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-debugging-2f-test

