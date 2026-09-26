#ifndef Y2038_SAFE_TIME_H
#define Y2038_SAFE_TIME_H

#include <stdint.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Explicit 64-bit timestamp representation (seconds since Unix Epoch).
 * Guaranteed to avoid Y2038 32-bit integer overflow across all architectures.
 */
typedef int64_t y2038_time_t;

/**
 * Struct to hold broken-down calendar time supporting 64-bit years.
 */
typedef struct {
    int64_t year;   // Full year (e.g. 2038, 2040)
    int32_t month;  // 1 - 12
    int32_t day;    // 1 - 31
    int32_t hour;   // 0 - 23
    int32_t minute; // 0 - 59
    int32_t second; // 0 - 59
} y2038_date_t;

/**
 * Returns current UTC time as a 64-bit Unix timestamp.
 */
y2038_time_t y2038_now(void);

/**
 * Converts a 64-bit Unix timestamp to a broken-down UTC date struct.
 */
bool y2038_gmtime(y2038_time_t timestamp, y2038_date_t *out_date);

/**
 * Converts a broken-down UTC date struct into a 64-bit Unix timestamp.
 */
y2038_time_t y2038_mktime(const y2038_date_t *date);

/**
 * Safely adds seconds to a timestamp without risk of 32-bit overflow.
 */
y2038_time_t y2038_add_seconds(y2038_time_t base_time, int64_t seconds_to_add);

/**
 * Formats a 64-bit timestamp as an ISO-8601 UTC string (YYYY-MM-DDTHH:MM:SSZ).
 */
bool y2038_format_iso8601(y2038_time_t timestamp, char *buffer, size_t buffer_len);

#ifdef __cplusplus
}
#endif

#endif // Y2038_SAFE_TIME_H
