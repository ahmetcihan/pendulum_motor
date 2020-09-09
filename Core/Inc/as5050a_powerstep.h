#ifndef __AS5050A_POWERSTEP_H
#define __AS5050A_POWERSTEP_H

typedef uint8_t u8;
typedef uint8_t u16;

union _my_data {
	uint8_t char_val[4];
	int32_t int_val;	/* signed data for goto_abs_pos direction */
};

#define Drv1_CS_Pin 		 GPIO_PIN_10
#define Drv1_CS_GPIO_Port 	 GPIOA
#define Drv_RST_Pin 		 GPIO_PIN_0
#define Drv_RST_GPIO_Port 	 GPIOB
#define Tx2_en				 GPIO_PIN_4

#define CS_DRV1_PIN_LOW 		HAL_GPIO_WritePin(Drv1_CS_GPIO_Port, Drv1_CS_Pin, GPIO_PIN_RESET)
#define CS_DRV1_PIN_HIGH		HAL_GPIO_WritePin(Drv1_CS_GPIO_Port, Drv1_CS_Pin, GPIO_PIN_SET)

#define READ_GETPARAM			0x20
#define ABS_POS_REG				0x01
#define EL_POS_REG				0x02
#define MARK_REG				0x03
#define SPEED_REG				0x04
#define ACC_REG					0x05
#define DEC_REG					0x06
#define MAX_SPEED_REG			0x07
#define MIN_SPEED_REG			0x08
#define ADC_OUT_REG				0x12
#define OCD_TH_REG				0x13
#define FS_SPD_REG				0x15
#define STEP_MODE_REG			0x16
#define ALARM_EN_REG			0x17
#define GATECFG1_REG			0x18
#define GATECFG2_REG			0x19
#define STATUS_REG				0x1B
#define CONFIG_REG				0x1A
#define KVAL_HOLD_REG			0x09
#define KVAL_RUN_REG			0x0A
#define KVAL_ACC_REG			0x0B
#define KVAL_DEC_REG			0x0C
#define INT_SPEED_REG			0x0D
#define ST_SLP_REG				0x0E
#define FN_SLP_ACC_REG			0x0F
#define FN_SLP_DEC_REG			0x10
#define K_THERM_REG				0x11
#define STALL_TH_REG			0x14
#define TVAL_HOLD_REG			0x09
#define	TVAL_RUN_REG			0x0A
#define	TVAL_ACC_REG			0x0B
#define	TVAL_DEC_REG			0x0C
#define	T_FAST_REG				0x0E
#define	TON_MIN_REG				0x0F
#define	TOFF_MIN_REG			0x10

#define RUN_Command_Forward 	0x51  /* The Run command produces a motion at SPD speed */
#define RUN_Command_Reverse 	0x50  /* The direction is selected by the DIR bit (last bit), '1' forward '0' reverse */
#define GOTO_ABS_POS_Forward	0x69  /* The GoTo_DIR command produces a motion to ABS_POS absolute position - forward */
#define GOTO_ABS_POS_Reverse	0x68  /* The GoTo_DIR command produces a motion to ABS_POS absolute position - reverse */
#define MOVE_Command 			0x40  /* The move command produces a motion of N_STEP microsteps */
#define SOFT_STOP_Command 		0xB0  /* SoftStop command causes an immediate deceleration to zero speed and a consequent motor stop */
#define HARD_STOP_Command 		0xB8  /* The HardStop command causes an immediate motor stop with infinite deceleration */
#define HARD_HiZ_Command 		0xA8  /* HardHiZ command forces the bridges to enter high impedance state */
#define RESET_POS_Command 		0xD8  /* The ResetPos command resets the ABS_POS register of first motor to zero */
#define GO_HOME_Command 		0x70  /* The GoHome command produces a motion to the HOME position (zero position) */
#define GO_MARK_Command 		0x78  /* The GoMark command produces a motion to the MARK position of first motor performing the minimum path */

void 	 POWERSTEP01_ALL_REGISTER_READ 	( void );
void 	 WRITE_REGISTERS_CURRENT_MODE  	( void );
void 	 READ_REGISTERS				  	( void );
void 	 SEND_Command_4_byte			( u8 command_data, u8 data1, u8 data2, u8 data3 );
void 	 SEND_Command_1_byte			( u8 command_data );
void 	 GOTO_ABS_POS_Command           ( u8 command_data );
void 	 POWERSTEP01_SPI_WRITE_8_BIT 	( uint8_t addr , uint8_t  data );
void 	 POWERSTEP01_SPI_WRITE_16_BIT 	( uint8_t addr , uint16_t data );
void 	 POWERSTEP01_SPI_WRITE_24_BIT 	( uint8_t addr , uint32_t data );
uint8_t  POWERSTEP01_SPI_READ_8_BIT 	( uint8_t addr );
uint16_t POWERSTEP01_SPI_READ_16_BIT 	( uint8_t addr );
uint32_t POWERSTEP01_SPI_READ_24_BIT 	( uint8_t addr );

void initilize_stepper_motors(void);

#endif /*__AS5050A_POWERSTEP_H */
