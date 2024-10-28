import java.util.concurrent.Callable;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Future;

public final class ValidateInput_n<V> implements Callable<V> {
    private final V input;
    private final ExecutorService pool;

    ValidateInput_n(V input, ExecutorService pool) {
        this.input = input;
        this.pool = pool;
    }

    @Override
    public V call() throws Exception {
        // If validation fails, throw an exception here
        // Subtask
        Future<V> future = pool.submit(new SanitizeInput_n<V>(input));
        return (V) future.get();
    }
}