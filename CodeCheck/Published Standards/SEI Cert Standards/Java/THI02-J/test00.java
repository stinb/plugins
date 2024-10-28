import java.util.concurrent.locks.Condition;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

public final class test00 implements Runnable {
  private static final Object lock = new Object();
  private static final Condition condition = lock.newCondition();
  private static int time = 0;
  private final int step; // Do Perform operations when field time
                          // reaches this value
 
  public ProcessStep(int step) {
    this.step = step;
  }
 
  @Override public void run() {
    try {
      synchronized (lock) {
        while (time != step) {
          lock.wait();
        }
 
        // Perform operations
 
        time++;
        condition.signal(); // UndCC_Violation
        lock.notify(); // UndCC_Violation
      }
    } catch (InterruptedException ie) {
      Thread.currentThread().interrupt(); // Reset interrupted status
    }
  }
 
  public static void main(String[] args) {
    for (int i = 4; i >= 0; i--) {
      new Thread(new ProcessStep(i)).start();
    }
  }
}
