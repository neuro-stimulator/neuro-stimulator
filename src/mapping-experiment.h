#ifndef MAPPING_EXPERIMENT_H
#define MAPPING_EXPERIMENT_H

#include "platform-types.h"

/* -------------- ERP ----------- */

#pragma pack(push, 1)
typedef struct experiment_erp_head_s {
    experiment_type_t type;                                            // 1 byte
    experiment_output_count_t outputCount;                             // 1 byte
    experiment_output_out_t out;                                       // 1 byte
    experiment_output_wait_t wait;                                     // 1 byte
    experiment_erp_random_t random;                                    // 1 byte
    experiment_erp_edge_t edge;                                        // 1 byte
    experiment_erp_sequence_size_t sequenceSize;                       // 2 byte
} experiment_erp_head_t;                                               // = 8 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_erp_outputs_s {
    experiment_output_type_t outputType;                               // 1 byte
    experiment_erp_output_pulse_up_t pulseUp;                          // 1 byte
    experiment_erp_output_pulse_down_t pulseDown;                      // 1 byte
    experiment_output_brightness_t brightness;                         // 1 byte
} experiment_erp_outputs_t;                                            // = 4 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_erp_s {
    experiment_erp_head_t head;                                        // 8 byte
    experiment_erp_outputs_t outputs[TOTAL_OUTPUT_COUNT];              // 4  * TOTAL_OUTPUT_COUNT byte
} experiment_erp_t;                                                    // = 8 + 4 * TOTAL_OUTPUT_COUNT byte
#pragma pack(pop)





/* -------------- CVEP ----------- */

#pragma pack(push, 1)
typedef struct experiment_cvep_head_s {
    experiment_type_t type;                                            // 1 byte
    experiment_output_count_t outputCount;                             // 1 byte
    experiment_output_type_t outputType;                               // 1 byte
    experiment_output_out_t out;                                       // 1 byte
    experiment_output_wait_t wait;                                     // 1 byte
    experiment_cvep_bit_shift_t bitShift;                              // 1 byte
    experiment_output_brightness_t brightness;                         // 1 byte
    experiment_cvep_pattern_t pattern;                                 // 4 byte
} experiment_cvep_head_t;                                              // = 10 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_cvep_outputs_s {
} experiment_cvep_outputs_t;                                           // 0 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_cvep_s {
    experiment_cvep_head_t head;                                       // 10 byte
    experiment_cvep_outputs_t outputs;                                 // 0 byte
} experiment_cvep_t;                                                   // = 10 byte
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
    experiment_fvep_output_time_on_t timeOn;                           // 4 byte
    experiment_fvep_output_time_off_t timeOff;                         // 4 byte
    experiment_output_brightness_t brightness;                         // 1 byte
} experiment_fvep_outputs_t;                                           // = 10 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_fvep_s {
    experiment_fvep_head_t head;                                       // 2 byte
    experiment_fvep_outputs_t outputs[TOTAL_OUTPUT_COUNT];             // 10 * TOTAL_OUTPUT_COUNT byte
} experiment_fvep_t;                                                   // = 2 + 10 * TOTAL_OUTPUT_COUNT byte
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
    experiment_output_out_t out;                                       // 1 byte
    experiment_output_wait_t wait;                                     // 1 byte
    experiment_output_brightness_t brightness;                         // 1 byte
    experiment_tvep_output_pattern_t pattern;                          // 4 byte
} experiment_tvep_output_t;                                            // = 9 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_tvep_s {
    experiment_tvep_head_t head;                                       // 2 byte
    experiment_tvep_output_t outputs[TOTAL_OUTPUT_COUNT];              // 9 * TOTAL_OUTPUT_COUNT byte
} experiment_tvep_t;                                                   // = 2 + 9 * TOTAL_OUTPUT_COUNT byte
#pragma pack(pop)

/* --------------- REA ------------ */
#pragma pack(push, 1)
typedef struct experiment_rea_head_s {
    experiment_type_t type;                                            // 1 byte
    experiment_output_count_t outputCount;                             // 1 byte
    experiment_output_type_t outputType;                               // 1 byte
    experiment_rea_cycle_count_t cycleCount;                           // 1 byte
    experiment_rea_wait_time_min_t waitTimeMin;                        // 1 byte
    experiment_rea_wait_time_max_t waitTimeMax;                        // 1 byte
    experiment_rea_miss_time_t missTime;                               // 1 byte
    experiment_rea_on_fail_t onFail;                                   // 1 byte
    experiment_output_brightness_t brightness;                         // 1 byte
} experiment_rea_head_t;                                               // = 9 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_rea_output_s {
} experiment_rea_output_t;                                             // 0 byte
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct experiment_rea_s {
    experiment_rea_head_t head;                                        // 10 byte
    experiment_rea_output_t outputs;                                   // 0 byte
} experiment_rea_t;
#pragma pack(pop)

/* -------------- UNION ----------- */

union ExperimentConfig {
    experiment_type_t type;                                            // 1 byte
    experiment_erp_t  experimentERP;                                   // 8 + 4 * TOTAL_OUTPUT_COUNT byte
    experiment_cvep_t experimentCVEP;                                  // 10 byte
    experiment_fvep_t experimentFVEP;                                  // 2 + 10 * TOTAL_OUTPUT_COUNT byte
    experiment_tvep_t experimentTVEP;                                  // 2 + 9 * TOTAL_OUTPUT_COUNT byte
    experiment_rea_t  experimentREA;                                   // 10 byte
};

union OutputConfig {
    experiment_erp_outputs_t    outputERP;                             // 4 byte
    experiment_cvep_outputs_t   outputCVEP;                            // 0 byte
    experiment_fvep_outputs_t   outputFVEP;                            // 10 byte
    experiment_tvep_output_t    outputTVEP;                            // 9 byte
    experiment_rea_output_t     outputREA;                             // 0 byte
};

#endif