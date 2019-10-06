/**
 * \file
 *
 * \brief LwIP socket api application implementation
 *
 * Copyright (c) 2019 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include "atmel_start.h"
#include "lwip_socket_api.h"
#include "hal_gpio.h"
#include "hal_delay.h"
#include "hal_usart_sync.h"
#include "DriveByWireIO.h"
#include "driver_examples.h"
#include "EthernetIO.h"
#include "FreeRTOS.h"
#include "main_context.h"

/* define to avoid compilation warning */
#define LWIP_TIMEVAL_PRIVATE 0

#define MAIN_TASK_LOOP_TIME 1 //milliseconds

void print_ipaddress(void)
{
	static char tmp_buff[16];
	printf("IP_ADDR    : %s\r\n",
	       ipaddr_ntoa_r((const ip_addr_t *)&(TCPIP_STACK_INTERFACE_0_desc.ip_addr), tmp_buff, 16));
	printf("NET_MASK   : %s\r\n",
	       ipaddr_ntoa_r((const ip_addr_t *)&(TCPIP_STACK_INTERFACE_0_desc.netmask), tmp_buff, 16));
	printf("GATEWAY_IP : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *)&(TCPIP_STACK_INTERFACE_0_desc.gw), tmp_buff, 16));
}

 uint32_t GetCurrentTime()
 {
	return xTaskGetTickCount();
 }

 void ProcessAlgorithms(main_context_t* ctx)
 {
	//Calculate new outputs etc. Call SetBlah()...
	//If an some output is complicated. i.e. something like a PID controller, make a new function or .c/.h file.
	SetAcceleration(ctx->vehicle_speed_commanded);

	return;
 }

void main_task(void* p)
{
	main_context_t* context = (main_context_t*)p;
	while (1)
	{
		if(xSemaphoreTake(context->sem, 5) != pdTRUE)
		{
			printf("Failed to take IO semaphore!\n");
			vTaskDelay(1);
		}
		
		context->current_time = GetCurrentTime();
		ProcessCurrentInputs(context);
		ProcessAlgorithms(context);
		ProcessCurrentOutputs(context);

		xSemaphoreGive(context->sem);
		vTaskDelay(MAIN_TASK_LOOP_TIME);
	}
}

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	static main_context_t ctx;
	memset(&ctx, 0, sizeof(ctx));
	ctx.sem = xSemaphoreCreateBinary();
	xSemaphoreGive(ctx.sem);

	xTaskCreate(ethernet_thread,
		"Ethernet_Task",
		2048,
		&ctx,
		0,
		NULL);

	xTaskCreate(main_task,
		"Main_Task",
		2048,
		&ctx,
		2,
		NULL);

	vTaskStartScheduler();
	
	//Should never reach here as vTaskStartScheduler is infinitely blocking
	//If we DO reach here then there is insufficient RAM
	printf("Insufficient RTOS heap available to create the idle or timer daemon tasks");

	return 0;
}
