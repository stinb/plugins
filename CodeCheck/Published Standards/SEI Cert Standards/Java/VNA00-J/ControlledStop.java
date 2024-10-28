import java.lang.Runnable;

final class ControlledStop implements Runnable {
    private volatile boolean done = false;

    @Override
    public void run() {
        while (!done) { // UndCC_Valid
            try {
                // ...
                Thread.currentThread().sleep(1000); // Do something
            } catch (InterruptedException ie) {
                Thread.currentThread().interrupt(); // Reset interrupted status
            }
        }
    }

    public void shutdown() {
        done = true;
    }
}