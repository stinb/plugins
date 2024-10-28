/* This class is thread-safe */
public final class CountHits {
    private static int counter;
    private static final Object lock = new Object();

    public void incrementCounter() {
        synchronized (lock) {
            counter++;  //UndCC_Valid
        }
    }
}
