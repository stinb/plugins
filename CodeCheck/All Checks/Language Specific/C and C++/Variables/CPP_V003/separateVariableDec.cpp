int Okay1;
int Okay2=0;
int FirstOkay1,Violation1; // UndCC_Violation
int FirstOkay2=0,Violation2=0; // UndCC_Violation

void func1() {
  int Okay3;
  int FirstOkay3,Violation3; // UndCC_Violation
  {
    int Okay4;
    int FirstOkay4,Violation4; // UndCC_Violation
  }
  for (int Okay5=0,Okay6,Okay7;Okay5<5;++Okay5);
}

struct S1 {
  S1() :
    Okay8(0),
    FirstOkay5(0),
    Violation5(0)
  {}
  int Okay8;
  int FirstOkay5,Violation5; // UndCC_Violation
};

enum E1 { Okay9, Okay10 };
