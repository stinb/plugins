import java.util.concurrent.atomic.AtomicInteger;

final class Adder_n2 {
    private final AtomicInteger a = new AtomicInteger();
    private final AtomicInteger b = new AtomicInteger();

    public int getSum() {
        return a.get() + b.get(); // UndCC_Violation
    }

    public void setValues(int a, int b) {
        this.a.set(a); // UndCC_Violation
        this.b.set(b);
    }
}