final class Task implements Runnable {
    @Override
    public void run() {
        // ...
        throw new NullPointerException();
        // ...
    }
}