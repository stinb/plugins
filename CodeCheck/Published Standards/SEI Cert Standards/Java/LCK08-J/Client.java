import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Lock;
import java.io.File;
import java.io.InputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public final class Client {
    private final Lock lock = new ReentrantLock();

    public void doSomething(File file) {
        InputStream in = null;
        lock.lock();
        try {
            in = new FileInputStream(file);
            // Perform operations on the open file
        } catch (FileNotFoundException fnf) {
            // Forward to handler
        } finally {
            lock.unlock(); // UndCC_Valid

            if (in != null) {
                try {
                    in.close();
                } catch (IOException e) {
                    // Forward to handler
                }
            }
        }
    }
}
