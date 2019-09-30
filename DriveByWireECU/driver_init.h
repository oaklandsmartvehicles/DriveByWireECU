/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef DRIVER_INIT_INCLUDED
#define DRIVER_INIT_INCLUDED

#include "atmel_start_pins.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <hal_atomic.h>
#include <hal_delay.h>
#include <hal_gpio.h>
#include <hal_init.h>
#include <hal_io.h>
#include <hal_sleep.h>

#include <hal_adc_sync.h>

#include <hal_usart_sync.h>
#include <hal_pwm.h>
#include <hpl_tc_base.h>
#include <hal_pwm.h>
#include <hpl_tc_base.h>
#include <hal_pwm.h>
#include <hpl_tc_base.h>
#include <hal_pwm.h>
#include <hpl_tc_base.h>
#include <hal_pwm.h>
#include <hpl_tc_base.h>
#include <hal_can_async.h>

#include <hal_mac_async.h>

extern struct adc_sync_descriptor ADC_0;

extern struct usart_sync_descriptor TARGET_IO;

extern struct pwm_descriptor PWM_0;

extern struct pwm_descriptor PWM_1;

extern struct pwm_descriptor PWM_4;

extern struct pwm_descriptor PWM_2;

extern struct pwm_descriptor       PWM_3;
extern struct can_async_descriptor CAN_0;

extern struct mac_async_descriptor COMMUNICATION_IO;

void ADC_0_PORT_init(void);
void ADC_0_CLOCK_init(void);
void ADC_0_init(void);

void TARGET_IO_PORT_init(void);
void TARGET_IO_CLOCK_init(void);
void TARGET_IO_init(void);

void PWM_0_PORT_init(void);
void PWM_0_CLOCK_init(void);
void PWM_0_init(void);

void PWM_1_PORT_init(void);
void PWM_1_CLOCK_init(void);
void PWM_1_init(void);

void PWM_4_PORT_init(void);
void PWM_4_CLOCK_init(void);
void PWM_4_init(void);

void PWM_2_PORT_init(void);
void PWM_2_CLOCK_init(void);
void PWM_2_init(void);

void PWM_3_PORT_init(void);
void PWM_3_CLOCK_init(void);
void PWM_3_init(void);

void COMMUNICATION_IO_CLOCK_init(void);
void COMMUNICATION_IO_init(void);
void COMMUNICATION_IO_PORT_init(void);
void COMMUNICATION_IO_example(void);

/**
 * \brief Perform system initialization, initialize pins and clocks for
 * peripherals
 */
void system_init(void);

#ifdef __cplusplus
}
#endif
#endif // DRIVER_INIT_INCLUDED
