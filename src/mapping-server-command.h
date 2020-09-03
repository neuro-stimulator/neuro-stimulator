#ifndef MAPPING_SERVER_COMMANDS_H
#define MAPPING_SERVER_COMMANDS_H

#include "mapping-experiment.h"

#define OUTPUT_BUFFER_LENGTH     128

#define HEADER_NO_ID                             0
#define COMMAND_STIMULATOR_STATE                 0x01

#define COMMAND_OUTPUT_ACTIVATED                 0x10
#define COMMAND_OUTPUT_DEACTIVATED               0x11
#define COMMAND_INPUT_ACTIVATED                  0x12

#define COMMAND_SEQUENCE_NEXT_PART_REQUEST       0x20

#define COMMAND_DEBUG                            0xF1

#pragma pack(push, 1)
typedef struct server_command_data_header_s {
    uint8_t commandId;                      // 1 byte
    uint8_t type;                           // 1 byte
    uint8_t length;                         // 1 byte
} server_command_data_header_t;             // 3 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct server_command_stimulator_state_s {
    server_command_data_header_t header;    // 2 byte
    uint8_t state;                          // 1 byte
    uint8_t noUpdate;                       // 1 byte
    uint32_t timestamp;                     // 4 byte
} server_command_stimulator_state_t;        // 9 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct server_command_io_change_s {
    server_command_data_header_t header;    // 2 byte
    uint8_t index;                          // 1 byte
    uint32_t timestamp;                     // 4 byte
} server_command_io_change_t;               // 8 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct server_command_sequence_part_request_s {
    server_command_data_header_t header;    // 2 byte
    uint16_t offset;                        // 2 byte
    uint8_t index;                          // 1 byte
    uint32_t timestamp;                     // 4 byte
} server_command_sequence_part_request_t;   // 10 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct server_command_debug_s {
    server_command_data_header_t header;    // 3 byte
    //ExperimentConfig experimentConfig;
} server_command_debug_t;
#pragma pack(pop)

union ServerCommandData {
    server_command_data_header_t header;                        // 3 byte
    server_command_io_change_t commandIOChange;                 // 7 byte
    server_command_stimulator_state_t commandStimulatorState;   // 7 byte
    server_command_debug_t commandDebug;
    server_command_sequence_part_request_t commandSequencePartRequest; // 8 byte

    char rawData[OUTPUT_BUFFER_LENGTH];
};

#endif