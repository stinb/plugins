class Test0 {
  void func1(int x) {

  }

  void func2() {
    int okay1; okay1 = 1;
    int okay2 = 1; ++okay2;
    int okay3; okay3;
    int okay4; func1(okay4);
    for (int okay5 = 0; okay5 < 5; ++okay5) ;

    int violation1;     // UndCC_Violation
    int violation2 = 1; // UndCC_Violation
  }
}
