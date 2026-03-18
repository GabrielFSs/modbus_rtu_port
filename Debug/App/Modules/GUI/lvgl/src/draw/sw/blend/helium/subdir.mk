################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_UPPER_SRCS += \
../App/Modules/GUI/lvgl/src/draw/sw/blend/helium/lv_blend_helium.S 

OBJS += \
./App/Modules/GUI/lvgl/src/draw/sw/blend/helium/lv_blend_helium.o 

S_UPPER_DEPS += \
./App/Modules/GUI/lvgl/src/draw/sw/blend/helium/lv_blend_helium.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/src/draw/sw/blend/helium/%.o: ../App/Modules/GUI/lvgl/src/draw/sw/blend/helium/%.S App/Modules/GUI/lvgl/src/draw/sw/blend/helium/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-sw-2f-blend-2f-helium

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-sw-2f-blend-2f-helium:
	-$(RM) ./App/Modules/GUI/lvgl/src/draw/sw/blend/helium/lv_blend_helium.d ./App/Modules/GUI/lvgl/src/draw/sw/blend/helium/lv_blend_helium.o

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-draw-2f-sw-2f-blend-2f-helium

