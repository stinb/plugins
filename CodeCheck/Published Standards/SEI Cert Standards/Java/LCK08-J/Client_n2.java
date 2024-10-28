import java.util.concurrent.locks.ReentrantLock;
import java.util.concurrent.locks.Lock;
import java.io.File;
import java.io.InputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public final class Client_n2 {
    private final Lock lock = new ReentrantLock();

    public void doSomething(File file) {
        InputStream in = null;
        try {
            in = new FileInputStream(file);
            lock.lock(); // UndCC_Violation, If the file cannot be opened, the lock may be unlocked
                         // without ever being locked in the first place
            // Perform operations on the open file
        } catch (FileNotFoundException fnf) {
            // Forward to handler
        } finally {
            lock.unlock();
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