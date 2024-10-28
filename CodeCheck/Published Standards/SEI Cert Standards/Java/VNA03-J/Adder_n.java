import java.util.concurrent.atomic.AtomicReference;
import java.math.BigInteger;

final class Adder_n {
    private final AtomicReference<BigInteger> first;
    private final AtomicReference<BigInteger> second;

    public Adder_n(BigInteger f, BigInteger s) {
        first = new AtomicReference<BigInteger>(f);
        second = new AtomicReference<BigInteger>(s);
    }

    public void update(BigInteger f, BigInteger s) { 
        first.set(f); // UndCC_Violation, Unsafe
        second.set(s);
    }

    public BigInteger add() {
        return first.get().add(second.get()); // UndCC_Violation, Unsafe
    }
}
