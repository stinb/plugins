
signed long okay = 1;
signed long okay2 = 2;
float violation1 = 3;
signed long answer;
double violation2;

int main() {
  answer = violation2/okay2; // UndCC_Violation
  answer = okay2/violation1; // UndCC_Violation
  //answer = okay%violation2; // Violation and analysis error
  answer = violation1/okay; // UndCC_Violation
  //answer = okay%violation2; // Violation and analysis error

  return 0;
}

