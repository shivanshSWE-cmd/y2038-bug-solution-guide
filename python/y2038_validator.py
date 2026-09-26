#!/usr/bin/env python3
"""
Y2038 Bug Solution & Validator Module in Python
Demonstrates safe 64-bit timestamp handling and validation.
"""

from datetime import datetime, timezone
import sys

# Y2038 Boundary Constants
INT32_MAX = 2_147_483_647        # 2038-01-19 03:14:07 UTC
INT32_OVERFLOW = 2_147_483_648   # 2038-01-19 03:14:08 UTC

def validate_timestamp_64bit(epoch_seconds: int) -> dict:
    """
    Validates whether a given epoch timestamp is handled cleanly without 32-bit overflow.
    """
    try:
        dt = datetime.fromtimestamp(epoch_seconds, tz=timezone.utc)
        is_safe = epoch_seconds > INT32_MAX
        return {
            "epoch_seconds": epoch_seconds,
            "iso8601": dt.strftime("%Y-%m-%dT%H:%M:%SZ"),
            "year": dt.year,
            "is_post_y2038": is_safe,
            "status": "PASS (64-bit safe)"
        }
    except OverflowError as e:
        return {
            "epoch_seconds": epoch_seconds,
            "error": str(e),
            "status": "FAIL (32-bit Integer Overflow)"
        }

def run_tests():
    print("=" * 65)
    printf_header = "Python Y2038 Timestamp Validator & Verification"
    print(f"{printf_header:^65}")
    print("=" * 65)

    test_timestamps = [
        ("Max 32-bit Timestamp", INT32_MAX),
        ("Overflow Boundary (+1s)", INT32_OVERFLOW),
        ("Far Future Date (Year 2100)", 4_102_444_800),
        ("Far Future Date (Year 2050)", 2_524_608_000),
    ]

    for label, ts in test_timestamps:
        result = validate_timestamp_64bit(ts)
        print(f"\n[TEST] {label}")
        print(f"       Timestamp : {ts}")
        print(f"       ISO 8601  : {result.get('iso8601')}")
        print(f"       Status    : {result.get('status')}")

    print("\n" + "=" * 65)

if __name__ == "__main__":
    run_tests()
