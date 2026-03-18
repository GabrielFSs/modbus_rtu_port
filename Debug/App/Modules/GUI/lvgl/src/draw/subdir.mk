################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/src/draw/lv_draw.c \
../App/Modules/GUI/lvgl/src/draw/lv_draw_3d.c \
../App/Modules/GUI/lvgl/src/draw/lv_draw_arc.c \
../App/Modules/GUI/lvgl/src/draw/lv_draw_blur.c \
../App/Modules/GUI/lvgl/src/draw/lv_draw_buf.c \
../App/Modules/GUI/lvgl/src/draw/lv_draw_image.c \
../App/Modules/GUI/lvgl/src/draw/lv_draw_label.c \
../App/Modules/GUI/lvgl/src/draw/lv_draw_line.c \
../App/Modules/GUI/lvgl/src/draw/lv_draw_mask.c \
../App/Modules/GUI/lvgl/src/draw/lv_draw_rect.c \
../App/Modules/GUI/lvgl/src/draw/lv_draw_triangle.c \
../App/Modules/GUI/lvgl/src/draw/lv_draw_vector.c \
../App/Modules/GUI/lvgl/src/draw/lv_image_decoder.c 

OBJS += \
./App/Modules/GUI/lvgl/src/draw/lv_draw.o \
./App/Modules/GUI/lvgl/src/draw/lv_draw_3d.o \
./App/Modules/GUI/lvgl/src/draw/lv_draw_arc.o \
./App/Modules/GUI/lvgl/src/draw/lv_draw_blur.o \
./App/Modules/GUI/lvgl/src/draw/lv_draw_buf.o \
./App/Modules/GUI/lvgl/src/draw/lv_draw_image.o \
./App/Modules/GUI/lvgl/src/draw/lv_draw_label.o \
./App/Modules/GUI/lvgl/src/draw/lv_draw_line.o \
./App/Modules/GUI/lvgl/src/draw/lv_draw_mask.o \
./App/Modules/GUI/lvgl/src/draw/lv_draw_rect.o \
./App/Modules/GUI/lvgl/src/draw/lv_draw_triangle.o \
./App/Modules/GUI/lvgl/src/draw/lv_draw_vector.o \
./App/Modules/GUI/lvgl/src/draw/lv_image_decoder.o 

C_DEPS += \
./App/Modules/GUI/lvgl/src/draw/lv_draw.d \
./App/Modules/GUI/lvgl/src/draw/lv_draw_3d.d \
./App/Modules/GUI/lvgl/src/draw/lv_draw_arc.d \
./App/Modules/GUI/lvgl/src/draw/lv_draw_blur.d \
./App/Modules/GUI/lvgl/src/draw/lv_draw_buf.d \
./App/Modules/GUI/lvgl/src/draw/lv_draw_image.d \
./App/Modules/GUI/lvgl/src/draw/lv_draw_label.d \
./App/Modules/GUI/lvgl/src/draw/lv_draw_line.d \
./App/Modules/GUI/lvgl/src/draw/lv_draw_mask.d \
./App/Modules/GUI/lvgl/src/draw/lv_draw_rect.d \
./App/Modules/GUI/lvgl/src/draw/lv_draw_triangle.d \
./App/Modules/GUI/lvgl/src/draw/lv_draw_vector.d \
./App/Modules/GUI/lvgl/src/draw/lv_image_decoder.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/src/draw/%.o App/Modules/GUI/lvgl/src/draw/%.su App/Modules/GUI/lvgl/src/draw/%.cyclo: ../App/Modules/GUI/lvgl/src/draw/%.c App/Modules/GUI/lvgl/src/draw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw:
	-$(RM) ./App/Modules/GUI/lvgl/src/draw/lv_draw.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw.d ./App/Modules/GUI/lvgl/src/draw/lv_draw.o ./App/Modules/GUI/lvgl/src/draw/lv_draw.su ./App/Modules/GUI/lvgl/src/draw/lv_draw_3d.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw_3d.d ./App/Modules/GUI/lvgl/src/draw/lv_draw_3d.o ./App/Modules/GUI/lvgl/src/draw/lv_draw_3d.su ./App/Modules/GUI/lvgl/src/draw/lv_draw_arc.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw_arc.d ./App/Modules/GUI/lvgl/src/draw/lv_draw_arc.o ./App/Modules/GUI/lvgl/src/draw/lv_draw_arc.su ./App/Modules/GUI/lvgl/src/draw/lv_draw_blur.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw_blur.d ./App/Modules/GUI/lvgl/src/draw/lv_draw_blur.o ./App/Modules/GUI/lvgl/src/draw/lv_draw_blur.su ./App/Modules/GUI/lvgl/src/draw/lv_draw_buf.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw_buf.d ./App/Modules/GUI/lvgl/src/draw/lv_draw_buf.o ./App/Modules/GUI/lvgl/src/draw/lv_draw_buf.su ./App/Modules/GUI/lvgl/src/draw/lv_draw_image.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw_image.d ./App/Modules/GUI/lvgl/src/draw/lv_draw_image.o ./App/Modules/GUI/lvgl/src/draw/lv_draw_image.su ./App/Modules/GUI/lvgl/src/draw/lv_draw_label.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw_label.d ./App/Modules/GUI/lvgl/src/draw/lv_draw_label.o ./App/Modules/GUI/lvgl/src/draw/lv_draw_label.su ./App/Modules/GUI/lvgl/src/draw/lv_draw_line.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw_line.d ./App/Modules/GUI/lvgl/src/draw/lv_draw_line.o ./App/Modules/GUI/lvgl/src/draw/lv_draw_line.su ./App/Modules/GUI/lvgl/src/draw/lv_draw_mask.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw_mask.d ./App/Modules/GUI/lvgl/src/draw/lv_draw_mask.o ./App/Modules/GUI/lvgl/src/draw/lv_draw_mask.su ./App/Modules/GUI/lvgl/src/draw/lv_draw_rect.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw_rect.d ./App/Modules/GUI/lvgl/src/draw/lv_draw_rect.o ./App/Modules/GUI/lvgl/src/draw/lv_draw_rect.su ./App/Modules/GUI/lvgl/src/draw/lv_draw_triangle.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw_triangle.d ./App/Modules/GUI/lvgl/src/draw/lv_draw_triangle.o ./App/Modules/GUI/lvgl/src/draw/lv_draw_triangle.su ./App/Modules/GUI/lvgl/src/draw/lv_draw_vector.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_draw_vector.d ./App/Modules/GUI/lvgl/src/draw/lv_draw_vector.o ./App/Modules/GUI/lvgl/src/draw/lv_draw_vector.su ./App/Modules/GUI/lvgl/src/draw/lv_image_decoder.cyclo ./App/Modules/GUI/lvgl/src/draw/lv_image_decoder.d ./App/Modules/GUI/lvgl/src/draw/lv_image_decoder.o ./App/Modules/GUI/lvgl/src/draw/lv_image_decoder.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw

