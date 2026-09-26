#include "y2038_safe_time.h"
#include <stdio.h>
#include <string.h>

#define SECONDS_PER_DAY 86400LL
#define SECONDS_PER_HOUR 3600LL
#define SECONDS_PER_MINUTE 60LL

static bool is_leap_year(int64_t year) {
    return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
}

static int days_in_month(int64_t year, int month) {
    static const int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (month == 2 && is_leap_year(year)) {
        return 29;
    }
    return days[month - 1];
}

y2038_time_t y2038_now(void) {
    time_t raw_now = time(NULL);
    return (y2038_time_t)raw_now;
}

bool y2038_gmtime(y2038_time_t timestamp, y2038_date_t *out_date) {
    if (!out_date) return false;

    int64_t seconds = timestamp;
    int64_t days = seconds / SECONDS_PER_DAY;
    int64_t rem_seconds = seconds % SECONDS_PER_DAY;

    if (rem_seconds < 0) {
        rem_seconds += SECONDS_PER_DAY;
        days -= 1;
    }

    out_date->hour = (int32_t)(rem_seconds / SECONDS_PER_HOUR);
    rem_seconds %= SECONDS_PER_HOUR;
    out_date->minute = (int32_t)(rem_seconds / SECONDS_PER_MINUTE);
    out_date->second = (int32_t)(rem_seconds % SECONDS_PER_MINUTE);

    // Compute year and day of year starting from 1970-01-01
    int64_t year = 1970;
    while (days < 0) {
        int64_t prev_year = year - 1;
        int days_in_prev = is_leap_year(prev_year) ? 366 : 365;
        days += days_in_prev;
        year--;
    }

    while (true) {
        int days_in_curr = is_leap_year(year) ? 366 : 365;
        if (days < days_in_curr) {
            break;
        }
        days -= days_in_curr;
        year++;
    }

    out_date->year = year;

    int month = 1;
    while (month <= 12) {
        int dim = days_in_month(year, month);
        if (days < dim) {
            break;
        }
        days -= dim;
        month++;
    }

    out_date->month = month;
    out_date->day = (int32_t)(days + 1);

    return true;
}

y2038_time_t y2038_mktime(const y2038_date_t *date) {
    if (!date) return 0;

    int64_t days = 0;
    int64_t year = 1970;

    if (date->year >= 1970) {
        for (int64_t y = 1970; y < date->year; y++) {
            days += is_leap_year(y) ? 366 : 365;
        }
    } else {
        for (int64_t y = date->year; y < 1970; y++) {
            days -= is_leap_year(y) ? 366 : 365;
        }
    }

    for (int m = 1; m < date->month; m++) {
        days += days_in_month(date->year, m);
    }

    days += (date->day - 1);

    y2038_time_t timestamp = (days * SECONDS_PER_DAY) +
                             (date->hour * SECONDS_PER_HOUR) +
                             (date->minute * SECONDS_PER_MINUTE) +
                             date->second;

    return timestamp;
}

y2038_time_t y2038_add_seconds(y2038_time_t base_time, int64_t seconds_to_add) {
    return base_time + seconds_to_add;
}

bool y2038_format_iso8601(y2038_time_t timestamp, char *buffer, size_t buffer_len) {
    y2038_date_t d;
    if (!y2038_gmtime(timestamp, &d)) return false;

    int written = snprintf(buffer, buffer_len, "%04lld-%02d-%02dT%02d:%02d:%02dZ",
                           (long long)d.year, d.month, d.day, d.hour, d.minute, d.second);
    return written > 0 && (size_t)written < buffer_len;
}
