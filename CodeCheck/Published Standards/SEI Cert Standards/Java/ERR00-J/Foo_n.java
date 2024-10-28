class Foo_n implements Runnable {
    public void run() {
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) { // UndCC_Violation, all comments
            // Ignore
        }
    }
}