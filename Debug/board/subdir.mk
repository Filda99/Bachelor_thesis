################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/pin_mux.c 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/pin_mux.o 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/pin_mux.d 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DDEBUG -DFRDM_KL25Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -D__REDLIB__ -DFSL_RTOS_FREE_RTOS -DSDK_OS_FREE_RTOS -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z\freertos" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z\drivers" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z\CMSIS" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z\utilities" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z\drivers" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z\utilities" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z\CMSIS" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z\board" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z\source" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z\startup" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


