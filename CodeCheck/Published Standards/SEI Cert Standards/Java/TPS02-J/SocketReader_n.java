import java.net.Socket;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public final class SocketReader_n implements Runnable { // Thread-safe class
    private final Socket socket;
    private final BufferedReader in;
    private final Object lock = new Object();

    public SocketReader_n(String host, int port) throws IOException {
        this.socket = new Socket(host, port);
        this.in = new BufferedReader(
                new InputStreamReader(this.socket.getInputStream()));
    }

    // Only one thread can use the socket at a particular time
    @Override
    public void run() {
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
        socket.close();
    }
}