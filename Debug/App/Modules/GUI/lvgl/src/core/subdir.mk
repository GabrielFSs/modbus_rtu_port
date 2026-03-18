################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/src/core/lv_group.c \
../App/Modules/GUI/lvgl/src/core/lv_obj.c \
../App/Modules/GUI/lvgl/src/core/lv_obj_class.c \
../App/Modules/GUI/lvgl/src/core/lv_obj_draw.c \
../App/Modules/GUI/lvgl/src/core/lv_obj_event.c \
../App/Modules/GUI/lvgl/src/core/lv_obj_id_builtin.c \
../App/Modules/GUI/lvgl/src/core/lv_obj_pos.c \
../App/Modules/GUI/lvgl/src/core/lv_obj_property.c \
../App/Modules/GUI/lvgl/src/core/lv_obj_scroll.c \
../App/Modules/GUI/lvgl/src/core/lv_obj_style.c \
../App/Modules/GUI/lvgl/src/core/lv_obj_style_gen.c \
../App/Modules/GUI/lvgl/src/core/lv_obj_tree.c \
../App/Modules/GUI/lvgl/src/core/lv_observer.c \
../App/Modules/GUI/lvgl/src/core/lv_refr.c 

OBJS += \
./App/Modules/GUI/lvgl/src/core/lv_group.o \
./App/Modules/GUI/lvgl/src/core/lv_obj.o \
./App/Modules/GUI/lvgl/src/core/lv_obj_class.o \
./App/Modules/GUI/lvgl/src/core/lv_obj_draw.o \
./App/Modules/GUI/lvgl/src/core/lv_obj_event.o \
./App/Modules/GUI/lvgl/src/core/lv_obj_id_builtin.o \
./App/Modules/GUI/lvgl/src/core/lv_obj_pos.o \
./App/Modules/GUI/lvgl/src/core/lv_obj_property.o \
./App/Modules/GUI/lvgl/src/core/lv_obj_scroll.o \
./App/Modules/GUI/lvgl/src/core/lv_obj_style.o \
./App/Modules/GUI/lvgl/src/core/lv_obj_style_gen.o \
./App/Modules/GUI/lvgl/src/core/lv_obj_tree.o \
./App/Modules/GUI/lvgl/src/core/lv_observer.o \
./App/Modules/GUI/lvgl/src/core/lv_refr.o 

