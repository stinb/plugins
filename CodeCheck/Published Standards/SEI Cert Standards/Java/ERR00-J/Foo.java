class Foo implements Runnable {
    public void run() {
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) { // UndCC_Valid, interrupted
            Thread.currentThread().interrupt(); // Reset interrupted status
        }
    }
}