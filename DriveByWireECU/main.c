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
#include "hal_usart_sync.h""
#include "DriveByWireIO.h"


/* define to avoid compilation warning */
#define LWIP_TIMEVAL_PRIVATE 0

void print_ipaddress(void)
{
	static char tmp_buff[16];
	printf("IP_ADDR    : %s\r\n",
	       ipaddr_ntoa_r((const ip_addr_t *)&(TCPIP_STACK_INTERFACE_0_desc.ip_addr), tmp_buff, 16));
	printf("NET_MASK   : %s\r\n",
	       ipaddr_ntoa_r((const ip_addr_t *)&(TCPIP_STACK_INTERFACE_0_desc.netmask), tmp_buff, 16));
	printf("GATEWAY_IP : %s\r\n", ipaddr_ntoa_r((const ip_addr_t *)&(TCPIP_STACK_INTERFACE_0_desc.gw), tmp_buff, 16));
}

/*
 * NOTE:
 * If compilation results in error: "redefinition of 'struct timeval'",
 * perform below step to make compilation successful.
 *
 * Navigate to sockets.h file in $PROJECT_LOCAION$\lwip\lwip-1.4.0\src\include\lwip &
 * change the MACRO LWIP_TIMEVAL_PRIVATE like below
 *
 * #define LWIP_TIMEVAL_PRIVATE		1
 *				to
 * #define LWIP_TIMEVAL_PRIVATE		0
 *
 */

 unsigned long long GetCurrentTime()
 {
	//TODO: Implement mechanism to count time from clock ticks or timer or RTC, or something idk
 }

 void ProcessAlgorithms(unsigned long long microseconds_elapsed, dbw_inputs_t* inputs)
 {
	//Calculate new outputs etc. Call SetBlah()...
	//If an some output is complicated. i.e. something like a PID controller, make a new function or .c/.h file.

	SetReverseDrive(inputs->reverse_commanded);

	return;
 }

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/*Handles Socket API */
	//printf("\r\nSocket API implementation\r\n");
	//basic_socket();

	int keep_running = 1;
	unsigned long long current_time_us;
	dbw_inputs_t* inputs = NULL;
	while (keep_running)
	{
		//TODO: Calculate time elapsed in microseconds since last loop start
		current_time_us = GetCurrentTime();

		ProcessCurrentInputs(current_time_us);

		inputs = GetCurrentInputs();

		ProcessAlgorithms(current_time_us, inputs);

		ProcessCurrentOutputs(current_time_us);

		SendUpdateToPC();

		//arbitrary throttling of main loop. 
		delay_us(100);
	}
	return 0;
}
