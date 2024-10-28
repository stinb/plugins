
public class Main {
  
  private enum enum01 {
    v1,
    v2
  }
  
  static void foo01() {} // UNDCC_Violation
  static void foo02() {} // UNDCC_Valid

  @Deprecated
  static void foo03() {}
  private int foo04() {return 2;} // UNDCC_Violation

  public static void main(String[] args) {
    ITest mO = new ITest();
    mO.fun01();
    mO.fun02();
    foo02();
  }
}

// Interface
interface InterTest {
  public void fun01(); // UNDCC_Valid
  public void fun02(); // UNDCC_Valid
  public void fun03(); // UNDCC_Valid
}

abstract class aTest {
  abstract void afun01(); // UNDCC_Valid
}
class t {
  public void foo() {} // UNDCC_Valid - can be violation with override option
}
class tt extends t{
  public void foo() {} // UNDCC_Valid - can be violation with override option
}

class ITest implements InterTest {
  public ITest() {} // UNDCC_Valid
  public void fun01() {} // UNDCC_Valid
  public void fun02() {} // UNDCC_Valid
  public void fun03() {} // UNDCC_Valid - can be violation with override option
  @Override
  protected void finalize() throws Throwable {super.finalize();} // UNDCC_Valid
}

