################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_arc.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_border.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_fill.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_img.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_label.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_layer.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_line.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_stm32_hal.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_triangle.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_utils.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_vector.c \
../App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_nema_gfx_path.c 

OBJS += \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_arc.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_border.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_fill.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_img.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_label.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_layer.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_line.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_stm32_hal.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_triangle.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_utils.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_vector.o \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_nema_gfx_path.o 

C_DEPS += \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_arc.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_border.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_fill.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_img.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_label.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_layer.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_line.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_stm32_hal.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_triangle.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_utils.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_vector.d \
./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_nema_gfx_path.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/src/draw/nema_gfx/%.o App/Modules/GUI/lvgl/src/draw/nema_gfx/%.su App/Modules/GUI/lvgl/src/draw/nema_gfx/%.cyclo: ../App/Modules/GUI/lvgl/src/draw/nema_gfx/%.c App/Modules/GUI/lvgl/src/draw/nema_gfx/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-nema_gfx

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-nema_gfx:
	-$(RM) ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_arc.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_arc.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_arc.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_arc.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_border.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_border.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_border.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_border.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_fill.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_fill.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_fill.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_fill.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_img.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_img.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_img.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_img.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_label.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_label.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_label.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_label.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_layer.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_layer.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_layer.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_layer.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_line.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_line.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_line.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_line.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_stm32_hal.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_stm32_hal.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_stm32_hal.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_stm32_hal.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_triangle.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_triangle.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_triangle.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_triangle.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_utils.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_utils.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_utils.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_utils.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_vector.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_vector.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_vector.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_draw_nema_gfx_vector.su ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_nema_gfx_path.cyclo ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_nema_gfx_path.d ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_nema_gfx_path.o ./App/Modules/GUI/lvgl/src/draw/nema_gfx/lv_nema_gfx_path.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-nema_gfx

