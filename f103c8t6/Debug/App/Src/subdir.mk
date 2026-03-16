################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../App/Src/app_main.c \
../App/Src/output.c \
../App/Src/pwm.c 

OBJS += \
./App/Src/app_main.o \
./App/Src/output.o \
./App/Src/pwm.o 

C_DEPS += \
./App/Src/app_main.d \
./App/Src/output.d \
./App/Src/pwm.d 


# Each subdirectory must supply rules for building sources it contributes
App/Src/%.o App/Src/%.su App/Src/%.cyclo: ../App/Src/%.c App/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../App/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-App-2f-Src

clean-App-2f-Src:
	-$(RM) ./App/Src/app_main.cyclo ./App/Src/app_main.d ./App/Src/app_main.o ./App/Src/app_main.su ./App/Src/output.cyclo ./App/Src/output.d ./App/Src/output.o ./App/Src/output.su ./App/Src/pwm.cyclo ./App/Src/pwm.d ./App/Src/pwm.o ./App/Src/pwm.su

.PHONY: clean-App-2f-Src

