#include "main.h"
#include "usart.h"
#include "as5050a_powerstep.h"

UART_HandleTypeDef huart2;

void MX_USART2_UART_Init(void){

	huart2.Instance = USART2;
	huart2.Init.BaudRate = 230400;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart2) != HAL_OK){
		Error_Handler();
	}
	HAL_UART_Receive_IT(&huart2, &rx_byte, 1);

}
void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle){
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	if(uartHandle->Instance==USART2){
		__HAL_RCC_USART2_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(USART2_IRQn);
	}
}
void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle){

	if(uartHandle->Instance==USART2){
		__HAL_RCC_USART2_CLK_DISABLE();

		/**USART2 GPIO Configuration
		PA2     ------> USART2_TX
		PA3     ------> USART2_RX
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

		HAL_NVIC_DisableIRQ(USART2_IRQn);
	}
} 
void USART_ReceiveData( void ) {

	if(rx_buffer[0] == 'S' && rx_buffer[1] == 'T' && rx_buffer[2] == 'P'){
		if(rx_buffer[3] != device_address)	return;

		motor_command = rx_buffer[4];
		motor_requested_pos = 256* rx_buffer[5] + rx_buffer[6];
		motor_speed[0] = rx_buffer[7];
		motor_speed[1] = rx_buffer[8];
		motor_speed[2] = rx_buffer[9];

		if(are_motors_initilized == 1){
			//HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_5);
		}

//		usart_is_busy = 1;
//		HAL_GPIO_WritePin( GPIOA , Tx2_en , GPIO_PIN_SET );
//
//		tx_buffer[0] = 's';
//		tx_buffer[1] = 't';
//		tx_buffer[2] = 'p';
//		tx_buffer[3] = device_address;
//		tx_buffer[4] = (char)(abs_position/65536)%256;
//		tx_buffer[5] = (char)(abs_position/256)%256;
//		tx_buffer[6] = (char)(abs_position)%256;
//		tx_buffer[7] = 0x0D;
//		tx_buffer[8] = 0x0A;
//		usart_send_start_delay = 2;
//		usart_send_start_command = 1;
	}
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
