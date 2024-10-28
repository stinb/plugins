import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutionException;
import java.util.concurrent.Future;

public final class ValidationService_n {
    private final ExecutorService pool;

    public ValidationService_n(int poolSize) {
        pool = Executors.newFixedThreadPool(poolSize);
    }

    public void shutdown() {
        pool.shutdown();
    }

    public StringBuilder fieldAggregator(String... inputs)
            throws InterruptedException, ExecutionException {

        StringBuilder sb = new StringBuilder();
        // Stores the results
        Future<String>[] results = new Future[inputs.length];

        // Submits the tasks to thread pool
        for (int i = 0; i < inputs.length; i++) {
            results[i] = pool.submit(      //UndCC_Violation
                    new ValidateInput_n<String>(inputs[i], pool));
        }

        for (int i = 0; i < inputs.length; i++) { // Aggregates the results
            sb.append(results[i].get());
        }
        return sb;
    }
}
