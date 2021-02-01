################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/app_error.c \
../src/app_fifo.c \
../src/app_uart_fifo.c \
../src/console.c \
../src/main.c \
../src/nrf_delay.c \
../src/nrf_drv_clock.c \
../src/nrf_drv_common.c \
../src/nrf_drv_gpiote.c \
../src/retarget.c \
../src/system_nrf51.c 

S_UPPER_SRCS += \
../src/gcc_startup_nrf51.S 

OBJS += \
./src/app_error.o \
./src/app_fifo.o \
./src/app_uart_fifo.o \
./src/console.o \
./src/gcc_startup_nrf51.o \
./src/main.o \
./src/nrf_delay.o \
./src/nrf_drv_clock.o \
./src/nrf_drv_common.o \
./src/nrf_drv_gpiote.o \
./src/retarget.o \
./src/system_nrf51.o 

S_UPPER_DEPS += \
./src/gcc_startup_nrf51.d 

C_DEPS += \
./src/app_error.d \
./src/app_fifo.d \
./src/app_uart_fifo.d \
./src/console.d \
./src/main.d \
./src/nrf_delay.d \
./src/nrf_drv_clock.d \
./src/nrf_drv_common.d \
./src/nrf_drv_gpiote.d \
./src/retarget.d \
./src/system_nrf51.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -DNRF51 -I"C:\NRFdev\projekty\freertos\inc" -I"C:\NRFdev\projekty\freertos\src\freertos\include" -isystem"C:\NRFdev\NRFSDK8\components\toolchain\gcc" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\clock" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\common" -isystem"C:\NRFdev\NRFSDK8\components\libraries\fifo" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\gpiote" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\nrf_soc_nosd" -isystem"C:\NRFdev\NRFSDK8\components\libraries\util" -isystem"C:\NRFdev\NRFSDK8\components\device" -isystem"C:\NRFdev\NRFSDK8\components\toolchain" -isystem"C:\NRFdev\NRFSDK8\components\drivers_nrf\hal" -isystem"C:\NRFdev\NRFSDK8\examples\bsp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.S
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections  -g3 -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


