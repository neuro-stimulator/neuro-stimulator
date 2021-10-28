#ifndef MAPPING_EXPERIMENT_H
#define MAPPING_EXPERIMENT_H

#include "platform-types.h"

/* -------------- ERP ----------- */

#pragma pack(push, 1)
typedef struct experiment_erp_head_s {
    experiment_type_t type;                                            // 1 byte
    experiment_output_count_t outputCount;                             // 1 byte
    experiment_output_out_t out;                                       // 8 byte
    experiment_output_wait_t wait;                                     // 8 byte
    experiment_erp_random_t random;                                    // 1 byte
    experiment_erp_edge_t edge;                                        // 1 byte
    experiment_erp_sequence_size_t sequenceSize;                       // 2 byte
} experiment_erp_head_t;                                               // = 22 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_erp_outputs_s {
    experiment_output_type_t outputType;                               // 1 byte
    experiment_erp_output_pulse_up_t pulseUp;                          // 8 byte
    experiment_erp_output_pulse_down_t pulseDown;                      // 8 byte
    experiment_output_brightness_t brightness;                         // 1 byte
} experiment_erp_outputs_t;                                            // = 18 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_erp_sequence_data_s {
    uint16_t pointer;                                          // 2 byte
    uint16_t accIndex;                                         // 2 byte counters[0]
    uint16_t accOffset;                                        // 2 byte counters[1]
    uint16_t requestIndex;                                     // 2 byte counters[2]
    uint16_t requestOffset;                                    // 2 byte counters[3]
} experiment_erp_sequence_data_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_erp_runtime_data_s {
    ushort randomBase;                                                 // 2 byte
    uint8_t currentOutput;                                             // 1 byte
    experiment_erp_sequence_data_t sequence_data;                      //  byte
    // Multifunkční pole accumulatorů
    // Využito hlavně v ERP pro sekvence
    uint32_t accumulators[TOTAL_OUTPUT_COUNT];                         // 4 * TOTAL_OUTPUT_COUNT byte

} experiment_erp_runtime_data_t;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_erp_s {
    experiment_erp_head_t head;                                        // 22 byte
    experiment_erp_outputs_t outputs[TOTAL_OUTPUT_COUNT];              // 18  * TOTAL_OUTPUT_COUNT byte
    experiment_erp_runtime_data_t data;
} experiment_erp_t;                                                    // = 22 + 18 * TOTAL_OUTPUT_COUNT byte
#pragma pack(pop)





/* -------------- CVEP ----------- */

#pragma pack(push, 1)
typedef struct experiment_cvep_head_s {
    experiment_type_t type;                                            // 1 byte
    experiment_output_count_t outputCount;                             // 1 byte
    experiment_output_type_t outputType;                               // 1 byte
    experiment_output_out_t out;                                       // 8 byte
    experiment_output_wait_t wait;                                     // 8 byte
    experiment_cvep_bit_shift_t bitShift;                              // 1 byte
    experiment_output_brightness_t brightness;                         // 1 byte
    experiment_cvep_pattern_t pattern;                                 // 4 byte
} experiment_cvep_head_t;                                              // = 24 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_cvep_outputs_s {
    // NO DATA
} experiment_cvep_outputs_t;                                           // 0 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_cvep_runtime_data_s {
    uint8_t counter;                                                   // 1 byte
} experiment_cvep_runtime_data_t;                                      // 1 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_cvep_s {
    experiment_cvep_head_t head;                                       // 24 byte
    experiment_cvep_outputs_t outputs;                                 // 0 byte
    experiment_cvep_runtime_data_t data;                               // 1 byte
} experiment_cvep_t;                                                   // = 24 byte
#pragma pack(pop)





/* -------------- FVEP ----------- */

#pragma pack(push, 1)
typedef struct experiment_fvep_head_s {
    experiment_type_t type;                                            // 1 byte
    experiment_output_count_t outputCount;                             // 1 byte
} experiment_fvep_head_t;                                              // = 2 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_fvep_outputs_s {
    experiment_output_type_t outputType;                               // 1 byte
    experiment_fvep_output_time_on_t timeOn;                           // 8 byte
    experiment_fvep_output_time_off_t timeOff;                         // 8 byte
    experiment_output_brightness_t brightness;                         // 1 byte
} experiment_fvep_outputs_t;                                           // = 18 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_fvep_runtime_data_s {
    // NO DATA
} experiment_fvep_runtime_data_t;                                      // 0 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_fvep_s {
    experiment_fvep_head_t head;                                       // 2 byte
    experiment_fvep_outputs_t outputs[TOTAL_OUTPUT_COUNT];             // 18 * TOTAL_OUTPUT_COUNT byte
    experiment_fvep_runtime_data_t data;                               // 0 byte
} experiment_fvep_t;                                                   // = 2 + 18 * TOTAL_OUTPUT_COUNT byte
#pragma pack(pop)





