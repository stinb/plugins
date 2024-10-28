// Thread-safe class
public final class SocketReader1 implements Runnable {
  // Other methods...
 
  public void readData() throws IOException {
    String string;
    while (!Thread.interrupted() && (string = in.readLine()) != null) {
      // Blocks until end of stream (null)
    }
  }
 
  public static void main(String[] args)
                          throws IOException, InterruptedException {
    SocketReader1 reader = new SocketReader("somehost", 25);
    Thread thread = new Thread(reader); // UNDCC_Violation
    thread.start();
    Thread.sleep(1000);
    thread.interrupt(); // Interrupt the thread
  }
}
