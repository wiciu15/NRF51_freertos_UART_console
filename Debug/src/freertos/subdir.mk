################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/freertos/croutine.c \
../src/freertos/event_groups.c \
../src/freertos/list.c \
../src/freertos/port.c \
../src/freertos/port_cmsis.c \
../src/freertos/port_cmsis_systick.c \
../src/freertos/queue.c \
../src/freertos/tasks.c \
../src/freertos/timers.c 

OBJS += \
./src/freertos/croutine.o \
./src/freertos/event_groups.o \
./src/freertos/list.o \
./src/freertos/port.o \
./src/freertos/port_cmsis.o \
./src/freertos/port_cmsis_systick.o \
./src/freertos/queue.o \
./src/freertos/tasks.o \
./src/freertos/timers.o 

C_DEPS += \
./src/freertos/croutine.d \
./src/freertos/event_groups.d \
./src/freertos/list.d \
./src/freertos/port.d \
./src/freertos/port_cmsis.d \
./src/freertos/port_cmsis_systick.d \
./src/freertos/queue.d \
./src/freertos/tasks.d \
./src/freertos/timers.d 


# Each subdirectory must supply rules for building sources it contributes
src/freertos/%.o: ../src/freertos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DNRF51 -I"C:\NRFdev\projekty\freertos\inc" -I"C:\NRFdev\projekty\freertos\src\freertos\include" -isystem"C:\NRFdev\NRFSDK8\components\toolchain\gcc" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\clock" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\common" -isystem"C:\NRFdev\NRFSDK8\components\libraries\fifo" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\gpiote" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\nrf_soc_nosd" -isystem"C:\NRFdev\NRFSDK8\components\libraries\util" -isystem"C:\NRFdev\NRFSDK8\components\device" -isystem"C:\NRFdev\NRFSDK8\components\toolchain" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\hal" -isystem"C:\NRFdev\NRFSDK8\examples\bsp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


