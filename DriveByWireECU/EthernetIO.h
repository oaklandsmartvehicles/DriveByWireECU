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
typedef struct EthernetInputs_t
{
	//Decoding: RAW * 0.01 (m/s)
	uint16_t vehicle_speed_commanded;
	
	//Decoding: RAW * 0.1 (degrees)
	uint16_t steering_angle_commanded;
	
	//Decoding:
	//	0x1:	parking_brake_commanded 
	//	0x2:	reverse_commanded
	//	0x4:	autonomous mode 
	uint8_t boolean_commands;	

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
} EthernetOutputs;

void ethernet_thread(void *p);

#endif /* ETHERNETIO_H_ */