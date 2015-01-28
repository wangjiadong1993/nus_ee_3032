################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc176x.c \
../src/lcddemo_graphics.c \
../src/lcddemo_main.c \
../src/lcddemo_shapes.c \
../src/lcddemo_terminal.c 

OBJS += \
./src/cr_startup_lpc176x.o \
./src/lcddemo_graphics.o \
./src/lcddemo_main.o \
./src/lcddemo_shapes.o \
./src/lcddemo_terminal.o 

C_DEPS += \
./src/cr_startup_lpc176x.d \
./src/lcddemo_graphics.d \
./src/lcddemo_main.d \
./src/lcddemo_shapes.d \
./src/lcddemo_terminal.d 


# Each subdirectory must supply rules for building sources it contributes
src/cr_startup_lpc176x.o: ../src/cr_startup_lpc176x.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"/Users/wangjiadong/LPCXpresso/workspace/RDB1768cmsis_LCDlib/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_CMSISv1p30_LPC17xx/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_MCU/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/RDB1768cmsis_LCDdemo/bitmaps" -O0 -Os -g3 -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/cr_startup_lpc176x.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"/Users/wangjiadong/LPCXpresso/workspace/RDB1768cmsis_LCDlib/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_CMSISv1p30_LPC17xx/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_MCU/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/RDB1768cmsis_LCDdemo/bitmaps" -O0 -g3 -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


