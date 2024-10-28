
struct Date1_62 {
	unsigned short nWeekDay_62  : 3;    // 0..7   (3 bits)
	unsigned short nMonthDay_62 : 6;    // 0..31  (6 bits)
	unsigned short nMonth_62    : 5;    // 0..12  (5 bits)
	unsigned short nYear_62     : 8;    // 0..100 (8 bits)
	unsigned short good_62		: 1;	/* UndCC_Valid */
};

struct Date2_62 {
	unsigned nWeekDay_62  : 3;    // 0..7   (3 bits)
	unsigned nMonthDay_62 : 6;    // 0..31  (6 bits)
	unsigned           	  : 0;    // Force alignment to next boundary.
	unsigned nMonth_62    : 5;    // 0..12  (5 bits)
	unsigned nYear_62     : 8;    // 0..100 (8 bits)
	short bad_62		  : 1;    //* UndCC_Violation */
};
