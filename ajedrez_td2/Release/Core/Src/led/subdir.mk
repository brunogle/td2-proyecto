################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/led/ws2812.c 

OBJS += \
./Core/Src/led/ws2812.o 

C_DEPS += \
./Core/Src/led/ws2812.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/led/%.o Core/Src/led/%.su Core/Src/led/%.cyclo: ../Core/Src/led/%.c Core/Src/led/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-led

clean-Core-2f-Src-2f-led:
	-$(RM) ./Core/Src/led/ws2812.cyclo ./Core/Src/led/ws2812.d ./Core/Src/led/ws2812.o ./Core/Src/led/ws2812.su

.PHONY: clean-Core-2f-Src-2f-led

