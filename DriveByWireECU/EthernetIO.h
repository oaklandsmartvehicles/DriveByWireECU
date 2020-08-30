/*
 * EthernetIO.h
 *
 * Created: 10/2/2019 8:02:09 AM
 *  Author: John
 */ 
 #ifndef ETHERNETIO_H_
 #define ETHERNETIO_H_

 #include "stdint.h"

 //!!!WARNING!!! 
//If you add members to this struct you MUST update the transmitting 
//software with the new structure definition. 
// DONT USE ANYTHING SMALLER THAN uint16_t
typedef struct EthernetInputs_t
{
	//Decoding:
	//	0x1:	parking_brake_commanded
	//	0x2:	reverse_commanded
	//	0x4:	autonomous mode
	//	0x8:	override_pid
	//  0x10:	tele_operation_mode
	uint16_t boolean_commands;
	
	//Decoding: RAW * 0.01 (m/s)
	uint16_t vehicle_speed_commanded;
	
	//Decoding: RAW * 0.1 (degrees)
	uint16_t steering_angle_commanded;

	//Decoding: RAW * 0.000001
	//Range: ~ 0.000001 - 1000
	uint32_t speed_p_gain;
	uint32_t speed_i_gain;
	uint32_t speed_d_gain;
	uint32_t steering_p_gain;
	uint32_t steering_i_gain;
	uint32_t steering_d_gain;
	
	//total size should be 
} EthernetInputs;

//!!!WARNING!!! 
//If you add members to this struct you MUST update the receiving 
//software with the new structure definition. 
typedef struct EthernetOutputs_t
{
	//Encoding: value / 0.01 (m/s)
	uint16_t vehicle_speed;
	
	//Encoding: value / 0.1 (degrees)
	uint16_t steering_angle;	

	//Decoding:
	//	0x1:	estop_state
	uint8_t boolean_states;

	//Decoding: value / 0.00001 (percent)
	uint32_t speed_p_term;
	uint32_t speed_i_term;
	uint32_t speed_d_term;
	uint32_t steering_p_term;
	uint32_t steering_i_term;
	uint32_t steering_d_term;

} EthernetOutputs;

void ethernet_thread(void *p);

#endif /* ETHERNETIO_H_ */