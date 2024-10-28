import java.nio.channels.SocketChannel;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.ByteBuffer;

public final class SocketReader implements Runnable {
    private final SocketChannel sc;
    private final Object lock = new Object();

    public SocketReader(String host, int port) throws IOException {
        sc = SocketChannel.open(new InetSocketAddress(host, port));
    }

    @Override
    public void run() {
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
}