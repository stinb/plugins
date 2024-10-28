import java.util.concurrent.atomic.AtomicBoolean;

final class ControlledStop2 implements Runnable {
    private final AtomicBoolean done = new AtomicBoolean(false);

    @Override
    public void run() {
        while (!done.get()) { // UndCC_Valid
            try {
                // ...
                Thread.currentThread().sleep(1000); // Do something
            } catch (InterruptedException ie) {
                Thread.currentThread().interrupt(); // Reset interrupted status
            }
        }
    }

    public void shutdown() {
        done.set(true);
    }
}