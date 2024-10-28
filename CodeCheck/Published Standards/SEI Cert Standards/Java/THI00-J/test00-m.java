
public class Foo implements Runnable {
  @Override public void run() {
    // ...
  }
 
  public static void main(String[] args) {
    Foo foo = new Foo();
    new Thread(foo).run(); // UndCC_Violation
    
    Foo foo01 = new Foo();
    new Thread(foo01).start(); // UndCC_Valid 
  }
  
  public void sampleRunTest() {
    
    Thread thread = new Thread(new Runnable() {
      @Override public void run() {
        // ...
      }
    });
    
    ((Runnable) thread).run();  // UndCC_Valid - THI00-J-EX0: Does not start a new thread
  }
    
}
