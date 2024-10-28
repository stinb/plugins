import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

final class Flag3 {
    private boolean flag = true;
    private final ReadWriteLock lock = new ReentrantReadWriteLock();
    private final Lock readLock = lock.readLock();
    private final Lock writeLock = lock.writeLock();

    public void toggle() { // UndCC_Valid, writelock
        writeLock.lock();
        try {
            flag ^= true; // Same as flag = !flag;
        } finally {
            writeLock.unlock();
        }
    }

    public boolean getFlag() { // UndCC_Valid, readlock
        readLock.lock();
        try {
            return flag;
        } finally {
            readLock.unlock();
        }
    }
}