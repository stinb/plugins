void longjmp() {}
int _setjmp() { return 0; }

int main() {
  longjmp(); // UndCC_Valid
  _setjmp(); // UndCC_Valid
}
