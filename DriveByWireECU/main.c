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
#include "PID.h"

/* define to avoid compilation warning */
#define LWIP_TIMEVAL_PRIVATE 0

#define MAIN_TASK_LOOP_TIME 1 //milliseconds

#define PARKING_BRAKE_DUTY_CYCLE 0.25
#define COME_TO_STOP_BRAKE_DUTY_CYCLE 0.5
#define EMERGENCY_STOP_BRAKE_DUTY_CYCLE 1.0

#define MAX_STEERING_ANGLE 50.0
#define MIN_STEERING_ANGLE -50.0

#define MAX_VEHICLE_SPEED 12.0
#define MIN_VEHICLE_SPEED -1.0

#define MAX_STEERING_DUTY_CYCLE 1.0
#define MAX_ACCEL_DUTY_CYCLE 1.0
#define MIN_ACCEL_DUTY_CYCLE -0.25

//duty cycle (0.0 - 1.0) / degrees
#define STEERING_P_GAIN (1.0 / 30.0) //100% duty cycle at angles greater than 60 deg
//duty cycle / degrees
#define STEERING_I_GAIN (0.05 / (1 * 1000)) //5% duty cycle for every second we are 1 deg off.
#define STEERING_D_GAIN 0.0

//duty cycle (0.0 - 1.0) / m/s
#define SPEED_P_GAIN (1.0 / 1.0) //100% duty cycle at speed errors > 2.2 MPH
#define SPEED_I_GAIN (0.05 / (0.1 * 1000)) //5% duty cycle for every second we are .2 MPH off of our target.
#define SPEED_D_GAIN 0.0

static main_context_t ctx;

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
int ConvertAngleToPIDInt(float angle)
{
	return (int)(angle * 1000.0);
}
int ConvertSpeedToPIDInt(float speed)
{
	return (int)(speed * 1000.0);
}
float ConvertPIDIntToDutyCycle(int PID_int)
{
	return ((float)PID_int) / 1000.0;
}
int ConvertDutyCycleToPIDInt(float duty_cycle)
{
	return ((int)duty_cycle) * 1000.0;
}
//PID Callback functions
int SteeringPIDSource()
{
	return ConvertAngleToPIDInt(ctx.steering_angle_commanded);
}
int SpeedPIDSource()
{
	return ConvertSpeedToPIDInt(ctx.vehicle_speed_commanded);
}
void SteeringPIDOutput(int pid_output)
{
	ctx.steering_torque_pid_out = ConvertPIDIntToDutyCycle(pid_output);
}
void SpeedPIDOutput(int pid_output)
{
	ctx.acceleration_pid_out = ConvertPIDIntToDutyCycle(pid_output);
}
 unsigned long GetPIDTime()
 {
	return (unsigned long) GetCurrentTime();
 }


 void ProcessAlgorithms(main_context_t* ctx)
 {
	ctx->estop_indicator = 0;
	setEnabled(&ctx->steering_controller, ctx->autonomous_mode && !ctx->estop_in && !ctx->park_brake_commanded);
	setEnabled(&ctx->speed_controller, ctx->autonomous_mode && !ctx->estop_in && !ctx->park_brake_commanded);
	tick(&ctx->steering_controller);
	tick(&ctx->speed_controller);

	if( ctx->last_eth_input_rx_time - ctx->current_time > 250)
	{
		ctx->pc_comm_active = 0;
		ctx->autonomous_mode = 0;
	}
	
	if(ctx->autonomous_mode)
	{
		SetEStopState(ctx->estop_in);
		SetSafetyLight1On(1);
		
		if(ctx->estop_in)
		{
			SetFrontBrake(EMERGENCY_STOP_BRAKE_DUTY_CYCLE);
			SetAcceleration(0.0);
			ctx->estop_indicator = 1;
		}
		else if(ctx->park_brake_commanded)
		{
			SetFrontBrake(PARKING_BRAKE_DUTY_CYCLE);
			SetAcceleration(0.0);
		}
		else
		{
			//regular autonomous mode
			float accel = ctx->acceleration_pid_out;
			
			//if reverse commanded
			if( accel < 0.0 )
			{
				//if we are moving forward
				if(ctx->vehicle_speed > 0)
				{
					//Don't engage reverse while we are moving forward.
					accel = 0.0;
					SetFrontBrake(COME_TO_STOP_BRAKE_DUTY_CYCLE);
				}
				else //not moving forward and reverse commanded
				{
					SetReverseDrive(1);
					accel = -accel;
				}
			}
			else //reverse not commanded
			{
				SetReverseDrive(0);
			}
			if( accel > 1.0)
				accel = 1.0;
			SetAcceleration( accel );

			float SteeringTorqueFromPID = ctx->steering_torque_pid_out;
			SetSteerDirection(SteeringTorqueFromPID < 0.0);

			//limit the torque 0 to 1
			if( SteeringTorqueFromPID < 0.0 )
				SteeringTorqueFromPID = -SteeringTorqueFromPID;
			if( SteeringTorqueFromPID > 1.0 )
				SteeringTorqueFromPID = 1.0;

			SetSteeringTorque( SteeringTorqueFromPID );
		}
	}
	else //not in autonomous mode
	{
		SetSafetyLight1On(0);
		SetSteeringTorque(0.0);
		SetAcceleration(0.0);
		SetReverseDrive(0);
	}
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
	
	//Initialize PID controllers.
	ctx.steering_controller.p = STEERING_P_GAIN;
	ctx.steering_controller.i = STEERING_I_GAIN;
	ctx.steering_controller.d = STEERING_D_GAIN;
	ctx.steering_controller.pidSource = SteeringPIDSource;
	ctx.steering_controller.pidOutput = SteeringPIDOutput;
	setInputBounds(&(ctx.steering_controller), ConvertAngleToPIDInt(MIN_STEERING_ANGLE), ConvertAngleToPIDInt(MAX_STEERING_ANGLE));
	setOutputBounds(&(ctx.steering_controller), ConvertDutyCycleToPIDInt(MAX_STEERING_DUTY_CYCLE)*-1, ConvertDutyCycleToPIDInt(MAX_STEERING_DUTY_CYCLE));
	ctx.steering_controller.getSystemTime = GetPIDTime;

	ctx.speed_controller.p = SPEED_P_GAIN;
	ctx.speed_controller.i = SPEED_I_GAIN;
	ctx.speed_controller.d = SPEED_D_GAIN;
	ctx.speed_controller.pidSource = SpeedPIDSource;
	ctx.speed_controller.pidOutput = SpeedPIDOutput;
	setInputBounds(&(ctx.speed_controller), ConvertSpeedToPIDInt(MIN_VEHICLE_SPEED), ConvertSpeedToPIDInt(MAX_VEHICLE_SPEED));
	setOutputBounds(&(ctx.speed_controller), ConvertDutyCycleToPIDInt(MIN_ACCEL_DUTY_CYCLE), ConvertDutyCycleToPIDInt(MAX_ACCEL_DUTY_CYCLE));
	ctx.speed_controller.getSystemTime = GetPIDTime;
	
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
