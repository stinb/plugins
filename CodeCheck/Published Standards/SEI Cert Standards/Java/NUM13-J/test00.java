public class Test00 {
  private static void printFloat(float x) {
    System.out.println(x);
  }

  private static void printFloatObj(float x) {
    System.out.println(x);
  }

  private static void printDouble(double x) {
    System.out.println(x);
  }

  private static void printDoubleObj(double x) {
    System.out.println(x);
  }

  public static void printInt(int a) {
    printFloat(a); // UndCC_Violation - precision of parameter lost
  }

  public static void main(String[] args) {
    byte   a = 111;
    short  b = 11111;
    int    c = 1111111111;
    long   d = 1111111111111111111L;
    float  e;
    double f;
    Float  floatObj;
    Double doubleObj;

    System.out.println("\nMantissas: floats");
    e = 1L << 24;
    System.out.println(e);
    e = (1L << 24) + 1;
    System.out.println(e);
    e = (1L << 24) + 2;
    System.out.println(e);

    System.out.println("\nMantissas: doubles");
    f = 1L << 53;
    System.out.println(f);
    f = (1L << 53) + 1;
    System.out.println(f);
    f = (1L << 53) + 2;
    System.out.println(f);

    System.out.println("\nConversion by method parameterization to float");
    printFloat(a);          // UndCC_Valid
    printFloat(b);          // UndCC_Valid
    printFloat(c);          // UndCC_Violation - precision of variable lost
    printFloat(d);          // UndCC_Violation - precision of variable lost
    printFloat(1);          // UndCC_Valid
    printFloat(1111111111); // UndCC_Violation - precision of literal lost

    System.out.println("\nConversion by method parameterization to double");
    printDouble(a);                    // UndCC_Valid
    printDouble(b);                    // UndCC_Valid
    printDouble(c);                    // UndCC_Valid
    printDouble(d);                    // UndCC_Violation - precision of variable lost
    printDouble(1111111111);           // UndCC_Valid
    printDouble(1111111111111111111L); // UndCC_Violation - precision of literal lost

    System.out.println("\nConversion by constructor parameterization to float");
    floatObj = new Float(a);          // UndCC_Valid
    printFloatObj(floatObj);
    floatObj = new Float(b);          // UndCC_Valid
    printFloatObj(floatObj);
    floatObj = new Float(c);          // UndCC_Violation - precision of variable lost
    printFloatObj(floatObj);
    floatObj = new Float(d);          // UndCC_Violation - precision of variable lost
    printFloatObj(floatObj);
    floatObj = new Float(1);          // UndCC_Valid
    printFloatObj(floatObj);
    floatObj = new Float(1111111111); // UndCC_Violation - precision of literal lost
    printFloatObj(floatObj);

    System.out.println("\nConversion by constructor parameterization to double");
    doubleObj = new Double(a);                    // UndCC_Valid
    printDoubleObj(doubleObj);
    doubleObj = new Double(b);                    // UndCC_Valid
    printDoubleObj(doubleObj);
    doubleObj = new Double(c);                    // UndCC_Valid
    printDoubleObj(doubleObj);
    doubleObj = new Double(d);                    // UndCC_Violation - precision of variable lost
    printDoubleObj(doubleObj);
    doubleObj = new Double(1111111111);           // UndCC_Valid
    printDoubleObj(doubleObj);
    doubleObj = new Double(1111111111111111111L); // UndCC_Violation - precision of literal lost
    printDoubleObj(doubleObj);

    System.out.println("\nConversion by cast to float");
    e = (float) a;          // UndCC_Valid
    printFloat(e);
    e = (float) b;          // UndCC_Valid
    printFloat(e);
    e = (float) c;          // UndCC_Violation - precision of variable lost
    printFloat(e);
    e = (float) d;          // UndCC_Violation - precision of variable lost
    printFloat(e);
    e = (float) 1;          // UndCC_Valid
    printFloat(e);
    e = (float) 1111111111; // UndCC_Violation - precision of literal lost
    printFloat(e);

    System.out.println("\nConversion by cast to double");
    f = (double) a;                    // UndCC_Valid
    printDouble(f);
    f = (double) b;                    // UndCC_Valid
    printDouble(f);
    f = (double) c;                    // UndCC_Valid
    printDouble(f);
    f = (double) d;                    // UndCC_Violation - precision of variable lost
    printDouble(f);
    f = (double) 1111111111;           // UndCC_Valid
    printDouble(f);
    f = (double) 1111111111111111111L; // UndCC_Violation - precision of literal lost
    printDouble(f);

    System.out.println("\nConversion by assignment to float");
    e = a;          // UndCC_Valid
    printFloat(e);
    e = b;          // UndCC_Valid
    printFloat(e);
    e = c;          // UndCC_Violation - precision of variable lost
    printFloat(e);
    e = d;          // UndCC_Violation - precision of variable lost
    printFloat(e);
    e = 1;          // UndCC_Valid
    printFloat(e);
    e = 1111111111; // UndCC_Violation - precision of literal lost
    printFloat(e);

    System.out.println("\nConversion by assignment to double");
    f = a;                    // UndCC_Valid
    printDouble(f);
    f = b;                    // UndCC_Valid
    printDouble(f);
    f = c;                    // UndCC_Valid
    printDouble(f);
    f = d;                    // UndCC_Violation - precision of variable lost
    printDouble(f);
    f = 1111111111;           // UndCC_Valid
    printDouble(f);
    f = 1111111111111111111L; // UndCC_Violation - precision of literal lost
    printDouble(f);

    System.out.println();
  }
}
