################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/src/osal/lv_cmsis_rtos2.c \
../App/Modules/GUI/lvgl/src/osal/lv_freertos.c \
../App/Modules/GUI/lvgl/src/osal/lv_linux.c \
../App/Modules/GUI/lvgl/src/osal/lv_mqx.c \
../App/Modules/GUI/lvgl/src/osal/lv_os.c \
../App/Modules/GUI/lvgl/src/osal/lv_os_none.c \
../App/Modules/GUI/lvgl/src/osal/lv_pthread.c \
../App/Modules/GUI/lvgl/src/osal/lv_rtthread.c \
../App/Modules/GUI/lvgl/src/osal/lv_sdl2.c \
../App/Modules/GUI/lvgl/src/osal/lv_windows.c 

OBJS += \
./App/Modules/GUI/lvgl/src/osal/lv_cmsis_rtos2.o \
./App/Modules/GUI/lvgl/src/osal/lv_freertos.o \
./App/Modules/GUI/lvgl/src/osal/lv_linux.o \
./App/Modules/GUI/lvgl/src/osal/lv_mqx.o \
./App/Modules/GUI/lvgl/src/osal/lv_os.o \
./App/Modules/GUI/lvgl/src/osal/lv_os_none.o \
./App/Modules/GUI/lvgl/src/osal/lv_pthread.o \
./App/Modules/GUI/lvgl/src/osal/lv_rtthread.o \
./App/Modules/GUI/lvgl/src/osal/lv_sdl2.o \
./App/Modules/GUI/lvgl/src/osal/lv_windows.o 

C_DEPS += \
./App/Modules/GUI/lvgl/src/osal/lv_cmsis_rtos2.d \
./App/Modules/GUI/lvgl/src/osal/lv_freertos.d \
./App/Modules/GUI/lvgl/src/osal/lv_linux.d \
./App/Modules/GUI/lvgl/src/osal/lv_mqx.d \
./App/Modules/GUI/lvgl/src/osal/lv_os.d \
./App/Modules/GUI/lvgl/src/osal/lv_os_none.d \
./App/Modules/GUI/lvgl/src/osal/lv_pthread.d \
./App/Modules/GUI/lvgl/src/osal/lv_rtthread.d \
./App/Modules/GUI/lvgl/src/osal/lv_sdl2.d \
./App/Modules/GUI/lvgl/src/osal/lv_windows.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/src/osal/%.o App/Modules/GUI/lvgl/src/osal/%.su App/Modules/GUI/lvgl/src/osal/%.cyclo: ../App/Modules/GUI/lvgl/src/osal/%.c App/Modules/GUI/lvgl/src/osal/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-osal

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-osal:
	-$(RM) ./App/Modules/GUI/lvgl/src/osal/lv_cmsis_rtos2.cyclo ./App/Modules/GUI/lvgl/src/osal/lv_cmsis_rtos2.d ./App/Modules/GUI/lvgl/src/osal/lv_cmsis_rtos2.o ./App/Modules/GUI/lvgl/src/osal/lv_cmsis_rtos2.su ./App/Modules/GUI/lvgl/src/osal/lv_freertos.cyclo ./App/Modules/GUI/lvgl/src/osal/lv_freertos.d ./App/Modules/GUI/lvgl/src/osal/lv_freertos.o ./App/Modules/GUI/lvgl/src/osal/lv_freertos.su ./App/Modules/GUI/lvgl/src/osal/lv_linux.cyclo ./App/Modules/GUI/lvgl/src/osal/lv_linux.d ./App/Modules/GUI/lvgl/src/osal/lv_linux.o ./App/Modules/GUI/lvgl/src/osal/lv_linux.su ./App/Modules/GUI/lvgl/src/osal/lv_mqx.cyclo ./App/Modules/GUI/lvgl/src/osal/lv_mqx.d ./App/Modules/GUI/lvgl/src/osal/lv_mqx.o ./App/Modules/GUI/lvgl/src/osal/lv_mqx.su ./App/Modules/GUI/lvgl/src/osal/lv_os.cyclo ./App/Modules/GUI/lvgl/src/osal/lv_os.d ./App/Modules/GUI/lvgl/src/osal/lv_os.o ./App/Modules/GUI/lvgl/src/osal/lv_os.su ./App/Modules/GUI/lvgl/src/osal/lv_os_none.cyclo ./App/Modules/GUI/lvgl/src/osal/lv_os_none.d ./App/Modules/GUI/lvgl/src/osal/lv_os_none.o ./App/Modules/GUI/lvgl/src/osal/lv_os_none.su ./App/Modules/GUI/lvgl/src/osal/lv_pthread.cyclo ./App/Modules/GUI/lvgl/src/osal/lv_pthread.d ./App/Modules/GUI/lvgl/src/osal/lv_pthread.o ./App/Modules/GUI/lvgl/src/osal/lv_pthread.su ./App/Modules/GUI/lvgl/src/osal/lv_rtthread.cyclo ./App/Modules/GUI/lvgl/src/osal/lv_rtthread.d ./App/Modules/GUI/lvgl/src/osal/lv_rtthread.o ./App/Modules/GUI/lvgl/src/osal/lv_rtthread.su ./App/Modules/GUI/lvgl/src/osal/lv_sdl2.cyclo ./App/Modules/GUI/lvgl/src/osal/lv_sdl2.d ./App/Modules/GUI/lvgl/src/osal/lv_sdl2.o ./App/Modules/GUI/lvgl/src/osal/lv_sdl2.su ./App/Modules/GUI/lvgl/src/osal/lv_windows.cyclo ./App/Modules/GUI/lvgl/src/osal/lv_windows.d ./App/Modules/GUI/lvgl/src/osal/lv_windows.o ./App/Modules/GUI/lvgl/src/osal/lv_windows.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-osal

