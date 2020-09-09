#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"
#include "as5050a_powerstep.h"
#include "stdio.h"
#include "math.h"

SPI_HandleTypeDef hspi2;
union _my_data my_data;

void SEND_Command_1_byte(u8 command_data ) {
	uint8_t tx_data[0];

	tx_data[0] = command_data;

	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data[0], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
}
void SEND_Command_4_byte( u8 command_data, u8 data1, u8 data2, u8 data3 ) {
	uint8_t tx_data[4];

	tx_data[0] = command_data;
	tx_data[1] = data1;
	tx_data[2] = data2;
	tx_data[3] = data3;

	for(u8 i = 0; i < 4; i++){
		CS_DRV1_PIN_LOW;
		while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
		HAL_SPI_Transmit(&hspi2, &tx_data[i], 1, 50);
		while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
		CS_DRV1_PIN_HIGH;
	}
}
void GOTO_ABS_POS_Command(u8 command_data ) {
	uint8_t tx_data[4];
	uint32_t temp;

	temp = (564);
	(command_data == GOTO_ABS_POS_Forward) ?  (temp = temp) :  (temp = - temp);

	my_data.int_val = temp;
	tx_data[0] = command_data;
	tx_data[1] = my_data.char_val[2];
	tx_data[2] = my_data.char_val[1];
	tx_data[3] = my_data.char_val[0];

	for(u8 i = 0; i < 4; i++){
		CS_DRV1_PIN_LOW;
		while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
		HAL_SPI_Transmit(&hspi2, &tx_data[i], 1, 50);
		while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
		CS_DRV1_PIN_HIGH;
	}
}
void initilize_stepper_motors(void){
	static char temporary = 0;
	static int my_timer = 0;

	my_timer++;

	switch (temporary) {
	case 0:
		HAL_GPIO_WritePin(Drv_RST_GPIO_Port, Drv_RST_Pin, GPIO_PIN_RESET);
		if (my_timer > 20) {
			temporary++;
		}

		break;
	case 1:
		HAL_GPIO_WritePin(Drv_RST_GPIO_Port, Drv_RST_Pin, GPIO_PIN_SET);
		if (my_timer > 40) {
			temporary++;
		}
		break;
	case 2:
		SEND_Command_1_byte(HARD_HiZ_Command );
		temporary++;
		break;
	case 3:
		SEND_Command_1_byte(HARD_HiZ_Command );
		temporary++;
		break;
	case 4:
		//POWERSTEP01_SPI_WRITE_16_BIT( ACC_REG, 0x008A );
		temporary++;
		break;
	case 5:
		//POWERSTEP01_SPI_WRITE_16_BIT( DEC_REG, 0x008A );
		temporary++;
		break;
	case 6:
		POWERSTEP01_SPI_WRITE_16_BIT( MAX_SPEED_REG, 0x0055 );
		temporary++;
		break;
	case 7:
		if(current_mode_control == 1){
			POWERSTEP01_SPI_WRITE_8_BIT( STEP_MODE_REG, 0x0F);
		}
		else{
			POWERSTEP01_SPI_WRITE_8_BIT( STEP_MODE_REG, 0x07);
		}
		temporary++;
		break;
	case 8:
		POWERSTEP01_SPI_WRITE_16_BIT( FS_SPD_REG, 0x03FF );
		temporary++;
		break;
	case 9:
		if(current_mode_control == 1){
			POWERSTEP01_SPI_WRITE_8_BIT( KVAL_HOLD_REG, 0x05 );
		}
		else{
			POWERSTEP01_SPI_WRITE_8_BIT( KVAL_HOLD_REG, 0x05 );
		}
		temporary++;
		break;
	case 10:
		if(current_mode_control == 1){
			POWERSTEP01_SPI_WRITE_8_BIT( KVAL_RUN_REG, 0x05 );
		}
		else{
			POWERSTEP01_SPI_WRITE_8_BIT( KVAL_RUN_REG, 0x15 );
		}
		temporary++;
		break;
	case 11:
		if(current_mode_control == 1){
			POWERSTEP01_SPI_WRITE_8_BIT( KVAL_ACC_REG, 0x05 );
		}
		else{
			POWERSTEP01_SPI_WRITE_8_BIT( KVAL_ACC_REG, 0x15 );
		}
		temporary++;
		break;
	case 12:
		if(current_mode_control == 1){
			POWERSTEP01_SPI_WRITE_8_BIT( KVAL_DEC_REG, 0x05 );
		}
		else{
			POWERSTEP01_SPI_WRITE_8_BIT( KVAL_DEC_REG, 0x15 );
		}
		temporary++;
		break;
	case 13:
		//POWERSTEP01_SPI_WRITE_8_BIT( FN_SLP_ACC_REG, 0x3F );
		temporary++;
		break;
	case 14:
		//POWERSTEP01_SPI_WRITE_8_BIT( FN_SLP_DEC_REG, 0x3F );
		temporary++;
		break;
	case 15:
		//POWERSTEP01_SPI_WRITE_8_BIT( ALARM_EN_REG, 0xFF );
		temporary++;
		break;
	case 16:
		if(current_mode_control == 1){
			POWERSTEP01_SPI_WRITE_16_BIT( CONFIG_REG, 0x8310 );
		}
		else{
			POWERSTEP01_SPI_WRITE_16_BIT( CONFIG_REG, 0x0310 );
		}
		temporary++;
		break;
	case 17:
		if(current_mode_control == 1){
			POWERSTEP01_SPI_WRITE_16_BIT( GATECFG1_REG, 0x0080 );
		}
		else{
			POWERSTEP01_SPI_WRITE_16_BIT( GATECFG1_REG, 0x00E0 );
		}
		temporary++;
		break;
	case 18:
		if(current_mode_control == 1){
			POWERSTEP01_SPI_WRITE_8_BIT( GATECFG2_REG, 0xFF );
		}
		else{
			POWERSTEP01_SPI_WRITE_8_BIT( GATECFG2_REG, 0xE0 );
		}
		temporary++;
		break;
	case 19:
		temporary++;
		break;
	case 20:
		POWERSTEP01_SPI_WRITE_24_BIT( ABS_POS_REG, 0x00 );
		temporary++;
		break;
	case 21:
		are_motors_initilized = 1;
		break;
	}
}
void WRITE_REGISTERS_CURRENT_MODE( void ) {
	int move_speed_yaw = 0;

	move_speed_yaw = 300;
	SEND_Command_4_byte(RUN_Command_Forward, 0, ((move_speed_yaw / 256)%256), (move_speed_yaw % 256));
}
void POWERSTEP01_SPI_WRITE_8_BIT(uint8_t addr, uint8_t data) {
	uint8_t tx_data[2];

	tx_data[0] = addr;
	tx_data[1] = data;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data[0], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data[1], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;

}
void POWERSTEP01_SPI_WRITE_16_BIT(uint8_t addr, uint16_t data) {
	uint8_t tx_data[3];

	tx_data[0] = addr;
	tx_data[1] = (data & 0xFF00) >> 8;
	tx_data[2] = (data & 0x00FF);
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data[0], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data[1], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data[2], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;

}
void POWERSTEP01_SPI_WRITE_24_BIT(uint8_t addr, uint32_t data) {
	uint8_t tx_data[4];

	tx_data[0] = addr;
	tx_data[1] = (data & 0xFF0000) >> 16;
	tx_data[2] = (data & 0x00FF00) >> 8;
	tx_data[3] = (data & 0x0000FF);
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data[0], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data[1], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data[2], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data[3], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;

}
uint8_t POWERSTEP01_SPI_READ_8_BIT(uint8_t addr) {
	uint8_t rx_data = 0;
	uint8_t tx_data = addr | READ_GETPARAM;

	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data, 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Receive(&hspi2, &rx_data, 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	return rx_data;
}
uint16_t POWERSTEP01_SPI_READ_16_BIT(uint8_t addr) {
	uint8_t tx_data = addr | READ_GETPARAM;
	uint8_t rx_data[2];

	rx_data[0] = 0;
	rx_data[1] = 0;

	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data, 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Receive(&hspi2, &rx_data[0], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Receive(&hspi2, &rx_data[1], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	return (((uint16_t) rx_data[0]) << 8) | ((uint16_t) rx_data[1]);
}
uint32_t POWERSTEP01_SPI_READ_24_BIT(uint8_t addr) {
	uint8_t tx_data = addr | READ_GETPARAM;
	uint8_t rx_data[3];

	rx_data[0] = 0;
	rx_data[1] = 0;
	rx_data[2] = 0;

	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Transmit(&hspi2, &tx_data, 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Receive(&hspi2, &rx_data[0], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Receive(&hspi2, &rx_data[1], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	CS_DRV1_PIN_LOW;
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	HAL_SPI_Receive(&hspi2, &rx_data[2], 1, 50);
	while(HAL_SPI_GetState(&hspi2) == HAL_SPI_STATE_BUSY_TX_RX);
	CS_DRV1_PIN_HIGH;
	return (((uint32_t) rx_data[0]) << 16) | ((uint32_t) rx_data[1] << 8)
			| ((uint32_t) rx_data[2]);
}
