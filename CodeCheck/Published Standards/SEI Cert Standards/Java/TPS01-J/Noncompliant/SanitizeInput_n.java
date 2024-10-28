import java.util.concurrent.Callable;

public final class SanitizeInput_n<V> implements Callable<V> {
    private final V input;

    SanitizeInput_n(V input) {
        this.input = input;
    }

    @Override
    public V call() throws Exception {
        // Sanitize input and return
        return (V) input;
    }
}