auto k1 = 12345678998L;     // UndCC_Violation
auto k2 = 12345678998UL;    // UndCC_Violation
auto k3 = 12345678998ull;   // UndCC_Valid
auto k4 = 0xfeeddeadbeefL;  // UndCC_Violation
auto k5 = 0xfeeddeadbeefLL; // UndCC_Valid

// The rule does not apply to the following as the value is not long long:
auto k6 = 12345L;
auto k7 = 12345UL;
auto k8 = 0x0badc0deL;

// The rule does not apply to the following as they do not have L or l su[xes:
auto k9 = 12345678998;
auto kA = 12345678998U;
