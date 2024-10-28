class Foo_5 {
    private volatile Helper_5 helper; // UndCC_Valid

    public Helper_5 getHelper() {
        return helper;
    }

    public void initialize() {
        helper = new Helper_5(42);
    }
}

// Immutable Helper
public final class Helper_5 {
    private final int n;

    public Helper_5(int n) {
        this.n = n;
    }
    // ...
}