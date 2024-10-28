import java.io.*;
import java.net.*;

// Thread-safe class
public final class SocketReader3_fn1 implements Runnable {
  private final Socket socket;
  private final BufferedReader in;
  private volatile boolean done = false;
  private final Object lock = new Object();
 
  public SocketReader3_fn1(String host, int port) throws IOException {
    this.socket = new Socket(host, port);
    this.in = new BufferedReader(
        new InputStreamReader(this.socket.getInputStream())
    );
  }
 
  // Only one thread can use the socket at a particular time
  @Override public void run() {
    try {
      synchronized (lock) {
        readData();
      }
    } catch (IOException ie) {
      // Forward to handler
    }
  }
 
  public void readData() throws IOException {
    String string;
    while (!done && (string = in.readLine()) != null) {
      // Blocks until end of stream (null)
    }
  }
 
  public void shutdown() {
    done = true;
  }
 
  public static void main(String[] args)
                          throws IOException, InterruptedException {
    SocketReader3_fn1 reader = new SocketReader3_fn1("somehost", 25);
    java.lang.Thread thread = new java.lang.Thread(reader); // UNDCC_Violation
    thread.start();
    Thread.sleep(1000);
    reader.shutdown(); // Shut down the thread
  }
}
