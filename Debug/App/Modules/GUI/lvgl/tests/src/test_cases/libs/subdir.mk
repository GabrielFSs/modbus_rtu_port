################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_barcode.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bin_decoder.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bmp.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_ffmpeg.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_font_stress.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_freetype.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_gif.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libjpeg_turbo.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libpng.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libwebp.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_lodepng.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_memmove.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_qrcode.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_svg_decoder.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tiny_ttf.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tjpgd.c 

OBJS += \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_barcode.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bin_decoder.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bmp.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_ffmpeg.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_font_stress.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_freetype.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_gif.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libjpeg_turbo.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libpng.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libwebp.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_lodepng.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_memmove.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_qrcode.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_svg_decoder.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tiny_ttf.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tjpgd.o 

C_DEPS += \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_barcode.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bin_decoder.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bmp.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_ffmpeg.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_font_stress.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_freetype.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_gif.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libjpeg_turbo.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libpng.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libwebp.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_lodepng.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_memmove.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_qrcode.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_svg_decoder.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tiny_ttf.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tjpgd.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/tests/src/test_cases/libs/%.o App/Modules/GUI/lvgl/tests/src/test_cases/libs/%.su App/Modules/GUI/lvgl/tests/src/test_cases/libs/%.cyclo: ../App/Modules/GUI/lvgl/tests/src/test_cases/libs/%.c App/Modules/GUI/lvgl/tests/src/test_cases/libs/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-tests-2f-src-2f-test_cases-2f-libs

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-tests-2f-src-2f-test_cases-2f-libs:
	-$(RM) ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_barcode.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_barcode.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_barcode.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_barcode.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bin_decoder.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bin_decoder.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bin_decoder.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bin_decoder.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bmp.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bmp.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bmp.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_bmp.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_ffmpeg.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_ffmpeg.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_ffmpeg.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_ffmpeg.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_font_stress.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_font_stress.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_font_stress.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_font_stress.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_freetype.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_freetype.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_freetype.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_freetype.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_gif.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_gif.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_gif.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_gif.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libjpeg_turbo.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libjpeg_turbo.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libjpeg_turbo.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libjpeg_turbo.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libpng.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libpng.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libpng.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libpng.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libwebp.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libwebp.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libwebp.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_libwebp.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_lodepng.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_lodepng.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_lodepng.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_lodepng.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_memmove.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_memmove.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_memmove.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_memmove.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_qrcode.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_qrcode.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_qrcode.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_qrcode.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_svg_decoder.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_svg_decoder.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_svg_decoder.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_svg_decoder.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tiny_ttf.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tiny_ttf.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tiny_ttf.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tiny_ttf.su ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tjpgd.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tjpgd.d ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tjpgd.o ./App/Modules/GUI/lvgl/tests/src/test_cases/libs/test_tjpgd.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-tests-2f-src-2f-test_cases-2f-libs

