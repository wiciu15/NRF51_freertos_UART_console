################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/freertos/portable/MemMang/heap_1.c 

OBJS += \
./src/freertos/portable/MemMang/heap_1.o 

C_DEPS += \
./src/freertos/portable/MemMang/heap_1.d 


# Each subdirectory must supply rules for building sources it contributes
src/freertos/portable/MemMang/%.o: ../src/freertos/portable/MemMang/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DNRF51 -I"C:\NRFdev\projekty\freertos\inc" -I"C:\NRFdev\projekty\freertos\src\freertos\include" -isystem"C:\NRFdev\NRFSDK8\components\toolchain\gcc" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\clock" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\common" -isystem"C:\NRFdev\NRFSDK8\components\libraries\fifo" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\gpiote" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\nrf_soc_nosd" -isystem"C:\NRFdev\NRFSDK8\components\libraries\util" -isystem"C:\NRFdev\NRFSDK8\components\device" -isystem"C:\NRFdev\NRFSDK8\components\toolchain" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\hal" -isystem"C:\NRFdev\NRFSDK8\examples\bsp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


