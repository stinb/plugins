// Custom

struct Date1 {
  unsigned short weekDay  : 3; // 0..7   (3 bits)
  unsigned short monthDay : 6; // 0..31  (6 bits)
  unsigned short month    : 5; // 0..12  (5 bits)
  unsigned short year     : 8; // 0..100 (8 bits)
  unsigned short good     : 1; // UndCC_Valid - unsigned
};

struct Date2 {
  unsigned weekDay  : 3; // 0..7   (3 bits)
  unsigned monthDay : 6; // 0..31  (6 bits)
  unsigned          : 0; // Force alignment to next boundary
  unsigned month    : 5; // 0..12  (5 bits)
  unsigned year     : 8; // 0..100 (8 bits)
  short bad         : 1; // UndCC_Violation
};

struct Date3 {
  unsigned weekDay  : 3; // 0..7   (3 bits)
  unsigned monthDay : 6; // 0..31  (6 bits)
  unsigned          : 0; // Force alignment to next boundary
  unsigned month    : 5; // 0..12  (5 bits)
  unsigned year     : 8; // 0..100 (8 bits)
  short             : 1; // UndCC_Valid - unnamed
};
