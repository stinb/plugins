import java.util.Map;
import java.util.HashMap;

final class KeyedCounter {
    private final Map<String, Integer> map = new HashMap<String, Integer>();
    private final Object lock = new Object();

    public void increment(String key) {
        synchronized (lock) {
            Integer old = map.get(key);
            int oldValue = (old == null) ? 0 : old.intValue();
            if (oldValue == Integer.MAX_VALUE) {
                throw new ArithmeticException("Out of range");
            }
            map.put(key, oldValue + 1);
        }
    }

    public Integer getCount(String key) {
        synchronized (lock) {
            return map.get(key);
        }
    }
}