################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_buf_g2d.c \
../App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d.c \
../App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_fill.c \
../App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_img.c \
../App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_buf_map.c \
../App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_utils.c 

OBJS += \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_buf_g2d.o \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d.o \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_fill.o \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_img.o \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_buf_map.o \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_utils.o 

C_DEPS += \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_buf_g2d.d \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d.d \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_fill.d \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_img.d \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_buf_map.d \
./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_utils.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/src/draw/nxp/g2d/%.o App/Modules/GUI/lvgl/src/draw/nxp/g2d/%.su App/Modules/GUI/lvgl/src/draw/nxp/g2d/%.cyclo: ../App/Modules/GUI/lvgl/src/draw/nxp/g2d/%.c App/Modules/GUI/lvgl/src/draw/nxp/g2d/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-nxp-2f-g2d

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-nxp-2f-g2d:
	-$(RM) ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_buf_g2d.cyclo ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_buf_g2d.d ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_buf_g2d.o ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_buf_g2d.su ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d.cyclo ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d.d ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d.o ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d.su ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_fill.cyclo ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_fill.d ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_fill.o ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_fill.su ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_img.cyclo ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_img.d ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_img.o ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_draw_g2d_img.su ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_buf_map.cyclo ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_buf_map.d ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_buf_map.o ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_buf_map.su ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_utils.cyclo ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_utils.d ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_utils.o ./App/Modules/GUI/lvgl/src/draw/nxp/g2d/lv_g2d_utils.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-nxp-2f-g2d

