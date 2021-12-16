#include "stm32f0xx_hal.h"
#include "main.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "as5050a_powerstep.h"

UART_HandleTypeDef huart2;

void SystemClock_Config(void);

void HAL_IncTick(void){
	_1_msec = 1;
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	static int cntr = 0;

	if (htim->Instance == TIM1) {
		cntr++;
		_100_usec = 1;
		if((cntr % 100) == 0) _10_msec = 1;
		if((cntr % 1000) == 0) _100_msec = 1;
		if((cntr % 10000) == 0) _1_sec = 1;
	}
}
void clear_usart_buffer(void) {
	rx_counter = 0;
	for (uint8_t i = 0; i < RX_ARRAY_SIZE; i++) {
		rx_buffer[i] = 0;
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART2) {
		rx_buffer[rx_counter] = rx_byte;
		buffer_clear_timer = 10;
		buffer_cleared = 0;
		if ((rx_buffer[rx_counter] == 0x0A) && (rx_buffer[rx_counter - 1] == 0x0D)) {
			control_ReceiveData = 1;

		}
		if (rx_counter < RX_ARRAY_SIZE) {
			rx_counter++;
		}
		HAL_UART_Receive_IT(&huart2, &rx_byte, 1);
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if (huart->Instance == USART2) {
		usart_send_finish_delay = 1;
		usart_send_finish_command = 1;
	}
}
void read_address(void){
	device_address = 0;

	if(HAL_GPIO_ReadPin(GPIOC,ADDRESS_PIN_01) == 0){
		device_address = device_address | 0x01;
	}
	if(HAL_GPIO_ReadPin(GPIOC,ADDRESS_PIN_02) == 0){
		device_address = device_address | 0x02;
	}
	if(HAL_GPIO_ReadPin(GPIOA,ADDRESS_PIN_03) == 0){
		device_address = device_address | 0x04;
	}
	if(HAL_GPIO_ReadPin(GPIOA,ADDRESS_PIN_04) == 0){
		device_address = device_address | 0x08;
	}
}
int main(void){
	unsigned int pos = 0;

	HAL_Init();
	SystemClock_Config();

	MX_GPIO_Init();
	MX_SPI2_Init();
	MX_TIM1_Init();
	//MX_TIM3_Init();
	MX_USART2_UART_Init();

	_100_usec = 0;
	_1_msec = 0;
	_10_msec = 0;
	_100_msec = 0;
	_1_sec = 0;

	control_ReceiveData = 0;
	rx_counter = 0;
	are_motors_initilized = 0;
	status_byte = 12;
	motor_command = 0;
	current_mode_control = 0;

	usart_send_start_command = 0;
	usart_send_start_delay = 0;
	usart_send_finish_command = 0;
	usart_send_finish_delay = 0;
	usart_is_busy = 0;

	HAL_GPIO_WritePin( GPIOA , Tx2_en , GPIO_PIN_RESET );

	while (1){

		if(buffer_cleared == 0) {
			if (buffer_clear_timer == 0) {
				buffer_cleared = 1;
				clear_usart_buffer();
				HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
				if(are_motors_initilized == 1){
					usart_is_busy = 0;
					if(usart_is_busy == 0){
						abs_position = POWERSTEP01_SPI_READ_24_BIT(0x01);

						switch (motor_command) {
						case 1:
							//spd = 65535;
							//SEND_Command_4_byte(RUN_Command_Reverse, ((spd/65536)%256), ((spd/256)%256), (spd%256));
							SEND_Command_4_byte(RUN_Command_Reverse, motor_speed[0], motor_speed[1], motor_speed[2]);
							break;
						case 2:
							//spd = motor_speed;
							SEND_Command_4_byte(RUN_Command_Forward, motor_speed[0], motor_speed[1], motor_speed[2]);
							break;
						case 3 :
							SEND_Command_1_byte(HARD_STOP_Command);
							break;
						case 4 :
							SEND_Command_1_byte(RESET_POS_Command);
							break;
						case 5 :
							pos = motor_requested_pos;
							SEND_Command_4_byte(0x60, ((pos / 65535)%256), ((pos / 256)%256), (pos % 256));
							break;
						default:
							break;
						}
					}
				}
			}
		}

		if(control_ReceiveData == 1){
			USART_ReceiveData();
			control_ReceiveData = 0;
		}

		if(_100_usec == 1){
			_100_usec = 0;
			if (buffer_clear_timer > 0) buffer_clear_timer--;
			if (usart_send_start_delay > 0) usart_send_start_delay--;
			if (usart_send_finish_delay > 0) usart_send_finish_delay--;
			if(usart_send_finish_command == 1){
				if(usart_send_finish_delay == 0){
					usart_send_finish_command = 0;
					HAL_GPIO_WritePin( GPIOA , Tx2_en , GPIO_PIN_RESET );
					usart_is_busy = 0;
				}
			}
			if(usart_send_start_command == 1){
				if(usart_send_start_delay == 0){
					usart_send_start_command = 0;
					HAL_UART_Transmit_IT(&huart2,&tx_buffer[0],9);
				}
			}
		}
		if(_1_msec == 1){
			_1_msec = 0;

		}
		if(_10_msec == 1){
			_10_msec = 0;
		}
		if(_100_msec == 1){
			_100_msec = 0;

			if(are_motors_initilized == 0){
				initilize_stepper_motors();
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_RESET);
			}
			else{
				HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
			}
		}
		if(_1_sec == 1){
			_1_sec = 0;
			read_address();
		}
	}
}
void SystemClock_Config(void){
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL4;
	RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK){
		Error_Handler();
	}

	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
							  |RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK){
		Error_Handler();
	}
}
void Error_Handler(void){
}
