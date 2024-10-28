
import java.util.concurrent.ConcurrentMap;
import java.util.concurrent.atomic.AtomicInteger;
import java.util.concurrent.ConcurrentHashMap;

final class KeyedCounter2 {
    private final ConcurrentMap<String, AtomicInteger> map = new ConcurrentHashMap<String, AtomicInteger>();
    private final Object lock = new Object();

    public void increment(String key) {
        AtomicInteger value = new AtomicInteger();
        AtomicInteger old = map.putIfAbsent(key, value);

        if (old != null) {
            value = old;
        }

        synchronized (lock) {
            if (value.get() == Integer.MAX_VALUE) {
                throw new ArithmeticException("Out of range");
            }
            value.incrementAndGet(); // Increment the value atomically
        }
    }

    public Integer getCount(String key) {
        AtomicInteger value = map.get(key);
        return (value == null) ? null : value.get();
    }

    // Other accessors ...
}