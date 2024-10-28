// okay

void okay1() { for (int obj=0; obj<10; ++obj) obj++; };
void okay2() { int obj; do obj while (++obj); }
void okay3() { int obj; while (++obj) obj++; }
void okay4() { int obj; if (obj++) obj++; }
void okay5() { int obj; switch (obj++) { case 0: obj++; break; } }



void violation1() { int obj; obj = obj++; } // UndCC_Violation
void violation2() { int obj; obj++ = obj; } // UndCC_Violation
void violation3() { int obj; obj == ++obj; } // UndCC_Violation
void violation4() { int obj; func(obj,++obj); } // UndCC_Violation
void violation5() { int array[10],obj; array[obj++] = obj; } // UndCC_Violation
void violation6() { int array[10],obj; array[obj++] = array[obj++]; } // UndCC_Violation

// not violations, but obscure and hard to detect

void exception1() { int obj; obj = ++obj; } // UndCC_Violation
void exception2() { int obj; obj && ++obj; } // UndCC_Violation
void exception3() { int obj; obj,++obj; } // UndCC_Violation
