################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/examples/assets/animimg001.c \
../App/Modules/GUI/lvgl/examples/assets/animimg002.c \
../App/Modules/GUI/lvgl/examples/assets/animimg003.c \
../App/Modules/GUI/lvgl/examples/assets/img_caret_down.c \
../App/Modules/GUI/lvgl/examples/assets/img_cogwheel_argb.c \
../App/Modules/GUI/lvgl/examples/assets/img_cogwheel_indexed16.c \
../App/Modules/GUI/lvgl/examples/assets/img_cogwheel_rgb.c \
../App/Modules/GUI/lvgl/examples/assets/img_hand.c \
../App/Modules/GUI/lvgl/examples/assets/img_multilang_like.c \
../App/Modules/GUI/lvgl/examples/assets/img_skew_strip.c \
../App/Modules/GUI/lvgl/examples/assets/img_star.c \
../App/Modules/GUI/lvgl/examples/assets/img_svg_img.c \
../App/Modules/GUI/lvgl/examples/assets/img_transform_avatar_15.c \
../App/Modules/GUI/lvgl/examples/assets/imgbtn_left.c \
../App/Modules/GUI/lvgl/examples/assets/imgbtn_mid.c \
../App/Modules/GUI/lvgl/examples/assets/imgbtn_right.c 

OBJS += \
./App/Modules/GUI/lvgl/examples/assets/animimg001.o \
./App/Modules/GUI/lvgl/examples/assets/animimg002.o \
./App/Modules/GUI/lvgl/examples/assets/animimg003.o \
./App/Modules/GUI/lvgl/examples/assets/img_caret_down.o \
./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_argb.o \
./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_indexed16.o \
./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_rgb.o \
./App/Modules/GUI/lvgl/examples/assets/img_hand.o \
./App/Modules/GUI/lvgl/examples/assets/img_multilang_like.o \
./App/Modules/GUI/lvgl/examples/assets/img_skew_strip.o \
./App/Modules/GUI/lvgl/examples/assets/img_star.o \
./App/Modules/GUI/lvgl/examples/assets/img_svg_img.o \
./App/Modules/GUI/lvgl/examples/assets/img_transform_avatar_15.o \
./App/Modules/GUI/lvgl/examples/assets/imgbtn_left.o \
./App/Modules/GUI/lvgl/examples/assets/imgbtn_mid.o \
./App/Modules/GUI/lvgl/examples/assets/imgbtn_right.o 

