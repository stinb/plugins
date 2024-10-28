public final class Container implements Runnable {
  private final Vector<Integer> vector = new Vector<Integer>(1000);
 
  public Vector<Integer> getVector() {
    return vector;
  }
 
  @Override public synchronized void run() {
    Random number = new Random(123L);
    int i = vector.capacity();
    while (i > 0) {
      vector.add(number.nextInt(100));
      i--;
    }
  }
 
  public static void main(String[] args) throws InterruptedException {
    Thread thread = new Thread(new Container());
    thread.start();
    Thread.sleep(5000);
    thread.stop(); // UNDCC_Violation
    stop();
  }
  
  void stop() {
    
  }
}
