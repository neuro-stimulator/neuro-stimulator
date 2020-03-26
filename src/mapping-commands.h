#ifndef MAPPING_COMMANDS_H
#define MAPPING_COMMANDS_H

#include "mapping-experiment.h"

#define COMMAND_REBOOT              0x00
#define COMMAND_STIMULATOR_STATE    0x01
#define COMMAND_DISPLAY             0x02
#define COMMAND_MANAGE_EXPERIMENT   0x03
#define COMMAND_SETUP_EXPERIMENT    0x10
#define COMMAND_SETUP_OUTPUT        0x11
#define COMMAND_NEXT_SEQUENCE_PART  0x20

// Backdor pro manuální nastavování zapnutí/vypnutí jednotlivých výstupů
#define COMMAND_BACKDOR_1           0xF0
#define COMMAND_DEBUG               0xF1

#pragma pack(push, 1)
typedef struct command_reboot_s {
    uint8_t type;
} command_reboot_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct command_stimulator_state_s {
    uint8_t type;
} command_stimulator_state_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct command_lcd_s {
    uint8_t type;
    uint8_t action;
    uint8_t column;
    uint8_t row;
    char text[124];
} command_lcd_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct command_manage_experiment_s {
    uint8_t type;
    uint8_t action;
} command_manage_experiment_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct command_setup_experiment_s {
    uint8_t type;
    uint8_t action;
    ExperimentConfig experimentConfig;
} command_setup_experiment_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct command_setup_output_s {
    uint8_t type;
    uint8_t index;
    OutputConfig outputConfig;
} command_setup_output_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct command_stimul_config_s {
    uint8_t type;
    uint8_t index;
    uint8_t up;
    uint8_t down;
    uint8_t brightness;
} command_stimul_config_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct command_sequence_part_s {
    uint8_t type;
    uint8_t index;
    uint32_t data;
} command_sequence_part_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct command_backdor_1_s {
    uint8_t type;
    uint8_t index;
    uint8_t brightness;
} command_backdor_1_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct command_debug_s {
    uint8_t type;
    uint8_t memoryType;
} comand_debug_t;
#pragma pack(pop)

union CommandData {
    // Společná hlavička pro všechny příkazy
    // Pomocí této hlavičky lze rozlišit, jaký příkaz se má vykonat
    uint8_t type;
    command_reboot_t commandReboot;
    command_stimulator_state_t commandStimulatorState;
    command_lcd_t commandLCD;
    command_manage_experiment_t commandManageExperiment;
    command_setup_experiment_t commandSetupExperiment;
    command_setup_output_t commandSetupOutput;
    command_stimul_config_t commandStimulConfig;
    command_sequence_part_t commandSequencePart;

    // Zadní vrátka do systému
    command_backdor_1_t commandBackdor1;
    comand_debug_t commandDebug;


    char rawData[128];
};

#endif