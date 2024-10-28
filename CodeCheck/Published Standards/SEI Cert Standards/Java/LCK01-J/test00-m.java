public class test {
  
  public static void main(String[] args){
    System.out.println("Hello World");
  }
  
  
  
  private final Boolean lockA = Boolean.FALSE;
  
  public void doSomethingA() {
    synchronized (lockA) { // UndCC_Violation
      // ...
    }
  }
  
  
  private int countB = 0; // cause
  private final Integer lockB = countB; // Boxed primitive Lock is shared
  
  public void doSomethingB() {
    synchronized (lockB) { // UndCC_Violation
      countB++;
      // ...
    }
  }
  
  
  private final String lockC = new String("LOCK").intern();
  
  public void doSomethingC() {
    synchronized (lockC) { // UndCC_Violation
      // ...
    }
  }
  
  
// This bug was found in jetty-6.1.3 BoundedThreadPool
  private final String lockD = "LOCK";
  
  public void doSomethingD() {
    synchronized (lockD) { // UndCC_Violation
      // ...
    }
  }
  
   /////////// UndCC_Valid /////////////
  
  private int countE = 0;
  private final Integer LockE = new Integer(countE);
  
  public void doSomethingE() {
    synchronized (LockE) { // UndCC_Valid
      countE++; 
      // ...
    }
  }
  
  
  private final String lockF = new String("LOCK");
  
  public void doSomethingF() {
    synchronized (lockF) { // UndCC_Valid
      // ...
    }
  }
  
  private final Object lockG = new Object();
  
  public void doSomethingG() {
    synchronized (lockG) { // UndCC_Valid
      // ...
    }
  }
  
  ////////// TEST ////////////
  
  private final Integer lockZ = new Integer(0);
  private Integer lockY = new Integer();
  
  public void doSomethingZ() {
    doSomethingY(lockZ); // UndCC_Valid
    doSomethingY(new Integer()); // UndCC_Valid
    doSomethingX(lockB); // cause
    doSomethingX(new Integer(0)); // UndCC_Valid
    
    doSomethingW(lockY); // cause
    
    synchronized (lockZ) {
      synchronized (lockY) { // UndCC_Violation
        synchronized (this) {
          //
        }
      }
    }
  }
  
  public void doSomethingY(final Integer lockXY) {
    synchronized (lockXY) {
      // ...
    }
  }
  
  public void doSomethingYY( Integer lockXYY) {
    synchronized (lockXYY) {
      // ...
    }
  }
  
  public void doSomethingYYY( final Boolean lockXYYY) { // cause
    synchronized (lockXYYY) { // UndCC_Violation
      // ...
    }
  }
  
  
  public void doSomethingX(final Integer lockX) {
    synchronized (lockX) { // UndCC_Violation
      // ...
    }
  }
  
  public void doSomethingW(final Integer lockXW) {
    private final Integer lockW = lockXW;
    synchronized (lockW) { // UndCC_Violation
      // ...
    }
  }
  
}
