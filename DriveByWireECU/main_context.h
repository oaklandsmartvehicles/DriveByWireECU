/*
 * main_context.h
 *
 * Created: 10/5/2019 4:36:43 PM
 *  Author: John
 */ 
#ifndef MAIN_CONTEXT_H_
#define MAIN_CONTEXT_H_
#include "FreeRTOS.h"
#include "semphr.h"
#include "EthernetIO.h"

typedef struct main_context_t
{
	SemaphoreHandle_t sem;

	uint32_t last_eth_input_rx_time;
	uint32_t current_time;

	//commanded from the driving agent
	float vehicle_speed_commanded;
	float steering_angle_commanded;
	uint8_t parking_brake_commanded;
	uint8_t reverse_commanded;
	uint8_t safety_lights_1_on_commanded;
	uint8_t safety_lights_2_on_commanded;

	//actual measured / current values 
	float vehicle_speed;
	float steering_angle;
	uint8_t parking_brake;
	uint8_t reverse;
	uint8_t safety_lights_1_on;
	uint8_t safety_lights_2_on;
	 
} main_context_t;

#endif /* MAIN_CONTEXT_H_ */