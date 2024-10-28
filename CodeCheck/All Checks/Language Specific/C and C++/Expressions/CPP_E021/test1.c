// Custom

const char *s1 = "\x41g";    // UndCC_Violation
const char *s2 = "\\x41g";   // UndCC_Valid
const char *s3 = "\\\x41g";  // UndCC_Violation
const char *s4 = "\\\\x41g"; // UndCC_Valid
