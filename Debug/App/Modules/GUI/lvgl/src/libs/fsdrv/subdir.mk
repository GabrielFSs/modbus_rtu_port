################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_cbfs.c \
../App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_fatfs.c \
../App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_frogfs.c \
../App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_littlefs.c \
../App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_memfs.c \
../App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_posix.c \
../App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_stdio.c \
../App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_uefi.c \
../App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_win32.c 

OBJS += \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_cbfs.o \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_fatfs.o \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_frogfs.o \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_littlefs.o \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_memfs.o \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_posix.o \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_stdio.o \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_uefi.o \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_win32.o 

C_DEPS += \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_cbfs.d \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_fatfs.d \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_frogfs.d \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_littlefs.d \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_memfs.d \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_posix.d \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_stdio.d \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_uefi.d \
./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_win32.d 


# Each subdirectory must supply rules for building sources it contributes
App/Modules/GUI/lvgl/src/libs/fsdrv/%.o App/Modules/GUI/lvgl/src/libs/fsdrv/%.su App/Modules/GUI/lvgl/src/libs/fsdrv/%.cyclo: ../App/Modules/GUI/lvgl/src/libs/fsdrv/%.c App/Modules/GUI/lvgl/src/libs/fsdrv/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../App/Drivers/hal -I../App/Core/Inc -I../App/Core/Src -I../App/Drivers/ports -I../App/Modules/modbus -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App" -I../App/BSP -I"C:/Users/gab_p/STM32CubeIDE/workspace_1.12.1/ModbusRTU/App/BSP/stm32f407vet6_blackpill_v2.3.1" -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Device/ST/STM32F4xx/Include -IC:/Users/gab_p/STM32Cube/Repository/STM32Cube_FW_F4_V1.28.3/Drivers/CMSIS/Include -I../App/Middlewares/fatfs/source -I../App/Modules/modbus-master -I../App/Modules/modbus/modbus-slave/include -I../App/Modules/modbus/modbus-slave/rtu -I../App/Modules/modbus/modbus-slave/port -I../App/Modules/modbus/modbus-master/Includes -I../App/Modules/modbus/modbus-master/port -I../App/Modules/modbus/modbus-master/rtu -I../App/Modules/modbus/modbus-sniffer -I../App/Modules/GUI -I../App/Modules/GUI/lvgl -I../App/Modules/GUI/lvgl/configs -I../App/Modules/GUI/lvgl/libs -I../App/Modules/GUI/lvgl/src -I../App/Modules/GUI/lvgl/src/core -I../App/Modules/GUI/lvgl/src/display -I../App/Modules/Display -I../App/Modules/modbus/modbus-manager/port -I../App/Modules/modbus/modbus-manager -I../App/Modules/modbus/modbus-slave -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-libs-2f-fsdrv

clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-libs-2f-fsdrv:
	-$(RM) ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_cbfs.cyclo ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_cbfs.d ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_cbfs.o ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_cbfs.su ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_fatfs.cyclo ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_fatfs.d ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_fatfs.o ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_fatfs.su ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_frogfs.cyclo ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_frogfs.d ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_frogfs.o ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_frogfs.su ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_littlefs.cyclo ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_littlefs.d ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_littlefs.o ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_littlefs.su ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_memfs.cyclo ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_memfs.d ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_memfs.o ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_memfs.su ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_posix.cyclo ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_posix.d ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_posix.o ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_posix.su ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_stdio.cyclo ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_stdio.d ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_stdio.o ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_stdio.su ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_uefi.cyclo ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_uefi.d ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_uefi.o ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_uefi.su ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_win32.cyclo ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_win32.d ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_win32.o ./App/Modules/GUI/lvgl/src/libs/fsdrv/lv_fs_win32.su

.PHONY: clean-App-2f-Modules-2f-GUI-2f-lvgl-2f-src-2f-libs-2f-fsdrv

