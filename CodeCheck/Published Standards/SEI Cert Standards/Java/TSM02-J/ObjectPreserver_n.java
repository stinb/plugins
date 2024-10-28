import java.util.concurrent.ConcurrentHashMap;

public final class ObjectPreserver_n implements Runnable {
    private static final ObjectPreserver_n lifeLine = new ObjectPreserver_n();

    private ObjectPreserver_n() { // UndCC_Violation, class initialization with thread and access to member
                                  // 'lifeline'
        Thread thread = new Thread(this);
        thread.setDaemon(true);
        thread.start(); // Keep this object alive
        lifeLine.notify();
    }

    // Neither this class nor HashMap will be garbage-collected.
    // References from HashMap to other objects
    // will also exhibit this property
    private static final ConcurrentHashMap<Integer, Object> protectedMap = new ConcurrentHashMap<Integer, Object>();

    public synchronized void run() {
        try {
            wait();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt(); // Reset interrupted status
        }
    }

    // Objects passed to this method will be preserved until
    // the unpreserveObject() method is called
    public static void preserveObject(Object obj) {
        protectedMap.put(0, obj);
    }

    // Returns the same instance every time
    public static Object getObject() {
        return protectedMap.get(0);
    }

    // Unprotect the objects so that they can be garbage-collected
    public static void unpreserveObject() {
        protectedMap.remove(0);
    }
}