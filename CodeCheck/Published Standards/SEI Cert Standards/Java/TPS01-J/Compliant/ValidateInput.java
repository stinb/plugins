import java.util.concurrent.Callable;

// Does not use same thread pool
public final class ValidateInput<V> implements Callable<V> {
    private final V input;

    ValidateInput(V input) {
        this.input = input;
    }

    @Override
    public V call() throws Exception {
        // If validation fails, throw an exception here
        return (V) new SanitizeInput().sanitize(input);
    }
}