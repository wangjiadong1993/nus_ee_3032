################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lcd_bitmap.c \
../src/lcd_driver.c \
../src/lcd_shapes.c \
../src/lcd_terminal.c \
../src/lcd_text.c \
../src/system_fixed_be_8_15.c 

OBJS += \
./src/lcd_bitmap.o \
./src/lcd_driver.o \
./src/lcd_shapes.o \
./src/lcd_terminal.o \
./src/lcd_text.o \
./src/system_fixed_be_8_15.o 

C_DEPS += \
./src/lcd_bitmap.d \
./src/lcd_driver.d \
./src/lcd_shapes.d \
./src/lcd_terminal.d \
./src/lcd_text.d \
./src/system_fixed_be_8_15.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_CMSISv1p30_LPC17xx/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/RDB1768cmsis_LCDlib/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_MCU/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


