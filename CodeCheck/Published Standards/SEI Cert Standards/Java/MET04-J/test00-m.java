
class Super {
  protected void doLogic() {
    System.out.println("Super invoked");
  }
  
  protected void FA() {}
  
  void FB() {}
  
  void FC() {}
  
  void FD() {}
  
  private void FE(int x) {}
  
  private void FF(int x, char b) {}
  
  private void FG(int x, char b) {}
}


public class Sub extends Super {
  
  public void doLogic() { // UNDCC_Violation
    System.out.println("Sub invoked");
    // Do sensitive operations
  }
  
   @Override
  protected void FA() {} // UNDCC_Valid
  
  void FB() {} // UNDCC_Valid
  
  void FC() {} // UNDCC_Valid
  
  protected void FD() {}  // UNDCC_Violation
  
  void FE(int x) {} // UNDCC_Violation
  
  protected void FF(int x, char b) {} // UNDCC_Violation
  
  public void FG(char b, int x) {} // UNDCC_Valid
  
  @Override
  public Sub clone() // UNDCC_Valid - EX0: For classes that implement the java.lang.Cloneable interface, the accessibility of the Object.clone() method should be increased from protected to public.
  { Sub foo = new Sub(); return foo; }
  
  public static void main(String[] args) {}
}

abstract class aSuper
{
  abstract void FH();
}

class aSub extends aSuper
{
  @Override
  public void FH() {} // UNDCC_Valid - abstract methods may be flagged
}
