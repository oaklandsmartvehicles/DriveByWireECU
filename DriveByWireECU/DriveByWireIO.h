/*
 * DriveByWireIO.h
 *
 * Created: 9/25/2019 7:52:07 PM
 *  Author: John Brooks
 */ 


#ifndef DRIVEBYWIREIO_H_
#define DRIVEBYWIREIO_H_

typedef struct dbw_inputs_t{
	float wheel_speed_left;
	float wheel_speed_right;
	float steering_position;

	//commanded values from driving agent rx'd over Ethernet
	float vehicle_speed_commanded;
	int reverse_commanded;
	float steering_angle_commanded;
	int parking_brake_commanded;
	int safety_lights_1_on_commanded;
	int safety_lights_2_on_commanded; 
} dbw_inputs_t;

typedef struct dbw_outputs{

} dbw_outputs;


void ProcessCurrentInputs(double time_elapsed);
dbw_inputs_t* GetCurrentInputs();
void ProcessCurrentOutputs(double time_elapsed);
void SendUpdateToPC();

//non-zero values turn lights on
void SetSafetyLight1On(int on);
void SetSafetyLight2On(int on);

//Cycles safety light mode pin until the desired mode is achieved.
void SetSafetyLight1Mode(int mode);
void SetSafetyLight2Mode(int mode);

//non zero values steer right, zero steers left.
void SetAcceleration(float duty_cycle);

//non zero values steer right, zero steers left.
void SetSteerDirection(int right);

//Applies power to the steering motor as duty cycle percentage
void SetSteeringTorque(float duty_cycle);

//Puts the vehicle in reverse if value is non-zero.
void SetReverseDrive(int reverse);

//Sets the front brake PWM as duty cycle percentage.
void SetFrontBrake(float duty_cycle);

//Sets the rear drum brake PWM to the engaged position if value is non-zero
void SetParkingBrake(int engaged);

#endif /* DRIVEBYWIREIO_H_ */