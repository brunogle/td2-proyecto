################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/engine/attacks.c \
../Core/Src/engine/board.c \
../Core/Src/engine/engine.c \
../Core/Src/engine/interface.c \
../Core/Src/engine/moves.c 

OBJS += \
./Core/Src/engine/attacks.o \
./Core/Src/engine/board.o \
./Core/Src/engine/engine.o \
./Core/Src/engine/interface.o \
./Core/Src/engine/moves.o 

C_DEPS += \
./Core/Src/engine/attacks.d \
./Core/Src/engine/board.d \
./Core/Src/engine/engine.d \
./Core/Src/engine/interface.d \
./Core/Src/engine/moves.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/engine/%.o Core/Src/engine/%.su Core/Src/engine/%.cyclo: ../Core/Src/engine/%.c Core/Src/engine/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-engine

clean-Core-2f-Src-2f-engine:
	-$(RM) ./Core/Src/engine/attacks.cyclo ./Core/Src/engine/attacks.d ./Core/Src/engine/attacks.o ./Core/Src/engine/attacks.su ./Core/Src/engine/board.cyclo ./Core/Src/engine/board.d ./Core/Src/engine/board.o ./Core/Src/engine/board.su ./Core/Src/engine/engine.cyclo ./Core/Src/engine/engine.d ./Core/Src/engine/engine.o ./Core/Src/engine/engine.su ./Core/Src/engine/interface.cyclo ./Core/Src/engine/interface.d ./Core/Src/engine/interface.o ./Core/Src/engine/interface.su ./Core/Src/engine/moves.cyclo ./Core/Src/engine/moves.d ./Core/Src/engine/moves.o ./Core/Src/engine/moves.su

.PHONY: clean-Core-2f-Src-2f-engine

