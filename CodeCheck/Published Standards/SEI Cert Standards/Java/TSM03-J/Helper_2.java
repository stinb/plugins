class Foo_2 {
    private final Helper_2 helper; // UndCC_Valid

    public Helper_2 getHelper() {
        return helper;
    }

    public Foo_2() {
        // Point 1
        helper = new Helper_2(42);
        // Point 2
    }
}

public class Helper_2 {
    private int n;

    public Helper_2(int n) {
        this.n = n;
    }
    // ...
}