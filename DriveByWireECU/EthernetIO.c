/*
 * EthernetIO.c
 *
 * Created: 10/2/2019 8:02:24 AM
 *  Author: John Brooks
 */ 
#include "EthernetIO.h"
#include "sockets.h"
#include "lwip/sys.h"
#include "lwip/api.h"
#include "lwip/tcpip.h"
#include "webserver_tasks.h"
#include "main_context.h"

#define ECU_IP "192.168.2.100"
#define ECU_PORT "1234"
#define BROADCAST_PORT "1235"
#define PC_IP "255.255.255.255"
#define PC_PORT "1236"
#define SUBNET_MASK "255.255.255.0"

#define TRANSMIT_INTERVAL 10 //milliseconds

struct sockaddr_in ecu_addr, pc_addr;
static int lwip_initialized = 0;

int InitializeLWIP()
{
	if(lwip_initialized)
		return 0;
	sys_sem_t sem;
	err_t     err_sem;
	err_sem = sys_sem_new(&sem, 0); /* Create a new semaphore. */
	tcpip_init(tcpip_init_done, &sem);
	sys_sem_wait(&sem); /* Block until the lwIP stack is initialized. */
	sys_sem_free(&sem); /* Free the semaphore. */
	print_ipaddress();
	lwip_initialized = 1;
	return 0; 
}

void decode_ethernet_inputs(EthernetInputs* inputs, main_context_t* ctx)
{
	ctx->steering_angle_commanded = (float)inputs->steering_angle_commanded * 0.1;
	ctx->vehicle_speed_commanded = (float)inputs->vehicle_speed_commanded * 0.01;
	ctx->parking_brake_commanded = (inputs->boolean_commands & 0x1) != 0;
	ctx->reverse_commanded = (inputs->boolean_commands & 0x2) != 0;
	ctx->safety_lights_1_on_commanded = (inputs->boolean_commands & 0x4) != 0;
	ctx->safety_lights_2_on_commanded = (inputs->boolean_commands & 0x8) != 0;
}
void encode_ethernet_outputs(EthernetOutputs* outputs, main_context_t* ctx)
{
	outputs->steering_angle = ctx->steering_angle / 0.1;
	outputs->vehicle_speed = ctx->vehicle_speed / 0.01;
}

void ethernet_thread(void *p)
{
	main_context_t* ctx = (main_context_t*)p;
	EthernetInputs eth_inputs;
	EthernetOutputs eth_outputs;

	memset(&eth_inputs, 0, sizeof(eth_inputs));
	memset(&eth_outputs, 0, sizeof(eth_outputs));

	InitializeLWIP();

	struct sockaddr_in sa, ra;
	int s_create, new_socket;
	int socket_check;
	int opt = 1;
	int num_bytes_received = 0;

	/*Create a socket*/
	s_create = socket(AF_INET, SOCK_DGRAM, 0);

	setsockopt(s_create, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

	//Destination
	memset(&ra, 0, sizeof(ra));
	ra.sin_family 		= AF_INET;
	ra.sin_addr.s_addr	= htonl(INADDR_BROADCAST);
	ra.sin_port        	= htons(12089);
	ra.sin_len			= sizeof(ra);

	//Source
	memset(&sa, 0, sizeof(sa));
	sa.sin_family		= AF_INET;
	sa.sin_addr.s_addr	= htonl(INADDR_ANY);
	sa.sin_port			= htons(12090);
	sa.sin_len			= sizeof(sa);

	/* bind the connection to port */
	socket_check = bind(s_create, (struct sockaddr *)&sa, sizeof(sa));
	if (socket_check < 0) {
		LWIP_DEBUGF(LWIP_DBG_ON, ("Bind error=%d\n", socket_check));
		return;
	}

	while(1)
	{
		if( xSemaphoreTake(ctx->sem, 100) != pdTRUE)
		{
			vTaskDelay(1);
			continue;
		}

		encode_ethernet_outputs(&eth_outputs, ctx);

		num_bytes_received = sendto(s_create, &eth_outputs, sizeof(eth_outputs), 0, &ra, sizeof(ra));

		num_bytes_received = recv(s_create, &eth_inputs, sizeof(eth_inputs), MSG_DONTWAIT);
		if(num_bytes_received > 0)
		{
			ctx->last_eth_input_rx_time = xTaskGetTickCount();
			decode_ethernet_inputs(&eth_inputs, ctx);
		}

		xSemaphoreGive(ctx->sem);
		vTaskDelay(TRANSMIT_INTERVAL);
	}
}