C_DEPS += \
./App/Modules/GUI/lvgl/examples/assets/animimg001.d \
./App/Modules/GUI/lvgl/examples/assets/animimg002.d \
./App/Modules/GUI/lvgl/examples/assets/animimg003.d \
./App/Modules/GUI/lvgl/examples/assets/img_caret_down.d \
./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_argb.d \
./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_indexed16.d \
./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_rgb.d \
./App/Modules/GUI/lvgl/examples/assets/img_hand.d \
./App/Modules/GUI/lvgl/examples/assets/img_multilang_like.d \
./App/Modules/GUI/lvgl/examples/assets/img_skew_strip.d \
./App/Modules/GUI/lvgl/examples/assets/img_star.d \
./App/Modules/GUI/lvgl/examples/assets/img_svg_img.d \
./App/Modules/GUI/lvgl/examples/assets/img_transform_avatar_15.d \
./App/Modules/GUI/lvgl/examples/assets/imgbtn_left.d \
./App/Modules/GUI/lvgl/examples/assets/imgbtn_mid.d \
./App/Modules/GUI/lvgl/examples/assets/imgbtn_right.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/examples/assets/%.o App/Modules/GUI/lvgl/examples/assets/%.su App/Modules/GUI/lvgl/examples/assets/%.cyclo: ../App/Modules/GUI/lvgl/examples/assets/%.c App/Modules/GUI/lvgl/examples/assets/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-examples-2f-assets

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-examples-2f-assets:
	-$(RM) ./App/Modules/GUI/lvgl/examples/assets/animimg001.cyclo ./App/Modules/GUI/lvgl/examples/assets/animimg001.d ./App/Modules/GUI/lvgl/examples/assets/animimg001.o ./App/Modules/GUI/lvgl/examples/assets/animimg001.su ./App/Modules/GUI/lvgl/examples/assets/animimg002.cyclo ./App/Modules/GUI/lvgl/examples/assets/animimg002.d ./App/Modules/GUI/lvgl/examples/assets/animimg002.o ./App/Modules/GUI/lvgl/examples/assets/animimg002.su ./App/Modules/GUI/lvgl/examples/assets/animimg003.cyclo ./App/Modules/GUI/lvgl/examples/assets/animimg003.d ./App/Modules/GUI/lvgl/examples/assets/animimg003.o ./App/Modules/GUI/lvgl/examples/assets/animimg003.su ./App/Modules/GUI/lvgl/examples/assets/img_caret_down.cyclo ./App/Modules/GUI/lvgl/examples/assets/img_caret_down.d ./App/Modules/GUI/lvgl/examples/assets/img_caret_down.o ./App/Modules/GUI/lvgl/examples/assets/img_caret_down.su ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_argb.cyclo ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_argb.d ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_argb.o ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_argb.su ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_indexed16.cyclo ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_indexed16.d ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_indexed16.o ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_indexed16.su ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_rgb.cyclo ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_rgb.d ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_rgb.o ./App/Modules/GUI/lvgl/examples/assets/img_cogwheel_rgb.su ./App/Modules/GUI/lvgl/examples/assets/img_hand.cyclo ./App/Modules/GUI/lvgl/examples/assets/img_hand.d ./App/Modules/GUI/lvgl/examples/assets/img_hand.o ./App/Modules/GUI/lvgl/examples/assets/img_hand.su ./App/Modules/GUI/lvgl/examples/assets/img_multilang_like.cyclo ./App/Modules/GUI/lvgl/examples/assets/img_multilang_like.d ./App/Modules/GUI/lvgl/examples/assets/img_multilang_like.o ./App/Modules/GUI/lvgl/examples/assets/img_multilang_like.su ./App/Modules/GUI/lvgl/examples/assets/img_skew_strip.cyclo ./App/Modules/GUI/lvgl/examples/assets/img_skew_strip.d ./App/Modules/GUI/lvgl/examples/assets/img_skew_strip.o ./App/Modules/GUI/lvgl/examples/assets/img_skew_strip.su ./App/Modules/GUI/lvgl/examples/assets/img_star.cyclo ./App/Modules/GUI/lvgl/examples/assets/img_star.d ./App/Modules/GUI/lvgl/examples/assets/img_star.o ./App/Modules/GUI/lvgl/examples/assets/img_star.su ./App/Modules/GUI/lvgl/examples/assets/img_svg_img.cyclo ./App/Modules/GUI/lvgl/examples/assets/img_svg_img.d ./App/Modules/GUI/lvgl/examples/assets/img_svg_img.o ./App/Modules/GUI/lvgl/examples/assets/img_svg_img.su ./App/Modules/GUI/lvgl/examples/assets/img_transform_avatar_15.cyclo ./App/Modules/GUI/lvgl/examples/assets/img_transform_avatar_15.d ./App/Modules/GUI/lvgl/examples/assets/img_transform_avatar_15.o ./App/Modules/GUI/lvgl/examples/assets/img_transform_avatar_15.su ./App/Modules/GUI/lvgl/examples/assets/imgbtn_left.cyclo ./App/Modules/GUI/lvgl/examples/assets/imgbtn_left.d ./App/Modules/GUI/lvgl/examples/assets/imgbtn_left.o ./App/Modules/GUI/lvgl/examples/assets/imgbtn_left.su ./App/Modules/GUI/lvgl/examples/assets/imgbtn_mid.cyclo ./App/Modules/GUI/lvgl/examples/assets/imgbtn_mid.d ./App/Modules/GUI/lvgl/examples/assets/imgbtn_mid.o ./App/Modules/GUI/lvgl/examples/assets/imgbtn_mid.su ./App/Modules/GUI/lvgl/examples/assets/imgbtn_right.cyclo ./App/Modules/GUI/lvgl/examples/assets/imgbtn_right.d ./App/Modules/GUI/lvgl/examples/assets/imgbtn_right.o ./App/Modules/GUI/lvgl/examples/assets/imgbtn_right.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-examples-2f-assets

