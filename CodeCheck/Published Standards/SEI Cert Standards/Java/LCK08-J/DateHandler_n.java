import java.util.Date;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantLock;

final class DateHandler_n {

    private final Date date = new Date();

    private final Lock lock = new ReentrantLock();

    // str could be null
    public void doSomething(String str) {
        lock.lock();
        String dateString = date.toString();
        if (str.equals(dateString)) {
            // ...
        }
        // ...

        lock.unlock(); // UndCC_Violation
    }
}