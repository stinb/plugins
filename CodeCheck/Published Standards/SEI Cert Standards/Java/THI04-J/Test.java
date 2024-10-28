public final class SocketReader implements Runnable {
  private final SocketChannel sc;
  private final Object lock = new Object();

  public SocketReader(String host, int port) throws IOException {
    InetSocketAddress sc = SocketChannel.open(new InetSocketAddress(host, port));
  }

  @Override public void run() {
  ByteBuffer buf = ByteBuffer.allocate(1024);
  try {
    synchronized (lock) {
      while (!Thread.interrupted()) {
        sc.read(buf);
        // ...
      }
    }
  } catch (IOException ie) {
    // Forward to handler
  }
  }

  public static void main(String[] args)
          throws IOException, InterruptedException {
     SocketReader reader = new SocketReader("somehost", 25);
     Thread thread = new Thread(reader); // UNDCC_Valid
     thread.start();
     Thread.sleep(1000);
     thread.interrupt();
  }
}
