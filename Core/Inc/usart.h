#ifndef __usart_H
#define __usart_H

#define TX_ARRAY_SIZE 				 (uint8_t)32
#define RX_ARRAY_SIZE 				 (uint8_t)32

uint8_t rx_buffer[RX_ARRAY_SIZE];
uint8_t	tx_buffer[RX_ARRAY_SIZE];
uint8_t rx_counter;
uint8_t rx_byte, tx_byte;

void MX_USART2_UART_Init(void);
void USART_ReceiveData(void);

uint32_t buffer_clear_timer;
uint8_t buffer_cleared;
uint8_t			control_ReceiveData;

#endif /*__ usart_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
