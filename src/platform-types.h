#ifndef PLATFORM_TYPES_H
#define PLATFORM_TYPES_H

#ifdef USE_MBED
#include "mbed.h"
// #include "TextLCD.h"
#else
#include <stdint.h>
#endif // USE_MBED

typedef uint8_t experiment_type_t;
typedef uint8_t experiment_output_count_t;
typedef uint8_t experiment_output_type_t;
typedef uint8_t experiment_output_brightness_t;
typedef us_timestamp_t experiment_output_out_t;
typedef us_timestamp_t experiment_output_wait_t;

// ERP specific data types
typedef uint8_t experiment_erp_random_t;
typedef uint8_t experiment_erp_edge_t;
typedef uint16_t experiment_erp_sequence_size_t;
typedef us_timestamp_t experiment_erp_output_pulse_up_t;
typedef us_timestamp_t experiment_erp_output_pulse_down_t;

// CVEP specific data types
typedef uint8_t experiment_cvep_bit_shift_t;
typedef uint32_t experiment_cvep_pattern_t;

// FVEP specific data types
typedef us_timestamp_t experiment_fvep_output_time_on_t;
typedef us_timestamp_t experiment_fvep_output_time_off_t;

// TVEP specific data types
typedef uint8_t experiment_tvep_output_pattern_length_t;
typedef uint32_t experiment_tvep_output_pattern_t;

// REA specific data types
typedef uint8_t experiment_rea_cycle_count_t;
typedef uint8_t experiment_rea_wait_time_min_t;
typedef uint8_t experiment_rea_wait_time_max_t;
typedef uint8_t experiment_rea_miss_time_t;
typedef uint8_t experiment_rea_on_fail_t;

#endif