// Test: Destructor must set all ptr data members to 0

class Class1 {
  int okay1A;           // OKAY: not ptr
  int *okay1B;          // OKAY: set to 0
  int *okay1C;          // OKAY: we ignore conditional paths
  int *okay1D;          // OKAY: at least one set is to 0
  int *okay1E;
  int *violation1A;     // UndCC_Violation: skipped
  int *violation1B;     // UndCC_Violation: set to NULL
  int *violation1C;     // UndCC_Violation: set to 0x0
  int *violation1D;     // UndCC_Violation: set to another member
  ~Class1();
};
Class1::~Class1()
{
  okay1B = 0;
  if (0)
    okay1C = 0;
  okay1D = NULL;
  okay1D = 0;
  violation1B = NULL;
  violation1C = 0x0;
  violation1D = okay1E = 0;
};
