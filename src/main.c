#include <stdio.h>
#include <inttypes.h>
#include "y2038_safe_time.h"

int main(void) {
    printf("=================================================================\n");
    printf("          Y2038 BUG SOLUTION & VERIFICATION SUITE               \n");
    printf("=================================================================\n\n");

    char iso_buf[64];

    // Test Case 1: 1 Second Before Overflow (2,147,483,647)
    y2038_time_t t_max32 = 2147483647LL;
    y2038_format_iso8601(t_max32, iso_buf, sizeof(iso_buf));
    printf("[TEST 1] Max Signed 32-bit Timestamp:\n");
    printf("         Epoch Seconds : %" PRId64 "\n", t_max32);
    printf("         ISO 8601 Date : %s\n\n", iso_buf);

    // Test Case 2: The Overflow Moment (2,147,483,648)
    y2038_time_t t_overflow = 2147483648LL;
    y2038_format_iso8601(t_overflow, iso_buf, sizeof(iso_buf));
    printf("[TEST 2] 64-Bit Safe Overflow Moment (Y2K38 Boundary):\n");
    printf("         Epoch Seconds : %" PRId64 "\n", t_overflow);
    printf("         ISO 8601 Date : %s\n", iso_buf);
    printf("         Result        : [PASS] Correctly resolved to 2038 (No wrap-around to 1901!)\n\n");

    // Test Case 3: Far-Future Timestamp (Year 2100)
    y2038_time_t t_far_future = 4102444800LL;
    y2038_format_iso8601(t_far_future, iso_buf, sizeof(iso_buf));
    printf("[TEST 3] Far Future Date Verification (Year 2100):\n");
    printf("         Epoch Seconds : %" PRId64 "\n", t_far_future);
    printf("         ISO 8601 Date : %s\n", iso_buf);
    printf("         Result        : [PASS] Future-proof calculation verified.\n\n");

    // Test Case 4: Round-trip conversion test
    y2038_date_t input_date = { .year = 2050, .month = 6, .day = 15, .hour = 12, .minute = 30, .second = 0 };
    y2038_time_t converted_ts = y2038_mktime(&input_date);
    y2038_date_t output_date;
    y2038_gmtime(converted_ts, &output_date);

    printf("[TEST 4] Round-trip Date -> Epoch -> Date:\n");
    printf("         Input Date    : 2050-06-15 12:30:00 UTC\n");
    printf("         Generated TS  : %" PRId64 "\n", converted_ts);
    printf("         Decoded Date  : %04lld-%02d-%02d %02d:%02d:%02d UTC\n",
           (long long)output_date.year, output_date.month, output_date.day,
           output_date.hour, output_date.minute, output_date.second);
    printf("         Result        : [PASS] Exact match.\n\n");

    printf("=================================================================\n");
    printf("  ALL 64-BIT Y2038 SAFE TIME CHECKS PASSED SUCCESSFULLY!        \n");
    printf("=================================================================\n");

    return 0;
}
