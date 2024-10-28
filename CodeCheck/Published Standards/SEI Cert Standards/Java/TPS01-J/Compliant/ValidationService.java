import java.util.concurrent.ExecutionException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;

public final class ValidationService {
    private final ExecutorService pool;

    public ValidationService(int poolSize) {
        pool = Executors.newFixedThreadPool(poolSize);
    }

    public void shutdown() {
        pool.shutdown();
    }

    public StringBuilder fieldAggregator(String... inputs)
            throws InterruptedException, ExecutionException {
        StringBuilder sb = new StringBuilder();
        Future<String>[] results = new Future[inputs.length];

        for (int i = 0; i < inputs.length; i++) {
            // Don't pass-in thread pool
            results[i] = pool.submit(new ValidateInput<String>(inputs[i]));
        }

        for (int i = 0; i < inputs.length; i++) { // Aggregates the results
            sb.append(results[i].get());
        }
        return sb;
    }
}