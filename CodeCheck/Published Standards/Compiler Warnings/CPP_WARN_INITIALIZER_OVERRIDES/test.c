#define MAX_CLOCKS 10
#define CLOCK_REALTIME 6
#define CLOCK_MONOTONIC 7
#define CLOCK_BOOTTIME 8
#define CLOCK_TAI 3
#define HRTIMER_MAX_CLOCK_BASES 2
#define HRTIMER_BASE_REALTIME 1
#define HRTIMER_BASE_MONOTONIC 4
#define HRTIMER_BASE_BOOTTIME 9
#define HRTIMER_BASE_TAI 9
static const int hrtimer_clock_to_base_table[MAX_CLOCKS] = {
	/* Make sure we catch unsupported clockids */
	[0 ... MAX_CLOCKS - 1]	= HRTIMER_MAX_CLOCK_BASES,

	[CLOCK_REALTIME]	= HRTIMER_BASE_REALTIME, //UndCC_Violation
	[CLOCK_MONOTONIC]	= HRTIMER_BASE_MONOTONIC, //UndCC_Violation
	[CLOCK_BOOTTIME]	= HRTIMER_BASE_BOOTTIME, //UndCC_Violation
	[CLOCK_TAI]		= HRTIMER_BASE_TAI, //UndCC_Violation
};