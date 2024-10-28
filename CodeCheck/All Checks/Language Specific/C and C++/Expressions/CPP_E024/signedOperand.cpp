
signed long okay = 1;
signed long okay2 = 2;
signed float violation1 = 3;
signed long answer;
signed double violation2;

void main() {
answer = violation2/okay2; // UndCC_Violation
answer = okay2/violation1; // UndCC_Violation
answer = okay%violation2; // UndCC_Violation
answer = violation1/okay; // UndCC_Violation
answer = okay%violation2; // UndCC_Violation

return 0;
}
