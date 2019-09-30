/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hal_usart_sync.h>

#include <hpl_adc_base.h>

struct can_async_descriptor CAN_0;

struct adc_sync_descriptor ADC_0;

struct usart_sync_descriptor TARGET_IO;

struct pwm_descriptor PWM_0;

struct pwm_descriptor PWM_1;

struct pwm_descriptor PWM_4;

struct pwm_descriptor PWM_2;

struct pwm_descriptor PWM_3;

struct mac_async_descriptor COMMUNICATION_IO;

void ADC_0_PORT_init(void)
{

	// Disable digital pin circuitry
	gpio_set_pin_direction(SteeringPosition, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(SteeringPosition, PINMUX_PB08B_ADC1_AIN0);
}

void ADC_0_CLOCK_init(void)
{
	hri_mclk_set_APBDMASK_ADC1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, ADC1_GCLK_ID, CONF_GCLK_ADC1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void ADC_0_init(void)
{
	ADC_0_CLOCK_init();
	ADC_0_PORT_init();
	adc_sync_init(&ADC_0, ADC1, (void *)NULL);
	adc_sync_enable_channel(&ADC_0, 0);
}

void TARGET_IO_PORT_init(void)
{

	gpio_set_pin_function(PB25, PINMUX_PB25D_SERCOM2_PAD0);

	gpio_set_pin_function(PB24, PINMUX_PB24D_SERCOM2_PAD1);
}

void TARGET_IO_CLOCK_init(void)
{
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_CORE, CONF_GCLK_SERCOM2_CORE_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	hri_gclk_write_PCHCTRL_reg(GCLK, SERCOM2_GCLK_ID_SLOW, CONF_GCLK_SERCOM2_SLOW_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));

	hri_mclk_set_APBBMASK_SERCOM2_bit(MCLK);
}

void TARGET_IO_init(void)
{
	TARGET_IO_CLOCK_init();
	usart_sync_init(&TARGET_IO, SERCOM2, (void *)NULL);
	TARGET_IO_PORT_init();
}

void PWM_0_PORT_init(void)
{

	gpio_set_pin_function(PA05, PINMUX_PA05E_TC0_WO1);
}

void PWM_0_CLOCK_init(void)
{

	hri_mclk_set_APBAMASK_TC0_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC0_GCLK_ID, CONF_GCLK_TC0_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void PWM_0_init(void)
{
	PWM_0_CLOCK_init();
	PWM_0_PORT_init();
	pwm_init(&PWM_0, TC0, _tc_get_pwm());
}

void PWM_1_PORT_init(void)
{
}

void PWM_1_CLOCK_init(void)
{

	hri_mclk_set_APBAMASK_TC1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC1_GCLK_ID, CONF_GCLK_TC1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void PWM_1_init(void)
{
	PWM_1_CLOCK_init();
	PWM_1_PORT_init();
	pwm_init(&PWM_1, TC1, _tc_get_pwm());
}

void PWM_4_PORT_init(void)
{

	gpio_set_pin_function(PB09, PINMUX_PB09E_TC4_WO1);
}

void PWM_4_CLOCK_init(void)
{

	hri_mclk_set_APBCMASK_TC4_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC4_GCLK_ID, CONF_GCLK_TC4_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void PWM_4_init(void)
{
	PWM_4_CLOCK_init();
	PWM_4_PORT_init();
	pwm_init(&PWM_4, TC4, _tc_get_pwm());
}

void PWM_2_PORT_init(void)
{

	gpio_set_pin_function(PB15, PINMUX_PB15E_TC5_WO1);
}

void PWM_2_CLOCK_init(void)
{

	hri_mclk_set_APBCMASK_TC5_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC5_GCLK_ID, CONF_GCLK_TC5_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void PWM_2_init(void)
{
	PWM_2_CLOCK_init();
	PWM_2_PORT_init();
	pwm_init(&PWM_2, TC5, _tc_get_pwm());
}

void PWM_3_PORT_init(void)
{

	gpio_set_pin_function(PB03, PINMUX_PB03E_TC6_WO1);
}

void PWM_3_CLOCK_init(void)
{

	hri_mclk_set_APBDMASK_TC6_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, TC6_GCLK_ID, CONF_GCLK_TC6_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
}

void PWM_3_init(void)
{
	PWM_3_CLOCK_init();
	PWM_3_PORT_init();
	pwm_init(&PWM_3, TC6, _tc_get_pwm());
}

void CAN_0_PORT_init(void)
{

	gpio_set_pin_function(PB13, PINMUX_PB13H_CAN1_RX);

	gpio_set_pin_function(PB12, PINMUX_PB12H_CAN1_TX);
}
/**
 * \brief CAN initialization function
 *
 * Enables CAN peripheral, clocks and initializes CAN driver
 */
void CAN_0_init(void)
{
	hri_mclk_set_AHBMASK_CAN1_bit(MCLK);
	hri_gclk_write_PCHCTRL_reg(GCLK, CAN1_GCLK_ID, CONF_GCLK_CAN1_SRC | (1 << GCLK_PCHCTRL_CHEN_Pos));
	can_async_init(&CAN_0, CAN1);
	CAN_0_PORT_init();
}

void COMMUNICATION_IO_PORT_init(void)
{

	gpio_set_pin_function(PC11, PINMUX_PC11L_GMAC_GMDC);

	gpio_set_pin_function(PC12, PINMUX_PC12L_GMAC_GMDIO);

	gpio_set_pin_function(PA13, PINMUX_PA13L_GMAC_GRX0);

	gpio_set_pin_function(PA12, PINMUX_PA12L_GMAC_GRX1);

	gpio_set_pin_function(PC20, PINMUX_PC20L_GMAC_GRXDV);

	gpio_set_pin_function(PA15, PINMUX_PA15L_GMAC_GRXER);

	gpio_set_pin_function(PA18, PINMUX_PA18L_GMAC_GTX0);

	gpio_set_pin_function(PA19, PINMUX_PA19L_GMAC_GTX1);

	gpio_set_pin_function(PA14, PINMUX_PA14L_GMAC_GTXCK);

	gpio_set_pin_function(PA17, PINMUX_PA17L_GMAC_GTXEN);
}

void COMMUNICATION_IO_CLOCK_init(void)
{
	hri_mclk_set_AHBMASK_GMAC_bit(MCLK);
	hri_mclk_set_APBCMASK_GMAC_bit(MCLK);
}

void COMMUNICATION_IO_init(void)
{
	COMMUNICATION_IO_CLOCK_init();
	mac_async_init(&COMMUNICATION_IO, GMAC);
	COMMUNICATION_IO_PORT_init();
}

void COMMUNICATION_IO_example(void)
{
	mac_async_enable(&COMMUNICATION_IO);
	mac_async_write(&COMMUNICATION_IO, (uint8_t *)"Hello World!", 12);
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA06

	gpio_set_pin_level(SteerLeft,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SteerLeft, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SteerLeft, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA07

	gpio_set_pin_level(SteerRight,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SteerRight, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SteerRight, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB01

	gpio_set_pin_level(SafetyLights2Mode,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SafetyLights2Mode, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SafetyLights2Mode, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB06

	gpio_set_pin_level(SafetyLights2On,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SafetyLights2On, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SafetyLights2On, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PB07

	// Set pin direction to input
	gpio_set_pin_direction(WheelSpeed2, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(WheelSpeed2,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_DOWN);

	gpio_set_pin_function(WheelSpeed2, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC01

	gpio_set_pin_level(SafetyLights1On,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SafetyLights1On, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SafetyLights1On, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC10

	gpio_set_pin_level(SafetyLights1Mode,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(SafetyLights1Mode, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(SafetyLights1Mode, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC18

	gpio_set_pin_level(LED_0,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(LED_0, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED_0, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PC30

	gpio_set_pin_level(ReverseDrive,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(ReverseDrive, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(ReverseDrive, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PD00

	// Set pin direction to input
	gpio_set_pin_direction(WheelSpeed, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(WheelSpeed,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_DOWN);

	gpio_set_pin_function(WheelSpeed, GPIO_PIN_FUNCTION_OFF);

	ADC_0_init();

	TARGET_IO_init();

	PWM_0_init();

	PWM_1_init();

	PWM_4_init();

	PWM_2_init();

	PWM_3_init();
	CAN_0_init();

	COMMUNICATION_IO_init();
}
