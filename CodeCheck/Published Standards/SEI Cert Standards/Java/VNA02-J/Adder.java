final class Adder {
    private int a;
    private int b;

    public synchronized int getSum() { // UndCC_Valid
        // Check for overflow
        return a + b;
    }

    public synchronized void setValues(int a, int b) { // UndCC_Valid
        this.a = a;
        this.b = b;
    }
}