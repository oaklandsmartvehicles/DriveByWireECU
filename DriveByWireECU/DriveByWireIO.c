/*
 * DriveByWireIO.c
 *
 * Created: 9/29/2019 4:01:47 PM
 *  Author: John
 */ 
 #include "DriveByWireIO.h"
 #include "hal_gpio.h"
 #include "hal_delay.h"
 #include "hal_usart_sync.h"
 #include "driver_init.h"

 typedef union adc_val_t {
	 uint8_t ADC_Read_8[2];
	 uint16_t ADC_Read;
 } adc_val_t;

 dbw_inputs_t inputs;

 //interpolation table of steering positions[]
 const float steering_positions[] = {0, 1, 2};
 const float steering_voltages[] = {0, 1.65, 3.3};
 int steering_table_size = sizeof(steering_positions) / sizeof(float);
 

 float LinarlyInterpolate(float val_x, float left_x, float right_x, float left_y, float right_y)
 {
	float delta_x = right_x - left_x;
	float percent_x = (val_x - left_x) / delta_x;
	return left_y + (percent_x * (right_y - left_y));
 }

 float ReadSteeringPosition()
 {
	//TODO: determine voltage mapping between linear potentiometer and steering angle.
	//fill out linear interpolation table above
	if( steering_table_size < 2 )
		return 0.0f;

	//Read the steering position
	adc_val_t adc_val;
	adc_sync_read_channel(&ADC_0, 0, adc_val.ADC_Read_8, 2);
	float steering_voltage = ((float)adc_val.ADC_Read / (float)0xFFF) * 3.3f;
	for(int i = 0; i < steering_table_size; ++i)
	{
		//maximum value
		if( i == steering_table_size - 1 )
			return steering_positions[i];

		if( steering_voltage <= steering_voltages[i+1] )
		{
			//minimum value
			if( i == 0 )
				return steering_positions[0];

			//some intermediate value
			return LinarlyInterpolate(steering_voltage, steering_voltages[i], steering_voltages[i+1], steering_positions[i], steering_positions[i+1]);
		}
	}
	//if execution reaches here, then the above code is broken
	return 0.0f;
 }

 void ProcessCurrentInputs(double time_elapsed)
 {
	inputs.steering_position = ReadSteeringPosition();
	
 }

 dbw_inputs_t* GetCurrentInputs()
 {
	return &inputs;
 }

 void ProcessCurrentOutputs(double time_elapsed)
 {

 }
 void SendUpdateToPC()
 {

 }

 //non-zero values turn lights on
 void SetSafetyLight1On(int on)
 {

 }
 void SetSafetyLight2On(int on)
 {

 }

 //Cycles safety light mode pin until the desired mode is achieved.
 void SetSafetyLight1Mode(int mode)
 {

 }
 void SetSafetyLight2Mode(int mode)
 {

 }

 //non zero values steer right, zero steers left.
 void SetSteerDirection(int right)
 {

 }

 //Applies power to the steering motor as duty cycle percentage
 void SetSteeringTorque(float duty_cycle)
 {

 }

 //Puts the vehicle in reverse if value is non-zero.
 void SetReverseDrive(int reverse)
 {
 
 }

 //Sets the front brake PWM as duty cycle percentage.
 void SetFrontBrake(float duty_cycle)
 {


 }

 //Sets the rear drum brake PWM to the engaged position if value is non-zero
 void SetParkingBrake(int engaged)
 {

 }