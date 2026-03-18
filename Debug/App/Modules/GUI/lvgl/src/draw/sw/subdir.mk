################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_arc.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_blur.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_border.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_box_shadow.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_fill.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_grad.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_img.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_letter.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_line.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask_rect.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_transform.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_triangle.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_utils.c \
../App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_vector.c 

OBJS += \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_arc.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_blur.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_border.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_box_shadow.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_fill.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_grad.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_img.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_letter.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_line.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask_rect.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_transform.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_triangle.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_utils.o \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_vector.o 

C_DEPS += \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_arc.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_blur.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_border.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_box_shadow.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_fill.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_grad.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_img.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_letter.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_line.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask_rect.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_transform.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_triangle.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_utils.d \
./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_vector.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/src/draw/sw/%.o App/Modules/GUI/lvgl/src/draw/sw/%.su App/Modules/GUI/lvgl/src/draw/sw/%.cyclo: ../App/Modules/GUI/lvgl/src/draw/sw/%.c App/Modules/GUI/lvgl/src/draw/sw/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-sw

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-sw:
	-$(RM) ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_arc.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_arc.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_arc.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_arc.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_blur.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_blur.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_blur.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_blur.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_border.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_border.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_border.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_border.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_box_shadow.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_box_shadow.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_box_shadow.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_box_shadow.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_fill.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_fill.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_fill.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_fill.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_grad.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_grad.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_grad.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_grad.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_img.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_img.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_img.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_img.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_letter.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_letter.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_letter.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_letter.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_line.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_line.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_line.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_line.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask_rect.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask_rect.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask_rect.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_mask_rect.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_transform.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_transform.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_transform.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_transform.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_triangle.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_triangle.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_triangle.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_triangle.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_utils.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_utils.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_utils.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_utils.su ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_vector.cyclo ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_vector.d ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_vector.o ./App/Modules/GUI/lvgl/src/draw/sw/lv_draw_sw_vector.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-sw

