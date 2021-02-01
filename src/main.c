#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"

#include "app_uart.h"
#include "app_error.h"
#include "console.h"

#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "timers.h"
#include "sdk_errors.h"
#include "nrf_drv_clock.h"
#include "nordic_common.h"

#include "kitties.h"

#define TASK_DELAY        200           /**< Task delay. Delays a LED0 task for 200 ms */
#define TIMER_PERIOD      2000          /**< Timer period. LED1 timer will expire after 1000 ms */

TaskHandle_t uart_console_task_handle;
TimerHandle_t uart_console_idle_timer_handle;

/**@brief UART console task entry function.
 *
 * @param[in] pvParameter   Pointer that will be used as the parameter for the task.
 */
static void uart_console_task_function (void * pvParameter){

	UNUSED_PARAMETER(pvParameter);
	console_init();
	console_put_line("[RTOS] Console task init OK");
	xTimerStart(uart_console_idle_timer_handle,0);

	while(1){
		while(console_chars_available()>1){
		console_put_char(console_get_char());  //echo excess chars from fifo buffer
		}
		if(console_chars_available()==1){  //last char from buffer is used as a command
			uint8_t input_number=console_get_char();
			console_put_char(input_number);console_put_newline();
			switch(input_number){
			case '1':
				console_put_line("it's me, mario");
				break;
			case '2':
				console_put_line("there, general Kenobi");
				break;
			case '3':
				console_put_line("my friend");
				break;
			case '4':
				console_put_line("Have some kitty");
				for(uint8_t i=0;i<=(sizeof(kitty1)/sizeof(kitty1[0])-1);i++){     //draw some kitties
					console_put_line(kitty1[i]);
				}
				break;
			case '5':
				console_put_line("Thats another kitty for you");
				for(uint8_t i=0;i<=(sizeof(kitty2)/sizeof(kitty2[0])-1);i++){
					console_put_line(kitty2[i]);
				}
				break;
			default:
				console_put_line("Wait. That's illegal");
			}
			xTimerReset(uart_console_idle_timer_handle,5); //re-evaluate or start again the idle timer counter
			vTaskSuspend(uart_console_task_handle); //task is suspended until idle timer callback to prevent multiple commands input for 1 prompt
		}
		vTaskDelay(100);
	}
}

/**@brief Callback of uart_console_idle timer
 * when timer expires the console is cleared
 *
 * @param[in] pvParameter   Pointer that will be used as the parameter for the task.
 */
static void uart_console_idle_timer_callback( void * pvParameter){
	UNUSED_PARAMETER(pvParameter);
	console_clear_screen();
	console_flush_input_buffer(); //flush buffer to ignore chars entered when console task was suspended
	console_put_string("Hello\r\nYour response [1-5]: ");
	vTaskResume(uart_console_task_handle);
}

int main(void){

	SystemInit();
	ret_code_t err_code;

	err_code=nrf_drv_clock_init(NULL);
	APP_ERROR_CHECK(err_code);

	UNUSED_VARIABLE(xTaskCreate(uart_console_task_function,"UARTConsole",configMINIMAL_STACK_SIZE +200,NULL,2,&uart_console_task_handle));
	uart_console_idle_timer_handle=xTimerCreate("UARTIdleTimer",TIMER_PERIOD,pdFALSE,NULL,uart_console_idle_timer_callback);


	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

	vTaskStartScheduler();
	while (true)
	{
		//you shouldn't be here when freertos is running

	}

	return 0;
}
