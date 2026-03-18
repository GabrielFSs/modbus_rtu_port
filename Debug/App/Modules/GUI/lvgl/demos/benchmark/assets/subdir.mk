################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_avatar.c \
../App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_argb.c \
../App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_rgb.c \
../App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_12_aligned.c \
../App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_14_aligned.c \
../App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_16_aligned.c \
../App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_18_aligned.c \
../App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_20_aligned.c \
../App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_24_aligned.c \
../App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_26_aligned.c 

OBJS += \
./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_avatar.o \
./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_argb.o \
./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_rgb.o \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_12_aligned.o \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_14_aligned.o \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_16_aligned.o \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_18_aligned.o \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_20_aligned.o \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_24_aligned.o \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_26_aligned.o 

C_DEPS += \
./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_avatar.d \
./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_argb.d \
./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_rgb.d \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_12_aligned.d \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_14_aligned.d \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_16_aligned.d \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_18_aligned.d \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_20_aligned.d \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_24_aligned.d \
./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_26_aligned.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/demos/benchmark/assets/%.o App/Modules/GUI/lvgl/demos/benchmark/assets/%.su App/Modules/GUI/lvgl/demos/benchmark/assets/%.cyclo: ../App/Modules/GUI/lvgl/demos/benchmark/assets/%.c App/Modules/GUI/lvgl/demos/benchmark/assets/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-demos-2f-benchmark-2f-assets

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-demos-2f-benchmark-2f-assets:
	-$(RM) ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_avatar.cyclo ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_avatar.d ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_avatar.o ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_avatar.su ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_argb.cyclo ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_argb.d ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_argb.o ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_argb.su ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_rgb.cyclo ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_rgb.d ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_rgb.o ./App/Modules/GUI/lvgl/demos/benchmark/assets/img_benchmark_lvgl_logo_rgb.su ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_12_aligned.cyclo ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_12_aligned.d ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_12_aligned.o ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_12_aligned.su ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_14_aligned.cyclo ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_14_aligned.d ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_14_aligned.o ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_14_aligned.su ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_16_aligned.cyclo ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_16_aligned.d ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_16_aligned.o ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_16_aligned.su ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_18_aligned.cyclo ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_18_aligned.d ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_18_aligned.o ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_18_aligned.su ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_20_aligned.cyclo ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_20_aligned.d ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_20_aligned.o ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_20_aligned.su ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_24_aligned.cyclo ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_24_aligned.d ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_24_aligned.o ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_24_aligned.su ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_26_aligned.cyclo ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_26_aligned.d ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_26_aligned.o ./App/Modules/GUI/lvgl/demos/benchmark/assets/lv_font_benchmark_montserrat_26_aligned.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-demos-2f-benchmark-2f-assets

