class Foo {
    private Helper helper;
    private Helper init_helper = new Helper(42);

    public synchronized Helper getHelper() { // UndCC_Valid
        return helper;
    }

    public synchronized void initialize() {
        helper = new Helper(42);
    }
}

public class Helper {
    private int n;

    public Helper(int n) {
        this.n = n;
    }
    // ...
}
