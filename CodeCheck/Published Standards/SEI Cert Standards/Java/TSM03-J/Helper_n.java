class Foo_n {
    private Helper_n helper;

    public Helper_n getHelper() { 
        return helper; // UndCC_Violation
    }

    public void initialize() {
        helper = new Helper_n(42);
    }
}

public class Helper_n {
    private int n;

    public Helper_n(int n) {
        this.n = n;
    }
    // ...
}