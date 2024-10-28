// tests from CPP_D010, MISRA04_9.3, and MISRA08_8-5-3.
enum colour { red=3, blue, green, yellow=5 };        /* UndCC_Violation */

enum Color { Red=3, Blue=4, Green=5, Yellow=5 };        /* UndCC_Valid */


enum invalidInit { // UndCC_Violation
	TEST0,
	TEST1,
	TESTbig0 = 1000,
	TESTbig1,
};

enum validInit {
	FIRST = 10000,
	SECOND,
	THIRD,
	FOURTH,
	FIFTH,
};

enum validRegular { a, b, c, d, e, f };
