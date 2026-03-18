################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_3d.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_arc.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_border.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_box_shadow.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_fill.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_grad.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_image.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_label.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_layer.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_line.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_mask_rect.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_triangle.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_vector.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_fbo_cache.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_image_cache.c \
../App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_utils.c 

OBJS += \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_3d.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_arc.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_border.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_box_shadow.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_fill.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_grad.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_image.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_label.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_layer.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_line.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_mask_rect.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_triangle.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_vector.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_fbo_cache.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_image_cache.o \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_utils.o 

C_DEPS += \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_3d.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_arc.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_border.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_box_shadow.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_fill.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_grad.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_image.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_label.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_layer.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_line.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_mask_rect.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_triangle.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_vector.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_fbo_cache.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_image_cache.d \
./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_utils.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/src/draw/nanovg/%.o App/Modules/GUI/lvgl/src/draw/nanovg/%.su App/Modules/GUI/lvgl/src/draw/nanovg/%.cyclo: ../App/Modules/GUI/lvgl/src/draw/nanovg/%.c App/Modules/GUI/lvgl/src/draw/nanovg/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-nanovg

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-nanovg:
	-$(RM) ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_3d.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_3d.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_3d.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_3d.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_arc.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_arc.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_arc.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_arc.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_border.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_border.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_border.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_border.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_box_shadow.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_box_shadow.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_box_shadow.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_box_shadow.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_fill.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_fill.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_fill.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_fill.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_grad.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_grad.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_grad.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_grad.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_image.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_image.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_image.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_image.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_label.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_label.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_label.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_label.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_layer.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_layer.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_layer.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_layer.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_line.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_line.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_line.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_line.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_mask_rect.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_mask_rect.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_mask_rect.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_mask_rect.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_triangle.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_triangle.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_triangle.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_triangle.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_vector.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_vector.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_vector.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_draw_nanovg_vector.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_fbo_cache.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_fbo_cache.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_fbo_cache.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_fbo_cache.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_image_cache.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_image_cache.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_image_cache.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_image_cache.su ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_utils.cyclo ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_utils.d ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_utils.o ./App/Modules/GUI/lvgl/src/draw/nanovg/lv_nanovg_utils.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-nanovg

