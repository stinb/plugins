import java.io.*;
import java.net.*;

// Thread-safe class
public final class SocketReader4_fp1 implements Runnable {
  private final java.net.Socket primarySocket;
  private final BufferedReader in;
  private final Object lock = new Object();

  public SocketReader4_fp1(String host, int port) throws IOException {
    this.primarySocket = new Socket(host, port);
    this.in = new BufferedReader(
      new InputStreamReader(this.primarySocket.getInputStream())
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
    try {
      while ((string = in.readLine()) != null) {
        // Blocks until end of stream (null)
      }
    } finally {
      shutdown();
    }
  }

  public void shutdown() throws IOException {
    primarySocket.close();
  }

  public static void main(String[] args)
  throws IOException, InterruptedException {
    SocketReader4_fp1 reader = new SocketReader4_fp1("somehost", 25);
    Thread thread = new Thread(reader); // UNDCC_Valid
    thread.start();
    Thread.sleep(1000);
    reader.shutdown();
  }
}
