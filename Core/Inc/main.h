#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f0xx_hal.h"

#define ADDRESS_PIN_01	GPIO_PIN_14	//C
#define ADDRESS_PIN_02	GPIO_PIN_15	//C
#define ADDRESS_PIN_03	GPIO_PIN_0	//A
#define ADDRESS_PIN_04	GPIO_PIN_1	//A

void Error_Handler(void);

volatile uint8_t _100_usec, _1_msec,_10_msec,_100_msec,_1_sec;

uint32_t 	abs_position;
uint32_t	motor_requested_pos;

uint8_t are_motors_initilized;
uint8_t motor_command;
uint8_t	motor_speed[3];
uint8_t status_byte;
uint8_t current_mode_control;
uint8_t	device_address;

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
