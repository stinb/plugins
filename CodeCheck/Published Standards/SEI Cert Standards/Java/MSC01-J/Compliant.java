public class Compliant {
    public final int DURATION = 10000; // In milliseconds

    public void nop() throws InterruptedException {
        while (true) { // UndCC_Valid
            // Useful operations
            Thread.sleep(DURATION);
        }
    }

    public void nop_2() {
        while (true) { // UndCC_Valid
            Thread.yield();
        }
    }

    public void infiniteLoopUsingFor() {
        for (;;) { // UndCC_Valid
            Thread.yield();
        }
    }

    public void infiniteLoopUsingDoWhile() {
        do { // UndCC_Valid
            Thread.yield();
        } while (true);
    }

}