/* -------------- TVEP ----------- */

#pragma pack(push, 1)
typedef struct experiment_tvep_head_s {
    experiment_type_t type;                                            // 1 byte
    experiment_output_count_t outputCount;                             // 1 byte
} experiment_tvep_head_t;                                              // = 2 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_tvep_output_s {
    experiment_tvep_output_pattern_length_t patternLength;             // 1 byte
    experiment_output_type_t outputType;                               // 1 byte
    experiment_output_out_t out;                                       // 8 byte
    experiment_output_wait_t wait;                                     // 8 byte
    experiment_output_brightness_t brightness;                         // 1 byte
    experiment_tvep_output_pattern_t pattern;                          // 4 byte
} experiment_tvep_output_t;                                            // = 15 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_tvep_runtime_data_s {
    uint16_t counters[TOTAL_OUTPUT_COUNT];                             // 2 * TOTAL_OUTPUT_COUNT byte
} experiment_tvep_runtime_data_t;                                      // = 2 * TOTAL_OUTPUT_COUNT byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_tvep_s {
    experiment_tvep_head_t head;                                       // 2 byte
    experiment_tvep_output_t outputs[TOTAL_OUTPUT_COUNT];              // 15 * TOTAL_OUTPUT_COUNT byte
    experiment_tvep_runtime_data_t data;                               // 2 * TOTAL_OUTPUT_COUNT byte
} experiment_tvep_t;                                                   // = 2 + 15 * TOTAL_OUTPUT_COUNT + 2 * TOTAL_OUTPUT_COUNT byte
#pragma pack(pop)

/* --------------- REA ------------ */
#pragma pack(push, 1)
typedef struct experiment_rea_head_s {
    experiment_type_t type;                                            // 1 byte
    experiment_output_count_t outputCount;                             // 1 byte
    experiment_output_type_t outputType;                               // 1 byte
    experiment_rea_cycle_count_t cycleCount;                           // 1 byte
    experiment_rea_wait_time_t waitTimeMin;                            // 8 byte
    experiment_rea_wait_time_t waitTimeMax;                            // 8 byte
    experiment_rea_miss_time_t missTime;                               // 8 byte
    experiment_rea_on_fail_t onFail;                                   // 1 byte
    experiment_output_brightness_t brightness;                         // 1 byte
} experiment_rea_head_t;                                               // = 30 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_rea_output_s {
    // NO DATA
} experiment_rea_output_t;                                             // 0 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_rea_runtime_data_s {
    uint16_t counter;                                                 // 2 byte
    uint8_t usedOutput;                                               // 1 byte
} experiment_rea_runtime_data_t;                                      // = 3 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_rea_s {
    experiment_rea_head_t head;                                        // 30 byte
    experiment_rea_output_t outputs;                                   // 0 byte
    experiment_rea_runtime_data_t data;                                // 3 byte
} experiment_rea_t;                                                    // = 30 byte
#pragma pack(pop)

/* -------------- UNION ----------- */

union ExperimentConfig {
    experiment_type_t type;                                            // 1 byte
    experiment_erp_t  experimentERP;                                   // 22 + 4 * TOTAL_OUTPUT_COUNT byte
    experiment_cvep_t experimentCVEP;                                  // 24 byte
    experiment_fvep_t experimentFVEP;                                  // 2 + 18 * TOTAL_OUTPUT_COUNT byte
    experiment_tvep_t experimentTVEP;                                  // 2 + 15 * TOTAL_OUTPUT_COUNT byte
    experiment_rea_t  experimentREA;                                   // 10 byte
};

union OutputConfig {
    experiment_erp_outputs_t    outputERP;                             // 4 byte
    experiment_cvep_outputs_t   outputCVEP;                            // 0 byte
    experiment_fvep_outputs_t   outputFVEP;                            // 15 byte
    experiment_tvep_output_t    outputTVEP;                            // 18 byte
    experiment_rea_output_t     outputREA;                             // 0 byte
};

#endif