#ifndef MAPPING_DISCO_H
#define MAPPING_DISOC_H

#include "mbed.h"

/*------------ Serial pin definition --------------*/
#define TX_PIN          SERIAL_TX
#define RX_PIN          SERIAL_RX

#define GRAPHICS_TX_PIN PB_10
#define GRAPHICS_RX_PIN PB_11

/*------------- I2C pin definition ----------------*/
#define SCL_PIN         PB_8
#define SDA_PIN         PB_9
#define DISPLAY_ADDRESS 0x4E

/*---------- Information LED definition -----------*/
#define LED_READY       PG_11
#define LED_BUSSY       PG_9

/*------------ Outputs pin definition -------------*/
#define OUTPUT_PIN_1    PA_8
#define OUTPUT_PIN_2    PC_9
#define OUTPUT_PIN_3    PC_8
#define OUTPUT_PIN_4    PC_7
#define OUTPUT_PIN_5    PB_4
#define OUTPUT_PIN_6    PB_5
#define OUTPUT_PIN_7    PB_8
#define OUTPUT_PIN_8    PB_9

/*------------- Input pin definition -------------*/

#define INPUT_PIN_1     PG_4
#define INPUT_PIN_2     PG_5
#define INPUT_PIN_3     PG_6
#define INPUT_PIN_4     PG_7
#define INPUT_PIN_5     PG_2
#define INPUT_PIN_6     PG_3
#define INPUT_PIN_7     PD_14
#define INPUT_PIN_8     PD_15

#endif