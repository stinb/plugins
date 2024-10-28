public final class CountBoxes_n2 implements Runnable {
    private static volatile int counter;
    // ...
    private final Object lock = new Object();

    public synchronized void run() {
        counter++; // UndCC_Violation
        // ...
    }

    public static void main(String[] args) {
        for (int i = 0; i < 2; i++) {
            new Thread(new CountBoxes_n2()).start();
        }
    }
}
