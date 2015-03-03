################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Lib_main.c \
../src/Load_3032.c \
../src/cr_startup_lpc176x.c 

OBJS += \
./src/Lib_main.o \
./src/Load_3032.o \
./src/cr_startup_lpc176x.o 

C_DEPS += \
./src/Lib_main.d \
./src/Load_3032.d \
./src/cr_startup_lpc176x.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__USE_CMSIS=CMSISv1p30_LPC17xx -DNDEBUG -D__CODE_RED -D__REDLIB__ -Os -Os -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


