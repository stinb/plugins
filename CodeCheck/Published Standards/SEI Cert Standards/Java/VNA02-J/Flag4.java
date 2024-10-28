import java.util.concurrent.atomic.AtomicBoolean;

final class Flag4 {
    private AtomicBoolean flag = new AtomicBoolean(true); // UndCC_Valid, AtomicBoolean

    public void toggle() {
        boolean temp;
        do {
            temp = flag.get();
        } while (!flag.compareAndSet(temp, !temp));
    }

    public AtomicBoolean getFlag() {
        return flag;
    }
}