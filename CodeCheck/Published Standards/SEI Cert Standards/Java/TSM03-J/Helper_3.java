import java.util.Vector;

class Foo_3 {
    private final Vector<Helper_3> helper; // UndCC_Valid

    public Foo_3() {
        helper = new Vector<Helper_3>();
    }

    public Helper_3 getHelper() {
        if (helper.isEmpty()) {
            initialize();
        }
        return helper.elementAt(0);
    }

    public synchronized void initialize() {
        if (helper.isEmpty()) {
            helper.add(new Helper_3(42));
        }
    }
}

public class Helper_3 {
    private int n;

    public Helper_3(int n) {
        this.n = n;
    }
    // ...
}