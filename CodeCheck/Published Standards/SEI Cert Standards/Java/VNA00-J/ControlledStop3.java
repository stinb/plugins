final class ControlledStop3 implements Runnable {
    private boolean done = false;

    @Override
    public void run() {
        while (!isDone()) { // UndCC_Valid
            try {
                // ...
                Thread.currentThread().sleep(1000); // Do something
            } catch (InterruptedException ie) {
                Thread.currentThread().interrupt(); // Reset interrupted status
            }
        }
    }

    public synchronized boolean isDone() {
        return done;
    }

    public synchronized void shutdown() {
        done = true;
    }
}