public class Test2 {
  public static void main(String[] args) {
    Byte by1 = 0;
    Byte by2 = 0;
    by1 == by2;       // UndCC_Violation
    by1 != by2;       // UndCC_Violation
    by1.equals(by2);  // UndCC_Valid
    !by1.equals(by2); // UndCC_Valid

    Short sh1 = 0;
    Short sh2 = 0;
    sh1 == sh2;       // UndCC_Violation
    sh1 != sh2;       // UndCC_Violation
    sh1.equals(sh2);  // UndCC_Valid
    !sh1.equals(sh2); // UndCC_Valid

    Integer in1 = 0;
    Integer in2 = 0;
    in1 == in2;       // UndCC_Violation
    in1 != in2;       // UndCC_Violation
    in1.equals(in2);  // UndCC_Valid
    !in1.equals(in2); // UndCC_Valid

    Long lo1 = 0L;
    Long lo2 = 0L;
    lo1 == lo2;       // UndCC_Violation
    lo1 != lo2;       // UndCC_Violation
    lo1.equals(lo2);  // UndCC_Valid
    !lo1.equals(lo2); // UndCC_Valid

    Float fl1 = 0.0f;
    Float fl2 = 0.0f;
    fl1 == fl2;       // UndCC_Violation
    fl1 != fl2;       // UndCC_Violation
    fl1.equals(fl2);  // UndCC_Valid
    !fl1.equals(fl2); // UndCC_Valid

    Double do1 = 0.0d;
    Double do2 = 0.0d;
    do1 == do2;       // UndCC_Violation
    do1 != do2;       // UndCC_Violation
    do1.equals(do2);  // UndCC_Valid
    !do1.equals(do2); // UndCC_Valid

    Character ch1 = '\u0000';
    Character ch2 = '\u0000';
    ch1 == ch2;       // UndCC_Violation
    ch1 != ch2;       // UndCC_Violation
    ch1.equals(ch2);  // UndCC_Valid
    !ch1.equals(ch2); // UndCC_Valid

    Boolean bo1 = false;
    Boolean bo2 = false;
    bo1 == bo2;       // UndCC_Violation
    bo1 != bo2;       // UndCC_Violation
    bo1.equals(bo2);  // UndCC_Valid
    !bo1.equals(bo2); // UndCC_Valid
  }
}
