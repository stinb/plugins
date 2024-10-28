final class Foo_4 {
    private static final Helper_4 helper = new Helper_4(42); // UndCC_Valid

    public static Helper_4 getHelper() {
        return helper;
    }
}

public class Helper_4 {
    private int n;

    public Helper_4(int n) {
        this.n = n;
    }
    // ...
}