struct Type1 {};

void okay1()
{
  try {
  } catch (Type1 &exception) {
    throw;  // okay to rethrow
  }
}

void violation1()
{
  try {
  } catch (int exception) {
    throw; // rethrow, built-in type // UndCC_Violation
  }
}

void violation2()
{
  try {
  } catch (Type1 *exception) {
    throw; // rethrow, by pointer // UndCC_Violation
  }
}
