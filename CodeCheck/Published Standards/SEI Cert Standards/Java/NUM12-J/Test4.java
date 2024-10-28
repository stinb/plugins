// Custom

// Limits of integers:
// byte:   -128                 to 127
// short:  -32768               to 32767
// char:   0                    to 65535
// int:    -2147483648          to 2147483647
// long:   -9223372036854775808 to 9223372036854775807

// Approximate limits of floating points:
// float:  -3 * 10 ^ 38         to 3 * 10 ^ 38
// double: -1 * 10 ^ 308        to 1 * 10 ^ 308

// Limits of floating points for consecutive integers:
// float:  -16777216            to 16777216
// double: -9007199254740992    to 9007199254740992

class Test4 {
  public static void workWithByte(byte input) {
    byte   b = (byte)   input; // UndCC_Valid
    short  s = (short)  input; // UndCC_Valid
    char   c = (char)   input; // UndCC_Violation(0) - Flag these by turning on option
    int    i = (int)    input; // UndCC_Valid
    long   l = (long)   input; // UndCC_Valid

    float  f = (float)  input; // UndCC_Valid
    double d = (double) input; // UndCC_Valid
  }

  public static void workWithShort(short input) {
    byte   b = (byte)   input; // UndCC_Violation
    short  s = (short)  input; // UndCC_Valid
    char   c = (char)   input; // UndCC_Violation
    int    i = (int)    input; // UndCC_Valid
    long   l = (long)   input; // UndCC_Valid

    float  f = (float)  input; // UndCC_Valid
    double d = (double) input; // UndCC_Valid
  }

  public static void workWithChar(char input) {
    byte   b = (byte)   input; // UndCC_Violation
    short  s = (short)  input; // UndCC_Violation
    char   c = (char)   input; // UndCC_Valid
    int    i = (int)    input; // UndCC_Valid
    long   l = (long)   input; // UndCC_Valid

    float  f = (float)  input; // UndCC_Valid
    double d = (double) input; // UndCC_Valid
  }

  public static void workWithInt(int input) {
    byte   b = (byte)   input; // UndCC_Violation
    short  s = (short)  input; // UndCC_Violation
    char   c = (char)   input; // UndCC_Violation
    int    i = (int)    input; // UndCC_Valid
    long   l = (long)   input; // UndCC_Valid

    float  f = (float)  input; // UndCC_Valid - precision possibly lost, but still in range
    double d = (double) input; // UndCC_Valid
  }

  public static void workWithLong(long input) {
    byte   b = (byte)   input; // UndCC_Violation
    short  s = (short)  input; // UndCC_Violation
    char   c = (char)   input; // UndCC_Violation
    int    i = (int)    input; // UndCC_Violation
    long   l = (long)   input; // UndCC_Valid

    float  f = (float)  input; // UndCC_Valid - precision possibly lost, but still in range
    double d = (double) input; // UndCC_Valid - precision possibly lost, but still in range
  }

  public static void workWithFloat(float input) {
    byte   b = (byte)   input; // UndCC_Violation
    short  s = (short)  input; // UndCC_Violation
    char   c = (char)   input; // UndCC_Violation
    int    i = (int)    input; // UndCC_Violation
    long   l = (long)   input; // UndCC_Violation

    float  f = (float)  input; // UndCC_Valid
    double d = (double) input; // UndCC_Valid
  }

  public static void workWithDouble(double input) {
    byte   b = (byte)   input; // UndCC_Violation
    short  s = (short)  input; // UndCC_Violation
    char   c = (char)   input; // UndCC_Violation
    int    i = (int)    input; // UndCC_Violation
    long   l = (long)   input; // UndCC_Violation

    float  f = (float)  input; // UndCC_Violation
    double d = (double) input; // UndCC_Valid
  }
}
