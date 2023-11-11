################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/lighting/lighting.c 

OBJS += \
./Core/Src/lighting/lighting.o 

C_DEPS += \
./Core/Src/lighting/lighting.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/lighting/%.o Core/Src/lighting/%.su Core/Src/lighting/%.cyclo: ../Core/Src/lighting/%.c Core/Src/lighting/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-lighting

clean-Core-2f-Src-2f-lighting:
	-$(RM) ./Core/Src/lighting/lighting.cyclo ./Core/Src/lighting/lighting.d ./Core/Src/lighting/lighting.o ./Core/Src/lighting/lighting.su

.PHONY: clean-Core-2f-Src-2f-lighting

