#ifndef MAPPING_NUCLEO_H
#define MAPPING_NUCLEO_H

#include "mbed.h"

/*------------ Serial pin definition --------------*/
#define TX_PIN          SERIAL_TX
#define RX_PIN          SERIAL_RX

#define GRAPHICS_TX_PIN PC_4
#define GRAPHICS_RX_PIN PB_11

/*------------- I2C pin definition ----------------*/
#define SCL_PIN         PB_8
#define SDA_PIN         PB_9
#define DISPLAY_ADDRESS 0x4E

/*---------- Information LED definition -----------*/
#define LED_READY       PA_13
#define LED_BUSSY       PA_14

/*------------ Outputs pin definition -------------*/
#define OUTPUT_PIN_1    PA_8
#define OUTPUT_PIN_2    PB_10
#define OUTPUT_PIN_3    PB_4
#define OUTPUT_PIN_4    PB_5
#define OUTPUT_PIN_5    PB_3
#define OUTPUT_PIN_6    PA_10
#define OUTPUT_PIN_7    PC_8
#define OUTPUT_PIN_8    PC_9

#define INPUT_PIN_1     PC_10
#define INPUT_PIN_2     PC_11
#define INPUT_PIN_3     PC_6
#define INPUT_PIN_4     PC_5
#define INPUT_PIN_5     PC_14
#define INPUT_PIN_6     PC_15
#define INPUT_PIN_7     PA_0
#define INPUT_PIN_8     PA_1

#endif