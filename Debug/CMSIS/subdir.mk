################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../CMSIS/system_MKL25Z4.c 

OBJS += \
./CMSIS/system_MKL25Z4.o 

C_DEPS += \
./CMSIS/system_MKL25Z4.d 


# Each subdirectory must supply rules for building sources it contributes
CMSIS/%.o: ../CMSIS/%.c CMSIS/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DDEBUG -DFRDM_KL25Z -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -D__REDLIB__ -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z_driver_examples_tpm_pwm_twochannel\drivers" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z_driver_examples_tpm_pwm_twochannel\utilities" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z_driver_examples_tpm_pwm_twochannel\CMSIS" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z_driver_examples_tpm_pwm_twochannel\board" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z_driver_examples_tpm_pwm_twochannel\source" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z_driver_examples_tpm_pwm_twochannel" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z_driver_examples_tpm_pwm_twochannel\CMSIS" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z_driver_examples_tpm_pwm_twochannel\drivers" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z_driver_examples_tpm_pwm_twochannel\utilities" -I"C:\Users\filip\Documents\MCUXpressoIDE_11.5.1_7266\workspace\frdmkl25z_driver_examples_tpm_pwm_twochannel\startup" -O0 -fno-common -g -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


