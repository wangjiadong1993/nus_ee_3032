################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/cr_startup_lpc176x.c \
../src/fat_access.c \
../src/fat_cache.c \
../src/fat_filelib.c \
../src/fat_format.c \
../src/fat_misc.c \
../src/fat_string.c \
../src/fat_table.c \
../src/fat_write.c \
../src/file.c \
../src/main.c \
../src/sd.c \
../src/spi.c 

OBJS += \
./src/cr_startup_lpc176x.o \
./src/fat_access.o \
./src/fat_cache.o \
./src/fat_filelib.o \
./src/fat_format.o \
./src/fat_misc.o \
./src/fat_string.o \
./src/fat_table.o \
./src/fat_write.o \
./src/file.o \
./src/main.o \
./src/sd.o \
./src/spi.o 

C_DEPS += \
./src/cr_startup_lpc176x.d \
./src/fat_access.d \
./src/fat_cache.d \
./src/fat_filelib.d \
./src/fat_format.d \
./src/fat_misc.d \
./src/fat_string.d \
./src/fat_table.d \
./src/fat_write.d \
./src/file.d \
./src/main.d \
./src/sd.d \
./src/spi.d 


# Each subdirectory must supply rules for building sources it contributes
src/cr_startup_lpc176x.o: ../src/cr_startup_lpc176x.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__USE_CMSIS=CMSISv1p30_LPC17xx -DDEBUG -D__CODE_RED -D__REDLIB__ -I"/Users/wangjiadong/LPCXpresso/workspace/LPC1769_CMSIS_SD_CARD/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_CMSISv1p30_LPC17xx/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_MCU/inc" -O0 -Os -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"src/cr_startup_lpc176x.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__USE_CMSIS=CMSISv1p30_LPC17xx -DDEBUG -D__CODE_RED -D__REDLIB__ -I"/Users/wangjiadong/LPCXpresso/workspace/LPC1769_CMSIS_SD_CARD/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_CMSISv1p30_LPC17xx/inc" -I"/Users/wangjiadong/LPCXpresso/workspace/Lib_MCU/inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -D__REDLIB__ -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


