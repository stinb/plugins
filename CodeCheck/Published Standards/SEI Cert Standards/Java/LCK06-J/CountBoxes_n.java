public final class CountBoxes_n implements Runnable {
  private static volatile int counter;
  // ...
  private final Object lock = new Object();

  @Override
  public void run() {
    synchronized (lock) {
      counter++; // UndCC_Violation
      // ...
    }
  }

  public static void main(String[] args) {
    for (int i = 0; i < 2; i++) {
      new Thread(new CountBoxes_n()).start();
    }
  }
}