C_DEPS += \
./App/Modules/GUI/lvgl/src/core/lv_group.d \
./App/Modules/GUI/lvgl/src/core/lv_obj.d \
./App/Modules/GUI/lvgl/src/core/lv_obj_class.d \
./App/Modules/GUI/lvgl/src/core/lv_obj_draw.d \
./App/Modules/GUI/lvgl/src/core/lv_obj_event.d \
./App/Modules/GUI/lvgl/src/core/lv_obj_id_builtin.d \
./App/Modules/GUI/lvgl/src/core/lv_obj_pos.d \
./App/Modules/GUI/lvgl/src/core/lv_obj_property.d \
./App/Modules/GUI/lvgl/src/core/lv_obj_scroll.d \
./App/Modules/GUI/lvgl/src/core/lv_obj_style.d \
./App/Modules/GUI/lvgl/src/core/lv_obj_style_gen.d \
./App/Modules/GUI/lvgl/src/core/lv_obj_tree.d \
./App/Modules/GUI/lvgl/src/core/lv_observer.d \
./App/Modules/GUI/lvgl/src/core/lv_refr.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/src/core/%.o App/Modules/GUI/lvgl/src/core/%.su App/Modules/GUI/lvgl/src/core/%.cyclo: ../App/Modules/GUI/lvgl/src/core/%.c App/Modules/GUI/lvgl/src/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-core

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-core:
	-$(RM) ./App/Modules/GUI/lvgl/src/core/lv_group.cyclo ./App/Modules/GUI/lvgl/src/core/lv_group.d ./App/Modules/GUI/lvgl/src/core/lv_group.o ./App/Modules/GUI/lvgl/src/core/lv_group.su ./App/Modules/GUI/lvgl/src/core/lv_obj.cyclo ./App/Modules/GUI/lvgl/src/core/lv_obj.d ./App/Modules/GUI/lvgl/src/core/lv_obj.o ./App/Modules/GUI/lvgl/src/core/lv_obj.su ./App/Modules/GUI/lvgl/src/core/lv_obj_class.cyclo ./App/Modules/GUI/lvgl/src/core/lv_obj_class.d ./App/Modules/GUI/lvgl/src/core/lv_obj_class.o ./App/Modules/GUI/lvgl/src/core/lv_obj_class.su ./App/Modules/GUI/lvgl/src/core/lv_obj_draw.cyclo ./App/Modules/GUI/lvgl/src/core/lv_obj_draw.d ./App/Modules/GUI/lvgl/src/core/lv_obj_draw.o ./App/Modules/GUI/lvgl/src/core/lv_obj_draw.su ./App/Modules/GUI/lvgl/src/core/lv_obj_event.cyclo ./App/Modules/GUI/lvgl/src/core/lv_obj_event.d ./App/Modules/GUI/lvgl/src/core/lv_obj_event.o ./App/Modules/GUI/lvgl/src/core/lv_obj_event.su ./App/Modules/GUI/lvgl/src/core/lv_obj_id_builtin.cyclo ./App/Modules/GUI/lvgl/src/core/lv_obj_id_builtin.d ./App/Modules/GUI/lvgl/src/core/lv_obj_id_builtin.o ./App/Modules/GUI/lvgl/src/core/lv_obj_id_builtin.su ./App/Modules/GUI/lvgl/src/core/lv_obj_pos.cyclo ./App/Modules/GUI/lvgl/src/core/lv_obj_pos.d ./App/Modules/GUI/lvgl/src/core/lv_obj_pos.o ./App/Modules/GUI/lvgl/src/core/lv_obj_pos.su ./App/Modules/GUI/lvgl/src/core/lv_obj_property.cyclo ./App/Modules/GUI/lvgl/src/core/lv_obj_property.d ./App/Modules/GUI/lvgl/src/core/lv_obj_property.o ./App/Modules/GUI/lvgl/src/core/lv_obj_property.su ./App/Modules/GUI/lvgl/src/core/lv_obj_scroll.cyclo ./App/Modules/GUI/lvgl/src/core/lv_obj_scroll.d ./App/Modules/GUI/lvgl/src/core/lv_obj_scroll.o ./App/Modules/GUI/lvgl/src/core/lv_obj_scroll.su ./App/Modules/GUI/lvgl/src/core/lv_obj_style.cyclo ./App/Modules/GUI/lvgl/src/core/lv_obj_style.d ./App/Modules/GUI/lvgl/src/core/lv_obj_style.o ./App/Modules/GUI/lvgl/src/core/lv_obj_style.su ./App/Modules/GUI/lvgl/src/core/lv_obj_style_gen.cyclo ./App/Modules/GUI/lvgl/src/core/lv_obj_style_gen.d ./App/Modules/GUI/lvgl/src/core/lv_obj_style_gen.o ./App/Modules/GUI/lvgl/src/core/lv_obj_style_gen.su ./App/Modules/GUI/lvgl/src/core/lv_obj_tree.cyclo ./App/Modules/GUI/lvgl/src/core/lv_obj_tree.d ./App/Modules/GUI/lvgl/src/core/lv_obj_tree.o ./App/Modules/GUI/lvgl/src/core/lv_obj_tree.su ./App/Modules/GUI/lvgl/src/core/lv_observer.cyclo ./App/Modules/GUI/lvgl/src/core/lv_observer.d ./App/Modules/GUI/lvgl/src/core/lv_observer.o ./App/Modules/GUI/lvgl/src/core/lv_observer.su ./App/Modules/GUI/lvgl/src/core/lv_refr.cyclo ./App/Modules/GUI/lvgl/src/core/lv_refr.d ./App/Modules/GUI/lvgl/src/core/lv_refr.o ./App/Modules/GUI/lvgl/src/core/lv_refr.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-core

