import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.Future;
import java.util.concurrent.ExecutionException;

final class PoolService3 {
    private final ExecutorService pool = Executors.newFixedThreadPool(10);

    public void doSomething() {
        Future<?> future = pool.submit(new Task());     //UndCC_Valid, uses submit

        // ...

        try {
            future.get();
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt(); // Reset interrupted status
        } catch (ExecutionException e) {
            Throwable exception = e.getCause();
            // Forward to exception reporter
        }
    }
}
