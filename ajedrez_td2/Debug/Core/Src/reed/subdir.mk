################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/reed/reed.c 

OBJS += \
./Core/Src/reed/reed.o 

C_DEPS += \
./Core/Src/reed/reed.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/reed/%.o Core/Src/reed/%.su Core/Src/reed/%.cyclo: ../Core/Src/reed/%.c Core/Src/reed/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-reed

clean-Core-2f-Src-2f-reed:
	-$(RM) ./Core/Src/reed/reed.cyclo ./Core/Src/reed/reed.d ./Core/Src/reed/reed.o ./Core/Src/reed/reed.su

.PHONY: clean-Core-2f-Src-2f-reed

