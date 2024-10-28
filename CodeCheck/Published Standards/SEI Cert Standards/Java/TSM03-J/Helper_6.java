class Foo_6 {
    private volatile Helper_6 helper; // UndCC_Valid

    public Helper_6 getHelper() {
        return helper;
    }

    public void initialize() {
        helper = new Helper_6(42);
    }
}

// Mutable but thread-safe Helper
public class Helper_6 {
    private volatile int n;
    private final Object lock = new Object();

    public Helper_6(int n) {
        this.n = n;

    }

    public void setN(int value) {
        synchronized (lock) {
            n = value;
        }
    }
}