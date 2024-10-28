public class Test00 {

  public static void main(String[] args){

  }

  private static final Boolean returnX(Boolean x) {
    return x;
  }
  private static final Integer returnX(Integer x) {
    return x;
  }
  private static final String returnX(String x) {
    return x;
  }

  

  private static final Boolean lockA = returnX(Boolean.FALSE);

  public static void doSomethingA() {
    synchronized (lockA) { // UndCC_Violation
      // ...
    }
  }


  private static int countB = 0;
  private static final Integer lockB = returnX(countB); // Boxed primitive Lock is shared

  public static void doSomethingB() {
    synchronized (lockB) { // UndCC_Violation
      countB++;
      // ...
    }
  }

  private static final String lockC = returnX(new String("LOCK").intern());

  public static void doSomethingC() {
    synchronized (lockC) { // UndCC_Violation
      // ...
    }
  }

  private static final String lockD = returnX("LOCK");

  public static void doSomethingD() {
    synchronized (lockD) { // UndCC_Violation
      // ...
    }
  }

}
