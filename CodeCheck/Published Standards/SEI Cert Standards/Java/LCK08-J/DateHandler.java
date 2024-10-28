import java.util.Date;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

final class DateHandler {

    private final Date date = new Date();

    private final Lock lock = new ReentrantLock();

    // str could be null
    public void doSomething(String str) {
        lock.lock();
        try {
            String dateString = date.toString();
            if (str != null && str.equals(dateString)) {
                // ...
            }
            // ...

        } finally {
            lock.unlock(); // UndCC_Valid
        }
    }
}