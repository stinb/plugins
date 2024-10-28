public class TestMain {
public static void main(String[] args){
    System.out.println("CodeCheck Test File");
  }
}

class Test {
  
  public void funA() {
    synchronized (getClass()) { // UndCC_Violation
      // ...
    }
  }
  
  public void funB() {
    synchronized (this.getClass()) { // UndCC_Violation
      // ...
    }
  }
  
  public void funC() {
    synchronized (Test.class) { // UndCC_Valid
      // ...
    }
  }
  
  public void funD() {
    synchronized ( 
      Test.class /* getClass() */ ) { // UndCC_Valid
      // ...
    }
  }
  
  private class TelperTest {
    public void funE() {
      synchronized (getClass()) { // UndCC_Violation
        // ...
      }
    }
    
    public void funF() {
      synchronized (this.getClass()) { // UndCC_Violation
        // ...
      }
    }
  }
  
  private final Object lock = new Object();
  
  public void funZ() {
    TestMain x = new TestMain();
    synchronized (x.getClass()) { // UndCC_Violation
      // ...
    }  
  }

}
