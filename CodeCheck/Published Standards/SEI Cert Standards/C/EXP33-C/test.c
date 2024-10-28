void set_flag(int number, int *sign_flag) {
if (sign_flag == 0) {
  return;
}

if (number > 0) {
  *sign_flag = 1;
} else if (number < 0) {
  *sign_flag = -1;
}
}
int is_negative(int number) {
  int sign;
  set_flag(number, &sign); // UNDCC_Violation
  return sign < 0;
}
int is_negative1(int number) {
  int sign=0;
  set_flag(number, &sign); // UNDCC_Valid
  return sign < 0;
}
int is_negative2(int number) {
  int sign;
  sign = 0;
  set_flag(number, &sign); // UNDCC_Valid
  return sign < 0;
}
int is_negative3(int number) {
  int sign;
  set_flag(number, &sign); // UNDCC_Violation
  sign = 0;
  return sign < 0;
}

