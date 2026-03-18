################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/tests/src/test_cases/_test_template.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_align_flex.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_anim.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_anim_timeline.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_area.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_array.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_async.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_bindings.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_circle_buf.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_click.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_color.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_config.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_stress.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_vector_graphic.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_widgets.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_display.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_draw_buf.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_event.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_event_trickle.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_file_explorer.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_flex_grow.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_font_loader.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_font_manager.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_fs.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_gesture_pinch.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_grid.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_grid_fr.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_gridnav.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_group.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_hover.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_indev.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_event.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_key_remap.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_lcd.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_ll.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_align.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_flex.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_grid.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_math.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_mem.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_observer.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_palette.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_profiler.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_recolor.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_screen_load.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_snapshot.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_style.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_svg.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_anim.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_file.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_theme.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_tick.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_translation.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_tree.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_txt.c \
../App/Modules/GUI/lvgl/tests/src/test_cases/test_utils.c 

OBJS += \
./App/Modules/GUI/lvgl/tests/src/test_cases/_test_template.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_align_flex.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim_timeline.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_area.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_array.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_async.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_bindings.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_circle_buf.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_click.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_color.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_config.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_stress.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_vector_graphic.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_widgets.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_display.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_draw_buf.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_event.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_event_trickle.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_file_explorer.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_flex_grow.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_loader.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_manager.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_fs.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_gesture_pinch.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid_fr.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_gridnav.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_group.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_hover.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_event.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_key_remap.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_lcd.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_ll.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_align.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_flex.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_grid.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_math.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_mem.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_observer.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_palette.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_profiler.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_recolor.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_screen_load.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_snapshot.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_style.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_anim.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_file.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_theme.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_tick.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_translation.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_tree.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_txt.o \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_utils.o 

C_DEPS += \
./App/Modules/GUI/lvgl/tests/src/test_cases/_test_template.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_align_flex.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim_timeline.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_area.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_array.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_async.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_bindings.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_circle_buf.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_click.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_color.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_config.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_stress.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_vector_graphic.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_widgets.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_display.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_draw_buf.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_event.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_event_trickle.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_file_explorer.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_flex_grow.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_loader.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_manager.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_fs.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_gesture_pinch.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid_fr.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_gridnav.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_group.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_hover.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_event.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_key_remap.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_lcd.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_ll.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_align.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_flex.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_grid.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_math.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_mem.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_observer.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_palette.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_profiler.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_recolor.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_screen_load.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_snapshot.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_style.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_anim.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_file.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_theme.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_tick.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_translation.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_tree.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_txt.d \
./App/Modules/GUI/lvgl/tests/src/test_cases/test_utils.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/tests/src/test_cases/%.o App/Modules/GUI/lvgl/tests/src/test_cases/%.su App/Modules/GUI/lvgl/tests/src/test_cases/%.cyclo: ../App/Modules/GUI/lvgl/tests/src/test_cases/%.c App/Modules/GUI/lvgl/tests/src/test_cases/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-tests-2f-src-2f-test_cases

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-tests-2f-src-2f-test_cases:
	-$(RM) ./App/Modules/GUI/lvgl/tests/src/test_cases/_test_template.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/_test_template.d ./App/Modules/GUI/lvgl/tests/src/test_cases/_test_template.o ./App/Modules/GUI/lvgl/tests/src/test_cases/_test_template.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_align_flex.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_align_flex.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_align_flex.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_align_flex.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim_timeline.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim_timeline.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim_timeline.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_anim_timeline.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_area.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_area.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_area.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_area.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_array.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_array.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_array.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_array.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_async.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_async.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_async.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_async.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_bindings.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_bindings.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_bindings.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_bindings.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_circle_buf.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_circle_buf.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_circle_buf.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_circle_buf.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_click.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_click.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_click.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_click.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_color.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_color.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_color.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_color.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_config.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_config.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_config.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_config.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_stress.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_stress.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_stress.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_stress.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_vector_graphic.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_vector_graphic.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_vector_graphic.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_vector_graphic.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_widgets.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_widgets.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_widgets.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_demo_widgets.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_display.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_display.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_display.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_display.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_draw_buf.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_draw_buf.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_draw_buf.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_draw_buf.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_event.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_event.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_event.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_event.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_event_trickle.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_event_trickle.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_event_trickle.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_event_trickle.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_file_explorer.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_file_explorer.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_file_explorer.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_file_explorer.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_flex_grow.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_flex_grow.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_flex_grow.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_flex_grow.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_loader.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_loader.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_loader.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_loader.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_manager.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_manager.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_manager.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_font_manager.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_fs.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_fs.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_fs.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_fs.su
	-$(RM) ./App/Modules/GUI/lvgl/tests/src/test_cases/test_gesture_pinch.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_gesture_pinch.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_gesture_pinch.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_gesture_pinch.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid_fr.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid_fr.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid_fr.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_grid_fr.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_gridnav.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_gridnav.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_gridnav.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_gridnav.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_group.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_group.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_group.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_group.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_hover.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_hover.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_hover.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_hover.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_event.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_event.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_event.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_event.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_key_remap.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_key_remap.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_key_remap.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_indev_key_remap.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_lcd.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_lcd.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_lcd.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_lcd.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_ll.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_ll.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_ll.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_ll.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_align.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_align.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_align.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_align.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_flex.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_flex.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_flex.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_flex.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_grid.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_grid.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_grid.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_margin_grid.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_math.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_math.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_math.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_math.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_mem.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_mem.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_mem.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_mem.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_observer.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_observer.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_observer.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_observer.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_palette.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_palette.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_palette.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_palette.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_profiler.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_profiler.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_profiler.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_profiler.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_recolor.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_recolor.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_recolor.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_recolor.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_screen_load.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_screen_load.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_screen_load.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_screen_load.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_snapshot.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_snapshot.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_snapshot.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_snapshot.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_style.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_style.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_style.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_style.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_anim.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_anim.d
	-$(RM) ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_anim.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_anim.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_file.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_file.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_file.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_svg_file.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_theme.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_theme.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_theme.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_theme.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_tick.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_tick.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_tick.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_tick.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_translation.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_translation.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_translation.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_translation.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_tree.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_tree.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_tree.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_tree.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_txt.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_txt.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_txt.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_txt.su ./App/Modules/GUI/lvgl/tests/src/test_cases/test_utils.cyclo ./App/Modules/GUI/lvgl/tests/src/test_cases/test_utils.d ./App/Modules/GUI/lvgl/tests/src/test_cases/test_utils.o ./App/Modules/GUI/lvgl/tests/src/test_cases/test_utils.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-tests-2f-src-2f-test_cases

