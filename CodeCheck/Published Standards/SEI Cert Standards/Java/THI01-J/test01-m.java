final class HandleRequest implements Runnable {
  public void run() {
    // Do something
  }
}
 
public final class NetworkHandler implements Runnable {
  private static ThreadGroup tg = new ThreadGroup("Chief"); // UndCC_Violation
 
  @Override public void run() {
    new Thread(tg, new HandleRequest(), "thread1").start();
    new Thread(tg, new HandleRequest(), "thread2").start();
    new Thread(tg, new HandleRequest(), "thread3").start();
  }
 
  public static void printActiveCount(int point) {
    System.out.println("Active Threads in Thread Group " + tg.getName() + // UndCC_Violation
        " at point(" + point + "):" + " " + tg.activeCount()); // UndCC_Violation
  }
 
  public static void printEnumeratedThreads(Thread[] ta, int len) {
    System.out.println("Enumerating all threads...");
    for (int i = 0; i < len; i++) {
      System.out.println("Thread " + i + " = " + ta[i].getName());
    }
  }
 
  public static void main(String[] args) throws InterruptedException {
    // Start thread controller
    Thread thread = new Thread(tg, new NetworkHandler(), "controller");
    thread.start();
 
    // Gets the active count (insecure)
    Thread[] ta = new Thread[tg.activeCount()]; // UndCC_Violation
 
    printActiveCount(1); // P1
    // Delay to demonstrate TOCTOU condition (race window)
    Thread.sleep(1000);
    // P2: the thread count changes as new threads are initiated
    printActiveCount(2); 
    // Incorrectly uses the (now stale) thread count obtained at P1
    int n = tg.enumerate(ta);  // UndCC_Violation
    // Silently ignores newly initiated threads
    printEnumeratedThreads(ta, n);
                                   // (between P1 and P2)
 
    // This code destroys the thread group if it does
    // not have any live threads
    for (Thread thr : ta) {
      thr.interrupt();
      while(thr.isAlive());
    }
    tg.destroy(); // UndCC_Violation
    
    
    test t = new test("test"); // UndCC_Violation
    t.activeCount(); // UndCC_Violation
    t.getName(); // UndCC_Violation
  }
}



public final class NetworkHandlerC {
  private final ExecutorService executor;
  
  NetworkHandler(int poolSize) {
    this.executor = Executors.newFixedThreadPool(poolSize);
  }
  
  public void startThreads() {
    for (int i = 0; i < 3; i++) {
      executor.execute(new HandleRequest());
    }
  }
  
  public void shutdownPool() {
    executor.shutdown();
  }
  
  public static void main(String[] args)  {
    NetworkHandler nh = new NetworkHandler(3);
    nh.startThreads();
    nh.shutdownPool();
    
    
  }
}

class test extends ThreadGroup {
  @Override
  String getName()
  {
      return "o";
  }
}
