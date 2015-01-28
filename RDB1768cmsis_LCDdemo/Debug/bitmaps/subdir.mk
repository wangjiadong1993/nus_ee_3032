################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../bitmaps/arm98x45.c \
../bitmaps/codered128x37.c \
../bitmaps/cortex128x57.c \
../bitmaps/crlogo35x51.c \
../bitmaps/crlogo69x102.c \
../bitmaps/nxp128x38.c 

OBJS += \
./bitmaps/arm98x45.o \
./bitmaps/codered128x37.o \
./bitmaps/cortex128x57.o \
./bitmaps/crlogo35x51.o \
./bitmaps/crlogo69x102.o \
./bitmaps/nxp128x38.o 

C_DEPS += \
./bitmaps/arm98x45.d \
./bitmaps/codered128x37.d \
./bitmaps/cortex128x57.d \
./bitmaps/crlogo35x51.d \
./bitmaps/crlogo69x102.d \
./bitmaps/nxp128x38.d 


# Each subdirectory must supply rules for building sources it contributes
bitmaps/%.o: ../bitmaps/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__USE_CMSIS=CMSISv1p30_LPC17xx -D__CODE_RED -D__REDLIB__ -I"/Users/wangjiadong/LPCXpresso/workspace/RDB1768cmsis_LCDlib/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_CMSISv1p30_LPC17xx/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_MCU/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/RDB1768cmsis_LCDdemo/bitmaps" -O0 -g3 -c -fmessage-length=0 -fno-builtin -ffunction-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


