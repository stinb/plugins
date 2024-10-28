import java.util.concurrent.atomic.AtomicReference;
import java.math.BigInteger;

final class Adder {
    // ...
    private final AtomicReference<BigInteger> first;
    private final AtomicReference<BigInteger> second;

    public Adder(BigInteger f, BigInteger s) {
        first = new AtomicReference<BigInteger>(f);
        second = new AtomicReference<BigInteger>(s);
    }

    public synchronized void update(BigInteger f, BigInteger s) {
        first.set(f);
        second.set(s);
    }

    public synchronized BigInteger add() {
        return first.get().add(second.get());
    }
}